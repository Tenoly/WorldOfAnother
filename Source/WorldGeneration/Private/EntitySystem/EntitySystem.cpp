// Fill out your copyright notice in the Description page of Project Settings.


#include "EntitySystem.h"

#include "Entity.h"

void UEntitySystem::Init(const FWorldConfig& config) {
	for (int i = 0; i < config.NumberOfKingdoms; ++i) {
		UEntity* entity = NewObject<UEntity>(this);
		RootEntities.Add(entity);
		entity->Init(RootTemplate, nullptr, FRandomStream(config.RandomSeed));
	}
}

TArray<UEntity*> UEntitySystem::GetRootEntities() const { return RootEntities; }
