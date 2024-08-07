#include "Core.h"

#include <tchar.h>
#include "GameDefine.h"
#include "GameManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GDIEngine.h"
#include "Vector2D.h"
#include "AABB_OBB.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "MainMenu.h"
#include "Camera.h"
#include "EnemySpawner.h"

#pragma region debugData
int debugHit = 0;
#pragma endregion debugData

extern Bullet bulletPool[WEAPONTYPECOUNT][BULLETPOOL_MAX];
extern Enemy enemyPool[2][ENEMYPOOL_MAX];
extern occupiedArea territory[5];

double deltaTime;
double elipseTick;

LARGE_INTEGER startTick;
LARGE_INTEGER endTick;
LARGE_INTEGER frequency;

HWND hWnd;

int nowScene = 0;	// 현재 씬. 0 : 메뉴 / 1 : 게임
BOOL isSceneJustChanged = TRUE;

#pragma region DEVKEY
BOOL keypressedP;
BOOL keypressedO;
BOOL keypressed1;
BOOL keypressed2;
BOOL keypressed3;
BOOL keypressed4;
#pragma endregion DEVKEY

BOOL keypressedW;
BOOL keypressedS;
BOOL keypressedA;
BOOL keypressedD;
BOOL keypressedSpace;
BOOL keypressedQ;
BOOL keypressedE;


BOOL keypressedUp;
BOOL keypressedDown;
BOOL keypressedLeft;
BOOL keypressedRight;

BOOL mousePressedLeft;
BOOL mousePressedRight;

Vector2D mouse;
POINT mousePOINT;

const Vector2D UP = { 0, -1 };
const Vector2D DOWN = { 0, 1 };
const Vector2D LEFT = { -1, 0 };
const Vector2D RIGHT = { 1, 0 };

const POINT UP_P = { 0, -10 };
const POINT DOWN_P = { 0, 10 };
const POINT LEFT_P = { -10, 0 };
const POINT RIGHT_P = { 10, 0 };

Player player;
Camera mainCamera;
BOOL isCheatmode = FALSE;

// right, bottom만 기본값으로 넣어줄 것.
// 9600 6480
// (현재) 인덱스 16, 17에 있는 값은 6번을 두 개로 쪼갠 값이다.
// 배열을 추가할 일이 있다면 16 ~ 17번 앞에 끼워넣어서 꼭 뒤로 밀어내자
RECT buildPosition[44] = {
	// 건물 그림 + 콜리전, 6번 인덱스는 그림만 그리는 용도 콜리전 체크는 x
	{8980, 4820}, {7370, -320}, {8600, 2030}, {10, 4820}, {7390, 2030},
	{2720, 4730}, {4000, -1070}, {8930, -630}, {2900, 2170}, {10, -500},
	{10, 1890}, {4860, 4730}, {1250, 1890}, {5790, 2990}, {10, 3100},
	{ 1530, -500},

	// 6번 건물의 콜리전
	{4000, -1070, 4730, 848}, {5190, -1070, 5921, 848},
	//맵 외곽 콜리전
	{-50, -50 , 9650, 0}, {-50, -50, 0, 6530}, {-50, 6480, 9650, 6530}, {9600, -50, 9650, 6530},

	{8150, 550}, {5900, 1150}, {2900, 300}, {4550, 2800}, {1200, 3800}, {2450, 4550}, {900, 5200}, {7500, 5050}, {8600, 4000},		// 보라색
	{6930, 510}, {350, 1550}, {3500, 5350},								// 노란색
	{8780, 400}, {8737, 5648}, {4200, 3150},							// 핑크색
	{8730, 800}, {1380, 850}, {8400, 5200},	{5600, 3500},							// 하늘색
	{800, 650}, {4400, 1700}, {1600, 5500}											// 자두색

};

/// <summary>
/// 엔진을 초기화 한다.
/// frequency를 받고, GDI 엔진을 초기화한다.
/// </summary>
/// <returns></returns>
void Engine_Init()
{
	QueryPerformanceFrequency(&frequency);
	GDI_Initialize(hWnd);
}

void Engine_StartCheckTime()
{
	QueryPerformanceCounter(&startTick);
}

