// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TrackReactor.h"


// Sets default values
ATrackReactor::ATrackReactor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATrackReactor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrackReactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrackReactor::OnNotePrepare_Implementation(FNoteEvent Note, float LeadTime)
{
	// logic on note prepare
	UE_LOG(LogTemp, Log, TEXT("Prepare note: %d, for time: %f"), Note.Note, LeadTime);
}

void ATrackReactor::OnNotePlay_Implementation(FNoteEvent Note)
{
	// logic on note play
	UE_LOG(LogTemp, Log, TEXT("Play note: %d"), Note.Note);
}
