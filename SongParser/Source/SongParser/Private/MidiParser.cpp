#include "MidiParser.h"
#include "../ThirdParty/MidiFile/include/MidiFile.h"
#include <iostream>
#include <map>
#include <vector>

#include "SongDataTypes.h"

using namespace smf;
using namespace std;

UMidiParser::UMidiParser()
{
}

UMidiParser::~UMidiParser()
{
}

FString UMidiParser::GetTrackName(const FString& MidiFilePath, int32 TrackNumber)
{
	MidiFile midiFile;
	midiFile.read(TCHAR_TO_UTF8(*MidiFilePath));

	if (TrackNumber >= midiFile.getTrackCount()) {
		return TEXT("Track number is out of range.");
	}

	for (int event = 0; event < midiFile[TrackNumber].size(); event++) {
		if (midiFile[TrackNumber][event].isMeta() && midiFile[TrackNumber][event][1] == 0x03) {
			return FString(midiFile[TrackNumber][event].getMetaContent().c_str());
		}
	}
	return TEXT("Unnamed Track");
}

int32 UMidiParser::FindTrackNumberByName(const FString& MidiFilePath, const FString& TrackNameToFind)
{
	MidiFile midiFile;
	midiFile.read(TCHAR_TO_UTF8(*MidiFilePath));

	for (int track = 0; track < midiFile.getTrackCount(); track++) {
		FString trackName = GetTrackName(MidiFilePath, track);
		if (trackName.Equals(TrackNameToFind, ESearchCase::IgnoreCase)) {
			return track;
		}
	}
	return -1; // if not found
}

TArray<FNoteEvent> UMidiParser::ParseMidiTrackByType(const FString& FilePath, ESongTrackType TrackType, int32 Difficulty)
{
	FString TrackName = USongTrackHelper::GetMidiTrackName(TrackType);
	TArray<FNoteEvent> OutNotes = ParseMidiTrack(FilePath, FindTrackNumberByName(FilePath, TrackName), Difficulty);
	return OutNotes;
}

TArray<FNoteEvent> UMidiParser::ParseMidiTrack(const FString& FilePath, int32 TrackNumber, int32 Difficulty )
{
	TArray<FNoteEvent> OutNotes;
    
	MidiFile midiFile;
	string convertedFilePath = TCHAR_TO_UTF8(*FilePath);
	midiFile.read(convertedFilePath);

	if (TrackNumber < 0 || TrackNumber >= midiFile.getTrackCount()) {
		UE_LOG(LogTemp, Warning, TEXT("Track number is out of range."));
		return OutNotes;
	}

	midiFile.doTimeAnalysis();
	midiFile.linkNotePairs();

	// find target octave
	int targetOctave = -1; // set to -1 by default
	if (Difficulty >= 0) {
		targetOctave = (Difficulty + 5) % 11; // add 5 to suit the MIDI song standard
	}

	for (int event = 0; event < midiFile[TrackNumber].size(); event++) {
		if (!midiFile[TrackNumber][event].isNoteOn()) continue;

		int noteNumber = midiFile[TrackNumber][event][1];
		int velocity = midiFile[TrackNumber][event][2];
		double startTime = midiFile[TrackNumber][event].seconds;

		// Find octave
		int noteOctave = noteNumber / 12;

		if (targetOctave != -1 && noteOctave != targetOctave) continue; // Skip notes that are not in the target octave

		// Find end time
		double endTime = startTime + 1.0; // default end time is 1 second

		// create note event
		FNoteEvent noteEvent;
		noteEvent.Note = noteNumber;
		noteEvent.Velocity = velocity;
		noteEvent.StartTime = static_cast<float>(startTime);
		noteEvent.EndTime = static_cast<float>(endTime);

		OutNotes.Add(noteEvent);
	}

	return OutNotes;
}

bool UMidiParser::IsMidiFileValid(const FString& FilePath)
{
	MidiFile midiFile;
	string convertedFilePath = TCHAR_TO_UTF8(*FilePath);
	return midiFile.read(convertedFilePath);
}
