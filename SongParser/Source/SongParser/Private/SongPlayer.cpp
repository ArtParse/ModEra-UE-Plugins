#include "SongPlayer.h"

#include "MidiParser.h"
#include "RhythmSongParser.h"
#include "RuntimeAudioImporterLibrary.h"
#include "Actors/TrackReactor.h"

ASongPlayer* ASongPlayer::Instance = nullptr;


// Sets default values
ASongPlayer::ASongPlayer()
{
	//World = GWorld;

	// Создание AudioComponent и привязка его к корню
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
}

ASongPlayer* ASongPlayer::GetSongPlayer()
{
	if (!Instance)
	{
		Instance = NewObject<ASongPlayer>();
		Instance->AddToRoot(); // Предотвращает уничтожение сборщиком мусора
	}
	return Instance;
}


 void ASongPlayer::SetSongPlayer(ASongPlayer* NewSongPlayer)
{
Instance = NewSongPlayer;
}

ASongPlayer::ASongPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (Instance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Попытка создать второй экземпляр ASongPlayer"));
	}
	else
	{
		Instance = this;
	}
}

void ASongPlayer::SubscribeToTrackEvents(ESongTrackType TrackType, ATrackReactor* Subscriber)
{
	for (FSongTrack& Track : Tracks)
	{
		if (Track.TrackType == TrackType)
		{
			// Предполагаем, что у AActor есть методы OnNotePrepare и OnNotePlay
			Track.OnNotePrepare.AddDynamic(Subscriber, &ATrackReactor::OnNotePrepare);
			Track.OnNotePlay.AddDynamic(Subscriber, &ATrackReactor::OnNotePlay);
			break; // Прекратить цикл, если нашли нужный трек
		}
	}
}

void ASongPlayer::AddTrack(ESongTrackType TrackType, int32 Difficulty)
{
	if (!USongTrackHelper::IsValidSongInfo(CurrentSong)) {
		UE_LOG(LogSongPlayer, Error, TEXT("Invalid song info for path"));
		return ;
	}
	FString MidiFilePath = FPaths::Combine( FPaths::GetPath(CurrentSong.FilePath),TEXT("notes.mid"));

	if (!FPaths::FileExists(MidiFilePath)) {
		UE_LOG(LogSongPlayer, Error, TEXT("Midi file not found: %s"), *MidiFilePath);
		return;
	}

	FSongTrack NewTrack;
	NewTrack.TrackType = TrackType;
	NewTrack.Delay = SpawnDelay;

	NewTrack.Notes = UMidiParser::ParseMidiTrackByType(MidiFilePath, TrackType, Difficulty);
	Tracks.Add(NewTrack);
}

void ASongPlayer::RemoveTrack(ESongTrackType TrackType)
{
}


void ASongPlayer::PlaySong()
{
	AudioComponent->Play(PlayTime);
	SetIsPlaying(true);
}

void ASongPlayer::PauseSong()
{
	AudioComponent->Stop();
	SetIsPlaying(false);
}

//clear all tracks positions, to start parsing notes from beginning
void ASongPlayer::ClearTracksPositions()
{

	for (FSongTrack& Track : Tracks)
	{
		Track.CurrentIndex = 0;
	}
}


// Called every frame
void ASongPlayer::Tick(float DeltaTime)
{
    //Update play time
	if (bIsPlaying) {
		PlayTime += DeltaTime;
		GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(PlayTime));

		//trigger all notes events
		CheckTracksForNotes();
	};
	
}

bool ASongPlayer::LoadSong(FString FilePath)
{
	
	CurrentSong = USongParser::ParseSongInfoFromIni(FilePath);

	if (!USongTrackHelper::IsValidSongInfo(CurrentSong)) {
	UE_LOG(LogSongPlayer, Error, TEXT("Invalid song info for path: %s"), *FilePath);
		return false;
	}
	

//TODO change path to ogg file
	FString MusicFilePath = FPaths::Combine(FPaths::GetPath(FilePath), TEXT("song.ogg"));
	LoadSongSound(FilePath);


	return true;
}

