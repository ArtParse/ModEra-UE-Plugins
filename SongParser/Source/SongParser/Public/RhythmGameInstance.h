#pragma once

#include "CoreMinimal.h"
#include "SongPlayer.h"
#include "Engine/GameInstance.h"
#include "RhythmGameInstance.generated.h"

UCLASS()
class SONGPARSER_API URhythmGameInstance : public UGameInstance
{
	GENERATED_BODY()

	/*
	UPROPERTY()
	ASongPlayer* SongPlayer;*/

public:

	virtual void Init() override;

	/*UFUNCTION(BlueprintCallable,BlueprintPure, Category = "RhythmGame")
	ASongPlayer* GetSongPlayer() const { return SongPlayer; }*/
};
