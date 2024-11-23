// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EntityTemplate.generated.h"

/**
 * 
 */
UCLASS()
class WORLDGENERATION_API UEntityTemplate : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="General") TArray<FString> NameBases;
	UPROPERTY(EditDefaultsOnly, Category="Creation") bool UsedInCreation = false;
	UPROPERTY(EditDefaultsOnly, Category="Sub Entities") TMap<UEntityTemplate*, FIntPoint> SubEntities;  
};
