#include "VoronoiWrapper.h"

#include "Kismet/KismetMathLibrary.h"
#include "Voronoi/Voronoi.h"

/**
 * Revert an edge from end to start, in place.
 */
void FvwEdge::Flip() {
	TArray<FVector2D> points;
	for (FVector2D p : Points) {
		points.Insert(p, 0);
	}

	Points = points;
}

/**
 * Creates and returns an array of seeds.
 * @return The array of seeds.
 */
TArray<FvwSeed> UVoronoiWrapper::CreateSeeds() {
	TArray<FvwSeed> seeds;
	for (int j = 0; j < NumberOfSeeds.Y; ++j) {
		for (int i = 0; i < NumberOfSeeds.X; ++i) {
			FvwSeed seed;
			seed.Index = j * NumberOfSeeds.X + i;
			seed.Point = FVector2D((i+RandomStream.FRand()*SeedRandomness)*SubdivisionSize.X, (j+RandomStream.FRand()*SeedRandomness)*SubdivisionSize.Y);

			seeds.Add(seed);
		}
	}
	return seeds;
}

/**
 * Creates and returns an array of sites usable by Unreal Voronoi. Also creates cells to be filled later.
 * @param seeds The list of seeds used to create sites and cells.
 * @return The array of sites.
 */
TArray<FVector> UVoronoiWrapper::CreateSitesAndCells(TArray<FvwSeed> seeds) {
	TArray<FVector> sites;
	for (FvwSeed seed : seeds) {
		sites.Add(FVector(seed.Point, 0.f));
		UvwCell* cell = NewObject<UvwCell>();
		cell->Seed = seed;
		Cells.Add(cell);
	}
	return sites;
}

/**
 * Uses Unreal 3D Voronoi implementation to get edges, then trim it to only keep single occurence of edges of a 2D Voronoi.
 * @param sites An array of sites to feed to the Unreal Voronoi computation.
 * @return A map of edges (under very simple tuple form) pointing to an array of adjacent cells.
 */
TMap<TTuple<FVector, FVector>, TArray<int32>> UVoronoiWrapper::GetEdges(TArray<FVector> sites) {
	// Unreal 3D Voronoi
	FBox bounds = FBox(FVector4(), FVector4(NumberOfSeeds.X*SubdivisionSize.X, NumberOfSeeds.Y*SubdivisionSize.Y, 10.f, 1.f));
	FVoronoiDiagram voronoiDiagram;
	voronoiDiagram.Initialize(sites, bounds, 0.f);

	TArray<TTuple<FVector, FVector>> edges;
	TArray<int32> edgeInfos;
	voronoiDiagram.ComputeCellEdges(edges, edgeInfos);

	// Keep only 2D, unique edges, and add info on both adjacent cells
	TMap<TTuple<FVector, FVector>, TArray<int32>> uniqueEdges;
	for (int i = 0; i < edges.Num(); ++i) {
		// Remove all edges not contained in the 2D plan
		TTuple<FVector, FVector> edge = edges[i];
		if (edge.Key.Z != 0.f || edge.Value.Z != 0.f) continue;

		// Remove duplicates, but keep adjacent cell info 
		bool alreadyStored = false;
		TArray<TTuple<FVector, FVector>> keys;
		uniqueEdges.GetKeys(keys);
		for (TTuple<FVector, FVector> e : keys) {
			if (e.Key.Equals(edge.Key) && e.Value.Equals(edge.Value)) {
				alreadyStored = true;
				uniqueEdges.Find(e)->AddUnique(edgeInfos[i]);
				continue;
			}

			if (e.Key.Equals(edge.Value) && e.Value.Equals(edge.Key)) {
				alreadyStored = true;
				uniqueEdges.Find(e)->AddUnique(edgeInfos[i]);
			}
		}
		if (alreadyStored) continue;
		
		uniqueEdges.Add(edge, TArray<int32>());
		uniqueEdges.Find(edge)->Add(edgeInfos[i]);
	}

	// Add adjacent cell info to border edges (they only have one adjacent cell)
	TArray<TTuple<FVector, FVector>> keys;
	uniqueEdges.GetKeys(keys);
	for (TTuple<FVector, FVector> edge : keys) {
		TArray<int32>* info = uniqueEdges.Find(edge);
		if (info->Num() < 2)
			info->AddUnique(-1);
	}

	return uniqueEdges;
}

/**
 * Cells should have being initialized with CreateSitesAndCells() before calling this. Fill cells with edges infos.
 * @param seeds 
 * @param edges 
 */
void UVoronoiWrapper::FillCells(TArray<FvwSeed> seeds, TMap<TTuple<FVector, FVector>, TArray<int32>> edges) {
	TArray<TTuple<FVector, FVector>> keys;
	edges.GetKeys(keys);
	
	for (FvwSeed seed : seeds) {
		UvwCell* cell = Cells[seed.Index];
		TArray<FvwEdge> cellEdges;
		TArray<int32> cellNeighbours;
		for (TTuple<FVector, FVector> edge : keys) {
			TArray<int32> info = edges.FindRef(edge);
			int id = info.Find(seed.Index);
			if (id != INDEX_NONE) {
				cellEdges.Add(FvwEdge(edge));
				cellNeighbours.Add(info[1-id]);
			}
		}

		cell->Edges.Add(cellEdges[0]);
		cellEdges.RemoveAt(0);
		cell->Neighbours.Add(cellNeighbours[0]);
		cellNeighbours.RemoveAt(0);
		
		while (!cellEdges.IsEmpty()) {
			int32 nextId = -1;
			for (int i = 0; i < cellEdges.Num(); ++i) {
				if (cellEdges[i].Points[0].Equals(cell->Edges.Last().Points.Last())) {
					nextId = i;
					break;
				}
				if (cellEdges[i].Points.Last().Equals(cell->Edges.Last().Points.Last())) {
					cellEdges[i].Flip();					
					nextId = i;
					break;
				}
			}
			cell->Edges.Add(cellEdges[nextId]);
			cellEdges.RemoveAt(nextId);
			cell->Neighbours.Add(cellNeighbours[nextId]);
			cellNeighbours.RemoveAt(nextId);
		}
	}
}

