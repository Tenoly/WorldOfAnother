// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VoronoiWrapper.h"
#include "WorldGenDebugMenu.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class WORLDGENERATION_API UWorldGenDebugMenu : public UUserWidget {
	GENERATED_BODY()
public:
	TArray<UvwCell*> GetScaledCells() const;
	
protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
		const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	UPROPERTY(meta=(BindWidget)) TObjectPtr<UBorder> Background;
};
