// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenDebugMenu.h"

#include "CanvasItem.h"
#include "WorldGenerationUtils.h"
#include "WorldGenerator.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

int32 UWorldGenDebugMenu::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                      const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                      const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {
	for (UvwCell* cell : GetScaledCells()) {
		for (const FvwEdge& edge : cell->Edges) {
			FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), edge.Points);
		}
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

TArray<UvwCell*> UWorldGenDebugMenu::GetScaledCells() const {
	TArray<UvwCell*> cells;
	UWorldGenerator* wg = UWorldGenerationUtils::GetWorldGenerator(GetOwningPlayer());
	if (!wg) return cells;
	UVoronoiWrapper* vd = wg->VoronoiDiagram;

	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Background->Slot);
	if (!slot) return cells;
	
	FGeometry geometry = Background->GetParent()->GetCachedGeometry();
	FVector2D startPos = geometry.AbsoluteToLocal(Background->GetCachedGeometry().GetAbsolutePosition());

	for (UvwCell* cell : vd->Cells) {
		UvwCell* cCpy = NewObject<UvwCell>();
		FVector2D size = FVector2D(vd->NumberOfSeeds.X * vd->SubdivisionSize.X, vd->NumberOfSeeds.Y * vd->SubdivisionSize.Y);
		for (const FvwEdge& edge : cell->Edges) {
			FvwEdge eCpy;
			for (FVector2D pt : edge.Points) {
				eCpy.Points.Add(startPos+pt*slot->GetSize()/size);
			}
			cCpy->Edges.Add(eCpy);
		}
		cCpy->Seed = FvwSeed(cell->GetIndex(), startPos+cell->GetCenter()*slot->GetSize()/size);
		cCpy->Height = cell->Height;
		cells.Add(cCpy);
	}
	
	return cells;
}