void Engine_EndCheckTime()
{
	QueryPerformanceCounter(&endTick);
	elipseTick = (double)(endTick.QuadPart - startTick.QuadPart) / (double)(frequency.QuadPart);
	deltaTime = elipseTick * 1000;
}

double Engine_GetDeltaTime()
{
	return deltaTime;
}

LARGE_INTEGER Engine_GetFrequency()
{
	return frequency;
}

void Engine_Input()
{
	keypressed1 = GetAsyncKeyState('7');
	keypressed2 = GetAsyncKeyState('8');
	keypressed3 = GetAsyncKeyState('9');
	keypressed4 = GetAsyncKeyState('0');
	keypressedP = GetAsyncKeyState('P') & 0x8000;
	keypressedO = GetAsyncKeyState('O');

	keypressedW = GetAsyncKeyState('W');// &0x0001;
	keypressedS = GetAsyncKeyState('S');// &0x0001;
	keypressedA = GetAsyncKeyState('A');// &0x0001;
	keypressedD = GetAsyncKeyState('D');// &0x0001;
	keypressedSpace = GetAsyncKeyState(VK_SPACE);// &0x0001;
	keypressedQ = GetAsyncKeyState('Q');// &0x0001;
	keypressedE = GetAsyncKeyState('E');// &0x0001;

	keypressedUp = GetAsyncKeyState(VK_UP);// &0x0001;
	keypressedDown = GetAsyncKeyState(VK_DOWN);// &0x0001;
	keypressedLeft = GetAsyncKeyState(VK_LEFT);// &0x0001;
	keypressedRight = GetAsyncKeyState(VK_RIGHT);// &0x0001;

	mousePressedLeft = GetAsyncKeyState(VK_LBUTTON);
	mousePressedRight = GetAsyncKeyState(VK_RBUTTON);

	GetCursorPos(&mousePOINT);
	ScreenToClient(hWnd, &mousePOINT);
	mouse.x = (double)(mousePOINT.x) + mainCamera.LeftTopPOINT.x;
	mouse.y = (double)(mousePOINT.y) + mainCamera.LeftTopPOINT.y;
}

#pragma region MainMenu

void Engine_MenuInit()
{
	SoundStop(1);
	SoundStop(18);
	BGMSound(0);
	isSceneJustChanged = FALSE;
	BOOL isCheatmode = FALSE;
}

void Engine_MenuLoop()
{
	Engine_EndCheckTime();
	Engine_StartCheckTime();
	Engine_Input();
	Engine_MenuUpdate();
	Engine_MenuRender();
}

void Engine_MenuUpdate()
{
	KeyManager_Update();
	LeftMouseManager_Update();

	if (keypressedP)
	{
		SetCheatMode(TRUE);
		if (isCheatmode)
		{
			Sleep(500);
		}
	}

	if (keypressedO)
	{
		SetCheatMode(FALSE);
	}
		
	//KeyBoard_popUp();
	MainMenuButtons(hWnd, mousePOINT);
	SoundUpdate();
}

void Engine_MenuRender()
{
	GDI_StartDC();
	GDI_MainBackGround();
	GDI_MouseHover(GetHoldButton());
	// 배경음 감소 버튼 좌표 (220, 190)
	// 배경음 증가 버튼 좌표 (640, 190)
	// 효과음 감소 버튼 좌표 (220, 410)
	// 효과음 증가 버튼 좌표 (640, 410)
	GDI_Popup(GetBGMVolume(0), GetEffectVolume(6), GetPopUpflag());
	GDI_Creadit(GetPopUpflag3());
	GDI_HitMouseCursor(mousePOINT.x, mousePOINT.y);
	GDI_PrintBitMap();
	GDI_EndDC();
}

#pragma endregion MainMenu


#pragma region MainGame


void Engine_GameInit()
{
	SoundStop(0);
	BGMSound(1);
	GameManager_Init(&player);
	Player_Init(&player);
	Camera_Init(&mainCamera, &player);
	EnemyPool_Init();
	BulletPool_Init();
	EnemySpawner_Init();
	isSceneJustChanged = FALSE;
}

