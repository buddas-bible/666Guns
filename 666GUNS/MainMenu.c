#include "SoundManager.h"
#include "InputManager.h"
#include "Core.h"
#include "GameDefine.h"
#include "MainMenu.h"
#include "GDIEngine.h"
#include "GameManager.h"

///���θ޴� ��ư
//��ư ����: ���� 635 ���� 180 ���� 195~200
extern RECT Start_Button = { 1105, 450, 1740, 630 }; // �簢�� ���� ����
extern RECT Credit_Button = { 1105, 640, 1740, 820 }; // �簢�� ���� ����
extern RECT Exit_Button = { 1105, 835, 1740, 1015 }; // �簢�� ���� ����

/// ���θ޴� �˾���ư
// (80,200)
//��ư ����: ���� 90 ���� 88
extern RECT Option_Button = { 1820, 10, 1910, 98 }; // �簢�� ���� ����

extern RECT BGMVolUp_Button = { 710, 380, 838, 508 }; // �簢�� ���� ����
extern RECT BGMVolDown_Button = { 280, 380, 408, 508 }; // �簢�� ���� ����

extern RECT EffectVolUp_Button = { 710, 580, 838, 708 }; // �簢�� ���� ����
extern RECT EffectVolDown_Button = { 280, 580, 408, 708 }; // �簢�� ���� ����

//(450,170) +370 -30
/// �ΰ��� �˾���ư
extern RECT MainMenu_Button = { 550, 720, 900, 820 }; // �簢�� ���� ����
extern RECT Continue_Button = { 960, 720, 1310, 820  }; // �簢�� ���� ����

extern RECT IG_BGMVolUp_Button = { 1080, 350, 1208, 478 }; // �簢�� ���� ����
extern RECT IG_BGMVolDown_Button = { 650, 350, 778, 478 }; // �簢�� ���� ����

extern RECT IG_EffectVolUp_Button = { 1080, 550, 1208, 678 }; // �簢�� ���� ����
extern RECT IG_EffectVolDown_Button = { 650, 550, 778, 678 }; // �簢�� ���� ����


extern RECT GameOver_Click = { 0, 0, 1920, 1080 }; // �簢�� ���� ����

//�˾� �÷���
BOOL PopUpflag = FALSE;
BOOL PopUpflag2 = FALSE;
BOOL PopUpflag3 = FALSE;

//Ȧ�� �÷���
BOOL Holdingflag4 = FALSE;
BOOL Holdingflag5 = FALSE;
BOOL Holdingflag6 = FALSE;
BOOL Holdingflag7 = FALSE;

BOOL Clickflag8 = FALSE;
BOOL Clickflag9 = FALSE;
BOOL Clickflag10 = FALSE;

// 0000 ������ ����, �ȴ���
// 8000 ������ ����, ����
// 8001 ������ �ְ�, ����
// 0001 ������ �ְ�, �ȴ���

void MainMenuButtons(HWND hWnd, POINT Mousept)
{
	// ���콺�� ������ ��
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{		
		// ���� ��ư
		if (PtInRect(&Start_Button, Mousept))
		{
			ClickSound(4);
			Sleep(500);
			PopUpflag = FALSE;
			PopUpflag2 = FALSE;
			PopUpflag3 = FALSE;
			Engine_SetSceneNumber(Scene_Game);
		}
		// ���� ��ư
		if (PtInRect(&Exit_Button, Mousept))
		{
			ClickSound(4);
			Sleep(500);
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		}
		if (PtInRect(&Credit_Button, Mousept))
		{
			ClickSound(4);
		}
	}
	else
	{
		if (PtInRect(&Start_Button, Mousept))
		{
			if (Holdingflag4 == FALSE)
			{
				HoldSound();
				Holdingflag4 = TRUE;
			}
		}
		else
		{
			Holdingflag4 = FALSE;
		}

		if (PtInRect(&Credit_Button, Mousept))
		{
			if (Holdingflag5 == FALSE)
			{
				HoldSound();
				Holdingflag5 = TRUE;
			}
		}
		else
		{
			Holdingflag5 = FALSE;
		}

		if (PtInRect(&Exit_Button, Mousept))
		{
			if (Holdingflag6 == FALSE)
			{
				HoldSound();
				Holdingflag6 = TRUE;
			}
		}
		else
		{
			Holdingflag6 = FALSE;
		}

		if (PtInRect(&Option_Button, Mousept))
		{
			if (Holdingflag7 == FALSE)
			{
				HoldSound();
				Holdingflag7 = TRUE;
			}
		}
		else
		{
			Holdingflag7 = FALSE;
		}
	}

	if (LeftMouseManager_GetMouseState() == LMOUSEDOWN)
	{
		if (PtInRect(&Option_Button, Mousept))
		{
			ClickSound(4);
			// ���⼭ ���������� ���� ������
			if (PopUpflag == FALSE)
			{
				PopUpflag = TRUE;
				PopUpflag3 = FALSE;
			}
			else
			{
				PopUpflag = FALSE;
			}
		}

		if (PtInRect(&Credit_Button, Mousept))
		{
			// ���⼭ 
			if (PopUpflag3 == FALSE)
			{
				PopUpflag3 = TRUE;
				PopUpflag = FALSE;
			}
			else
			{
				PopUpflag3 = FALSE;
			}
			/*if (PopUpflag)
			{
				PopUpUI();
			}*/
		}
		
		// �ɼ� �˾� ������ ��
		if (PopUpflag)
		{
			if (LeftMouseManager_GetMouseState() == LMOUSEDOWN)
			{
				// ���� ��ư
				if (PtInRect(&BGMVolUp_Button, Mousept))
				{
					ClickSound(5);
					BGMVolumeUp();
				}
				if (PtInRect(&BGMVolDown_Button, Mousept))
				{
					ClickSound(5);
					BGMVolumeDown();
				}
				if (PtInRect(&EffectVolUp_Button, Mousept))
				{
					ClickSound(5);
					EffectVolumeUp();
				}
				if (PtInRect(&EffectVolDown_Button, Mousept))
				{
					ClickSound(5);
					EffectVolumeDown();
				}
			}
		}
	}
}

