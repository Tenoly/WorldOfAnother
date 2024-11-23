// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerationUtils.h"

#include "WorldGenerator.h"
#include "WorldGenInterface.h"

UWorldGenerator* UWorldGenerationUtils::GetWorldGenerator(const UObject* worldContextObject) {
	if (!worldContextObject) return nullptr;
	
	UGameInstance* gameInstance = worldContextObject->GetWorld()->GetGameInstance();
	if (gameInstance->Implements<UWorldGenInterface>())
		return Cast<IWorldGenInterface>(gameInstance)->GetWorldGenerator();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "GameInstance class should implement IWorldGenInterface");
	return nullptr;
}

int32 UWorldGenerationUtils::GetSeed(const UObject* worldContextObject) {
	return GetWorldGenerator(worldContextObject)->Config.RandomSeed;
}

TArray<UvwCell*> UWorldGenerationUtils::GetScaledCells(const UObject* worldContextObject, FVector2D startPos, FVector2D endSize) {
	TArray<UvwCell*> cells;
	UWorldGenerator* wg = GetWorldGenerator(worldContextObject);
	if (!wg) return cells;
	UVoronoiWrapper* vd = wg->VoronoiDiagram;

	for (UvwCell* cell : vd->Cells) {
		UvwCell* cCpy = NewObject<UvwCell>();
		FVector2D worldSize = FVector2D(vd->NumberOfSeeds.X * vd->SubdivisionSize.X, vd->NumberOfSeeds.Y * vd->SubdivisionSize.Y);
		for (const FvwEdge& edge : cell->Edges) {
			FvwEdge eCpy;
			for (FVector2D pt : edge.Points) {
				eCpy.Points.Add(startPos+pt*endSize/worldSize);
			}
			cCpy->Edges.Add(eCpy);
		}
		cCpy->Seed = FvwSeed(cell->GetIndex(), startPos+cell->GetCenter()*endSize/worldSize);
		cCpy->Height = cell->Height;
		cells.Add(cCpy);
	}
	
	return cells;
}
