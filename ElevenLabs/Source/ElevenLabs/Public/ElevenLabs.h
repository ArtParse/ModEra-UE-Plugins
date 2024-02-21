// Copyright by Mod Era 2023, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Http.h"
#include "ElevenLabs.generated.h"


class FElevenLabsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnElevenLabsResponseReceived, bool, bWasSuccessful, const FString&, FilePath);

UCLASS(BlueprintType, Blueprintable)
class ELEVENLABS_API UMyElevenLabsTTS : public UObject
{
    GENERATED_BODY()

public:
    UMyElevenLabsTTS();

    UPROPERTY(BlueprintAssignable, Category = "ElevenLabs TTS")
    FOnElevenLabsResponseReceived OnElevenLabsResponseReceivedEvent;

    /**
     * Sends text-to-speech to Eleven Labs API.
     *
     * @param VoiceID identifier of the voice
     * @param Language specified language
     * @param Text input text
     * @param Stability stability factor
     * @param SimilarityBoost boost factor for similarity
     */
    UFUNCTION(BlueprintCallable, Category = "ElevenLabs TTS", meta = (DisplayName = "Send TTS to Eleven Labs API"))
        void SendTTSToElevenLabsAPI(const FString& VoiceID, const FString& Language, const FString& Text, float Stability, float SimilarityBoost);

    /**
     * Create a new instance of UMyElevenLabsTTS.
     *
     * @return pointer to the new instance
     */    
    UFUNCTION(BlueprintCallable, Category = "ElevenLabs TTS", meta = (DisplayName = "Create MyElevenLabsTTS Instance"))
        static UMyElevenLabsTTS* CreateMyElevenLabsTTSInstance();
    
    /**
     * Sets the API key to the provided value.
     *
     * @param NewAPIKey The new API key to set
     *
     * @return void
     *
     * @throws ErrorType Description of any potential errors
     */
    UFUNCTION(BlueprintCallable, Category = "ElevenLabs TTS", meta = (DisplayName = "Set API Key"))
        void SetAPIKey(const FString& NewAPIKey);

private:

    FString SearchLanguage(const FString& Language) const;
    
    void HandleElevenLabsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    // Set the API key to the provided value
    FString APIKey;
};