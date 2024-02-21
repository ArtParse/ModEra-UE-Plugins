// Include necessary headers
#include "Generator/MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "SystemUtils.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
//#include "EditorAssetLibrary.h"
#endif

#include "RawMesh.h"
#include "StaticMeshAttributes.h"
#include "StaticMeshOperations.h"
#include "Generator/ProceduralActor.h"
#include "UObject/SavePackage.h"


void UMeshGenerator::CreatePackageFromMesh(UProceduralMeshComponent* ProceduralMeshComponent)
{
#if WITH_EDITOR
    if (!ProceduralMeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("ProceduralMeshComponent is null"));
        return;
    }

    // get mesh section
    FProcMeshSection* MeshSection = ProceduralMeshComponent->GetProcMeshSection(0);
    if (!MeshSection)
    {
        UE_LOG(LogTemp, Error, TEXT("Mesh section is null"));
        return;
    }

    // create package
    FString PackageName = TEXT("/Game/PathToSave/MyGeneratedMesh");
    FString AssetName = TEXT("MyGeneratedMesh");
    UPackage* Package = CreatePackage(*PackageName);
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, FName(*AssetName), RF_Public | RF_Standalone);

    // add mesh section to static mesh

    // register static mesh
    FAssetRegistryModule::AssetCreated(StaticMesh);
    Package->MarkPackageDirty();

    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
    UPackage::SavePackage(Package, StaticMesh, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName);
#endif
}


