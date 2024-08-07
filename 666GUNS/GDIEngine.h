#pragma once

#include <windows.h>
#include "Vector2D.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "Bullet.h"
#include "GameDefine.h"
#include "GameManager.h"


/// <summary>
/// GDI를 이용한 2D 그래픽스 엔진 초기화
/// </summary>
/// <param name="hWnd">그래픽스 엔진을 사용할 대상이 되는 윈도우의 핸들</param>
void GDI_Initialize(HWND hWnd);

/// <summary>
/// 게임 루프 실행시 매번 받아와야 하는 DC
/// </summary>
void GDI_StartDC();

/// <summary>
/// 사용한 GDI를 종료(Release 및 DeleteObjct)하는 함수
/// </summary>
void GDI_EndDC();

void GDI_ExtendedView(HWND hWnd);

/// <summary>
/// 전체 비트맵 출력
/// </summary>
void GDI_PrintBitMap();

#pragma region Ending
void GDI_SurviveEnding(double deltaTime);
void GDI_DeleteSix(double deltaTime);
void GDI_EndingCreadit();
void GDI_DeadEnding(double deltaTime);
void GDI_MousePress(double deltaTime);
#pragma endregion

#pragma region Main
void GDI_MainBackGround();

void GDI_Creadit(BOOL isUp);

void GDI_Popup(float backgroundVolume, float effectVolume, BOOL isUp);

void GDI_MouseHover(int buttonHover);
#pragma endregion

#pragma region InGame

#pragma region UI
/// <summary>
/// 인게임 UI를 그려주는 함수
/// </summary>
/// <param name="playerHP">플레이어의 남은 생명(기회) 수</param>
/// <param name="remainTime">남은 플레이 시간</param>
/// <param name="deltaTime"></param>
void GDI_GameUI(int playerHP, int remainTime, int playerDash, double deltaTime);

/// <summary>
/// 플레이어의 남은 생명(기회)을 그려주는 함수
/// </summary>
/// <param name="playerHP">플레이어의 남은 생명(기회) 수</param>
void GDI_PlayerHPUI(int playerHp);

/// <summary>
/// 남은 플레이 시간을 그려주는 함수
/// </summary>
/// <param name="remainTime">남은 플레이 시간</param>
void GDI_TimerUI(int remainTime);

/// <summary>
/// 플레이어의 남은 생명(기회)을 그려주는 함수
/// </summary>
/// <param name="playerHP">플레이어의 남은 생명(기회) 수</param>
void GDI_PlayerDashUI(int playerDash);

/// <summary>
/// 플레이어가 가지고 있는 무기를 그려주는 함수
/// </summary>
/// <param name="deltaTime"></param>
void GDI_WeaponUI(double deltaTime);

/// <summary>
/// 오버히트시 출력할 UI
/// </summary>
/// <param name="deltaTime"></param>
void GDI_OverHeatUI(double deltaTime);


/// <summary>
/// background를 그려주는 함수
/// </summary>
/// <param name="fileName">backgrond 비트맵 파일명</param>
/// <param name="cameraPosX">카메라 좌표 X</param>
/// <param name="cameraPosY">카메라 좌표 Y</param>
void GDI_BackGround(int cameraPosX, int cameraPosY);

/// <summary>
/// 점령지 애니메이션
/// </summary>
/// <param name="area">현재 점령해야하는 구역</param>
/// <param name="mainCamera">카메라 구조체</param>
/// <param name="player">플레이어 구조체</param>
/// <param name="deltaTime">델타타임</param>
void GDI_OccupiedArea(occupiedArea* area, Camera* mainCamera, Player* player, double deltaTime);

/// <summary>
/// 인게임 배경에 건물을 그려주는 함수
/// </summary>
/// <param name="buildingLocal"></param>
/// <param name="mainCamera"></param>
void GDI_Building(RECT* buildingLocal, Camera* mainCamera);


void GDI_GamePopupUI(float backgroundVolume, float effectVolume, BOOL isUp);
#pragma endregion

void GDI_OverHeatExplode(Player* player, Camera* mainCamera, double deltaTime);

#pragma region Player
/// <summary>
/// 다음 점령지를 반환해주는 함수
/// </summary>
/// <param name="player"></param>
/// <param name="area"></param>
/// <param name="mainCamera"></param>
void GDI_FindArea(Player* player, occupiedArea* area, Camera* mainCamera);

/// <summary>
/// 플레이어 객체를 그려주는 함수
/// </summary>
/// <param name="player"></param>
/// <param name="mainCamera"></param>
/// <param name="deltaTime"></param>
void GDI_PlayerObejct(Player* player, Camera* mainCamera, double deltaTime);


/// <summary>
/// 플레이어를 그려주는 함수
/// </summary>
/// <param name="deltaTime"></param>
void GDI_Player(Player* player, double deltaTime);

/// <summary>
/// 플레이어의 무기를 그려주는 함수
/// </summary>
/// <param name="player"></param>
/// <param name="deltaTime"></param> 
void GDI_PlayerWeapon(Player* player, double deltaTime);
#pragma endregion

#pragma region Enemy
/// <summary>
/// 적 객체를 그려주는 함수
/// </summary>
/// <param name="enemy"></param>
/// <param name="mainCamera"></param>
void GDI_EnemyObject(Enemy (*enemyPool)[ENEMYPOOL_MAX], Camera* mainCamera, double deltaTime);

/// <summary>
/// 적을 그려주는 함수
/// </summary>
void GDI_Enemy(Enemy* enemy, double deltaTime);

/// <summary>
/// 적 무기를 그려주는 함수
/// </summary>
/// <param name="enemy"></param>
void GDI_EnemyWeapon(Enemy* enemy, double deltaTime);
#pragma endregion

/// <summary>
/// 총알을 그려주는 함수
/// </summary>
/// <param name="bulletPool"></param>
/// <param name="mainCamera"></param>
void GDI_Bullet(Bullet (*bulletPool)[BULLETPOOL_MAX], Camera* mainCamera);

/// <summary>
/// 마우스 좌표에 따라 에임(저격 표시)를 그려주는 함수
/// </summary>
/// <param name="mousePosX">마우스의 X 좌표</param>
/// <param name="mousePosY">마우스의 Y 좌표</param>
void GDI_HitMouseCursor(int mousePosX, int mousePosY);
#pragma endregion

/// <summary>
/// 
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns></returns>
RECT GDI_Rect(int left, int top, int right, int bottom);


/// <summary>
/// 
/// </summary>
/// <returns></returns>
HDC GDI_GetDC();




