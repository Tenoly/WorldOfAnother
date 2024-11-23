// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldGenerator.generated.h"

class UEntitySystem;
class UWorldGenerator;
class UVoronoiWrapper;
class UvwCell;

USTRUCT()
struct FWorldConfig {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) int32 RandomSeed = 1000;
	UPROPERTY(EditAnywhere) int32 NumberOfKingdoms = 3;
};

UCLASS()
class UWorldRegion : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY() TArray<UvwCell*> Cells;
	
	bool Contains(UWorldRegion* region) const;
};

UENUM()
enum ERegionLayer {
	Cell,
	Biome,
	Territory
};

USTRUCT()
struct FRegionLayer {
	GENERATED_BODY()
	
	UPROPERTY() TArray<UWorldRegion*> Regions;
};

UENUM()
enum EBiomeSize {
	Small,
	Medium,
	Large
};

UCLASS(Blueprintable)
class WORLDGENERATION_API UWorldGenerator : public UObject {
	GENERATED_BODY()

public:
	UWorldGenerator();
	void GenerateWorld();
	
	UPROPERTY() FWorldConfig Config;
	UPROPERTY() FRandomStream Rnd;
	UPROPERTY() TMap<TEnumAsByte<ERegionLayer>, FRegionLayer> RegionLayers;
	
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UVoronoiWrapper> VoronoiDiagram;
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UEntitySystem> EntitySystem;
	

protected:
	void CreateRegions();
	void ExpandRegion(UWorldRegion* region);

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UEntitySystem> EntitySystemClass;
};
