// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SystemUtils.generated.h"

/**
 * 
 */
UCLASS()
class IMAGEMESH_API USystemUtils : public UObject
{
	
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UWorld* GetCurrentWorld();
};
