// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElevenLabs.h"

#define LOCTEXT_NAMESPACE "FElevenLabsModule"

void FElevenLabsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FElevenLabsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FElevenLabsModule, ElevenLabs)

UMyElevenLabsTTS::UMyElevenLabsTTS()
{
    // Ensure the "ResponseAudio" folder exists
    FString ResponseAudioFolder = FPaths::ProjectSavedDir() / "ResponseAudio";
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*ResponseAudioFolder))
    {
        PlatformFile.CreateDirectory(*ResponseAudioFolder);
    }

}

FString UMyElevenLabsTTS::SearchLanguage(const FString& Language) const
{
    FString VoiceLanguage = TEXT("en");

    if (Language.Equals(TEXT("German"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("de");
    }
    else if (Language.Equals(TEXT("Polish"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("pl");
    }
    else if (Language.Equals(TEXT("Spanish"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("es");
    }
    else if (Language.Equals(TEXT("Italian"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("it");
    }
    else if (Language.Equals(TEXT("French"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("fr");
    }
    else if (Language.Equals(TEXT("Portuguese"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("pt");
    }
    else if (Language.Equals(TEXT("Hindi"), ESearchCase::IgnoreCase))
    {
        VoiceLanguage = TEXT("hi");
    }

    return VoiceLanguage;
}


void UMyElevenLabsTTS::SendTTSToElevenLabsAPI(const FString& VoiceID, const FString& Language, const FString& Text, float Stability, float SimilarityBoost)
{
    FString APIEndpoint = FString::Printf(TEXT("https://api.elevenlabs.io/v1/text-to-speech/%s"), *VoiceID);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb("POST");
    HttpRequest->SetURL(APIEndpoint);
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("accept", "audio/mpeg");
    HttpRequest->SetHeader("xi-api-key", APIKey);

    // Construct the request body
    TSharedPtr<FJsonObject> RequestData = MakeShareable(new FJsonObject);
    RequestData->SetStringField("text", Text);
    //RequestData->SetStringField("voice_id", VoiceID);

    // Search for the language code based on the input language
    FString VoiceLanguage = SearchLanguage(Language);

    // Set model and language based on input
    if (VoiceLanguage.Equals(TEXT("en")))
    {
        RequestData->SetStringField("model_id", "eleven_monolingual_v1");
    }
    else
    {
        RequestData->SetStringField("model_id", "eleven_multilingual_v1");
        RequestData->SetStringField("language", VoiceLanguage);
    }

    TSharedPtr<FJsonObject> VoiceSettingsData = MakeShareable(new FJsonObject);
    VoiceSettingsData->SetNumberField("stability", Stability);
    VoiceSettingsData->SetNumberField("similarity_boost", SimilarityBoost);
    RequestData->SetObjectField("voice_settings", VoiceSettingsData);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestData.ToSharedRef(), Writer);
    HttpRequest->SetContentAsString(RequestBody);

    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UMyElevenLabsTTS::HandleElevenLabsResponse);
    HttpRequest->ProcessRequest();
}


void UMyElevenLabsTTS::HandleElevenLabsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {

    FString ResultPath;

    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get a valid response from ElevenLabs TTS API"));
        OnElevenLabsResponseReceivedEvent.Broadcast(bWasSuccessful, ResultPath);
        return;
    }

    if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // Create a unique file name using the current date and time
        FDateTime CurrentDateTime = FDateTime::Now();
        FString UniqueFileName = FString::Printf(TEXT("TTS_%s_%d.mp3"), *CurrentDateTime.ToString(TEXT("%Y%m%d_%H%M%S")), rand()); // for mp3

           // Save the received MP3 data to a file with the unique name
        FString SavePath = FPaths::ProjectSavedDir() / "ResponseAudio" / UniqueFileName;
        if (FFileHelper::SaveArrayToFile(Response->GetContent(), *SavePath))
        {
            UE_LOG(LogTemp, Log, TEXT("TTS audio data saved to: %s"), *SavePath);
            ResultPath = SavePath;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save TTS audio data to: %s"), *SavePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ElevenLabs TTS API returned an error: %s"), *Response->GetContentAsString());
    }

    // Call the event delegate
    OnElevenLabsResponseReceivedEvent.Broadcast(bWasSuccessful, ResultPath);
}

UMyElevenLabsTTS* UMyElevenLabsTTS::CreateMyElevenLabsTTSInstance()
{
    return NewObject<UMyElevenLabsTTS>();
}

void UMyElevenLabsTTS::SetAPIKey(const FString& NewAPIKey)
{
    APIKey = NewAPIKey;
}

