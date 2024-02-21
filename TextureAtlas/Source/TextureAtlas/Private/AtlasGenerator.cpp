// Copyright by Mod Era 2023, Inc. All Rights Reserved.


#include "AtlasGenerator.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"


FString UAtlasGenerator::GenerateAtlas(const TArray<FString>& TexturePaths, int32& OutCount, int32& OutWidth, int32& OutHeight, int32& OutAtlasSize, const FString& SavePath)
{
	//Set utput paths
	FString OutputPath;
	if (SavePath==TEXT(""))
	{
		OutputPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Atlas")));		
	}
	else
	{
		OutputPath = SavePath;
	}
	
	// Scale factor due texture's count in power of 2 (for square textures)
	const int TextureCount = TexturePaths.Num();
	
	// Load the example texture from the file path
	const FVector2D ExampleImage = GetImageSize(TexturePaths[0]);
	// Check if all textures are equal
	for(const FString& TexturePath : TexturePaths){
		const FVector2D OtherImage = GetImageSize(TexturePath);
		if(ExampleImage.X != OtherImage.X || ExampleImage.Y != OtherImage.Y){
			UE_LOG(LogTemp, Error, TEXT("UAtlasGenerator::GenerateAtlas Texture at %s is not equal to primary size: Width: %f, Height: %f"), 
				*TexturePath, ExampleImage.X, ExampleImage.Y);
			return TEXT("");
		}
	}

	// Calculate the size for atlas
	const int32 MinimalRequiredArea = TextureCount * ExampleImage.X * ExampleImage.Y;
	int32 AtlasSize = 2;
	while (AtlasSize * AtlasSize < MinimalRequiredArea) AtlasSize *= 2;
	
	//default atlas name from first file name
	FString AtlasName = TEXT("Atlas_") + FPaths::GetBaseFilename(TexturePaths[0]);
	
	// get executable path	
	const FString ExePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("TextureAtlas/ThirdParty/TextureAtlas.exe")));

	// check if executable exists
	if (!FPaths::FileExists(ExePath))
	{
		UE_LOG(LogTemp, Error, TEXT("UAtlasGenerator::GenerateAtlas Executable not found: %s"), *ExePath);
		return TEXT("");
	}

	// get arguments string
	FString Arguments;
	for (const FString& TexturePath : TexturePaths)
	{
		FString FullTexturePath = FPaths::ConvertRelativePathToFull(TexturePath);
		Arguments += TEXT("\"") + FullTexturePath + TEXT("\" ");
	}

	const FString FullOutputPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(OutputPath, AtlasName) + TEXT(".png"));
	//output atlas file
	Arguments += TEXT("-o \"") + FullOutputPath+ TEXT("\"");
	
	// get arguments string
	Arguments+= TEXT(" -s ")+ FString::FromInt(AtlasSize) + TEXT(" ");
	
	// start executable
	void* PipeRead = nullptr;
	void* PipeWrite = nullptr;
	FPlatformProcess::CreatePipe(PipeRead, PipeWrite);
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(*ExePath, *Arguments, false, false, false, nullptr, 0, nullptr, PipeWrite);
	
	// wait for process
	if (ProcHandle.IsValid())
	{
		FPlatformProcess::WaitForProc(ProcHandle);

		// reading process
		FString ProcessOutput = FPlatformProcess::ReadPipe(PipeRead);

		// close process
		FPlatformProcess::CloseProc(ProcHandle);
		FPlatformProcess::ClosePipe(PipeRead, PipeWrite);

		// Fill output atlas properties
		OutCount = TextureCount;
		OutWidth = ExampleImage.X;
		OutHeight = ExampleImage.Y;
		OutAtlasSize = AtlasSize;
		
		// return output file
		return FullOutputPath;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch process: %s"), *ExePath);
		return TEXT("");
	}
}

const FVector2D UAtlasGenerator::GetImageSize(const FString& ImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat ImageFormat = EImageFormat::PNG; // Or other formats like JPEG, TIFF, etc.
    
	// Create an image wrapper for the specified format
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
    
	// Load the image file into a buffer
	TArray<uint8> RawFileData;
	if (FFileHelper::LoadFileToArray(RawFileData, *ImagePath)) {
		// Parse the image data to get its size
		if (ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num())) {
			int32 Width = ImageWrapper->GetWidth();
			int32 Height = ImageWrapper->GetHeight();
			return FVector2D(Width, Height);
		}
	}

	return FVector2D(0, 0); // Return an empty size if loading failed
}
