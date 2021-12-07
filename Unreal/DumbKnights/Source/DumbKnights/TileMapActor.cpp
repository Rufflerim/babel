// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMapActor.h"
#include "TileActor.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATileMapActor::ATileMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	const FVector LineBegin = GetActorLocation();
	const FVector LineLeftEnd = GetActorLocation() + FVector(MapSize.X * TileSize, 0, 0);
	const FVector LineTopEnd = GetActorLocation() + FVector(0, 0, MapSize.Y * TileSize);
	const FVector LineTopLeftEnd = LineLeftEnd + LineTopEnd;
	const auto World = GetWorld();
	
	DrawDebugLine(World, LineBegin, LineLeftEnd, FColor::Magenta, false, 1000, 0, 50);
	DrawDebugLine(World, LineBegin, LineTopEnd, FColor::Magenta, false, 1000, 0, 50);
	DrawDebugLine(World, LineTopEnd, LineTopLeftEnd, FColor::Magenta, false, 1000, 0, 50);
	DrawDebugLine(World, LineLeftEnd, LineTopLeftEnd, FColor::Magenta, false, 1000, 0, 50);

}

// Called when the game starts or when spawned
void ATileMapActor::BeginPlay()
{
	Super::BeginPlay();
	int Index = 0;
	for (const int TileId : TileData)
	{
		const int Line = Index / static_cast<int>(MapSize.X);
		const int Col = Index % static_cast<int>(MapSize.X);

		FVector Location(Col * TileSize, 0, -Line * TileSize);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor<ATileActor>(Tiles[TileId], Location, Rotation, SpawnInfo);
		
		++Index;
	}
}

// Called every frame
void ATileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