void Engine_GameLoop()
{
	Engine_EndCheckTime();
	Engine_StartCheckTime();
	Engine_Input();
	Engine_GameUpdate();
	Engine_GameRender();
}

void Engine_GameUpdate()
{

	if (nowScene != Scene_Game)
	{
		return;
	}

	if (GameManager_CheckGameOver())
	{
		if (player.deathTime <= 0)
		{
			nowScene = Scene_Ending;
			isSceneJustChanged = TRUE;
			return;
		}
	}

	KeyManager_Update();
	KeyBoard_popUp(mousePOINT);

	if (GameManager_IsGamePaused())
	{
		return;
	}

	GameManager_Update();
	LeftMouseManager_Update();
	RightMouseManager_Update();
	KeyBoard_Sound_Input(&player);
	Player_Update();
	Mouse_Sound_Input(&player);
	EnemySpawner_SpawnEnemy(&mainCamera, deltaTime);
	Enemy_Update();
	Bullet_Update();
	Collision_Update();
	SoundUpdate();

	GameManager_Occupation(&player, deltaTime);

	if (isCheatmode)
	{
		if (keypressedP)
		{
			GameManager_DEVReduceTime();
		}

		if (keypressedO)
		{
			GameManager_DEVFullHP();
		}

		if (keypressed1)
		{
			GameManager_DEVSetWeapon(0);
		}

		if (keypressed2)
		{
			GameManager_DEVSetWeapon(1);
		}

		if (keypressed3)
		{
			GameManager_DEVSetWeapon(2);
		}

		if (keypressed4)
		{
			GameManager_DEVSetWeapon(3);
		}
	}
}

void Engine_GameRender()
{
	if (nowScene != Scene_Game)
	{
		return;
	}

	//GDI_Initialize(hWnd);
	GDI_StartDC();
	GDI_BackGround(mainCamera.LeftTopPOINT.x, mainCamera.LeftTopPOINT.y);

	GDI_OccupiedArea(GameManager_AreaReturn(), &mainCamera, &player, deltaTime);

	
	GDI_PlayerObejct(&player, &mainCamera, deltaTime);
	GDI_EnemyObject(enemyPool, &mainCamera, deltaTime);
	GDI_Bullet(bulletPool, &mainCamera);
	GDI_Building(buildPosition, &mainCamera);
	GDI_FindArea(&player, GameManager_AreaReturn(), &mainCamera);
	GDI_GameUI(GameManager_GetHP(),	GameManager_GetRemainTime(), player.dashCount, deltaTime);
	
	if (player.selectedWeapon[0]->explodeTime + player.selectedWeapon[1]->explodeTime > 0)
	{
		GDI_OverHeatExplode(&player, &mainCamera, deltaTime);
	}

	GDI_GamePopupUI(GetBGMVolume(0), GetEffectVolume(2), GetPopUpflag2());
	GDI_HitMouseCursor(mousePOINT.x, mousePOINT.y);
	GDI_PrintBitMap();

#pragma region DebugData
	if (isCheatmode)
	{
		TCHAR dtTEXT0[50] = _T("");
		wsprintf(dtTEXT0, TEXT("left : %d, right : %d"), player.selectedWeapon[0]->weaponIndex, player.selectedWeapon[1]->weaponIndex);
		TextOut(GDI_GetDC(), 1700, 580, dtTEXT0, (int)_tcslen(dtTEXT0));

		TCHAR dtTEXT1[50] = _T("");
		int cameraLTX = mainCamera.LeftTopPOINT.x;
		int cameraLTY = mainCamera.LeftTopPOINT.y;
		wsprintf(dtTEXT1, TEXT("camera x,y : %d , %d"), cameraLTX, cameraLTY);
		TextOut(GDI_GetDC(), 1700, 600, dtTEXT1, (int)_tcslen(dtTEXT1));

		TCHAR dtTEXT2[50] = _T("");
		int playerX = (int)player.centerPosition.x;
		int playerY = (int)player.centerPosition.y;
		wsprintf(dtTEXT2, TEXT("player x,y : %d , %d"), playerX, playerY);
		TextOut(GDI_GetDC(), 1700, 620, dtTEXT2, (int)_tcslen(dtTEXT2));

		TCHAR dtTEXT3[50] = _T("");
		wsprintf(dtTEXT3, TEXT("deltaTime : %d.%d ms"), (int)deltaTime, (int)(deltaTime * 1000) % 1000);
		TextOut(GDI_GetDC(), 1700, 640, dtTEXT3, (int)_tcslen(dtTEXT3));

		TCHAR dtTEXT4[50] = _T("");
		wsprintf(dtTEXT4, TEXT("enemy[0][0] status : %d"), (int)enemyPool[0][0].nowStatus);
		TextOut(GDI_GetDC(), 1700, 680, dtTEXT4, (int)_tcslen(dtTEXT4));
	}

	//SelectObject(GDI_GetDC(), (HBRUSH)GetStockObject(NULL_BRUSH));
	//SelectObject(GDI_GetDC(), (HPEN)GetStockObject(WHITE_PEN));

	//RECT playerLocalHitbox = Camera_WorldToCameraRECT(&mainCamera, &player.hitbox);

	//Rectangle(GDI_GetDC(),
	//	playerLocalHitbox.left,
	//	playerLocalHitbox.top,
	//	playerLocalHitbox.right,
	//	playerLocalHitbox.bottom);

	//RECT enemyLocalHitbox = Camera_WorldToCameraRECT(&mainCamera, &enemy.hitbox);

	//Rectangle(GDI_GetDC(),
	//	enemyLocalHitbox.left,
	//	enemyLocalHitbox.top,
	//	enemyLocalHitbox.right,
	//	enemyLocalHitbox.bottom);

	//for (int i = 0; i < WEAPONTYPECOUNT; i++)
	//{
	//	for (int j = 0; j < BULLETPOOL_MAX; j++)
	//	{
	//		if (!bulletPool[i][j].isActive)
	//		{
	//			continue;
	//		}

	//		POINT bulletPOINT;

	//		bulletPOINT.x = (int)bulletPool[i][j].centerPosition.x;
	//		bulletPOINT.y = (int)bulletPool[i][j].centerPosition.y;

	//		POINT bulletLocalHitbox = Camera_WorldToCameraPOINT(&mainCamera, &bulletPOINT);

	//		Ellipse(GDI_GetDC(),
	//			bulletLocalHitbox.x - bulletPool[i][j].radius,
	//			bulletLocalHitbox.y - bulletPool[i][j].radius,
	//			bulletLocalHitbox.x + bulletPool[i][j].radius,
	//			bulletLocalHitbox.y + bulletPool[i][j].radius
	//		);
	//	}
	//}
#pragma endregion DebugData

	GDI_EndDC();
}

