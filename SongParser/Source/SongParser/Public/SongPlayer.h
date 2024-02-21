// Copyright by Mod Era 2023, Inc. All Rights Reserved.
#pragma once


#include "CoreMinimal.h"
#include "RuntimeAudioImporterLibrary.h"
#include "SongDataTypes.h"
#include "GameFramework/Actor.h"
#include "Tickable.h"
#include "Actors/TrackReactor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SongPlayer.generated.h"


UCLASS(Blueprintable)
class SONGPARSER_API ASongPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASongPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;
	
	virtual void Tick(float DeltaTime) override;


	/**
	 * Load a song from the given file path.
	 *
	 * @param FilePath the path to the song file
	 *
	 * @return true if the song is successfully loaded, false otherwise
	 *
	 * @throws ErrorType description of the error that can occur
	 */
	UFUNCTION(BlueprintCallable)
	bool LoadSong(FString FilePath);

	/**
	 * Load the sound of a song from the given file path.
	 *
	 * @param FilePath the file path of the song sound
	 *
	 * @return true if the song sound is successfully loaded, false otherwise
	 *
	 * @throws ErrorType description of error
	 */
	bool LoadSongSound(FString FilePath);
	void OnAudioImportProgress(int32 Percentage);
	void OnAudioImportFinished(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave,
	                           ERuntimeImportStatus Status);
	// Called every frame
	void CheckTracksForNotes();
	void ProcessNotes(FSongTrack& Track, float CurrentTime, float Delay);

	/**
	 * Set the play time for the function.
	 *
	 * @param NewPlayTime the new play time to be set
	 *
	 * @return void
	 *
	 * @throws ErrorType description of error
	 */
	UFUNCTION(BlueprintCallable)
	void SetPlayTime(float NewPlayTime);
	int32 FindActualIndexForTrack(FSongTrack& Track, float CurrentTime);

	/**
	 * Retrieves the song player instance.
	 *
	 * @return a pointer to the song player
	 */
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "RhythmGame", meta = (WorldContext = "WorldContextObject"))
	static ASongPlayer* GetSongPlayer();

	/**
	 * Set the song player to be used.
	 *
	 * @param NewSongPlayer pointer to the new song player object
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "RhythmGame", meta = (WorldContext = "WorldContextObject"))
	static void SetSongPlayer(ASongPlayer* NewSongPlayer);

	/**
	 * Subscribes to track events of the specified type.
	 *
	 * @param TrackType the type of track events to subscribe to
	 * @param Subscriber the object subscribing to track events
	 *
	 * @return void
	 *
	 * @throws ErrorType description of error
	 */
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void SubscribeToTrackEvents(ESongTrackType TrackType, ATrackReactor* Subscriber);
	
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void AddTrack(ESongTrackType TrackType, int32 Difficulty = -1);
	
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void RemoveTrack(ESongTrackType TrackType);
	
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void SetCurrentTime(float NewPlayTime) {PlayTime = NewPlayTime;};
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	float GetCurrentTime() {return PlayTime;};
	
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	bool GetIsPlaying() {return bIsPlaying;};
	
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void PlaySong();
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void PauseSong();
	
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void SetIsPlaying(bool NewIsPlaying) {bIsPlaying = NewIsPlaying;};

	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	void ClearTracksPositions();

protected:
	ASongPlayer(const FObjectInitializer& ObjectInitializer);

private:
	static ASongPlayer* Instance;
	FSongInfo CurrentSong;
	USoundWave* CurrentAudio;
	bool bIsPlaying = false;
	float PlayTime = 0;
	float SpawnDelay = 2;
	TArray<FSongTrack> Tracks;
	UWorld* World;
};
