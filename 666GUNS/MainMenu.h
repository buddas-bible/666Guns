#pragma once
#include <Windows.h>

void MainMenuButtons(HWND hWnd, POINT Mousept);

void KeyBoard_popUp(POINT Mousept);

// 홀딩 상태 반환
int GetHoldButton();

// Scene 별로 마우스 입력상태 반환
void GameOver_MouseClick();
void GameClear_MouseClick();
void GameClear_CreditMouseClick();

// 팝업 상태 반환
BOOL GetPopUpflag();
BOOL GetPopUpflag2();
BOOL GetPopUpflag3();
BOOL Get_GameOver_PopUpflag();
BOOL Get_GameClear_PopUpflag();
