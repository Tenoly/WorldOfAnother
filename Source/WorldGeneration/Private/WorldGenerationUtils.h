// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoronoiWrapper.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldGenerationUtils.generated.h"

class UWorldGenerator;
/**
 * 
 */
UCLASS()
class WORLDGENERATION_API UWorldGenerationUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure) static UWorldGenerator* GetWorldGenerator(const UObject* worldContextObject);  
	UFUNCTION(BlueprintPure) static int32 GetSeed(const UObject* worldContextObject);
	UFUNCTION(BlueprintPure) static TArray<UvwCell*> GetScaledCells(const UObject* worldContextObject, FVector2D startPos, FVector2D endSize);
};
