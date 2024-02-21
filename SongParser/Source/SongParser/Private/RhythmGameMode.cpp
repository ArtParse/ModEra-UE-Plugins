// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmGameMode.h"

#include "RhythmGameInstance.h"
#include "SongPlayer.h"


void ARhythmGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Spawn song player
	SongPlayer = GetWorld()->SpawnActor<ASongPlayer>(ASongPlayer::StaticClass());

}
