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

// Ű���� �Է� �ʱ�ȭ�ϱ�
void KeyManager_Init()
{
	for (int i = 0; i < KEYCOUNT; ++i)
	{
		g_KeyManager.m_key[i].k_prevPush = FALSE;
		g_KeyManager.m_key[i].k_state = NONE;
	}
}

// ���� ���콺 �Է� �ʱ�ȭ�ϱ�
void LeftMouseManager_Init()
{
	g_MouseManager.Lm_Clicked = FALSE;
	g_MouseManager.Lm_state = LMOUSEUP;
}

//������ ���콺 �Է� �ʱ�ȭ�ϱ�
void RightMouseManager_Init()
{
	g_MouseManager.Rm_Clicked = FALSE;
	g_MouseManager.Rm_state = RMOUSEUP;

}

// Ű���� �Է� ���º��� ������Ʈ�ϱ�
void KeyManager_Update()
{
	HWND hWnd = GetFocus();

	if (NULL != hWnd)
	{
		for (int i = 0; i < KEYCOUNT; ++i)
		{
			// Ű�� ������ ��
			if (GetAsyncKeyState(GetKey[i]) & 0x8001)
			{
				// ���� ������ �ʾҴٸ�
				if (g_KeyManager.m_key[i].k_prevPush)
				{
					// ���� ���� ����
					g_KeyManager.m_key[i].k_state = HOLD;
				}
				// ������ �־��ٸ�
				else
				{
					// �������ִ� ����
					g_KeyManager.m_key[i].k_state = TAP;
				}
				// ���� ������ �ִ� ���·� ��ȯ
				g_KeyManager.m_key[i].k_prevPush = TRUE;
			}
			// Ű�� ������ �ʾ��� ��
			else
			{
				// ������ �ִ� ���¶��
				if (g_KeyManager.m_key[i].k_prevPush)
				{
					// ��� �� ����
					g_KeyManager.m_key[i].k_state = AWAY;
				}
				// ������ ���� �ʾҴٸ�
				else
				{
					// �ƹ��͵� ���� ���� ����
					g_KeyManager.m_key[i].k_state = NONE;
				}
				// ���� ������ ���� ���� ���·� ��ȯ
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
		/// ���� ���콺�� ������ ��
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			// ���� ������ �ʾҴٸ�
			if (g_MouseManager.Lm_Clicked)
			{
				// ���� ���� ����
				g_MouseManager.Lm_state = LMOUSEJUSTDOWN;
			}
			// ������ �־��ٸ�
			else
			{
				// �������ִ� ����
				g_MouseManager.Lm_state = LMOUSEDOWN;
			}
			// ���� ������ �ִ� ���·� ��ȯ
			g_MouseManager.Lm_Clicked = TRUE;
		}
		// ���콺�� ������ �ʾ��� ��
		else
		{
			// ������ �ִ� ���¶��
			if (g_MouseManager.Lm_Clicked)
			{
				// ��� �� ����
				g_MouseManager.Lm_state = LMOUSEJUSTUP;
			}
			// ������ ���� �ʾҴٸ�
			else
			{
				// �ƹ��͵� ���� ���� ����
				g_MouseManager.Lm_state = LMOUSEUP;
			}
			// ���� ������ ���� ���� ���·� ��ȯ
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
		/// ������ ���콺�� ������ ��
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			// ���� ������ �ʾҴٸ�
			if (g_MouseManager.Rm_Clicked)
			{
				// ���� ���� ����
				g_MouseManager.Rm_state = RMOUSEJUSTDOWN;
			}
			// ������ �־��ٸ�
			else
			{
				// �������ִ� ����
				g_MouseManager.Rm_state = RMOUSEDOWN;
			}
			// ���� ������ �ִ� ���·� ��ȯ
			g_MouseManager.Rm_Clicked = TRUE;
		}
		// ���콺�� ������ �ʾ��� ��
		else
		{
			// ������ �ִ� ���¶��
			if (g_MouseManager.Rm_Clicked)
			{
				// ��� �� ����
				g_MouseManager.Rm_state = RMOUSEJUSTUP;
			}
			// ������ ���� �ʾҴٸ�
			else
			{
				// �ƹ��͵� ���� ���� ����
				g_MouseManager.Rm_state = RMOUSEUP;
			}
			// ���� ������ ���� ���� ���·� ��ȯ
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


// Ű���� �Է»��� ��ȯ�ϱ�
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





