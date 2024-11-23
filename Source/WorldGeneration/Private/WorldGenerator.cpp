// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "VoronoiWrapper.h"
#include "EntitySystem/EntitySystem.h"

bool UWorldRegion::Contains(UWorldRegion* region) const {
	if (region == this) return true;

	for (const UvwCell* cell : region->Cells) {
		if (!Cells.Contains(cell))
			return false;
	}

	return true;
}

UWorldGenerator::UWorldGenerator() {
	Config.RandomSeed = 10000000 + FMath::RandRange(0, 89999999);
}

void UWorldGenerator::GenerateWorld() {
	Rnd = FRandomStream(Config.RandomSeed);
	VoronoiDiagram = NewObject<UVoronoiWrapper>(this);
	VoronoiDiagram->Generate(Rnd);

	EntitySystem = NewObject<UEntitySystem>(this, EntitySystemClass);
	EntitySystem->Init(Config);

	CreateRegions();
}

void UWorldGenerator::CreateRegions() {
	// Cell regions
	for (UvwCell* cell : VoronoiDiagram->Cells) {
		FRegionLayer* cellLayer = RegionLayers.Find(Cell);
		if (!cellLayer) {
			FRegionLayer newLayer;
			RegionLayers.Add(Cell, newLayer);
			cellLayer = &newLayer;
		}
		
		UWorldRegion* region = NewObject<UWorldRegion>();
		region->Cells.Add(cell);
		cellLayer->Regions.Add(region);
	}
	
	// Biome regions
	
}

