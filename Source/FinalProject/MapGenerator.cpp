// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"

#define CUBESIZE 500
#define SCALE 5
#define HALFSIZE 250

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateMap();

	/// Spawn ocean and land according to Map
	UWorld *WorldPointer = GetWorld();
	for (int32 i = 0; i < 20; i++) {
		for (int32 j = 0; j < 20; j++) {
			const FVector Location = FVector(CUBESIZE * i - HALFSIZE, CUBESIZE * j - HALFSIZE, 0);
			const FRotator Rotation = FRotator(0, 0, 0);
			AActor* ActorPointer;
			if (Map[i][j] == 2) {// map == 2 is ocean, map == 0 is land
				ActorPointer = WorldPointer->SpawnActor<AActor>(wall, Location, Rotation);
			}
			else {
				ActorPointer = WorldPointer->SpawnActor<AActor>(land, Location, Rotation);
				LandLocations.Add(Location);
			}
			ActorPointer->SetActorScale3D(FVector(SCALE, SCALE, 1));
		}
	}
	/// Spawn map finish

	///Spawn player
	FVector SpawnPoint = GenerateRandomSpawnPoint();
	UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), SpawnPoint.X, SpawnPoint.Y, SpawnPoint.Z);
	FRotator Rotation = FRotator(0, 0, 0);
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	controller->UnPossess();
	APawn* CharacterPointer = WorldPointer->SpawnActor<APawn>(player, SpawnPoint, Rotation);
	controller->Possess(CharacterPointer);
	///Spawn player finish


}




FVector AMapGenerator::GenerateRandomSpawnPoint() {
	FVector SpawnPoint;
	int32 idx = FMath::RandRange(0, LandLocations.Num() - 1);
	SpawnPoint.X = LandLocations[idx].X + FMath::RandRange(-HALFSIZE, HALFSIZE);
	SpawnPoint.Y = LandLocations[idx].Y + FMath::RandRange(-HALFSIZE, HALFSIZE);
	SpawnPoint.Z = 0;
	return SpawnPoint;
}

void AMapGenerator::CheckMap(int32 i, int32 j)
{
	if (Map[i][j] == 1) {
		Map[i][j] = 2;
		if (i > 1)
			CheckMap(i - 1, j);
		if (j > 1)
			CheckMap(i, j - 1);
		if (i + 1 < 20)  // if row change, 20 also need to change
			CheckMap(i + 1, j);
		if (j + 1 < 20)  // if col change, 20 also need to change
			CheckMap(i, j + 1);
	}
}

void AMapGenerator::GenerateMap()
{
	int32 row = 20;
	int32 col = 20;
	
	int32 CurrentRow = FMath::RandRange(0,19);
	int32 CurrentCol = FMath::RandRange(0,19);

	TArray<TArray<int32>> Direction = { {0,1}, {1,0},{0,-1}, {-1,0} };
	TArray<int32> LastDirection = { 0, 0 };
	TArray<int32> RandomDirection;

	int32 MaxTunnel = 60;
	int32 MaxLength = 6;
		
	for (int32 i = 0; i < row; i++) {
		TArray<int32> temp;
		temp.Init(1, 20);
		Map.Add(temp);
	}
	
	while (MaxTunnel) {
		do {
			int32 tempindex = FMath::RandRange(0, 3);
			RandomDirection = Direction[tempindex];
		} while (RandomDirection[0] == LastDirection[0] && RandomDirection[1] == LastDirection[1] ||
			RandomDirection[0] == LastDirection[0] && RandomDirection[1] == -LastDirection[1]);

		int32 RandomLength = FMath::RandRange(0, 6);
		int32 TunnelLength = 0;

		while (TunnelLength < RandomLength) {
			if (((CurrentRow == 0) && (RandomDirection[0] == -1)) ||
				((CurrentCol == 0) && (RandomDirection[1] == -1)) ||
				((CurrentRow == 19) && (RandomDirection[0] == 1)) ||
				((CurrentCol == 19) && (RandomDirection[1] == 1))) {
				break;
			}
			else {
				Map[CurrentRow][CurrentCol] = 0;
				CurrentRow += RandomDirection[0];
				CurrentCol += RandomDirection[1];
				TunnelLength++;
			}
		}

		if (TunnelLength) {
			LastDirection = RandomDirection;
			MaxTunnel--;
		}
	}

	for (int32 i = 0; i < row; i++) {
		CheckMap(i, 0);
		CheckMap(i, col - 1);
	}
	for (int32 j = 1; j + 1 < row; j++) {
		CheckMap(0, j);
		CheckMap(row - 1, j);
	}

	for (int32 i = 0; i < row; i++) {
		for (int32 j = 0; j < col; j++) {
			if (Map[i][j] == 1)
				Map[i][j] = 0;
		}
	}

	for (int32 i = 0; i < row; i++) {
			UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "),
				Map[i][0], Map[i][1], Map[i][2], Map[i][3], Map[i][4], Map[i][5], Map[i][6], Map[i][7],
				Map[i][8], Map[i][9], Map[i][10], Map[i][11], Map[i][12], Map[i][13], Map[i][14], Map[i][15], 
				Map[i][16], Map[i][17], Map[i][18], Map[i][19]);
	}
	UE_LOG(LogTemp, Warning, TEXT("hello"));
}

