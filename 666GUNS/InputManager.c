#include "InputManager.h"
#include "SoundManager.h"
#include "Player.h"

int GetKey[KEYCOUNT] =
{
	'W',
	'A',
	'S',
	'D',
	'Q',
	'E',

	VK_SPACE,
	VK_ESCAPE
};

// 키보드 입력 초기화하기
void KeyManager_Init()
{
	for (int i = 0; i < KEYCOUNT; ++i)
	{
		g_KeyManager.m_key[i].k_prevPush = FALSE;
		g_KeyManager.m_key[i].k_state = NONE;
	}
}

// 왼쪽 마우스 입력 초기화하기
void LeftMouseManager_Init()
{
	g_MouseManager.Lm_Clicked = FALSE;
	g_MouseManager.Lm_state = LMOUSEUP;
}

//오른쪽 마우스 입력 초기화하기
void RightMouseManager_Init()
{
	g_MouseManager.Rm_Clicked = FALSE;
	g_MouseManager.Rm_state = RMOUSEUP;

}

// 키보드 입력 상태별로 업데이트하기
void KeyManager_Update()
{
	HWND hWnd = GetFocus();

	if (NULL != hWnd)
	{
		for (int i = 0; i < KEYCOUNT; ++i)
		{
			// 키를 눌렀을 때
			if (GetAsyncKeyState(GetKey[i]) & 0x8001)
			{
				// 전에 누르지 않았다면
				if (g_KeyManager.m_key[i].k_prevPush)
				{
					// 누른 직후 상태
					g_KeyManager.m_key[i].k_state = HOLD;
				}
				// 누르고 있었다면
				else
				{
					// 누르고있는 상태
					g_KeyManager.m_key[i].k_state = TAP;
				}
				// 전에 누르고 있던 상태로 전환
				g_KeyManager.m_key[i].k_prevPush = TRUE;
			}
			// 키를 누르지 않았을 때
			else
			{
				// 누르고 있는 상태라면
				if (g_KeyManager.m_key[i].k_prevPush)
				{
					// 방금 땐 상태
					g_KeyManager.m_key[i].k_state = AWAY;
				}
				// 누르고 있지 않았다면
				else
				{
					// 아무것도 하지 않은 상태
					g_KeyManager.m_key[i].k_state = NONE;
				}
				// 전에 누르고 있지 않은 상태로 전환
				g_KeyManager.m_key[i].k_prevPush = FALSE;
			}
		}
	}

	else
	{
		for (int i = 0; i < KEYCOUNT; ++i)
		{
			g_KeyManager.m_key[i].k_prevPush = FALSE;
			if (TAP == g_KeyManager.m_key[i].k_state || HOLD == g_KeyManager.m_key[i].k_state)
			{
				g_KeyManager.m_key[i].k_state = AWAY;
			}
			else if (AWAY == g_KeyManager.m_key[i].k_state)
			{
				g_KeyManager.m_key[i].k_state = NONE;
			}
		}
	}
}

void LeftMouseManager_Update()
{
	HWND hWnd = GetFocus();

	if (NULL != hWnd)
	{
		/// 왼쪽 마우스를 눌렀을 때
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			// 전에 누르지 않았다면
			if (g_MouseManager.Lm_Clicked)
			{
				// 누른 직후 상태
				g_MouseManager.Lm_state = LMOUSEJUSTDOWN;
			}
			// 누르고 있었다면
			else
			{
				// 누르고있는 상태
				g_MouseManager.Lm_state = LMOUSEDOWN;
			}
			// 전에 누르고 있던 상태로 전환
			g_MouseManager.Lm_Clicked = TRUE;
		}
		// 마우스를 누르지 않았을 때
		else
		{
			// 누르고 있는 상태라면
			if (g_MouseManager.Lm_Clicked)
			{
				// 방금 땐 상태
				g_MouseManager.Lm_state = LMOUSEJUSTUP;
			}
			// 누르고 있지 않았다면
			else
			{
				// 아무것도 하지 않은 상태
				g_MouseManager.Lm_state = LMOUSEUP;
			}
			// 전에 누르고 있지 않은 상태로 전환
			g_MouseManager.Lm_Clicked = FALSE;
		}
	}
	else
	{
		g_MouseManager.Lm_Clicked = FALSE;
		if (LMOUSEJUSTDOWN == g_MouseManager.Lm_state || LMOUSEDOWN == g_MouseManager.Lm_state)
		{
			g_MouseManager.Lm_state = LMOUSEJUSTUP;
		}
		else if (LMOUSEJUSTUP == g_MouseManager.Lm_state)
		{
			g_MouseManager.Lm_state = LMOUSEUP;
		}
	}
}

void RightMouseManager_Update()
{
	HWND hWnd = GetFocus();

	if (NULL != hWnd)
	{
		/// 오른쪽 마우스를 눌렀을 때
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			// 전에 누르지 않았다면
			if (g_MouseManager.Rm_Clicked)
			{
				// 누른 직후 상태
				g_MouseManager.Rm_state = RMOUSEJUSTDOWN;
			}
			// 누르고 있었다면
			else
			{
				// 누르고있는 상태
				g_MouseManager.Rm_state = RMOUSEDOWN;
			}
			// 전에 누르고 있던 상태로 전환
			g_MouseManager.Rm_Clicked = TRUE;
		}
		// 마우스를 누르지 않았을 때
		else
		{
			// 누르고 있는 상태라면
			if (g_MouseManager.Rm_Clicked)
			{
				// 방금 땐 상태
				g_MouseManager.Rm_state = RMOUSEJUSTUP;
			}
			// 누르고 있지 않았다면
			else
			{
				// 아무것도 하지 않은 상태
				g_MouseManager.Rm_state = RMOUSEUP;
			}
			// 전에 누르고 있지 않은 상태로 전환
			g_MouseManager.Rm_Clicked = FALSE;
		}
	}
	else
	{
		g_MouseManager.Rm_Clicked = FALSE;
		if (LMOUSEJUSTDOWN == g_MouseManager.Rm_state || LMOUSEDOWN == g_MouseManager.Rm_state)
		{
			g_MouseManager.Rm_state = LMOUSEJUSTUP;
		}
		else if (LMOUSEJUSTUP == g_MouseManager.Rm_state)
		{
			g_MouseManager.Rm_state = LMOUSEUP;
		}
	}
}


// 키보드 입력상태 반환하기
KEY_STATE KeyManager_GetKeyState(KEY _key)
{
	return g_KeyManager.m_key[(int)_key].k_state;
}


MOUSE_STATE LeftMouseManager_GetMouseState()
{
	return g_MouseManager.Lm_state;
}

MOUSE_STATE RightMouseManager_GetMouseState()
{
	return g_MouseManager.Rm_state;
}





