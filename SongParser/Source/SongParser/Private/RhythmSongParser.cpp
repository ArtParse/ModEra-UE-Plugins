// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmSongParser.h"


#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/ConfigCacheIni.h"

#include "../ThirdParty/MidiFile/include/MidiFile.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace smf;


struct NoteEvent {
	int note;      
	int velocity;   
	double startTime; 
	double endTime;   
};


TArray<FSongInfo> USongParser::FindAndParseSongs()
{

	FString Directory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Data/Songs"));
	// song list
	TArray<FSongInfo> Songs;

	// Find all .ini files in the directory
	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> FoundFiles;
	FileManager.FindFilesRecursive(FoundFiles, *Directory, TEXT("*.ini"), true, false, false);

	for (const FString& FilePath : FoundFiles)
	{
		FSongInfo Song(ParseSongInfoFromIni(FilePath));
		
		Songs.Add(Song);
	}

	return Songs;
}


FSongInfo USongParser::ParseSongInfoFromIni(const FString& FilePath)
{
    FSongInfo SongInfo;

	// check if file exists
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FilePath);
		return SongInfo;

	}

	// save path to file
	SongInfo.FilePath = FilePath;
	
    FConfigFile ConfigFile;
    ConfigFile.Read(FilePath);

    const FString SectionName = TEXT("Song");

    // extract str values from ini
    ConfigFile.GetString(*SectionName, TEXT("name"), SongInfo.Name);
    ConfigFile.GetString(*SectionName, TEXT("artist"), SongInfo.Artist);
    ConfigFile.GetString(*SectionName, TEXT("album"), SongInfo.Album);
    ConfigFile.GetString(*SectionName, TEXT("genre"), SongInfo.Genre);
	ConfigFile.GetString(*SectionName, TEXT("charter"), SongInfo.Charter);
    ConfigFile.GetString(*SectionName, TEXT("icon"), SongInfo.Icon);

    // extract int values
    ConfigFile.GetInt(*SectionName, TEXT("year"), SongInfo.Year);
    ConfigFile.GetInt(*SectionName, TEXT("playlist_track"), SongInfo.PlaylistTrack);
    ConfigFile.GetInt(*SectionName, TEXT("song_length"), SongInfo.SongLength);
    ConfigFile.GetInt(*SectionName, TEXT("count"), SongInfo.Count);
    ConfigFile.GetInt(*SectionName, TEXT("diff_band"), SongInfo.DiffBand);
    ConfigFile.GetInt(*SectionName, TEXT("diff_guitar"), SongInfo.DiffGuitar);
	ConfigFile.GetInt(*SectionName, TEXT("diff_guitar_real"), SongInfo.DiffGuitarReal);
	ConfigFile.GetInt(*SectionName, TEXT("diff_guitar_coop"), SongInfo.DiffGuitarCoop);
    ConfigFile.GetInt(*SectionName, TEXT("diff_bass"), SongInfo.DiffBass);
	ConfigFile.GetInt(*SectionName, TEXT("diff_bass_real"), SongInfo.DiffBassReal);
    ConfigFile.GetInt(*SectionName, TEXT("diff_drums"), SongInfo.DiffDrums);
	ConfigFile.GetInt(*SectionName, TEXT("pro_drums"), SongInfo.ProDrums);
	ConfigFile.GetInt(*SectionName, TEXT("diff_drums_real"), SongInfo.DiffDrumsReal);
	ConfigFile.GetInt(*SectionName, TEXT("diff_rhythm"), SongInfo.DiffRhythm);
    ConfigFile.GetInt(*SectionName, TEXT("diff_keys"), SongInfo.DiffKeys);
	ConfigFile.GetInt(*SectionName, TEXT("diff_keys_real"), SongInfo.DiffKeysReal);
    ConfigFile.GetInt(*SectionName, TEXT("diff_guitarghl"), SongInfo.DiffGuitarghl);
    ConfigFile.GetInt(*SectionName, TEXT("diff_bassghl"), SongInfo.DiffBassghl);
	ConfigFile.GetInt(*SectionName, TEXT("diff_rhythm_ghl"), SongInfo.DiffRhythmghl);
    ConfigFile.GetInt(*SectionName, TEXT("preview_start_time"), SongInfo.PreviewStartTime);
    ConfigFile.GetInt(*SectionName, TEXT("diff_vocals"), SongInfo.DiffVocals);
	ConfigFile.GetInt(*SectionName, TEXT("diff_vocals_harm"), SongInfo.DiffVocalsHarm);
	ConfigFile.GetInt(*SectionName, TEXT("multiplier_note"), SongInfo.MultiplierNote);
    ConfigFile.GetInt(*SectionName, TEXT("album_track"), SongInfo.AlbumTrack);
    ConfigFile.GetInt(*SectionName, TEXT("video_start_time"), SongInfo.VideoStartTime);
    ConfigFile.GetInt(*SectionName, TEXT("delay"), SongInfo.Delay);

    return SongInfo;
}




// midi parser
vector<NoteEvent> ParseMidiFile(const string& filePath, int trackNumber) {
    MidiFile midifile;
    midifile.read(filePath);

    // Check if track number is out of range
    if (trackNumber >= midifile.getTrackCount()) {
        cerr << "Track number is out of range." << endl;
        return {};
    }

    vector<NoteEvent> notes;
    map<int, NoteEvent> noteStartEvents; // dictionary of note start events

    midifile.joinTracks(); // combining tracks
    midifile.doTimeAnalysis(); 
    midifile.linkNotePairs(); 

    for (int event = 0; event < midifile[trackNumber].size(); event++) {
        if (!midifile[trackNumber][event].isNoteOn() && !midifile[trackNumber][event].isNoteOff()) continue;

        int noteNumber = midifile[trackNumber][event][1];
        int velocity = midifile[trackNumber][event][2];
        
        if (midifile[trackNumber][event].isNoteOn()) {
            NoteEvent noteEvent = {noteNumber, velocity, midifile[trackNumber][event].seconds, 0};
            noteStartEvents[noteNumber] = noteEvent;
        } else if (midifile[trackNumber][event].isNoteOff()) {
            if (noteStartEvents.find(noteNumber) != noteStartEvents.end()) {
                noteStartEvents[noteNumber].endTime = midifile[trackNumber][event].seconds;
                notes.push_back(noteStartEvents[noteNumber]);
                noteStartEvents.erase(noteNumber); // remove note from dictionary
            }
        }
    }

    return notes;
}
