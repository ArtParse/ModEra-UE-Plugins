// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/SongDanceParser.h"

#include "SongDataTypes.h"

bool USongDanceParser::ParseDanceNotes(const FString& JsonString, TArray<FDanceNoteEvent>& OutDanceNotes)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		TArray<TSharedPtr<FJsonValue>> NotesArray = JsonObject->GetArrayField(TEXT("danceNotes"));

		for (int32 Index = 0; Index < NotesArray.Num(); ++Index)
		{
			TSharedPtr<FJsonObject> NoteObject = NotesArray[Index]->AsObject();
			FDanceNoteEvent DanceNote;

			DanceNote.Note.Note = NoteObject->GetIntegerField(TEXT("note"));
			DanceNote.Note.Velocity = NoteObject->GetIntegerField(TEXT("velocity"));
			DanceNote.Note.StartTime = NoteObject->GetNumberField(TEXT("startTime"));
			DanceNote.Note.EndTime = NoteObject->GetNumberField(TEXT("endTime"));
			DanceNote.X = NoteObject->GetIntegerField(TEXT("x"));
			DanceNote.Y = NoteObject->GetIntegerField(TEXT("y"));
			DanceNote.D = NoteObject->GetIntegerField(TEXT("d"));
			DanceNote.Limb = static_cast<ELimbType>(NoteObject->GetIntegerField(TEXT("limb")));

			OutDanceNotes.Add(DanceNote);
		}
		return true;
	}
	return false;
}
