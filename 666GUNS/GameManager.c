#include "GameManager.h"
#include "SoundManager.h"
#include "Weapon.h"
#include "Core.h"
#include "Player.h"
#include <stdlib.h>
#include <time.h>


#define AREA_WIDTH 450
#define AREA_HEIGHT 300
#define REMAINTIME_MAX 360

int gameoverStatus = GameStatus_Playing;
int playerHP = 3;
int remainTime = 360;
Weapon selectedWeapon[2];
Weapon nextWeapon;
BOOL justGetWeapon;
Player* player;
BOOL isGamePaused = FALSE;

occupiedArea territory[5] = {
	{0, 8250 , 1500	, 8250 + AREA_WIDTH,	1500 + AREA_HEIGHT },
	{1, 950	 , 1500	, 950  + AREA_WIDTH,	1500 + AREA_HEIGHT },
	{2, 950	 , 4400	, 950  + AREA_WIDTH,	4400 + AREA_HEIGHT },
	{3, 8250 , 4400	, 8250 + AREA_WIDTH,	4400 + AREA_HEIGHT },
	{4, 4850 , 3150	, 4850 + AREA_WIDTH,	3150 + AREA_HEIGHT } };

void GameManager_Init(Player* _player)
{
	player = _player;
	player->selectedWeapon[0] = &selectedWeapon[0];
	player->selectedWeapon[1] = &selectedWeapon[1];
	player->playerHP = &playerHP;

	gameoverStatus = GameStatus_Playing;

	BOOL isGamePaused = FALSE;

	playerHP = 3;
	remainTime = 360;

	selectedWeapon[0].weaponIndex = Weapon_Pistol;
	selectedWeapon[0].heatGauge = 0;
	selectedWeapon[0].fireDelay = 0;
	selectedWeapon[0].overHeatTime = 0;
	selectedWeapon[0].isOverHeat = FALSE;
	selectedWeapon[0].explodeTime = 0;

	selectedWeapon[1].weaponIndex = -1;	// 테스트용으로 샷건 넣어둠.
	selectedWeapon[1].heatGauge = 0;
	selectedWeapon[1].fireDelay = 0;
	selectedWeapon[1].overHeatTime = 0;
	selectedWeapon[1].isOverHeat = FALSE;
	selectedWeapon[1].explodeTime = 0;

	nextWeapon.weaponIndex = -1;
	nextWeapon.heatGauge = 0;

	for (int i = 0; i < 5; ++i)
	{
		territory[i].flags = FALSE;
	}
}

void GameManager_Update()
{
	justGetWeapon = FALSE;

	for (int i = 0; i < 2; i++)
	{
		if (selectedWeapon[i].overHeatTime > 0)
		{
			selectedWeapon[i].overHeatTime -= Engine_GetDeltaTime();
		}
	}

	if (selectedWeapon[WeaponSide_Left].overHeatTime <= 0)
	{
		if (selectedWeapon[WeaponSide_Left].isOverHeat)
		{
			selectedWeapon[WeaponSide_Left].heatGauge -= HEAT_DECREASE * Engine_GetDeltaTime() / 50;
			if (selectedWeapon[WeaponSide_Left].heatGauge <= 0)
			{
				selectedWeapon[WeaponSide_Left].isOverHeat = FALSE;
			}
		}

		selectedWeapon[WeaponSide_Left].heatGauge -= HEAT_DECREASE * Engine_GetDeltaTime() / 1000;
	}

	if (selectedWeapon[WeaponSide_Right].overHeatTime <= 0)
	{
		if (selectedWeapon[WeaponSide_Right].isOverHeat)
		{
			selectedWeapon[WeaponSide_Right].heatGauge -= HEAT_DECREASE * Engine_GetDeltaTime() / 50;
			if (selectedWeapon[WeaponSide_Right].heatGauge <= 0)
			{
				selectedWeapon[WeaponSide_Right].isOverHeat = FALSE;
			}
		}

		selectedWeapon[WeaponSide_Right].heatGauge -= HEAT_DECREASE * Engine_GetDeltaTime() / 1000;
	}



}

/*
= {
	{ 8200, 1500, 8700, 1800 }, // 제 1 점령지 // x 500, y 300
	{ 0, 0, 0, 0 },				// 제 2 점령지
	{ 0, 0, 0, 0 },				// 제 3 점령지
	{ 0, 0, 0, 0 },				// 제 4 점령지
	{ 4850, 3470, 5350, 3770 }	// 최종 점령지
};
*/

