#include <stdlib.h>
#include <time.h>
#include "EnemySpawner.h"
#include "GameDefine.h"
#include "GameManager.h"

#define RANGESPAWNRATIO 40

const double spawnInterval[5] = {2000,1800,1500,1200,1000};	// 스폰 딜레이 ms단위

int nowArea = 0;

int rangeEnemyCount = 0;

POINT spawnPOINT[16] = {
	{320,-500},{640,-500},{960,-500},{1280,-500},
	{2420,180},{2420,360},{2420,540},{2420,720},
	{320,1580},{640,1580},{960,1580},{1280,1580},
	{-500,180},{-500,360},{-500,540},{-500,720}
};

void EnemySpawner_Init()
{
	nowArea = 0;
	rangeEnemyCount = 0;
}

void EnemySpawner_SpawnEnemy(Camera* mainCamera, double deltaTime)
{
	static double spawnDelta = 1000000;

	if (GameManager_AreaReturn() != NULL)
	{
		nowArea = GameManager_AreaReturn()->index;
	}

	if (spawnDelta < spawnInterval[nowArea])
	{
		spawnDelta += deltaTime;
		return;
	}
	
	srand(time(NULL));
	srand(rand());
	int randomType = (rand() * rand()) % 100;

	if (randomType <= RANGESPAWNRATIO)
	{
		if (rangeEnemyCount >= ENEMYRANGECOUNT_MAX)
		{
			return;
		}

		++rangeEnemyCount;
		randomType = EnemyType_Range;
	}
	else
	{
		randomType = EnemyType_Melee;
	}

	int randomPosIndex = (rand() * rand()) % 16;
	POINT localPOINT = Camera_RelativeSpawnPOINT(mainCamera, &spawnPOINT[randomPosIndex]);

	Enemy* enemy = Enemy_GetEnemy(randomType);
	if (enemy == NULL)
	{
		return;
	}

	Enemy_StartEnemy(enemy, localPOINT);
	spawnDelta = 0;
}

void EnemySpawner_ReduceRangeCount()
{
	--rangeEnemyCount;
}

