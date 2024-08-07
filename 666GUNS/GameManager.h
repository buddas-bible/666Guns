#pragma once
#include <windows.h>
#include "GameDefine.h"
#include "Player.h"

/// <summary>
/// ������ ����ü
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
/// �÷��̾ ������ ���� ������ �ð��� �帣�� �Լ�
/// </summary>
/// <param name="player">�÷��̾� ����ü</param>
/// <param name="deltaTime">��ŸŸ��</param>
void GameManager_Occupation(Player* player, double deltaTime);

/// <summary>
/// ���� �������� ��ȯ���ִ� �Լ�
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

// ���� �ε��� ��ȯ�ϴ� �Լ�, 0�� ���ʽ��� 1�� �����ʽ��� 2�� ��������
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