#pragma endregion MainGame

#pragma region EndingScene
void Engine_EndingInit()
{
	if (GameManager_CheckGameOver() == 1)
	{
		SoundStop(1);
		BGMSound(18);
	}
	else if (GameManager_CheckGameOver() == 2)
	{
		SoundStop(1);
		BGMSound(2);
	}
	isSceneJustChanged = FALSE;
	BOOL isCheatmode = FALSE;
}

void Engine_EndingLoop()
{
	Engine_EndCheckTime();
	Engine_StartCheckTime();
	Engine_Input();
	Engine_EndingUpdate();
	Engine_EndingRender();
}

void Engine_EndingUpdate()
{

	LeftMouseManager_Update();

	if (GameManager_CheckGameOver() == 1)
	{
		GameClear_MouseClick();
	}
	else if (GameManager_CheckGameOver() == 2)
	{
		GameOver_MouseClick();
	}
	SoundUpdate();
}

void Engine_EndingRender()
{
	GDI_StartDC();
	if (GameManager_CheckGameOver() == 1)
	{
		GDI_SurviveEnding(deltaTime);
		GDI_DeleteSix(deltaTime);
		
	}
	else if(GameManager_CheckGameOver() == 2)
	{
		GDI_DeadEnding(deltaTime);
		GDI_MousePress(deltaTime);
	}
	GDI_PrintBitMap();
	GDI_EndDC();
}
#pragma endregion EndingScene

#pragma region CreditScene
void Engine_CreditInit()
{
	//SoundStop(18);
	isSceneJustChanged = FALSE;
	BOOL isCheatmode = FALSE;
}

