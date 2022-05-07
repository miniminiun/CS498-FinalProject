// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"

#define CUBESIZE 500
#define SCALE 5
#define HALFSIZE 250

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateMap();

	/// Spawn ocean and land according to Map
	for (int i = 0; i < 4; i++) {
		TArray<TArray<int32>> temp;
		TypeLandLocations.Add(temp);
	}
	UWorld *WorldPointer = GetWorld();
	for (int32 i = 0; i < 32; i++) {
		for (int32 j = 0; j < 32; j++) {
			const FVector Location = FVector(CUBESIZE * i - HALFSIZE, CUBESIZE * j - HALFSIZE, 0);
			const FRotator Rotation = FRotator(0, 0, 0);
			AActor* ActorPointer;
			if (Map[i][j] == 1) {// map == 2 is ocean, map == 0 is land
				ActorPointer = WorldPointer->SpawnActor<AActor>(wall, Location, Rotation);
			}
			else if (Map[i][j] == 2) {
				ActorPointer = WorldPointer->SpawnActor<AActor>(land1, Location, Rotation);
				TArray<int32> temp = { i, j };
				TypeLandLocations[0].Add(temp);
			}
			else if (Map[i][j] == 3) {
				ActorPointer = WorldPointer->SpawnActor<AActor>(land2, Location, Rotation);
				TArray<int32> temp = { i, j };
				TypeLandLocations[1].Add(temp);
			}
			else if (Map[i][j] == 4) {
				ActorPointer = WorldPointer->SpawnActor<AActor>(land3, Location, Rotation);
				TArray<int32> temp = { i, j };
				TypeLandLocations[2].Add(temp);
			}
			else if (Map[i][j] == 5) {
				ActorPointer = WorldPointer->SpawnActor<AActor>(land4, Location, Rotation);
				TArray<int32> temp = { i, j };
				TypeLandLocations[3].Add(temp);
			}
			else ActorPointer = NULL;

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

	//SpawnPoint.Z += 500;
	////spawn enemy1
	//SpawnPoint.X += 100;
	//WorldPointer->SpawnActor<AActor>(enemy1, SpawnPoint, Rotation);
	////spawn enemy2
	//SpawnPoint.X -= 100;
	//SpawnPoint.Y += 100;
	//WorldPointer->SpawnActor<AActor>(enemy2, SpawnPoint, Rotation);
	////spawn enemy3
	//SpawnPoint.Y -= 200;
	//WorldPointer->SpawnActor<AActor>(enemy3, SpawnPoint, Rotation);
	////spawn enemy4
	//SpawnPoint.Y += 100;
	//SpawnPoint.X -= 100;
	//WorldPointer->SpawnActor<AActor>(enemy4, SpawnPoint, Rotation);

	for (int i = 0; i < 4; i++) {
		int32 Num = TypeLandLocations[i].Num() * 0.2;
		environmentNumber.Add(Num);
		Num = TypeLandLocations[i].Num() * 0.1;
		enemyNumber.Add(Num);
	}

	SpawnEnemy();
	WorldPointer->GetTimerManager().SetTimer(timeHandler, this, &AMapGenerator::SpawnEnemy, 70, true);
	
	SpawnEnvironment();
	WorldPointer->GetTimerManager().SetTimer(timeHandler, this, &AMapGenerator::SpawnEnvironment, 70, true);

	//WorldPointer->GetTimerManager().SetTimer(timeHandler, this, &AMapGenerator::SpawnFinalBoss, 5, false);
	//Rotation.Roll = -30;
	//Spawn rock biome
	//AActor* biomePointer;
	///*int32 Num = TypeLandLocations[3].Num() * 0.2;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	//for (int32 i = 0; i < environmentNumber[3]; i++) {
	//	Rotation.Roll = FMath::RandRange(-30, +30);
	//	Rotation.Pitch = FMath::RandRange(-30, +30);
	//	SpawnPoint = GenerateRandomBiomeSpawnPoint(3);
	//	SpawnPoint.Z = 0;
	//	biomePointer = WorldPointer->SpawnActor<AActor>(rock, SpawnPoint, Rotation);
	//	float scale = FMath::RandRange(0.2, 1);
	//	biomePointer->SetActorScale3D(FVector(scale, scale, scale));
	//}
	//Rotation.Roll = 0;
	//Rotation.Pitch = 0;
	////spawn desert biome
	///*Num = TypeLandLocations[2].Num() * 0.2;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	//for (int32 i = 0; i < environmentNumber[2]; i++) {
	//	SpawnPoint = GenerateRandomBiomeSpawnPoint(2);
	//	SpawnPoint.Z = 30;
	//	biomePointer = WorldPointer->SpawnActor<AActor>(cactus, SpawnPoint, Rotation);
	//	float scale = FMath::RandRange(5, 10);
	//	biomePointer->SetActorScale3D(FVector(scale, scale, scale));
	//}

	////spawn ice biome
	//
	///*Num = TypeLandLocations[1].Num() * 0.2;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	//for (int32 i = 0; i < environmentNumber[1]; i++) {
	//	Rotation.Roll = FMath::RandRange(65, 115);
	//	Rotation.Pitch = FMath::RandRange(-30, 30);
	//	
	//	SpawnPoint = GenerateRandomBiomeSpawnPoint(1);
	//	SpawnPoint.Z = 40;
	//	biomePointer = WorldPointer->SpawnActor<AActor>(ice, SpawnPoint, Rotation);
	//	float scale = FMath::RandRange(10, 20);
	//	biomePointer->SetActorScale3D(FVector(scale, scale, scale));
	//}
	//Rotation.Roll = 0;
	//Rotation.Pitch = 0;
	////spawn forest biome
	///*Num = TypeLandLocations[0].Num() * 0.2;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	//for (int32 i = 0; i < environmentNumber[0]; i++) {
	//	Rotation.Yaw = FMath::RandRange(0, 360);
	//	SpawnPoint = GenerateRandomBiomeSpawnPoint(0);
	//	SpawnPoint.Z = 70;
	//	biomePointer = WorldPointer->SpawnActor<AActor>(tree, SpawnPoint, Rotation);
	//	float scale = FMath::RandRange(float(0.1), float(0.3));
	//	biomePointer->SetActorScale3D(FVector(scale, scale, scale));
	//}

	

}

void AMapGenerator::SpawnFinalBoss() {
	AActor* actorPointer;
	FRotator Rotation = FRotator(0, 0, 0);
	FVector SpawnPoint;
	UWorld* WorldPointer = GetWorld();
	do {
		SpawnPoint = GenerateRandomSpawnPoint();
		actorPointer = WorldPointer->SpawnActor<AActor>(finalboss, SpawnPoint, Rotation);
	} while (!actorPointer);
}

void AMapGenerator::SpawnEnvironment() {
	AActor* biomePointer;
	FRotator Rotation = FRotator(0, 0, 0);
	FVector SpawnPoint;
	UWorld* WorldPointer = GetWorld();
	/*int32 Num = TypeLandLocations[3].Num() * 0.2;
	UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	while(environmentNumber[3]) {
		Rotation.Roll = FMath::RandRange(-30, +30);
		Rotation.Pitch = FMath::RandRange(-30, +30);
		SpawnPoint = GenerateRandomBiomeSpawnPoint(3);
		SpawnPoint.Z = 0;
		biomePointer = WorldPointer->SpawnActor<AActor>(rock, SpawnPoint, Rotation);
		float scale = FMath::RandRange(float(0.5), float(1.0));
		biomePointer->SetActorScale3D(FVector(scale, scale, scale));
		environmentNumber[3]--;
	}
	Rotation.Roll = 0;
	Rotation.Pitch = 0;
	//spawn desert biome
	/*Num = TypeLandLocations[2].Num() * 0.2;
	UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	while(environmentNumber[2]) {
		SpawnPoint = GenerateRandomBiomeSpawnPoint(2);
		SpawnPoint.Z = 30;
		biomePointer = WorldPointer->SpawnActor<AActor>(cactus, SpawnPoint, Rotation);
		float scale = FMath::RandRange(5, 10);
		biomePointer->SetActorScale3D(FVector(scale, scale, scale));
		environmentNumber[2]--;
	}

	//spawn ice biome

	/*Num = TypeLandLocations[1].Num() * 0.2;
	UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	while(environmentNumber[1]) {
		Rotation.Roll = FMath::RandRange(65, 115);
		Rotation.Pitch = FMath::RandRange(-30, 30);

		SpawnPoint = GenerateRandomBiomeSpawnPoint(1);
		SpawnPoint.Z = 40;
		biomePointer = WorldPointer->SpawnActor<AActor>(ice, SpawnPoint, Rotation);
		float scale = FMath::RandRange(10, 20);
		biomePointer->SetActorScale3D(FVector(scale, scale, scale));
		environmentNumber[1]--;
	}
	Rotation.Roll = 0;
	Rotation.Pitch = 0;
	//spawn forest biome
	/*Num = TypeLandLocations[0].Num() * 0.2;
	UE_LOG(LogTemp, Warning, TEXT("%d"), Num);*/
	while(environmentNumber[0]) {
		Rotation.Yaw = FMath::RandRange(0, 360);
		SpawnPoint = GenerateRandomBiomeSpawnPoint(0);
		SpawnPoint.Z = 70;
		biomePointer = WorldPointer->SpawnActor<AActor>(tree, SpawnPoint, Rotation);
		float scale = FMath::RandRange(float(0.1), float(0.3));
		biomePointer->SetActorScale3D(FVector(scale, scale, scale));
		environmentNumber[0]--;
	}
}



void AMapGenerator::SpawnEnemy() {
	AActor* actorPointer;
	FVector SpawnPoint;
	FRotator Rotation = FRotator(0, 0, 0);
	UWorld* WorldPointer = GetWorld();
	int maximumTry = 10;
	while (enemyNumber[0] && maximumTry) {
		SpawnPoint = GenerateRandomBiomeSpawnPoint(0);
		SpawnPoint.Z = 300;
		actorPointer = WorldPointer->SpawnActor<AActor>(enemy1, SpawnPoint, Rotation);
		if (actorPointer) enemyNumber[0]--;
		maximumTry--;
	}
	maximumTry = 10;
	while (enemyNumber[1] && maximumTry) {
		SpawnPoint = GenerateRandomBiomeSpawnPoint(1);
		SpawnPoint.Z = 300;
		actorPointer = WorldPointer->SpawnActor<AActor>(enemy2, SpawnPoint, Rotation);
		if (actorPointer) enemyNumber[1]--;
		maximumTry--;
	}
	maximumTry = 10;
	while (enemyNumber[2] && maximumTry) {
		SpawnPoint = GenerateRandomBiomeSpawnPoint(2);
		SpawnPoint.Z = 300;
		actorPointer = WorldPointer->SpawnActor<AActor>(enemy3, SpawnPoint, Rotation);
		if (actorPointer) enemyNumber[2]--;
		maximumTry--;
	}
	maximumTry = 10;
	while (enemyNumber[3]) {
		SpawnPoint = GenerateRandomBiomeSpawnPoint(3);
		SpawnPoint.Z = 300;
		actorPointer = WorldPointer->SpawnActor<AActor>(enemy4, SpawnPoint, Rotation);
		if (actorPointer) enemyNumber[3]--;
		maximumTry--;
	}
}

void AMapGenerator::PrintMsg() {
	UE_LOG(LogTemp, Warning, TEXT("Timer chenggong"));
}

FVector AMapGenerator::GenerateRandomSpawnPoint() {
	FVector SpawnPoint;
	int32 idxtype = FMath::RandRange(0, 3);
	int32 idx = FMath::RandRange(0, TypeLandLocations[idxtype].Num() - 1);
	SpawnPoint.X = TypeLandLocations[idxtype][idx][0] * CUBESIZE - HALFSIZE + FMath::RandRange(-HALFSIZE + 100, HALFSIZE - 100);
	SpawnPoint.Y = TypeLandLocations[idxtype][idx][1] * CUBESIZE - HALFSIZE + FMath::RandRange(-HALFSIZE + 100, HALFSIZE - 100);
	SpawnPoint.Z = 0;
	return SpawnPoint;
}

FVector AMapGenerator::GenerateRandomBiomeSpawnPoint(int32 idxtype) {
	FVector SpawnPoint;
	int32 idx = FMath::RandRange(0, TypeLandLocations[idxtype].Num() - 1);
	SpawnPoint.X = TypeLandLocations[idxtype][idx][0] * CUBESIZE - HALFSIZE + FMath::RandRange(-HALFSIZE + 50, HALFSIZE - 50);
	SpawnPoint.Y = TypeLandLocations[idxtype][idx][1] * CUBESIZE - HALFSIZE + FMath::RandRange(-HALFSIZE, HALFSIZE - 20);
	SpawnPoint.Z = 0;
	return SpawnPoint;
}

void AMapGenerator::CheckMap(int32 i, int32 j, TArray<TArray<int32>>& SmallMap)
{
	if (SmallMap[i][j] == 1) {
		SmallMap[i][j] = 6;
		if (i > 1)
			CheckMap(i - 1, j, SmallMap);
		if (j > 1)
			CheckMap(i, j - 1, SmallMap);
		if (i + 1 < 10)  // if row change, 20 also need to change
			CheckMap(i + 1, j, SmallMap);
		if (j + 1 < 10)  // if col change, 20 also need to change
			CheckMap(i, j + 1, SmallMap);
	}
}

void Swap(int32 & a, int32 & b) {
	if (a > b) {
		int32 temp = a;
		a = b;
		b = temp;
	}
}

void AMapGenerator::GenerateMap()
{
	int32 row = 32;
	int32 col = 32;

	for (int32 i = 0; i < row; i++) {
		TArray<int32> temp;
		temp.Init(1, col);
		Map.Add(temp);
	}

	TArray<TArray<int32>> Direction = { {0,1}, {1,0},{0,-1}, {-1,0} };
	TArray<int32> LastDirection = { 0, 0 };
	TArray<int32> RandomDirection;
	int32 MaxTunnel = 40;
	int32 MaxLength = 2;

	for (int32 iter = 0; iter < 9; iter++) {
		TArray<TArray<int32>> SmallMap;
		for (int32 i = 0; i < 10; i++) {
			TArray<int32> temp;
			temp.Init(1, 10);
			SmallMap.Add(temp);
		}

		int32 OriginRow = FMath::RandRange(0, 9);
		int32 OriginCol = FMath::RandRange(0, 9);

		int32 CurrentRow = OriginRow;
		int32 CurrentCol = OriginCol;

		int32 type = 0;
		if (iter < 4) {
			type = iter + 2;
		}
		else {
			type = FMath::RandRange(2, 5);
		}

		IslandsType.Add(type);
		while (MaxTunnel) {
			do {
				int32 tempindex = FMath::RandRange(0, 3);
				RandomDirection = Direction[tempindex];
			} while (RandomDirection[0] == LastDirection[0] && RandomDirection[1] == LastDirection[1] ||
				RandomDirection[0] == LastDirection[0] && RandomDirection[1] == -LastDirection[1]);

			int32 RandomLength = FMath::RandRange(0, MaxLength);
			int32 TunnelLength = 0;

			while (TunnelLength < RandomLength) {
				if (((CurrentRow == 0) && (RandomDirection[0] == -1)) ||
					((CurrentCol == 0) && (RandomDirection[1] == -1)) ||
					((CurrentRow == 9) && (RandomDirection[0] == 1)) ||
					((CurrentCol == 9) && (RandomDirection[1] == 1))) {
					break;
				}
				else {
					SmallMap[CurrentRow][CurrentCol] = type;
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

		for (int32 i = 0; i < 10; i++) {
			CheckMap(i, 0, SmallMap);
			CheckMap(i, 9, SmallMap);
		}
		for (int32 j = 1; j + 1 < 10; j++) {
			CheckMap(0, j, SmallMap);
			CheckMap(9, j, SmallMap);
		}

		for (int32 i = 0; i < 10; i++) {
			for (int32 j = 0; j < 10; j++) {
				if (SmallMap[i][j] == 1)
					SmallMap[i][j] = type;
			}
		}
		
		int m = 1 + (iter / 3) * 10;
		int n = 1 + (iter % 3) * 10;
		for (int p = 0; p < 10; p++) {
			for (int q = 0; q < 10; q++) {
				if (SmallMap[p][q] == type)
					Map[p + m][q + n] = type;
			}
		}
		TArray<int32> IslandStartPoint;
		IslandStartPoint.Add(OriginRow + m);
		IslandStartPoint.Add(OriginCol + n);

		Islands.Add(IslandStartPoint);

		MaxTunnel = 40;
	}

	// generate spanning tree
	TArray<TArray<int32>> map = { {1,3},{0,2,4} ,{1,5}, {0,4,6},{1,3,5,7},{2,4,8},{3,7},{4,6,8},{5,7} };
	TArray<int32> visited = { 0,0,0,0,0,0,0,0,0 };
	TArray<TArray<int32>> Edges;
	int32 StartIsland = FMath::RandRange(0, 8);
	visited[StartIsland] = 1;

	for (int i = 0; i < 8; i++) {
		TArray<TArray<int32>> TempEdges;
		for (int j = 0; j < 9; j++) {
			if (visited[j]) {
				for (int k = 0; k < map[j].Num(); k++) {
					if (!visited[map[j][k]]) {
						TArray<int32> temp = { j, map[j][k] };
						TempEdges.Add(temp);
					}
				}
			}
		}

		TArray<int32> edge = TempEdges[FMath::RandRange(0, TempEdges.Num() - 1)];
		Edges.Add(edge);
		visited[edge[1]] = 1;
	}


	// connect map
	for (int32 i = 0; i < 8; i++) {
		int32 AIsland = Edges[i][0];
		int32 BIsland = Edges[i][1];
		int32 Ax = Islands[AIsland][0];
		int32 Ay = Islands[AIsland][1];
		int32 Bx = Islands[BIsland][0];
		int32 By = Islands[BIsland][1];

		int32 type1 = Map[Ax][Ay];
		int32 type2 = Map[Ax][Ay];
		int32 m = Ay;
		int32 race = Ay - By > 0 ? -1 : 1;
		for (; m != By; m += race) {
			if (Map[Ax][m] == 1) Map[Ax][m] = type1;
		}
		int32 n = Ax;
		race = Ax - Bx > 0 ? -1 : 1;
		for (; n != Bx; n += race) {
			if (Map[n][By] == 1) Map[n][By] = type2;
		}

	}

	for (int i = 0; i < Edges.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("%d %d"), Edges[i][0], Edges[i][1]);
	}
	


	for (int32 i = 0; i < row; i++) {
		UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),
			Map[i][0], Map[i][1], Map[i][2], Map[i][3], Map[i][4], Map[i][5], Map[i][6], Map[i][7],
			Map[i][8], Map[i][9], Map[i][10], Map[i][11], Map[i][12], Map[i][13], Map[i][14], Map[i][15], 
			Map[i][16], Map[i][17], Map[i][18], Map[i][19], Map[i][20], Map[i][21], Map[i][22], Map[i][23], Map[i][24], Map[i][25],
			Map[i][26], Map[i][27], Map[i][28], Map[i][29], Map[i][30], Map[i][31]);
	}
}

//void AMapGenerator::GenerateMap()
//{
//	int32 row = 20;
//	int32 col = 20;
//	
//	int32 CurrentRow = FMath::RandRange(0,19);
//	int32 CurrentCol = FMath::RandRange(0,19);
//
//	TArray<TArray<int32>> Direction = { {0,1}, {1,0},{0,-1}, {-1,0} };
//	TArray<int32> LastDirection = { 0, 0 };
//	TArray<int32> RandomDirection;
//
//	int32 MaxTunnel = 60;
//	int32 MaxLength = 6;
//		
//	for (int32 i = 0; i < row; i++) {
//		TArray<int32> temp;
//		temp.Init(1, 20);
//		Map.Add(temp);
//	}
//	
//	while (MaxTunnel) {
//		do {
//			int32 tempindex = FMath::RandRange(0, 3);
//			RandomDirection = Direction[tempindex];
//		} while (RandomDirection[0] == LastDirection[0] && RandomDirection[1] == LastDirection[1] ||
//			RandomDirection[0] == LastDirection[0] && RandomDirection[1] == -LastDirection[1]);
//
//		int32 RandomLength = FMath::RandRange(0, 6);
//		int32 TunnelLength = 0;
//
//		while (TunnelLength < RandomLength) {
//			if (((CurrentRow == 0) && (RandomDirection[0] == -1)) ||
//				((CurrentCol == 0) && (RandomDirection[1] == -1)) ||
//				((CurrentRow == 19) && (RandomDirection[0] == 1)) ||
//				((CurrentCol == 19) && (RandomDirection[1] == 1))) {
//				break;
//			}
//			else {
//				Map[CurrentRow][CurrentCol] = 0;
//				CurrentRow += RandomDirection[0];
//				CurrentCol += RandomDirection[1];
//				TunnelLength++;
//			}
//		}
//
//		if (TunnelLength) {
//			LastDirection = RandomDirection;
//			MaxTunnel--;
//		}
//	}
//
//	for (int32 i = 0; i < row; i++) {
//		CheckMap(i, 0);
//		CheckMap(i, col - 1);
//	}
//	for (int32 j = 1; j + 1 < row; j++) {
//		CheckMap(0, j);
//		CheckMap(row - 1, j);
//	}
//
//	for (int32 i = 0; i < row; i++) {
//		for (int32 j = 0; j < col; j++) {
//			if (Map[i][j] == 1)
//				Map[i][j] = 0;
//		}
//	}
//
//	for (int32 i = 0; i < row; i++) {
//			UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "),
//				Map[i][0], Map[i][1], Map[i][2], Map[i][3], Map[i][4], Map[i][5], Map[i][6], Map[i][7],
//				Map[i][8], Map[i][9], Map[i][10], Map[i][11], Map[i][12], Map[i][13], Map[i][14], Map[i][15], 
//				Map[i][16], Map[i][17], Map[i][18], Map[i][19]);
//	}
//	UE_LOG(LogTemp, Warning, TEXT("hello"));
//}

void AMapGenerator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (lightSource) {
		lightSource->AddActorLocalRotation(FRotator(DeltaTime * turnRate, 0,0));
	}
	if (sun) {
		FOutputDeviceNull ar;
		sun->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), ar, NULL, true);
	}
}

void AMapGenerator::decreaseEnemy(int32 i) {
	enemyNumber[i]++;
}

void AMapGenerator::incrementEnvironment(int32 i) {
	environmentNumber[i]++;
}
