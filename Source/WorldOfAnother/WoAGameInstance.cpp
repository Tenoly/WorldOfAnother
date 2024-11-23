// Fill out your copyright notice in the Description page of Project Settings.


#include "WoAGameInstance.h"

void UWoAGameInstance::OnStart() {
	Super::OnStart();
	WorldGenerator = NewObject<UWorldGenerator>(this, WorldGeneratorClass);
	WorldGenerator->GenerateWorld();

	DrawDebugLine(GetWorld(), FVector(0.f, 0.f, 0.f), FVector(500.f, 0.f, 0.f), FColor::Magenta, true);
}

UWorldGenerator* UWoAGameInstance::GetWorldGenerator() const {
	return WorldGenerator;
}