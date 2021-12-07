// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMapActor.generated.h"

UCLASS()
class DUMBKNIGHTS_API ATileMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FVector2D MapSize { FVector2D { 5, 5 } };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TMap<int, TSubclassOf<class ATileActor>> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int TileSize { 512 };
	
	TArray<int> TileData {
		1, 1, 2, 1, 1,
		1, 2, 2, 1, 1,
		1, 2, 1, 1, 1,
		2, 2, 1, 1, 1,
		1, 1, 1, 1, 1
	};
};
