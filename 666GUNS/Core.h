#pragma once
#include <windows.h>

void Engine_Init();

void Engine_StartCheckTime();
void Engine_EndCheckTime();
double Engine_GetDeltaTime();
LARGE_INTEGER Engine_GetFrequency();

void Engine_Input();					// 엔진의 입력을 처리하는 함수

void Engine_MenuInit();					//
void Engine_MenuLoop();					// 엔진에 넣어둔 메뉴의 루프를 순서에 맞춰 한번씩 호출하는 함수
void Engine_MenuUpdate();
void Engine_MenuRender();

void Engine_GameInit();					// 게임이 시작할 때 단 한번만 호출되는 함수
void Engine_GameLoop();					// 엔진에 넣어둔 게임의 루프를 순서에 맞춰 한번씩 호출하는 함수
void Engine_GameUpdate();				// 매 프레임 마다 수행할 동작들 // FixedUpdate()에 시간이 많이 사용되면 호출되지 않음
void Engine_GameRender();				// 매 프레임 마다 렌더링을 수행

void Engine_EndingInit();				// 엔딩씬 초기화
void Engine_EndingLoop();				// 엔딩 루프
void Engine_EndingUpdate();				// 엔딩 업데이트
void Engine_EndingRender();				// 엔딩 렌더러

void Engine_CreditInit();				// 크레딧씬 초기화
void Engine_CreditLoop();				// 크레딧 루프
void Engine_CreditUpdate();				// 크레딧 업데이트
void Engine_CreditRender();				// 크레딧 렌더러

/// Player의 Update
void Player_Update();

/// <summary>
/// 적 개체에 대한 Update
/// </summary>
void Enemy_Update();

/// 투사체의 Update
void Bullet_Update();

/// 충돌체크를 위한 Update
void Collision_Update();


int Engine_GetSceneNumber();			// 현재 씬이 어떤 씬인지 반환하는 함수
void Engine_SetSceneNumber(int sceneNumber);	// 현재 씬을 세팅하는 함수

BOOL IsSceneJustChanged();

void SetCheatMode(BOOL status);

