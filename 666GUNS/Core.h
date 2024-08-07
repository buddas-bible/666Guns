#pragma once
#include <windows.h>

void Engine_Init();

void Engine_StartCheckTime();
void Engine_EndCheckTime();
double Engine_GetDeltaTime();
LARGE_INTEGER Engine_GetFrequency();

void Engine_Input();					// ������ �Է��� ó���ϴ� �Լ�

void Engine_MenuInit();					//
void Engine_MenuLoop();					// ������ �־�� �޴��� ������ ������ ���� �ѹ��� ȣ���ϴ� �Լ�
void Engine_MenuUpdate();
void Engine_MenuRender();

void Engine_GameInit();					// ������ ������ �� �� �ѹ��� ȣ��Ǵ� �Լ�
void Engine_GameLoop();					// ������ �־�� ������ ������ ������ ���� �ѹ��� ȣ���ϴ� �Լ�
void Engine_GameUpdate();				// �� ������ ���� ������ ���۵� // FixedUpdate()�� �ð��� ���� ���Ǹ� ȣ����� ����
void Engine_GameRender();				// �� ������ ���� �������� ����

void Engine_EndingInit();				// ������ �ʱ�ȭ
void Engine_EndingLoop();				// ���� ����
void Engine_EndingUpdate();				// ���� ������Ʈ
void Engine_EndingRender();				// ���� ������

void Engine_CreditInit();				// ũ������ �ʱ�ȭ
void Engine_CreditLoop();				// ũ���� ����
void Engine_CreditUpdate();				// ũ���� ������Ʈ
void Engine_CreditRender();				// ũ���� ������

/// Player�� Update
void Player_Update();

/// <summary>
/// �� ��ü�� ���� Update
/// </summary>
void Enemy_Update();

/// ����ü�� Update
void Bullet_Update();

/// �浹üũ�� ���� Update
void Collision_Update();


int Engine_GetSceneNumber();			// ���� ���� � ������ ��ȯ�ϴ� �Լ�
void Engine_SetSceneNumber(int sceneNumber);	// ���� ���� �����ϴ� �Լ�

BOOL IsSceneJustChanged();

void SetCheatMode(BOOL status);