void Engine_CreditLoop()
{
	Engine_EndCheckTime();
	Engine_StartCheckTime();
	Engine_Input();
	Engine_CreditUpdate();
	Engine_CreditRender();
}
void Engine_CreditUpdate()
{
	GameClear_CreditMouseClick();
}

void Engine_CreditRender()
{
	GDI_StartDC();

	GDI_EndingCreadit();

	GDI_PrintBitMap();
	GDI_EndDC();
}
#pragma endregion CreditScene

/// <summary>
/// Player Update 함수
/// </summary>
void Player_Update()
{
	Vector2D dashAt;

	dashAt.x = 0;
	dashAt.y = 0;

	player.statusChanged = FALSE;
	player.isShooting = FALSE;

	if (player.deathTime > 0)
	{
		player.deathTime -= deltaTime;
		return;
	}

	if (player.gracePeriod > 0)
	{
		player.gracePeriod -= deltaTime;
	}

	if (player.selectedWeapon[0]->explodeTime > 0)
	{
		player.selectedWeapon[0]->explodeTime -= deltaTime;
	}
	else
	{
		player.selectedWeapon[0]->frame = 0;
		player.selectedWeapon[0]->frameDelta = 0;
	}

	if (player.selectedWeapon[1]->explodeTime > 0)
	{
		player.selectedWeapon[1]->explodeTime -= deltaTime;
	}
	else
	{
		player.selectedWeapon[1]->frame = 0;
		player.selectedWeapon[1]->frameDelta = 0;
	}


	if (player.selectedWeapon[0]->fireDelay > 0)
	{
		player.selectedWeapon[0]->fireDelay -= deltaTime;
	}

	if (player.selectedWeapon[1]->fireDelay > 0)
	{
		player.selectedWeapon[1]->fireDelay -= deltaTime;
	}

	if (player.dashCount < MAXDASHCOUNT)
	{
		if (player.dashCoolTimeCount >= DASHCOOLTIME)
		{
			player.dashCount++;
			player.dashCoolTimeCount = 0;
		}

		player.dashCoolTimeCount += deltaTime;
	}

	if (player.dashTimeCount > 0)
	{
		player.dashTimeCount -= deltaTime;

		Player_Dash(&player);
		mainCamera.Update(&mainCamera, deltaTime);

		return;
	}

	if (player.dashTimeCount <= 0)
	{
		player.isCheckHit = TRUE;
	}

	player.LookAt(&player, mouse);
	Player_SetDirection(&player, &mouse);

	if (player.isRight)
	{
		player.nowStatus = Anim_IdleRight;
	}
	else
	{
		player.nowStatus = Anim_IdleLeft;
	}


	if (KeyManager_GetKeyState(W) == HOLD)
	{
		player.Move(&player, UP, WALKSPEED);
		dashAt = Vector2D_Add(dashAt, UP);

		if (player.isRight)
		{
			player.nowStatus = Anim_MoveRight;
		}
		else
		{
			player.nowStatus = Anim_MoveLeft;
		}
	}

	if (KeyManager_GetKeyState(S) == HOLD)
	{
		player.Move(&player, DOWN, WALKSPEED);
		dashAt = Vector2D_Add(dashAt, DOWN);

		if (player.isRight)
		{
			player.nowStatus = Anim_MoveRight;
		}
		else
		{
			player.nowStatus = Anim_MoveLeft;
		}
	}

	if (KeyManager_GetKeyState(A) == HOLD)
	{
		player.Move(&player, LEFT, WALKSPEED);
		dashAt = Vector2D_Add(dashAt, LEFT);

		if (player.isRight)
		{
			player.nowStatus = Anim_MoveRight;
		}
		else
		{
			player.nowStatus = Anim_MoveLeft;
		}
	}

	if (KeyManager_GetKeyState(D) == HOLD)
	{
		player.Move(&player, RIGHT, WALKSPEED);
		dashAt = Vector2D_Add(dashAt, RIGHT);

		if (player.isRight)
		{
			player.nowStatus = Anim_MoveRight;
		}
		else
		{
			player.nowStatus = Anim_MoveLeft;
		}
	}

	if (KeyManager_GetKeyState(Q) == TAP)
	{
		ShootSound(10);
		//여기다가 무기 교체하는 기능
		GameManager_SetWeapon(WeaponSide_Left);
	}

	if (KeyManager_GetKeyState(E) == TAP)
	{
		ShootSound(10);
		//여기다가 무기 교체하는 기능
		GameManager_SetWeapon(WeaponSide_Right);
	}

	if (KeyManager_GetKeyState(SPACE) == TAP)
	{
		Player_DashStart(&player, dashAt);
	}

	if (keypressedUp)
	{
		Camera_DEVMOVE(&mainCamera, UP_P);
	}

	if (keypressedDown)
	{
		Camera_DEVMOVE(&mainCamera, DOWN_P);
	}

	if (keypressedLeft)
	{
		Camera_DEVMOVE(&mainCamera, LEFT_P);
	}

	if (keypressedRight)
	{
		Camera_DEVMOVE(&mainCamera, RIGHT_P);
	}

	if (mousePressedLeft && mousePressedRight)
	{
	}
	else if (mousePressedLeft)
	{
		player.FireWeapon(&player, WeaponSide_Left);
	}
	else if (mousePressedRight)
	{
		player.FireWeapon(&player, WeaponSide_Right);
	}

	mainCamera.Update(&mainCamera, deltaTime);
}


