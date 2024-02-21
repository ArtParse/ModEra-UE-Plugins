
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SongDataTypes.h"

#include "TrackReactor.generated.h"


UCLASS()
class SONGPARSER_API ATrackReactor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrackReactor();


	
	/**
	 * ATrackReactor::OnNotePrepare is a function that handles the preparation of a note.
	 *
	 * @param Note The note event to be prepared
	 * @param LeadTime The lead time for the note preparation
	 *
	 * @return None
	 *
	 * @throws None
	 */
    UFUNCTION(BlueprintNativeEvent, Category = "RhythmGame")
    void OnNotePrepare(FNoteEvent Note, float LeadTime);
	void OnNotePrepare_Implementation(FNoteEvent Note, float LeadTime);
	
    UFUNCTION(BlueprintNativeEvent, Category = "RhythmGame")
    void OnNotePlay(FNoteEvent Note);
	void OnNotePlay_Implementation(FNoteEvent Note);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
