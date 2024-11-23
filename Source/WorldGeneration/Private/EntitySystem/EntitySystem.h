// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityTemplate.h"
#include "WorldGenerator.h"
#include "UObject/Object.h"
#include "EntitySystem.generated.h"

class UEntity;
/**
 * 
 */
UCLASS(Blueprintable)
class WORLDGENERATION_API UEntitySystem : public UObject {
	GENERATED_BODY()

public:
	void Init(const FWorldConfig& config);
	TArray<UEntity*> GetRootEntities() const;

protected:
	UPROPERTY(EditDefaultsOnly) UEntityTemplate* RootTemplate;
	TArray<UEntity*> RootEntities;	
};
