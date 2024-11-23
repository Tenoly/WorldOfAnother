// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WorldGeneration/Private/WorldGenerator.h"
#include "WorldGeneration/Private/WorldGenInterface.h"
#include "WoAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WORLDOFANOTHER_API UWoAGameInstance : public UGameInstance, public IWorldGenInterface {
	GENERATED_BODY()

public:
	virtual UWorldGenerator* GetWorldGenerator() const override;
	
protected:
	virtual void OnStart() override;
	UPROPERTY() TObjectPtr<UWorldGenerator> WorldGenerator;

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UWorldGenerator> WorldGeneratorClass;
};
