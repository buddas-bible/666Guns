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
/// GDI�� �̿��� 2D �׷��Ƚ� ���� �ʱ�ȭ
/// </summary>
/// <param name="hWnd">�׷��Ƚ� ������ ����� ����� �Ǵ� �������� �ڵ�</param>
void GDI_Initialize(HWND hWnd);

/// <summary>
/// ���� ���� ����� �Ź� �޾ƿ;� �ϴ� DC
/// </summary>
void GDI_StartDC();

/// <summary>
/// ����� GDI�� ����(Release �� DeleteObjct)�ϴ� �Լ�
/// </summary>
void GDI_EndDC();

void GDI_ExtendedView(HWND hWnd);

/// <summary>
/// ��ü ��Ʈ�� ���
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
/// �ΰ��� UI�� �׷��ִ� �Լ�
/// </summary>
/// <param name="playerHP">�÷��̾��� ���� ����(��ȸ) ��</param>
/// <param name="remainTime">���� �÷��� �ð�</param>
/// <param name="deltaTime"></param>
void GDI_GameUI(int playerHP, int remainTime, int playerDash, double deltaTime);

/// <summary>
/// �÷��̾��� ���� ����(��ȸ)�� �׷��ִ� �Լ�
/// </summary>
/// <param name="playerHP">�÷��̾��� ���� ����(��ȸ) ��</param>
void GDI_PlayerHPUI(int playerHp);

/// <summary>
/// ���� �÷��� �ð��� �׷��ִ� �Լ�
/// </summary>
/// <param name="remainTime">���� �÷��� �ð�</param>
void GDI_TimerUI(int remainTime);

/// <summary>
/// �÷��̾��� ���� ����(��ȸ)�� �׷��ִ� �Լ�
/// </summary>
/// <param name="playerHP">�÷��̾��� ���� ����(��ȸ) ��</param>
void GDI_PlayerDashUI(int playerDash);

/// <summary>
/// �÷��̾ ������ �ִ� ���⸦ �׷��ִ� �Լ�
/// </summary>
/// <param name="deltaTime"></param>
void GDI_WeaponUI(double deltaTime);

/// <summary>
/// ������Ʈ�� ����� UI
/// </summary>
/// <param name="deltaTime"></param>
void GDI_OverHeatUI(double deltaTime);


/// <summary>
/// background�� �׷��ִ� �Լ�
/// </summary>
/// <param name="fileName">backgrond ��Ʈ�� ���ϸ�</param>
/// <param name="cameraPosX">ī�޶� ��ǥ X</param>
/// <param name="cameraPosY">ī�޶� ��ǥ Y</param>
void GDI_BackGround(int cameraPosX, int cameraPosY);

/// <summary>
/// ������ �ִϸ��̼�
/// </summary>
/// <param name="area">���� �����ؾ��ϴ� ����</param>
/// <param name="mainCamera">ī�޶� ����ü</param>
/// <param name="player">�÷��̾� ����ü</param>
/// <param name="deltaTime">��ŸŸ��</param>
void GDI_OccupiedArea(occupiedArea* area, Camera* mainCamera, Player* player, double deltaTime);

/// <summary>
/// �ΰ��� ��濡 �ǹ��� �׷��ִ� �Լ�
/// </summary>
/// <param name="buildingLocal"></param>
/// <param name="mainCamera"></param>
void GDI_Building(RECT* buildingLocal, Camera* mainCamera);


void GDI_GamePopupUI(float backgroundVolume, float effectVolume, BOOL isUp);
#pragma endregion

void GDI_OverHeatExplode(Player* player, Camera* mainCamera, double deltaTime);

#pragma region Player
/// <summary>
/// ���� �������� ��ȯ���ִ� �Լ�
/// </summary>
/// <param name="player"></param>
/// <param name="area"></param>
/// <param name="mainCamera"></param>
void GDI_FindArea(Player* player, occupiedArea* area, Camera* mainCamera);

/// <summary>
/// �÷��̾� ��ü�� �׷��ִ� �Լ�
/// </summary>
/// <param name="player"></param>
/// <param name="mainCamera"></param>
/// <param name="deltaTime"></param>
void GDI_PlayerObejct(Player* player, Camera* mainCamera, double deltaTime);


/// <summary>
/// �÷��̾ �׷��ִ� �Լ�
/// </summary>
/// <param name="deltaTime"></param>
void GDI_Player(Player* player, double deltaTime);

/// <summary>
/// �÷��̾��� ���⸦ �׷��ִ� �Լ�
/// </summary>
/// <param name="player"></param>
/// <param name="deltaTime"></param> 
void GDI_PlayerWeapon(Player* player, double deltaTime);
#pragma endregion

#pragma region Enemy
/// <summary>
/// �� ��ü�� �׷��ִ� �Լ�
/// </summary>
/// <param name="enemy"></param>
/// <param name="mainCamera"></param>
void GDI_EnemyObject(Enemy (*enemyPool)[ENEMYPOOL_MAX], Camera* mainCamera, double deltaTime);

/// <summary>
/// ���� �׷��ִ� �Լ�
/// </summary>
void GDI_Enemy(Enemy* enemy, double deltaTime);

/// <summary>
/// �� ���⸦ �׷��ִ� �Լ�
/// </summary>
/// <param name="enemy"></param>
void GDI_EnemyWeapon(Enemy* enemy, double deltaTime);
#pragma endregion

/// <summary>
/// �Ѿ��� �׷��ִ� �Լ�
/// </summary>
/// <param name="bulletPool"></param>
/// <param name="mainCamera"></param>
void GDI_Bullet(Bullet (*bulletPool)[BULLETPOOL_MAX], Camera* mainCamera);

/// <summary>
/// ���콺 ��ǥ�� ���� ����(���� ǥ��)�� �׷��ִ� �Լ�
/// </summary>
/// <param name="mousePosX">���콺�� X ��ǥ</param>
/// <param name="mousePosY">���콺�� Y ��ǥ</param>
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




