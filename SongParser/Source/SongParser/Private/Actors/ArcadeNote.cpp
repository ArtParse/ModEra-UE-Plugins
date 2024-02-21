
#include "Actors/ArcadeNote.h"


void AArcadeNote::SetNoteParameters(float NewSpeed, float NewLifetime)
{
	Speed=NewSpeed;
	Lifetime=NewLifetime;
}

// Sets default values
AArcadeNote::AArcadeNote()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent; // Set MeshComponent as the root component
}


// Called when the game starts or when spawned
void AArcadeNote::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArcadeNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Move actor forward in local space
	AddActorLocalOffset(FVector(DeltaTime * Speed, 0, 0));

	CurrentLifetime += DeltaTime;
	if (CurrentLifetime >= Lifetime)
	{
		Destroy();
	}
}