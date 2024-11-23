// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WoAHUD.generated.h"

class UWorldGenDebugMenu;

/**
 * 
 */
UCLASS()
class WORLDOFANOTHER_API AWoAHUD : public AHUD {
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> WGDebugMenuClass;
	UPROPERTY(EditDefaultsOnly) UMaterialInterface* MapMaterial;
	UPROPERTY() TObjectPtr<UWorldGenDebugMenu> WGDebugMenu;
	
};