/// <summary>
/// Enemy Update 함수
/// </summary>
void Enemy_Update()
{
	for (int type = 0; type < 2; ++type)
	{
		for (int index = 0; index < ENEMYPOOL_MAX; ++index)
		{
			if (enemyPool[type][index].isActive == FALSE)
			{
				continue;
			}

			enemyPool[type][index].statusChanged = FALSE;

			if (enemyPool[type][index].animTime > 0)
			{
				enemyPool[type][index].animTime -= deltaTime;
			}
			else
			{
				enemyPool[type][index].isShooting = FALSE;
			}

			if (enemyPool[type][index].fireDelay > 0)
			{
				enemyPool[type][index].fireDelay -= deltaTime;
			}

			if (enemyPool[type][index].deathTime > 0)
			{
				enemyPool[type][index].deathTime -= deltaTime;
				continue;
			}

			if (enemyPool[type][index].isDead)
			{
				enemyPool[type][index].isActive = FALSE;
				continue;
			}

			enemyPool[type][index].LookAt(&enemyPool[type][index], player.centerPosition);
			Enemy_SetDirection(&enemyPool[type][index], &player.centerPosition);

			if (enemyPool[type][index].isRight)
			{
				enemyPool[type][index].nowStatus = EAnim_IdleRight;
			}
			else
			{
				enemyPool[type][index].nowStatus = EAnim_IdleLeft;
			}

			if (type == EnemyType_Range)
			{
				if (Enemy_PlayerDistance(&enemyPool[type][index], &player) < 600)
				{
					Enemy_Attack(&enemyPool[type][index]);
				}
				else
				{
					Enemy_Follow(&enemyPool[type][index], &player);
				}
			}
			else if (type == EnemyType_Melee)
			{
				if (Enemy_PlayerDistance(&enemyPool[type][index], &player) < 125)
				{
					Enemy_Attack(&enemyPool[type][index]);
				}
				else
				{
					Enemy_Follow(&enemyPool[type][index], &player);
				}
			}
		}
	}
}


/// Bullet Update 함수
void Bullet_Update()
{
	for (int i = 0; i < WEAPONTYPECOUNT; i++)
	{
		for (int j = 0; j < BULLETPOOL_MAX; j++)
		{
			if (bulletPool[i][j].isActive == TRUE)
			{
				Bullet_Flying(&bulletPool[i][j], deltaTime);
			}
		}
	}
}

