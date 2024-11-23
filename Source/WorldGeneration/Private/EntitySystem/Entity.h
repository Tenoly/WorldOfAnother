// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityTemplate.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Entity.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class WORLDGENERATION_API UEntity : public UObject {
	GENERATED_BODY()
	
public:
	UFUNCTION() void Init(UEntityTemplate* temp, UEntity* parent, const FRandomStream& rnd);
	UFUNCTION() void CreateName(const FRandomStream& rnd);
	UFUNCTION() bool IsRoot() const;
	UFUNCTION() bool IsIndividual() const;
	UFUNCTION() TArray<UEntity*> GetAllIndividuals();
	UFUNCTION() int32 GetSize() const;
	
	UPROPERTY(BlueprintReadOnly/*Category="Info"*/) FString Name;
	UPROPERTY(/*Category="Info"*/) TObjectPtr<UEntityTemplate> Template;
	UPROPERTY(/*Category="Info"*/) FGameplayTagContainer Tags;

	UPROPERTY(/*Category="LinkedEntities"*/) TObjectPtr<UEntity> Parent;
	UPROPERTY(/*Category="LinkedEntities"*/) TObjectPtr<UEntity> Leader;
	UPROPERTY(/*Category="LinkedEntities"*/) TArray<UEntity*> SubEntities;
};
