// Copyright by Mod Era 2023, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SongDataTypes.h"
#include "RhythmSongParser.generated.h"

UCLASS()
class SONGPARSER_API USongParser : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Find and parse songs.
	 *
	 * @return array of song information
	 *
	 * @throws ErrorType description of error
	 */
	UFUNCTION(BlueprintCallable, Category = "RhythmGame")
	static TArray<FSongInfo> FindAndParseSongs();
	
	static FSongInfo ParseSongInfoFromIni(const FString& FilePath);
};
