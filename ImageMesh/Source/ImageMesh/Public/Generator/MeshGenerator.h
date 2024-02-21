
#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "UObject/Object.h"
#include "MeshGenerator.generated.h"


UENUM(BlueprintType)
enum class EMeshExtrusionMode : uint8
{
	MinDepth UMETA(DisplayName = "Minimum Depth"),
	MaxDepth UMETA(DisplayName = "Maximum Depth"),
	BasedOnImage UMETA(DisplayName = "Based on Image")
};

struct FUVSum {
	FVector2D UVSum;
	int Count;

	FUVSum() : UVSum(FVector2D(0, 0)), Count(0) {}
};

/**
 * This class represents a mesh generator used in the IMAGEMESH_API.
 */
UCLASS()
class IMAGEMESH_API UMeshGenerator : public UObject
{
	GENERATED_BODY()

public:
	

	/**
	 * Create a procedural mesh from an image and depth map.
	 *
	 * @param Image The input image
	 * @param DepthMap The depth map for the mesh
	 * @param MeshSize The size of the mesh
	 * @param SpawnTransform The transform for spawning the mesh
	 * @param NumPolygonsX The number of polygons in the X direction
	 * @param NumPolygonsY The number of polygons in the Y direction
	 * @param ExtrusionMode The mode for mesh extrusion
	 * @param bSearchOptimalPoint Flag for searching optimal points
	 *
	 * @return AProceduralActor pointer to the new mesh actor
	 *
	 * @throws None
	 */
	UFUNCTION(BlueprintCallable)
	static AProceduralActor* CreateProceduralMeshFromImage(UTexture2D* Image, UTexture2D* DepthMap, FVector MeshSize, FTransform SpawnTransform, int32 NumPolygonsX = 32, int32 NumPolygonsY =24, EMeshExtrusionMode ExtrusionMode=EMeshExtrusionMode::BasedOnImage, bool bSearchOptimalPoint=true);

private:

	static float GetDepthFromDepthMap(UTexture2D* DepthMap, int32 X, int32 Y);
	static void SetCustomMaterial(UProceduralMeshComponent* ProceduralMeshComponent, UTexture2D* Texture);
	static FVector2D FindMostContrastPoint(UTexture2D* DepthMap, int32 X, int32 Y, int32 Step, int32 Width, int32 Height);
	static float CalculateContrast(UTexture2D* DepthMap, int32 X, int32 Y, int32 Width, int32 Height);
	static void CreatePackageFromMesh(UProceduralMeshComponent* ProceduralMeshComponent);

};