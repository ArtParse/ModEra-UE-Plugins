// Copyright by Mod Era 2023, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RhythmGameMode.generated.h"

UCLASS()
class SONGPARSER_API ARhythmGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void BeginPlay();
	
	/**
	 * Returns the song player.
	 *
	 * @return ASongPlayer* the song player object
	 */
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "RhythmGame")
	ASongPlayer* GetSongPlayer() const { return SongPlayer; }

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "RhythmGame")
	ARhythmGameMode* GetRhythmGameMode() { return this; }

private:
	UPROPERTY()
	ASongPlayer* SongPlayer;
};
