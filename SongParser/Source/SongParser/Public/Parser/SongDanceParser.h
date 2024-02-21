// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "SongDanceParser.generated.h"


UCLASS()
class SONGPARSER_API USongDanceParser : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Parse the dance notes from the given JSON string.
	 *
	 * @param JsonString the JSON string to parse
	 * @param OutDanceNotes the array to store the parsed dance notes
	 *
	 * @return true if the parsing was successful, false otherwise
	 *
	 * @throws None
	 */
	UFUNCTION(BlueprintCallable, Category = "Dance Note Parser")
	static bool ParseDanceNotes(const FString& JsonString, TArray<FDanceNoteEvent>& OutDanceNotes);
};




