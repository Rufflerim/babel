// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "TileActor.generated.h"

/**
 * 
 */
UCLASS()
class DUMBKNIGHTS_API ATileActor : public APaperSpriteActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsWarp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsBlock;
};
