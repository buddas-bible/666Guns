#pragma once
#include <windows.h>

#include "Camera.h"
#include "Enemy.h"

void EnemySpawner_Init();

void EnemySpawner_SpawnEnemy(Camera* mainCamera, double deltaTime);

void EnemySpawner_ReduceRangeCount();