void GameManager_Occupation(Player* player, double deltaTime)
{
	static double totalTime = 0;
	static double secondTime = 0;
	
	occupiedArea* nowArea = NULL;
	nowArea = GameManager_AreaReturn();
	if (nowArea == NULL)
	{
		return;
	}

	if (CollisionCheck_RtoP(nowArea->area, player->centerPosition) == TRUE)
	{
		totalTime += deltaTime;
		secondTime += deltaTime;
		
		if (secondTime > 1000.0)
		{
			remainTime -= 1;
			secondTime = 0.0;
		}
	}

	if (nowArea->index != 4)
	{
		if (REMAINTIME_MAX == remainTime)
		{
			return;
		}

		if (((REMAINTIME_MAX - remainTime) / 60) - 1 == nowArea->index)
		{
			if ((REMAINTIME_MAX - remainTime) % 60 == 0)
			{
				InGameSound(23);
				nowArea->flags = TRUE;
				playerHP++;
				if (playerHP >= 4)
				{
					playerHP = 4;
				}
			}	
		}
	}
	else
	{
		if (remainTime == 120)
		{
			return;
		}

		if ((REMAINTIME_MAX - remainTime) % 120 == 0)
		{
			nowArea->flags = TRUE;
			remainTime = 0;
			// totalTime = 0.0;
		}
	}
}

occupiedArea* GameManager_AreaReturn()
{
	for (int i = 0; i < 5; ++i)
	{
		if (territory[i].flags == FALSE)
		{
			return &(territory[i]);
		}
	}
	return NULL;
}

int GameManager_CheckGameOver()
{
	if (remainTime <= 0)
	{
		gameoverStatus = GameStatus_Win;
	}
	else if (playerHP <= 0)
	{
		gameoverStatus = GameStatus_Lose;
	}

	return gameoverStatus;
}

void GameManager_ReduceHP(int damage)
{
	playerHP -= damage;
}

void GameManager_SetWeapon(int weaponSide)
{
	if (nextWeapon.weaponIndex == -1)
	{
		return;
	}

	selectedWeapon[weaponSide].weaponIndex = nextWeapon.weaponIndex;
	selectedWeapon[weaponSide].heatGauge = 0;
	selectedWeapon[weaponSide].fireDelay = 0;
	selectedWeapon[weaponSide].overHeatTime = 0;
	selectedWeapon[weaponSide].isOverHeat = FALSE;
	selectedWeapon[weaponSide].explodeTime = 0;
	selectedWeapon[weaponSide].frame = 0;
	selectedWeapon[weaponSide].frameDelta = 0;

	nextWeapon.weaponIndex = -1;
}

void GameManager_TakeNextWeapon(int weaponIndex)
{
	justGetWeapon = TRUE;

	if (nextWeapon.weaponIndex != -1)
	{
		return;
	}

	nextWeapon.weaponIndex = weaponIndex;
}

int GameManager_GetHeatGauge(int weaponSide)
{
	return selectedWeapon[weaponSide].heatGauge;
}

void GameManager_IncreaseGauge(int weaponSide, int increase)
{
	selectedWeapon[weaponSide].heatGauge += increase;

	if (selectedWeapon[weaponSide].heatGauge >= 100)
	{
		//오버히트 처리해야함 221124
		selectedWeapon[weaponSide].heatGauge = 100;
	}
}

int GameManager_GetHP()
{
	return playerHP;
}

int GameManager_GetRemainTime()
{
	return remainTime;
}

void GameManager_GambleWeapon()
{
	if (nextWeapon.weaponIndex != -1)
	{
		return;
	}

	srand(time(NULL));
	srand(rand()%2353 * rand()%342);
	int randomDrop = (rand()*rand()) % 100;

	if (randomDrop >= WEAPONDROPRATIO)
	{
		return;
	}

	int randomWeapon = (rand() * rand()) % 4;

	GameManager_TakeNextWeapon(randomWeapon);
}

int GameManager_GetWeaponIndex(int slotIndex)
{
	if (slotIndex == 2)
	{
		return nextWeapon.weaponIndex;
	}

	return selectedWeapon[slotIndex].weaponIndex;
}

BOOL GameManager_GetJustChanged()
{
	return justGetWeapon;
}

double* GameManager_GetWeaponHeatGuage(int slotIndex)
{
	return &selectedWeapon[slotIndex].heatGauge;
}

double GameManager_GetOverHeatTime(int slotIndex)
{
	return selectedWeapon[slotIndex].overHeatTime;
}

void GameManager_Pause()
{
	isGamePaused = TRUE;
}

void GameManager_Resume()
{
	isGamePaused = FALSE;
}

BOOL GameManager_IsGamePaused()
{
	return isGamePaused;
}

void GameManager_DEVReduceTime()
{
	remainTime -= 1;
}

void GameManager_DEVSetWeapon(int index)
{
	selectedWeapon[WeaponSide_Left].weaponIndex = index;
	selectedWeapon[WeaponSide_Left].heatGauge = 0;
	selectedWeapon[WeaponSide_Left].fireDelay = 0;
	selectedWeapon[WeaponSide_Left].overHeatTime = 0;
}

void GameManager_DEVFullHP()
{
	playerHP = 100;
}