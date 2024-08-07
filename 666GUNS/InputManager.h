#pragma once
#include <Windows.h>

// 전체 입력받는 키의 갯수
#define KEYCOUNT 8

// 키보드입력의 상태
typedef enum KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY,

}KEY_STATE;

//마우스 입력 상태
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

// 입력받는 키들
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

// 입력 초기화하기
void KeyManager_Init();
void LeftMouseManager_Init();
void RightMouseManager_Init();

// 입력 갱신하기
void KeyManager_Update();
void LeftMouseManager_Update();
void RightMouseManager_Update();

// 입력상태 반환하기
KEY_STATE KeyManager_GetKeyState(KEY _key);
MOUSE_STATE LeftMouseManager_GetMouseState();
MOUSE_STATE RightMouseManager_GetMouseState();

