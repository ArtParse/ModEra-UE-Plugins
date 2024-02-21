// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrackReactor.h"
#include "Actors/ArcadeNote.h"
#include "NoteSpawner.generated.h"

UCLASS()
class SONGPARSER_API ANoteSpawner : public ATrackReactor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANoteSpawner();

	void OnNotePrepare_Implementation(FNoteEvent Note, float LeadTime);

	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void SpawnNote(FNoteEvent Note);

public:
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Note type for spawning
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RhythmGame")
	TSubclassOf<AArcadeNote> NoteActorClass;

	UPROPERTY(Blueprintable,BlueprintReadWrite, EditAnywhere, Category = "RhythmGame")
	float Speed=100;

	UPROPERTY(Blueprintable,BlueprintReadWrite, EditAnywhere, Category = "RhythmGame")
	float Lifetime=10;

	// Spawning distance
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RhythmGame")
	float NoteHorizontalDistance = 1000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RhythmGame")
	float NoteVerticalDistance = 1000.0f;
	
};
