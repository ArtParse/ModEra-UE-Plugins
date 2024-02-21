// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemUtils.h"

#include "Kismet/GameplayStatics.h"


UWorld* USystemUtils::GetCurrentWorld()
{
	UWorld* World = nullptr;
	
	World = GEngine->GameViewport->GetWorld();

	if (World) {return World;} // 1st method
	
	
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(GEngine->GetWorldContexts()[0].World(), 0)) // second method
		{
		World = Controller->GetWorld();
		}
	else if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(GEngine->GetWorldContexts()[0].World(), 0))
	{
		World = Pawn->GetWorld();
	}

	return World;
}
