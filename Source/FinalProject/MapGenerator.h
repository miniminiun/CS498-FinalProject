// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/DirectionalLight.h"
#include "Misc/OutputDeviceNull.h"

#include "MapGenerator.generated.h"

UCLASS()
class FINALPROJECT_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	UPROPERTY(EditAnywhere, Category = "Sky");
	AActor* sun;
	UPROPERTY(EditAnywhere, Category = "Sky");
	ADirectionalLight* lightSource;
	UPROPERTY(EditAnywhere, Category = "Sky");
	float turnRate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void GenerateMap();
	void CheckMap(int32 i, int32 j, TArray<TArray<int32>> & SmallMap);
	FVector GenerateRandomSpawnPoint();
	FVector GenerateRandomBiomeSpawnPoint(int32 idxtype);
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> land1;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> land2;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> land3;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> land4;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> wall;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<APawn> player;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> rock;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> cactus;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> tree;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> ice;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<APawn> enemy1;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<APawn> enemy2;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<APawn> enemy3;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<APawn> enemy4;

	TArray<TArray<int32>> Map;
	TArray<TArray<int32>> Islands;
	TArray<int32> IslandsType;
	TArray<TArray<TArray<int32>>> TypeLandLocations;
};
