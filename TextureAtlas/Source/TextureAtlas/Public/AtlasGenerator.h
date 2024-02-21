// Copyright by Mod Era 2023, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AtlasGenerator.generated.h"

UCLASS()
class TEXTUREATLAS_API UAtlasGenerator : public UObject
{
	GENERATED_BODY()

	/**
	 * Generates an atlas from the given texture paths and saves it to the specified path.
	 *
	 * @param TexturePaths array of paths to the textures
	 * @param OutCount number of textures in the atlas
	 * @param OutWidth width of the atlas
	 * @param OutHeight height of the atlas
	 * @param OutAtlasSize size of the atlas
	 * @param SavePath path to save the atlas
	 *
	 * @return the generated atlas as a string
	 *
	 * @throws ErrorType description of the error
	 */
	UFUNCTION(BlueprintCallable)
	static FString GenerateAtlas(const TArray<FString>& TexturePaths, int32& OutCount, int32& OutWidth, int32& OutHeight, int32& OutAtlasSize, const FString& SavePath = TEXT(""));

	/**
	 * Retrieves the size of the image located at the specified path.
	 *
	 * @param ImagePath the path to the image
	 *
	 * @return the size of the image as a FVector2D
	 *
	 * @throws ErrorType if the image path is invalid or inaccessible
	 */
	static const FVector2D GetImageSize(const FString& ImagePath);
};
