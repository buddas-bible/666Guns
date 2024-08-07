#pragma once
#include <windows.h>
#include "GameDefine.h"
#include "Player.h"

/// <summary>
/// 점령지 구조체
/// </summary>
typedef struct
{
	int index;
	RECT area;
	BOOL flags;

}occupiedArea;


void GameManager_Init(Player* player);


void GameManager_Update();

/// <summary>
/// 플레이어가 점령지 위에 있으면 시간이 흐르는 함수
/// </summary>
/// <param name="player">플레이어 구조체</param>
/// <param name="deltaTime">델타타임</param>
void GameManager_Occupation(Player* player, double deltaTime);

/// <summary>
/// 다음 점령지를 반환해주는 함수
/// </summary>
/// <returns></returns>
occupiedArea* GameManager_AreaReturn();

int GameManager_CheckGameOver();

void GameManager_ReduceHP(int damage);

void GameManager_SetWeapon(int weaponSide);

void GameManager_TakeNextWeapon(int weaponIndex);

int GameManager_GetHeatGauge(int weaponSide);

void GameManager_IncreaseGauge(int weaponSide, int increase);

int GameManager_GetHP();

int GameManager_GetRemainTime();

void GameManager_GambleWeapon();

// 무기 인덱스 반환하는 함수, 0은 왼쪽슬롯 1은 오른쪽슬롯 2는 다음슬롯
int GameManager_GetWeaponIndex(int slotIndex);

BOOL GameManager_GetJustChanged();

double* GameManager_GetWeaponHeatGuage(int slotIndex);

double GameManager_GetOverHeatTime(int slotIndex);

void GameManager_Pause();

void GameManager_Resume();

BOOL GameManager_IsGamePaused();

void GameManager_DEVReduceTime();

void GameManager_DEVSetWeapon(int index);

void GameManager_DEVFullHP();
