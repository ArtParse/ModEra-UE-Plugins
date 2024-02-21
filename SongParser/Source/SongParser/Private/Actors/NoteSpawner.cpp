
#include "Actors/NoteSpawner.h"

ANoteSpawner::ANoteSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANoteSpawner::OnNotePrepare_Implementation(FNoteEvent Note, float LeadTime)
{
	Super::OnNotePrepare_Implementation(Note, LeadTime);
	SpawnNote(Note);
}

void ANoteSpawner::SpawnNote(FNoteEvent Note)
{
	if (!NoteActorClass) return; // check if NoteActorClass is set

	int32 NoteIndex = Note.Note % 12; // get the index of the note
	float SpawnDistance = NoteIndex * NoteHorizontalDistance; // calculate the spawn distance

	// calculate the spawn location
	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, SpawnDistance, 0.0f); 
	FRotator SpawnRotation = GetActorRotation(); 

	// Спауним актера в вычисленной локации с текущим поворотом
	AArcadeNote* NoteActor = GetWorld()->SpawnActor<AArcadeNote>(NoteActorClass, SpawnLocation, SpawnRotation);
	NoteActor->SetNoteParameters(Speed, Lifetime); // set the parameters of the spawned actor
}

void ANoteSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANoteSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

