// Fill out your copyright notice in the Description page of Project Settings.


#include "WoAHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "WorldGeneration/Private/WorldGenDebugMenu.h"
#include "WorldGeneration/Private/WorldGenerationUtils.h"

void AWoAHUD::DrawHUD() {
	Super::DrawHUD();

	APlayerController* pc = GetOwningPlayerController();
	
	for (UvwCell* cell : UWorldGenerationUtils::GetScaledCells(pc, FVector2D::Zero(), FVector2D(Canvas->SizeX, Canvas->SizeY))) {		
		TArray<FVector2D> vertices;
		FVector2D topLeft = cell->Edges[0].Points[0];
		FVector2D bottomRight = cell->Edges[0].Points[0];
		for (const FvwEdge& edge : cell->Edges) {
			for (int i = 0; i < edge.Points.Num(); ++i) {
				const FVector2D& p = edge.Points[i];
				vertices.AddUnique(FVector2D(p));
				
				if (p.X < topLeft.X) topLeft.X = p.X;
				if (p.Y < topLeft.Y) topLeft.Y = p.Y;
				if (p.X > bottomRight.X) bottomRight.X = p.X;
				if (p.Y > bottomRight.Y) bottomRight.Y = p.Y;

				if (i != edge.Points.Num()-1) {
					const FVector2D& np = edge.Points[i+1];
					DrawLine(p.X, p.Y, np.X, np.Y, FColor::Magenta, 2);
				}
			}
		}
		
		UMaterialInstanceDynamic* mid = UMaterialInstanceDynamic::Create(MapMaterial, this);
		mid->SetScalarParameterValue("BiomeType", cell->Height);
		
		TArray<int32> v;
		for (int32 n = 0; n < vertices.Num(); ++n) v.Add(n);
		while (v.Num() >= 3) {
			const int32 n0 = v[0];
			const int32 n1 = v[1];
			const int32 n2 = v[2];

			const FVector2D& v0 = vertices[n0];
			const FVector2D& v1 = vertices[n1];
			const FVector2D& v2 = vertices[n2];

			FVector2D uv0 = FVector2D((v0.X-topLeft.X)/(bottomRight.X-topLeft.X), (v0.Y-topLeft.Y)/(bottomRight.Y-topLeft.Y));
			FVector2D uv1 = FVector2D((v1.X-topLeft.X)/(bottomRight.X-topLeft.X), (v1.Y-topLeft.Y)/(bottomRight.Y-topLeft.Y));
			FVector2D uv2 = FVector2D((v2.X-topLeft.X)/(bottomRight.X-topLeft.X), (v2.Y-topLeft.Y)/(bottomRight.Y-topLeft.Y));

			DrawMaterialTriangle(mid, v0, v1, v2, v0, v1, v2);

			v.Remove(n1);
		}
		DrawText(FString::FromInt(cell->Height), FColor::White, -10+(topLeft.X+bottomRight.X)/2, -10+(topLeft.Y+bottomRight.Y)/2);
	}
}

void AWoAHUD::BeginPlay() {
	Super::BeginPlay();

	WGDebugMenu = CreateWidget<UWorldGenDebugMenu>(GetOwningPlayerController(), WGDebugMenuClass);
	//WGDebugMenu->AddToViewport();
}
