#pragma once
#include <Windows.h>

void MainMenuButtons(HWND hWnd, POINT Mousept);

void KeyBoard_popUp(POINT Mousept);

// Ȧ�� ���� ��ȯ
int GetHoldButton();

// Scene ���� ���콺 �Է»��� ��ȯ
void GameOver_MouseClick();
void GameClear_MouseClick();
void GameClear_CreditMouseClick();

// �˾� ���� ��ȯ
BOOL GetPopUpflag();
BOOL GetPopUpflag2();
BOOL GetPopUpflag3();
BOOL Get_GameOver_PopUpflag();
BOOL Get_GameClear_PopUpflag();