AProceduralActor* UMeshGenerator::CreateProceduralMeshFromImage(UTexture2D* Image, UTexture2D* DepthMap, FVector MeshSize, FTransform SpawnTransform, int32 NumPolygonsX, int32 NumPolygonsY, EMeshExtrusionMode ExtrusionMode, bool bSearchOptimalPoint)
{
    // Validate the input textures.
    if (!Image || !DepthMap) return nullptr;

    // Retrieve the dimensions of the input image.
    int32 Width = Image->GetSizeX();
    int32 Height = Image->GetSizeY();

    // Ensure the number of polygons is within a reasonable range.
    NumPolygonsX = FMath::Clamp(NumPolygonsX, 8, 128);
    if (NumPolygonsY < 0) {
        NumPolygonsY = FMath::RoundToInt(NumPolygonsX * (static_cast<float>(Height) / Width));
    }
    NumPolygonsX = FMath::Clamp(NumPolygonsX, 8, 128);

    // Calculate step sizes for iterating over the image based on the desired polygon count.
    float StepX = static_cast<float>(Width) / NumPolygonsX;
    float StepY = static_cast<float>(Height) / NumPolygonsY;

    // Prepare arrays to hold mesh data.
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector2D> UVs;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;

    // Placeholder values for depth processing. Actual depth map processing would set these appropriately.
    float MinDepthValue = 0.0f;
    float MaxDepthValue = 1.0f;
    
    // Iterate over each polygon position to generate vertices.
    for (int32 YIdx = 0; YIdx < NumPolygonsY; ++YIdx)
    {
        for (int32 XIdx = 0; XIdx < NumPolygonsX; ++XIdx)
        {
            int32 X = FMath::RoundToInt(XIdx * StepX);
            int32 Y = FMath::RoundToInt(YIdx * StepY);

            FVector2D BestPoint(X, Y);
            float BestDepthValue = GetDepthFromDepthMap(DepthMap, X, Y);

            // Adjust edge depth values based on the extrusion mode.
            if (ExtrusionMode == EMeshExtrusionMode::MinDepth && (XIdx == 0 || YIdx == 0 || XIdx == NumPolygonsX - 1 || YIdx == NumPolygonsY - 1))
            {
                BestDepthValue = MinDepthValue;
            }
            else if (ExtrusionMode == EMeshExtrusionMode::MaxDepth && (XIdx == 0 || YIdx == 0 || XIdx == NumPolygonsX - 1 || YIdx == NumPolygonsY - 1))
            {
                BestDepthValue = MaxDepthValue;
            }
            else if (bSearchOptimalPoint)
            {
                // Find the most contrasting point within a search radius to better define edges.
                BestPoint = FindMostContrastPoint(DepthMap, X, Y, FMath::RoundToInt(FMath::Min(StepX, StepY) * 0.45f), Width, Height);
                BestDepthValue = GetDepthFromDepthMap(DepthMap, BestPoint.X, BestPoint.Y);
            }

            // Calculate vertex position in 3D space.
            FVector VertexPosition = FVector(BestPoint.X / (float)Width * MeshSize.X, BestPoint.Y / (float)Height * MeshSize.Y, BestDepthValue * MeshSize.Z);
            Vertices.Add(VertexPosition);
            Normals.Add(FVector(0, 0, -1)); // Default normal vector assuming a flat surface.

            // Calculate UV coordinates based on vertex position within the texture space.
            UVs.Add(FVector2D(static_cast<float>(X) / Width, static_cast<float>(Y) / Height));

            // Generate triangles for the current polygon, ensuring we don't exceed the mesh bounds.
            if (XIdx < NumPolygonsX - 1 && YIdx < NumPolygonsY - 1)
            {
                int32 CurrentIndex = YIdx * NumPolygonsX + XIdx;
                int32 RightIndex = CurrentIndex + 1;
                int32 BottomIndex = CurrentIndex + NumPolygonsX;
                int32 BottomRightIndex = BottomIndex + 1;

                // Define two triangles for the current square based on vertex indices.
                Triangles.Add(CurrentIndex);
                Triangles.Add(BottomIndex);
                Triangles.Add(RightIndex);

                Triangles.Add(RightIndex);
                Triangles.Add(BottomIndex);
                Triangles.Add(BottomRightIndex);
            }
        }
    }

    // Initialize an array to accumulate vertex normals.
    TArray<FVector> VertexNormals;
    VertexNormals.AddZeroed(Vertices.Num());

    // Compute triangle normals and add them to the vertex normals for smooth shading.
    for (int32 i = 0; i < Triangles.Num(); i += 3)
    {
        int32 Index0 = Triangles[i];
        int32 Index1 = Triangles[i + 1];
        int32 Index2 = Triangles[i + 2];

        FVector Vertex0 = Vertices[Index0];
        FVector Vertex1 = Vertices[Index1];
        FVector Vertex2 = Vertices[Index2];

        // Calculate the normal of the triangle defined by three vertices.
        FVector Edge1 = Vertex1 - Vertex0;
        FVector Edge2 = Vertex2 - Vertex0;
        FVector TriangleNormal = FVector::CrossProduct(Edge2, Edge1).GetSafeNormal();

        // Accumulate the triangle normal to each of the triangle's vertices.
        VertexNormals[Index0] += TriangleNormal;
        VertexNormals[Index1] += TriangleNormal;
        VertexNormals[Index2] += TriangleNormal;
    }

    // Normalize the accumulated vertex normals to smooth the shading across the mesh.
    for (FVector& Normal : VertexNormals)
    {
        Normal.Normalize();
    }

    // Update the mesh normals with the computed smoothed normals.
    Normals = VertexNormals;
        
    // Spawn the procedural actor with the specified transform.
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Name = FName("ProceduralActor");
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    // Attempt to spawn the procedural actor in the world.
    AProceduralActor* ProceduralActor = USystemUtils::GetCurrentWorld()->SpawnActor<AProceduralActor>(AProceduralActor::StaticClass(), SpawnTransform, SpawnParameters);
    
    if (!ProceduralActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn ProceduralActor"));
        return nullptr;
    }

    // Create and configure the procedural mesh component.
    UProceduralMeshComponent* ProceduralMesh = ProceduralActor->GetProceduralMeshComponent();
    if (ProceduralMesh)
    {
        ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
        ProceduralMesh->AttachToComponent(ProceduralActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        // Apply the original image as a texture to the generated mesh.
        SetCustomMaterial(ProceduralMesh, Image);
    }

    return ProceduralActor;
}

float UMeshGenerator::GetDepthFromDepthMap(UTexture2D* DepthMap, int32 X, int32 Y)
{
    if (!DepthMap) return 0.0f;

    // Lock the texture for reading
    FTexture2DMipMap& Mip = DepthMap->PlatformData->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

    // Calculate the index into the texture data
    int32 TextureWidth = DepthMap->GetSizeX();
    uint8* RawData = static_cast<uint8*>(Data);

    // Assuming the depth map is a grayscale image
    float DepthValue = 0.0f;
    int32 Index = (Y * TextureWidth + X) * 4;
    for(int i = 0; i < 4; i++)
    {
        DepthValue += (RawData[Index] / 255.0f)/4.0f; // Normalize the value
    }

    // Unlock the texture
    Mip.BulkData.Unlock();

    return (DepthValue);
}

 void UMeshGenerator::SetCustomMaterial(UProceduralMeshComponent* ProceduralMeshComponent, UTexture2D* Texture)
{
    UMaterial* LoadedMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Material/DynamicMaterial/DisplacementMaterial.DisplacementMaterial"));
    if (LoadedMaterial)
    {
        UMaterialInstanceDynamic* MyDynamicMaterial = UMaterialInstanceDynamic::Create(LoadedMaterial, ProceduralMeshComponent);
        
        // Assign material to mesh component
        ProceduralMeshComponent->SetMaterial(0, MyDynamicMaterial);

        MyDynamicMaterial->SetTextureParameterValue(FName("DynamicTexture"), Texture);
    }
}


FVector2D UMeshGenerator::FindMostContrastPoint(UTexture2D* DepthMap, int32 X, int32 Y, int32 SearchRadius, int32 Width,
                                                int32 Height)
{
    float MaxContrast = -1.0f;
    FVector2D MostContrastPoint = FVector2D(X,Y);
    
    for (int32 SearchY = FMath::Max(0, Y - SearchRadius); SearchY <= FMath::Min(Height - 1, Y + SearchRadius); ++SearchY)
    {
        for (int32 SearchX = FMath::Max(0, X - SearchRadius); SearchX <= FMath::Min(Width - 1, X + SearchRadius); ++SearchX)
        {
            // calculate contrast
            float CurrentContrast = CalculateContrast(DepthMap, SearchX, SearchY, Width, Height);

            if (CurrentContrast > MaxContrast)
            {
                MaxContrast = CurrentContrast;
                MostContrastPoint = FVector2D(SearchX, SearchY);
            }
        }
    }

    return MostContrastPoint;
}


float UMeshGenerator::CalculateContrast(UTexture2D* DepthMap, int32 X, int32 Y, int32 Width, int32 Height)
{
    float CenterDepth = GetDepthFromDepthMap(DepthMap, X, Y);
    float MaxDifference = 0.0f;

    // check neighbors
    for (int32 OffsetY = -1; OffsetY <= 1; ++OffsetY)
    {
        for (int32 OffsetX = -1; OffsetX <= 1; ++OffsetX)
        {
            if (OffsetX == 0 && OffsetY == 0) continue; // skip center

            int32 NeighborX = FMath::Clamp(X + OffsetX, 0, Width - 1);
            int32 NeighborY = FMath::Clamp(Y + OffsetY, 0, Height - 1);

            float NeighborDepth = GetDepthFromDepthMap(DepthMap, NeighborX, NeighborY);
            float Difference = FMath::Abs(CenterDepth - NeighborDepth);

            MaxDifference = FMath::Max(MaxDifference, Difference);
        }
    }

    return MaxDifference;
}

