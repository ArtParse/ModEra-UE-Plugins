#pragma once
#include "SongDataTypes.h"

DEFINE_LOG_CATEGORY(LogSongPlayer);

FString USongTrackHelper::GetMidiTrackName(ESongTrackType TrackType)
{
	switch (TrackType)
	{
	case ESongTrackType::Drums:
		return TEXT("PART DRUMS");
	case ESongTrackType::Guitar:
		return TEXT("PART GUITAR");
	case ESongTrackType::Bass:
		return TEXT("PART BASS");
	case ESongTrackType::Keys:
		return TEXT("PART KEYS");
	case ESongTrackType::Vocals:
		return TEXT("PART VOCALS");
	default:
		return TEXT("Unknown");
	}
}


bool USongTrackHelper::IsValidSongInfo(const FSongInfo& SongInfo)
{
	bool bIsValid = !SongInfo.Name.IsEmpty() && !SongInfo.FilePath.IsEmpty() && SongInfo.SongLength > 0;
	
	return bIsValid;
}