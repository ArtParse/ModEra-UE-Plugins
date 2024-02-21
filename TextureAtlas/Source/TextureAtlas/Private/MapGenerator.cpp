// Copyright by Mod Era 2023, Inc. All Rights Reserved.

#include "MapGenerator.h"

FString UMapGenerator::GenerateNormalMap(const FString TexturePath)//, const FString SavePath)
{

	// Extract the base filename from the texture path
	FString BaseFilename = FPaths::GetBaseFilename(TexturePath, true); // true for extension
	FString Extension = FPaths::GetExtension(TexturePath);

	// add texture name suffix before the extension
	FString NewTextureName = BaseFilename + TEXT("_Normal.") + Extension;

	// Path to the output file
	FString BaseFilepath = FPaths::GetPath(TexturePath); // true for extension
	const FString FullOutputPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(BaseFilepath, NewTextureName));

	if (FPaths::FileExists(FullOutputPath))
	{
		return FullOutputPath;
	}

	// Load the texture from the file path
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("temptexture"),*TexturePath, LOAD_None, NULL);
	
	// get executable path	
	const FString ExePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("TextureAtlas/ThirdParty/normalmapgenerator.exe")));

	// check if executable exists
	if (!FPaths::FileExists(ExePath))
	{
		UE_LOG(LogTemp, Error, TEXT("UMapGenerator::GenerateNormalMap Executable not found: %s"), *ExePath);
		return TEXT("");
	}

	//output atlas file
	FString Arguments = TexturePath + TEXT(" -s 1.0 -it 2.0");

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
		
		// return output file
		return FullOutputPath;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch process: %s"), *ExePath);
		return TEXT("");
	}
	return TEXT("");
}

FString UMapGenerator::GenerateOcclusionMap(const FString TexturePath)
{	
	// Extract the base filename from the texture path
	FString BaseFilename = FPaths::GetBaseFilename(TexturePath, true); // true для сохранения расширения
	FString Extension = FPaths::GetExtension(TexturePath);

	// add texture name suffix before the extension
	FString NewTextureName = BaseFilename + TEXT("_AO.") + Extension;

	// Path to the output file
	FString BaseFilepath = FPaths::GetPath(TexturePath); // true для сохранения расширения
	const FString FullOutputPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(BaseFilepath, NewTextureName));
	
	if (FPaths::FileExists(FullOutputPath))
	{
		return FullOutputPath;
	}

	// Load the texture from the file path
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("temptexture"),*TexturePath, LOAD_None, NULL);
	
	// get executable path	
	const FString ExePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("TextureAtlas/ThirdParty/normalmapgenerator.exe")));

	// check if executable exists
	if (!FPaths::FileExists(ExePath))
	{
		UE_LOG(LogTemp, Error, TEXT("UMapGenerator::GenerateNormalMap Executable not found: %s"), *ExePath);
		return TEXT("");
	}

	//output atlas file
	FString Arguments = TexturePath + TEXT(" -s 1.0 -it 2.0");

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
		
		// return output file
		return FullOutputPath;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch process: %s"), *ExePath);
		return TEXT("");
	}
	return TEXT("");
}
