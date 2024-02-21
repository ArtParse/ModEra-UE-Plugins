// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcadeNote.generated.h"

UCLASS()
class SONGPARSER_API AArcadeNote : public AActor
{
	GENERATED_BODY()

public:
	
	void SetNoteParameters(float NewSpeed, float NewLifetime);

	
	UPROPERTY(Blueprintable, BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(Blueprintable,BlueprintReadWrite)
	float Speed=100;

	UPROPERTY(Blueprintable,BlueprintReadWrite)
	float Lifetime=10;

	float CurrentLifetime=0;	
	// Sets default values for this actor's properties
	AArcadeNote();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
