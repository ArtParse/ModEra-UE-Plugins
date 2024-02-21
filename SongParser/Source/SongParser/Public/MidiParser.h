// Copyright by Mod Era 2023, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "SongDataTypes.h"
#include "UObject/NoExportTypes.h"
#include "MidiParser.generated.h"

UCLASS(Blueprintable)
class SONGPARSER_API UMidiParser : public UObject
{
	GENERATED_BODY()

public:
	UMidiParser();
	virtual ~UMidiParser();
	
	UFUNCTION(BlueprintCallable, Category = "MIDI Parsing")
	static FString GetTrackName(const FString& MidiFilePath, int32 TrackNumber);
	
	UFUNCTION(BlueprintCallable, Category = "MIDI Parsing")
	static int32 FindTrackNumberByName(const FString& MidiFilePath, const FString& TrackNameToFind);

	UFUNCTION(BlueprintCallable, Category = "MIDI Parsing")
	static TArray<FNoteEvent> ParseMidiTrackByType(const FString& FilePath, ESongTrackType TrackType, int32 Difficulty = -1);
	
	UFUNCTION(BlueprintCallable, Category = "MIDI Parsing")
	static TArray<FNoteEvent> ParseMidiTrack(const FString& FilePath, int32 TrackNumber, int32 Difficulty = -1);

	bool IsMidiFileValid(const FString& FilePath);
};