void UVoronoiWrapper::AugmentEdges(UvwCell* cell, float segmentSize, float range) {
	for (int i = 0; i < cell->Edges.Num(); ++i) {
		FvwEdge& edge = cell->Edges[i];
		if (edge.Augmented) continue;

		const FVector2D start = edge.Points[0];
		const FVector2D end = edge.Points[1];
		FVector2D dir = end-start;
		dir.Normalize();
		FVector2D normal(dir.Y, dir.X);
		const float length = FVector2D::Distance(start, end);
		const int32 count = length/segmentSize;
		const float dist = length/(count+1);

		if (cell->Neighbours[i] == -1) {
			edge.Augmented = true;
			continue;
		}
		
		UvwCell* neighbour = Cells[cell->Neighbours[i]];
		FvwEdge& nEdge = neighbour->Edges[0];
		for (int k = 0; k < neighbour->Edges.Num(); ++k) {
			if (neighbour->Neighbours[k] == cell->GetIndex()) {
				nEdge = neighbour->Edges[k];
				break;
			}
		}
		
		for (int j = 0; j < count; ++j) {
			FVector2D newPoint = start + dir*(dist*(j+1));
			newPoint += RandomStream.FRandRange(-range, range)*normal;
			edge.Points.Insert(newPoint, j+1);
			int32 nIndex = (nEdge.Points[0] == edge.Points[0]) ? j+1 : 1;
			nEdge.Points.Insert(newPoint, nIndex);
		}

		edge.Augmented = true;
		nEdge.Augmented = true;
	}
}

/**
 * Create heights seeds and init the height of all cells
 */
void UVoronoiWrapper::InitHeight() {
	TMap<int32, float> heightSeed;
	for (int i = 0; i < Cells.Num()/10; ++i)
		heightSeed.Add(UKismetMathLibrary::RandomIntegerFromStream(RandomStream, Cells.Num()), RandomStream.FRandRange(HeightRange.X, HeightRange.Y));

	for (UvwCell* cell : Cells) {
		TArray<int32> keys;
		heightSeed.GetKeys(keys);

		float distSum = 0.f;
		float totalSum = 0.f;
		for (const int32 i : keys) {
			float dist = (cell->Seed.Point - Cells[i]->Seed.Point).Length();
			if (dist == 0.f)
				dist = 1.f;
			totalSum += heightSeed[i] * 1.f/dist;
			distSum += 1.f/dist;
		}

		cell->Height = totalSum/distSum;
	}
}

ECellType UvwCell::GetType() const {
	if (Height < 0.f)
		return Water;

	for (int32 neighbour : Neighbours) {
		const UvwCell* cell = VoronoiWrapper->Cells[neighbour];
		if (cell->Height < 0.f)
			return Coast;
	}

	return Land;
}

void UVoronoiWrapper::Generate(FRandomStream rand) {
	RandomStream = rand;
	TArray<FvwSeed> seeds = CreateSeeds();
	TArray<FVector> sites = CreateSitesAndCells(seeds);
	TMap<TTuple<FVector, FVector>, TArray<int32>> edges = GetEdges(sites);
	FillCells(seeds, edges);
	//for (FvwCell& cell : Cells)
	//	AugmentEdges(cell, 50, 10);
	InitHeight();
	
	if (ShowDebug)
		DrawDebug();
}

/**
 * Display the result of the generation
 */
void UVoronoiWrapper::DrawDebug() {
	for (UvwCell* cell : Cells) {
		DrawDebugString(GetWorld(), FVector(cell->GetCenter(), cell->Height+1.f), FString::FromInt(cell->GetIndex()));
		TArray<FVector> vertices;
		for (FvwEdge edge : cell->Edges) {
			for (FVector2D p : edge.Points) {
				vertices.AddUnique(FVector(p, cell->Height));
			}
			if (ShowEdges)
				DrawDebugLine(GetWorld(), FVector(edge.Points[0], 0.f), FVector(edge.Points.Last(), 0.f), FColor::Magenta, true);
		}

		TArray<int32> triangles;
		TArray<int32> v;
		for (int32 n = 0; n < vertices.Num(); ++n) v.Add(n);
		int32 last = 0;
		while (v.Num() > 3) {
			last = last%v.Num();
			int32 n0 = v[last];
			int32 n1 = v[(last+1)%v.Num()];
			int32 n2 = v[(last+2)%v.Num()];
			
			triangles.Add(n0);
			triangles.Add(n1);
			triangles.Add(n2);

			v.Remove(n1);
			last++;
		}
		triangles.Append(v);

		FColor color = (cell->Height < 0) ? FColor::Blue : (cell->Height > 20.f) ? FColor(45, 45, 45) : FColor::Green;
		DrawDebugMesh(GetWorld(), vertices, triangles, color, true, 100000);
	}
}
