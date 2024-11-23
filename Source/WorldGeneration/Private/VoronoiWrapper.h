// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoronoiWrapper.generated.h"

#pragma region DataStruct

USTRUCT()
struct FvwSeed {
	GENERATED_BODY()

	int32 Index;
	FVector2D Point;
};

USTRUCT()
struct FvwEdge {
	GENERATED_BODY()

	FvwEdge() {};
	FvwEdge(const TTuple<FVector, FVector>& edge) {
		Points.Add(FVector2D(edge.Key.X, edge.Key.Y));
		Points.Add(FVector2D(edge.Value.X, edge.Value.Y));
	}
	
	void Flip();
	
	TArray<FVector2D> Points;
	bool Augmented = false;
};

UENUM()
enum ECellType {
	Water,
	Coast,
	Land
};

class UVoronoiWrapper;
UCLASS(BlueprintType)
class UvwCell : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY() UVoronoiWrapper* VoronoiWrapper;
	UPROPERTY() FvwSeed Seed;
	UPROPERTY() TArray<FvwEdge> Edges;
	UPROPERTY() TArray<int32> Neighbours;
	UPROPERTY() float Height = 0.f;
	
	const FVector2D& GetCenter() const { return Seed.Point; }
	const int32& GetIndex() const { return Seed.Index; }
	ECellType GetType() const;
};

#pragma endregion DataStruct

/**
 * 
 */
UCLASS(BlueprintType)
class UVoronoiWrapper : public UObject {
	GENERATED_BODY()

public:
	void Generate(FRandomStream rand);
	void DrawDebug();
	
	UPROPERTY(EditAnywhere, Category="Generation") FVector2D SubdivisionSize = FVector2D(100.f, 100.f);
	UPROPERTY(EditAnywhere, Category="Generation") FIntPoint NumberOfSeeds = FIntPoint(30, 15);
	UPROPERTY(EditAnywhere, Category="Generation") FVector2D HeightRange = FVector2D(-40.f, 50.f);
	UPROPERTY(EditAnywhere, Category="Generation") float SeedRandomness = 1.f;

	UPROPERTY(EditAnywhere, Category="Debug") bool ShowDebug = true;
	UPROPERTY(EditAnywhere, Category="Debug", meta=(EditCondition="ShowDebug")) bool ShowEdges;
	
	UPROPERTY() FRandomStream RandomStream;

	UPROPERTY(BlueprintReadOnly) TArray<UvwCell*> Cells;

private:
	TArray<FvwSeed> CreateSeeds();
	TArray<FVector> CreateSitesAndCells(TArray<FvwSeed> seeds);
	TMap<TTuple<FVector, FVector>, TArray<int32>> GetEdges(TArray<FVector> sites);
	void AugmentEdges(UvwCell* cell, float segmentSize, float range);
	void FillCells(TArray<FvwSeed> seeds, TMap<TTuple<FVector, FVector>, TArray<int32>> edges);
	void InitHeight();
};
