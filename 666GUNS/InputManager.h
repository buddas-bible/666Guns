#pragma once
#include <Windows.h>

// ��ü �Է¹޴� Ű�� ����
#define KEYCOUNT 8

// Ű�����Է��� ����
typedef enum KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY,

}KEY_STATE;

//���콺 �Է� ����
typedef enum MOUSE_STATE
{
	LMOUSEUP,
	LMOUSEJUSTDOWN,
	LMOUSEDOWN,
	LMOUSEJUSTUP,

	RMOUSEUP,
	RMOUSEJUSTDOWN,
	RMOUSEDOWN,
	RMOUSEJUSTUP

}MOUSE_STATE;

// �Է¹޴� Ű��
typedef enum KEY
{
	W,
	A,
	S,
	D,
	Q,
	E,

	SPACE,
	ESC,

	LAST,
}KEY;

typedef struct KEYInfo
{
	KEY_STATE k_state;
	BOOL k_prevPush;
}KeyInfo;

typedef struct MOUSEInfo
{
	MOUSE_STATE Lm_state;
	MOUSE_STATE Rm_state;
	BOOL Lm_Clicked;
	BOOL Rm_Clicked;
}MouseInfo;

typedef struct KEY_Manager
{
	KeyInfo m_key[KEYCOUNT];
}Key_Manager;

static Key_Manager g_KeyManager;
static MouseInfo g_MouseManager;

// �Է� �ʱ�ȭ�ϱ�
void KeyManager_Init();
void LeftMouseManager_Init();
void RightMouseManager_Init();

// �Է� �����ϱ�
void KeyManager_Update();
void LeftMouseManager_Update();
void RightMouseManager_Update();

// �Է»��� ��ȯ�ϱ�
KEY_STATE KeyManager_GetKeyState(KEY _key);
MOUSE_STATE LeftMouseManager_GetMouseState();
MOUSE_STATE RightMouseManager_GetMouseState();