bool ASongPlayer::LoadSongSound(FString FilePath)
{
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogSongPlayer, Error, TEXT("Music File not found: %s"), *FilePath);
		return false;
	}

	// Создаем экземпляр импортера аудио
	URuntimeAudioImporterLibrary* RuntimeAudioImporter = NewObject<URuntimeAudioImporterLibrary>(this);
	if (!IsValid(RuntimeAudioImporter))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create audio importer"));
		return false;
	}

	// Устанавливаем коллбеки для отслеживания прогресса и результата импорта
	RuntimeAudioImporter->OnProgress.AddDynamic(this, &ASongPlayer::OnAudioImportProgress);
	RuntimeAudioImporter->OnResult.AddDynamic(this, &ASongPlayer::OnAudioImportFinished);

	// Начинаем импорт аудио из файла
	RuntimeAudioImporter->ImportAudioFromFile(FilePath, ERuntimeAudioFormat::Auto);
	return true; // Возвращаем true, так как процесс импорта начат
}

// Обработчик прогресса импорта аудио
void ASongPlayer::OnAudioImportProgress(int32 Percentage)
{
	UE_LOG(LogTemp, Log, TEXT("Audio importing percentage: %d%%"), Percentage);
}

// Обработчик завершения импорта аудио
void ASongPlayer::OnAudioImportFinished(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status)
{
	if (Status == ERuntimeImportStatus::SuccessfulImport && ImportedSoundWave)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully imported audio: %s"), *ImportedSoundWave->GetName());
		// Проигрываем импортированный аудиофайл
		if (AudioComponent)
		{
			AudioComponent->SetSound(ImportedSoundWave);
			AudioComponent->Play(0.0f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to import audio"));
	}
}


void ASongPlayer::CheckTracksForNotes()
{
	if (!bIsPlaying || Tracks.Num() == 0) return;
	
	for (FSongTrack& Track : Tracks)
	{
		// Обработка текущих нот
		ProcessNotes(Track, PlayTime, 0.0f);

		// Если в треке задана задержка, обрабатываем будущие ноты с учётом этой задержки
		if (Track.Delay > 0.0f)
		{
			ProcessNotes(Track, PlayTime, Track.Delay);
		}
	}
}



void ASongPlayer::ProcessNotes(FSongTrack& Track, float CurrentTime, float Delay)
{
	if(!World)
	{
		World = GWorld;
		UE_LOG(LogSongPlayer, Error, TEXT("ASongPlayer::ProcessNotes World is null"));
		return;
	}
	
	
	int32 Index = Track.CurrentIndex;
	float PreviousTime = CurrentTime - World->GetDeltaSeconds();

//	int32 TrackTypeValue = static_cast<int32>(Track.TrackType);
//	UE_LOG(LogSongPlayer, Log, TEXT("ASongPlayer::ProcessNotes Track type value: %d"), TrackTypeValue);
	
	if (Track.Notes.Num() == 0)
	{
		UE_LOG(LogSongPlayer, Error, TEXT("ASongPlayer::ProcessNotes Empty notes arraty")); 
		return; // Если массив нот пуст, выходим из функции
	}
	
	while (Index < Track.Notes.Num())
	{
		const FNoteEvent& Note = Track.Notes[Index];

		if (Note.StartTime <= CurrentTime + Delay && Note.StartTime > PreviousTime + Delay)
		{
			if (Delay == 0.0f)
			{
				// Воспроизведение текущих нот без задержки
				Track.OnNotePlay.Broadcast(Note);
			}
			else
			{
				// Подготовка будущих нот с задержкой
				Track.OnNotePrepare.Broadcast(Note, Delay);
			}
			Index++;
		}
		else if (Note.StartTime < PreviousTime + Delay) //update actual index
		{
			Index++;
		}
		else
		{
			// Если мы достигли ноты, время которой ещё не наступило, прерываем цикл
			if (Delay == 0.0f) Track.CurrentIndex = Index;
			break;

		}
	}
}


void ASongPlayer::SetPlayTime(float NewPlayTime)
{

	PlayTime = NewPlayTime;
	for (FSongTrack& Track : Tracks)
	{
		FindActualIndexForTrack(Track,PlayTime);
	}
}

int32 ASongPlayer::FindActualIndexForTrack(FSongTrack& Track, float CurrentTime)
{
	// Ищем индекс первой ноты, время начала которой больше или равно CurrentTime
	for (int32 i = 0; i < Track.Notes.Num(); ++i)
	{
		if (Track.Notes[i].StartTime >= CurrentTime)
		{
			return i; // Возвращаем индекс ноты, соответствующей текущему времени
		}
	}
	return 0; // Если все ноты уже должны были начаться, возвращаем 0
}
