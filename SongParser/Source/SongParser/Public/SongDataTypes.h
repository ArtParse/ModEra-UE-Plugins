// Copyright by Mod Era 2023, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "SongDataTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSongPlayer, Log, All);

UENUM(BlueprintType)
enum class ESongTrackType : uint8
{
	Drums UMETA(DisplayName = "Drums"),
	Guitar UMETA(DisplayName = "Guitar"),
	Bass UMETA(DisplayName = "Bass"),
	Keys UMETA(DisplayName = "Keys"),
	Vocals UMETA(DisplayName = "Vocals")
};


USTRUCT(BlueprintType)
struct FNoteEvent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Note; // 0-127

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Velocity; // 0-127

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float StartTime; // in seconds

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float EndTime; // in seconds
};


UENUM(BlueprintType)
enum class ELimbType : uint8
{
	LeftLeg UMETA(DisplayName = "LeftLeg"),
	RightLeg UMETA(DisplayName = "RightLeg"),
	AnyLeg UMETA(DisplayName = "AnyLeg"),
	LeftHand UMETA(DisplayName = "LeftHand"),
	RightHand UMETA(DisplayName = "RightHand"),
	AnyHand UMETA(DisplayName = "AnyHand"),
	Any UMETA(DisplayName = "Any")
};




USTRUCT(BlueprintType)
struct FDanceNoteEvent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FNoteEvent Note;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int X; // X coordinate

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Y; // Y coordinate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int D; //Direction (8 total, from 12pm clockwise)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELimbType Limb; //Direction (8 total, from 12pm clockwise)
	
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNotePrepareDelegate, FNoteEvent, Note, float, LeadTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotePlayDelegate, FNoteEvent, Note);

USTRUCT(BlueprintType)
struct FSongTrack
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, Category = "Song Midi")
	TArray<FNoteEvent> Notes;
	UPROPERTY(BlueprintReadOnly, Category = "Song Midi")
	ESongTrackType TrackType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Song Midi")
	int32 CurrentIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Song Midi")
	float Delay = 0;	
	
	// delegates
	FOnNotePrepareDelegate OnNotePrepare;
	FOnNotePlayDelegate OnNotePlay;
};

USTRUCT(BlueprintType)
struct FLyricEvent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LyricText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Time; // В секундах
};

// Struct for song information
USTRUCT(BlueprintType)
struct FSongInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString Name;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString Artist;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString Album;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString Genre;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString Charter;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 Year;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 PlaylistTrack;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 SongLength;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 Count;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffBand;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffGuitar;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffGuitarReal;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffGuitarCoop;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffBass;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffBassReal;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffDrums;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffDrumsReal;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 ProDrums;	
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffKeys;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffKeysReal;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffGuitarghl;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffRhythmghl;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffVocals;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffVocalsHarm;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 MultiplierNote;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffBassghl;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 PreviewStartTime;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 Frets;

	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString Icon;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 DiffRhythm;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 AlbumTrack;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 VideoStartTime;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	int32 Delay;
	UPROPERTY(BlueprintReadOnly, Category = "Song Info")
	FString FilePath;
	
	FSongInfo()
		: Year(-1), PlaylistTrack(-1), SongLength(-1), Count(-1), DiffBand(-1), DiffGuitar(-1),
		  DiffGuitarReal(-1), DiffGuitarCoop(-1), DiffBass(-1), DiffBassReal(-1), DiffDrums(-1),
		  DiffDrumsReal(-1), ProDrums(-1), DiffKeys(-1), DiffKeysReal(-1), DiffGuitarghl(-1),
		  DiffRhythmghl(-1), DiffVocals(-1), DiffVocalsHarm(-1), MultiplierNote(-1), DiffBassghl(-1),
		  PreviewStartTime(-1), Frets(-1), DiffRhythm(-1), AlbumTrack(-1), VideoStartTime(-1),
		  Delay(-1)
	{
		// Set default values
		Name = TEXT("");
		Artist = TEXT("");
		Album = TEXT("");
		Genre = TEXT("");
		Charter = TEXT("");
		Icon = TEXT("");
		FilePath = TEXT("");
	}

};

UCLASS()
class SONGPARSER_API USongTrackHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Song Track")
	static FString GetMidiTrackName(ESongTrackType TrackType);

	UFUNCTION(BlueprintCallable, Category = "Validation")
	static bool IsValidSongInfo(const FSongInfo& SongInfo);
};