void KeyBoard_popUp(POINT Mousept)
{
	if (KeyManager_GetKeyState(ESC) == TAP)
	{
		
		ClickSound(4);
		// ���⼭ ���������� ���� ������
		if (PopUpflag2 == FALSE)
		{
			GameManager_Pause();
			PopUpflag2 = TRUE;
		}
		else
		{
			GameManager_Resume();
			PopUpflag2 = FALSE;
		}
	}

	// �˾� ���� �Ǿ��� ��
	if (PopUpflag2)
	{
		// ���콺�� ������ ��
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
		{
			// ���� ��ư
			if (PtInRect(&MainMenu_Button, Mousept))
			{
				ClickSound(4);
				Sleep(500);
				GameManager_Resume();
				Engine_SetSceneNumber(Scene_Menu);
			}

			if (PtInRect(&Continue_Button, Mousept))
			{
				ClickSound(4);
				GameManager_Resume();
				PopUpflag2 = FALSE;
				
			}
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
		{
			// ���� ��ư
			if (PtInRect(&IG_BGMVolUp_Button, Mousept))
			{
				ClickSound(5);
				Sleep(100);
				BGMVolumeUp();
			}
			if (PtInRect(&IG_BGMVolDown_Button, Mousept))
			{
				ClickSound(5);
				Sleep(100);
				BGMVolumeDown();
			}
			if (PtInRect(&IG_EffectVolUp_Button, Mousept))
			{
				ClickSound(5);
				Sleep(100);
				EffectVolumeUp();
			}
			if (PtInRect(&IG_EffectVolDown_Button, Mousept))
			{
				ClickSound(5);
				Sleep(100);
				EffectVolumeDown();
			}
		}
	}
	
}

void GameOver_MouseClick()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ClickSound(4);
		// ���⼭ ���������� ���� ������
		if (Clickflag8 == FALSE)
		{
			Clickflag8 = TRUE;
		}
		else
		{
			Clickflag8 = FALSE;
		}

		if (Clickflag8)
		{
			SoundStop(2);
			Sleep(500);
			Engine_SetSceneNumber(Scene_Menu);
			Clickflag8 = FALSE;
		}
	}
	
		
}

void GameClear_MouseClick()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ClickSound(4);
		// ���⼭ ���������� ���� ������
		if (Clickflag9 == FALSE)
		{
			Clickflag9 = TRUE;
		}
		else
		{
			Clickflag9 = FALSE;
		}

		if (Clickflag9)
		{
			Sleep(500);
			Engine_SetSceneNumber(Scene_Credit);
			Clickflag9 = FALSE;
		}
	}
}

void GameClear_CreditMouseClick()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ClickSound(4);
		// ���⼭ ���������� ���� ������
		if (Clickflag10 == FALSE)
		{
			Clickflag10 = TRUE;
		}
		else
		{
			Clickflag10 = FALSE;
		}

		if (Clickflag10)
		{
			Sleep(500);
			Engine_SetSceneNumber(Scene_Menu);
			Clickflag10 = FALSE;
		}
	}
}

BOOL GetPopUpflag()
{
	return PopUpflag;
}

BOOL GetPopUpflag2()
{

	return PopUpflag2;
}

BOOL GetPopUpflag3()
{
	return PopUpflag3;
}

BOOL Get_GameOver_PopUpflag()
{
	return Clickflag8;
}

BOOL Get_GameClear_PopUpflag()
{
	return Clickflag9;
}

int GetHoldButton()
{
	if (Holdingflag4)
	{
		return 1;
	}

	if (Holdingflag5)
	{
		return 2;
	}

	if (Holdingflag6)
	{
		return 3;
	}

	if (Holdingflag7)
	{
		return 4;
	}

	return 0;
}