/// 물리 Update 함수
void Collision_Update()
{
	// 체크
	for (int weaponType = 0; weaponType < WEAPONTYPECOUNT; weaponType++)
	{
		for (int bulletIndex = 0; bulletIndex < BULLETPOOL_MAX; bulletIndex++)
		{
			if (bulletPool[weaponType][bulletIndex].isActive == FALSE)
			{
				continue;
			}

			for (int k = 0; k < 44; ++k)
			{
				if (k == 6)
				{
					continue;
				}

				if (CollisionCheck_BtoC(bulletPool[weaponType][bulletIndex], buildPosition[k]) == TRUE)
				{
					debugHit++;
					bulletPool[weaponType][bulletIndex].isActive = FALSE;
					goto ENDTHISBULLET;
					break;
				}
			}


			if (weaponType == Weapon_EnemyRifle)
			{
				if (CollisionCheck_BtoC(bulletPool[weaponType][bulletIndex], player.hitbox) == TRUE && player.isCheckHit)
				{
					if (player.gracePeriod <= 0 && player.dashTimeCount <= 0)
					{
						Player_Hit(&player);
					}
					bulletPool[weaponType][bulletIndex].isActive = FALSE;
				}

				break;
			}

			for (int enemyType = 0; enemyType < 2; ++enemyType)
			{
				for (int enemyIndex = 0; enemyIndex < ENEMYPOOL_MAX; ++enemyIndex)
				{
					if (enemyPool[enemyType][enemyIndex].isActive == FALSE)
					{
						continue;
					}
					
					if (CollisionCheck_BtoC(bulletPool[weaponType][bulletIndex], enemyPool[enemyType][enemyIndex].hitbox) == TRUE &&
						enemyPool[enemyType][enemyIndex].isCheckHit)
					{
						Enemy_Death(&enemyPool[enemyType][enemyIndex]);
						bulletPool[weaponType][bulletIndex].isActive = FALSE;

						GameManager_GambleWeapon();

						if (enemyType == EnemyType_Range)
						{
							EnemySpawner_ReduceRangeCount();
						}
						goto ENDTHISBULLET;	// 바로 아래 622로 jump. goto는 딱 여기서만 사용했음.
					}
				}
			}


		ENDTHISBULLET:;
		}
	}

	if (8 <= player.selectedWeapon[0]->frame && player.selectedWeapon[0]->frame <= 10)
	{
		for (int enemyType = 0; enemyType < 2; ++enemyType)
		{
			for (int enemyIndex = 0; enemyIndex < ENEMYPOOL_MAX; ++enemyIndex)
			{
				if (enemyPool[enemyType][enemyIndex].isActive == FALSE)
				{
					continue;
				}

				RECT explodeBox = { 
					player.centerPosition.x - 200, 
					player.centerPosition.y - 200, 
					player.centerPosition.x + 200,
					player.centerPosition.y + 200 };

				if (CollisionCheck_AABB(explodeBox, enemyPool[enemyType][enemyIndex].hitbox) == TRUE)
				{
					Enemy_Death(&enemyPool[enemyType][enemyIndex]);

					GameManager_GambleWeapon();

					if (enemyType == EnemyType_Range)
					{
						EnemySpawner_ReduceRangeCount();
					}
				}
			}
		}
	}

	if (8 <= player.selectedWeapon[1]->frame && player.selectedWeapon[1]->frame <= 10)
	{
		for (int enemyType = 0; enemyType < 2; ++enemyType)
		{
			for (int enemyIndex = 0; enemyIndex < ENEMYPOOL_MAX; ++enemyIndex)
			{
				if (enemyPool[enemyType][enemyIndex].isActive == FALSE)
				{
					continue;
				}

				RECT explodeBox = {
					player.centerPosition.x - 200,
					player.centerPosition.y - 200,
					player.centerPosition.x + 200,
					player.centerPosition.y + 200 };

				if (CollisionCheck_AABB(explodeBox, enemyPool[enemyType][enemyIndex].hitbox) == TRUE)
				{
					Enemy_Death(&enemyPool[enemyType][enemyIndex]);

					GameManager_GambleWeapon();

					if (enemyType == EnemyType_Range)
					{
						EnemySpawner_ReduceRangeCount();
					}
				}
			}
		}
	}
}


int Engine_GetSceneNumber()
{
	return nowScene;
}

void Engine_SetSceneNumber(int sceneNumber)
{
	nowScene = sceneNumber;
	isSceneJustChanged = TRUE;
}

BOOL IsSceneJustChanged()
{
	return isSceneJustChanged;
}

void SetCheatMode(BOOL status)
{
	isCheatmode = status;
}
