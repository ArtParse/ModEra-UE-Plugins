// Copyright by Mod Era 2023, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MapGenerator.generated.h"

UCLASS()
class TEXTUREATLAS_API UMapGenerator : public UObject
{
	GENERATED_BODY()

	/**
	* Generates a normal map based on the provided texture path.
	*
	* @param TexturePath the path of the texture to generate the normal map from
	*
	* @return the generated normal map
	*
	* @throws ErrorType description of error
	*/
	UFUNCTION(BlueprintCallable,category="Texture")
	static FString GenerateNormalMap(const FString TexturePath);

	/**
	 * Generates an occlusion map for the given texture path.
	 *
	 * @param TexturePath the path of the texture
	 *
	 * @return the generated occlusion map
	 *
	 * @throws ErrorType description of error
	 */
	UFUNCTION(BlueprintCallable,category="Texture")
	static FString GenerateOcclusionMap(const FString TexturePath);
};
