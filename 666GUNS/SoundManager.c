#include <conio.h>
#include <fmod.h>
#include "GameManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "MainMenu.h"

#define MAXSOUND 100

//��ư ����: ���� 450 ���� 130
extern RECT Credit_Button; // �簢�� ���� ����
extern RECT Start_Button; // �簢�� ���� ����
extern RECT Exit_Button; // �簢�� ���� ����

//��ư ����: ���� 128 ���� 128
extern RECT Option_Button; // �簢�� ���� ����
extern RECT BGMVolUp_Button; // �簢�� ���� ����
extern RECT BGMVolDown_Button; // �簢�� ���� ����

FMOD_SYSTEM* g_FMODSystem = NULL; //���� �ý��� ������ ����
/// �ϳ��� FMOD_SOUND�� �ټ��� FMOD_CHANNEL���� ���� ��� ����
FMOD_SOUND* pFMODSound[MAXSOUND]; //���� ��ü ������ ���� (�ϳ��� �ε����� �ϳ��� ���� ����)
FMOD_CHANNEL* ChannelName[MAXSOUND]; //���带 ����ϴ� ä�� ���� ����
FMOD_BOOL isPaused = 0;
FMOD_BOOL isPlaying = 0;
float BGMVolume = 0.5f;
float EffectVolume = 0.5f;

void SoundSystemInput()
{
	/// <summary>
	/// �ѹ��� ȣ���ؾ���
	FMOD_System_Create(&g_FMODSystem); //���� �ý����� ����
	FMOD_System_Init(g_FMODSystem, 32, FMOD_INIT_NORMAL, NULL); //���� �ý��� �ʱ�ȭ
	/// </summary>
}


// ���� ���� �Է� (ä�ε� ����)
/// �޴���� 0
/// �÷��̾� �̵� 1
/// ��ư Ŭ�� 2
/// �ΰ��� ��� 3
/// �� �߻� 4
/// �� �߻�2 5

//FMOD_DEFAULT -> ���� 1�� ��� (ȿ����)
//FMOD_LOOP_NORMAL -> ���� �ݺ� ��� (���)

// ���� ���� ���� �Է�
void LoadSoundLoopFile(const char* filePath, int num)
{
	//if (pFMODSound != 0)
	//	FMOD_Sound_Release(pFMODSound[0]); //���� ��ü�� ����
	FMOD_System_CreateSound(g_FMODSystem, filePath, FMOD_LOOP_NORMAL, 0, &(pFMODSound[num]));
}

// �ѹ� ��� ���� ���� �Է�
void LoadSoundDefaultFile(const char* filePath, int num)
{
	//if (pFMODSound != 0)
	//	FMOD_Sound_Release(pFMODSound[0]); //���� ��ü�� ����
	FMOD_System_CreateSound(g_FMODSystem, filePath, FMOD_DEFAULT, 0, &(pFMODSound[num]));
}

// ���� ���
void Play_Sound(int num)
{
	if (ChannelName[num] != 0)
		FMOD_Channel_IsPlaying(ChannelName[num], &isPlaying);
	if (isPlaying != 0)
		FMOD_Channel_Stop(ChannelName[num]); //���� ����� ����
	FMOD_System_PlaySound(g_FMODSystem, pFMODSound[num], NULL, isPaused, &ChannelName[num]);
}

// ��� �����
void LoadBGMSound()
{
	LoadSoundLoopFile(GAME_START, 0);
	LoadSoundLoopFile(IN_GAME, 1);
	LoadSoundLoopFile(END_GAME, 2);
	LoadSoundDefaultFile(ENDING_BGM, 18);
}

// ȿ���� �����
void LoadEffectSound()
{
	LoadSoundDefaultFile(IN_GAME_POPUP, 3);

	LoadSoundDefaultFile(TITLE_BUTTON_CLICK, 4);
	LoadSoundDefaultFile(POPUP_BUTTON_CLICK, 5);

	LoadSoundLoopFile(PLAYER_WALK, 6);
	LoadSoundDefaultFile(PLAYER_DASH, 7);
	LoadSoundDefaultFile(PLAYER_DAMAGED, 8);
	LoadSoundDefaultFile(OVERHEAT, 9);

	LoadSoundDefaultFile(WEAPON_CHANGE, 10);
	LoadSoundLoopFile(GUN_HANDGUN, 11);
	LoadSoundLoopFile(GUN_RIFLE, 12);
	LoadSoundLoopFile(GUN_LASER, 13);
	LoadSoundLoopFile(GUN_SHOTGUN, 14);
	LoadSoundDefaultFile(GUN_HANDGUN, 25);
	LoadSoundDefaultFile(GUN_RIFLE, 26);
	LoadSoundDefaultFile(GUN_LASER, 27);
	LoadSoundDefaultFile(GUN_SHOTGUN, 28);

	LoadSoundDefaultFile(ENEMY_DAMAGED, 15);
	LoadSoundDefaultFile(ENEMY_SHOOT, 16);
	LoadSoundDefaultFile(ENEMY_SWING, 17);

	LoadSoundDefaultFile(OBJECT_WALL, 19);
	LoadSoundDefaultFile(OBJECT_BOX1, 20);
	LoadSoundDefaultFile(OBJECT_BOX2, 21);
	LoadSoundDefaultFile(OBJECT_CAR, 22);

	LoadSoundDefaultFile(OCCUPATION, 23);
	LoadSoundDefaultFile(ENDING_LINE, 24);
}

void HoldSound()
{
	FMOD_System_PlaySound(g_FMODSystem, pFMODSound[5], NULL, isPaused, &(ChannelName[5]));
	SetEffectVolume(5);
}

// ��Ȳ�� ���� BGM (0 ~ 2) 
void BGMSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetBGMVolume(ChannelNum);
}

// ���� ���� 29
void EndingBGMSound()
{
	FMOD_System_PlaySound(g_FMODSystem, pFMODSound[18], NULL, isPaused, &(ChannelName[18]));
	FMOD_Channel_SetVolume(ChannelName[18], 1.0);
}

// �÷��̾� ���¿� ���� ���� (6 ~ 9)
void PlayerSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}

// �� ���¿� ���� ����(15 ~ 18)
void EnemySound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}

// ������Ʈ ���¿� ���� ���� (19 ~ 22)
void ObjectSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}

// ��Ȳ�� ���� Ŭ�� ���� (4 ~ 5)
void ClickSound(int BtnNum)
{
	Play_Sound(BtnNum);
	SetEffectVolume(BtnNum);
}

// ���� ��Ȳ�� ���� ���� (���� 10 ~ 14 / ���� 25 ~ 28) 
// 10�� ������ ���ⱳü ����
void ShootSound(int GunNum)
{
	Play_Sound(GunNum);
	SetEffectVolume(GunNum);
}

void ShotGunSound()
{
	Play_Sound(14);
	FMOD_Channel_SetVolume(ChannelName[14], 1.0f);
}

// �ΰ��� ���¿� ���� ���� (3, 23 ~ 24)
// �ΰ��� �˾� 3 ���� 23 ������ 24
void InGameSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}


//���� ����� �Ͻ�����
void SoundPaused()
{
	isPaused = !isPaused;
	FMOD_Channel_SetPaused(ChannelName[0], isPaused); 
}

//���� ����� ����
void SoundStop(int ChannelNum)
{
	FMOD_Channel_Stop(ChannelName[ChannelNum]); 
}

// ��� ���� ����
void SetBGMVolume(int ChannelNum)
{
	FMOD_Channel_SetVolume(ChannelName[ChannelNum], BGMVolume);
}

// ȿ���� ���� ����
void SetEffectVolume(int ChannelNum)
{
	FMOD_Channel_SetVolume(ChannelName[ChannelNum], EffectVolume);
}

//����� ������ ����
void BGMVolumeUp()
{
	BGMVolume += 0.1f;

	if (BGMVolume >= 1.0f)
	{
		BGMVolume = 1.0f;
	}

	FMOD_Channel_SetVolume(ChannelName[0], BGMVolume);
	FMOD_Channel_SetVolume(ChannelName[1], BGMVolume);
}

void BGMVolumeDown()
{
	BGMVolume -= 0.1f;

	if (BGMVolume <= 0.0f)
	{
		BGMVolume = 0.0f;
	}

	FMOD_Channel_SetVolume(ChannelName[0], BGMVolume);
	FMOD_Channel_SetVolume(ChannelName[1], BGMVolume);
}


//ȿ������ ������ ����
void EffectVolumeUp()
{
	EffectVolume += 0.1f;

	if (EffectVolume >= 1.0f)
	{
		EffectVolume = 1.0f;
	}
	FMOD_Channel_SetVolume(ChannelName[4], EffectVolume);
}

void EffectVolumeDown()
{
	EffectVolume -= 0.1f;

	if (EffectVolume <= 0.0f)
	{
		EffectVolume = 0.0f;
	}

	FMOD_Channel_SetVolume(ChannelName[4], EffectVolume);
}


// ���� �������ֱ�.
void SoundUpdate()
{
	FMOD_System_Update(g_FMODSystem);
}

//���� �ý��� ����
void SoundDestroy()
{
	FMOD_Sound_Release(pFMODSound);
	FMOD_System_Close(g_FMODSystem); //�ý����� �ݱ�
	FMOD_System_Release(g_FMODSystem); //�ý��� ����
}

// Ű���带 �Է¹޾� ���� ���
void KeyBoard_Sound_Input(Player* player)
{
	if (KeyManager_GetKeyState(W) == TAP)
	{
		PlayerSound(6);
	}

	if (KeyManager_GetKeyState(A) == TAP)
	{
		PlayerSound(6);
	}

	if (KeyManager_GetKeyState(S) == TAP)
	{
		PlayerSound(6);
	}

	if (KeyManager_GetKeyState(D) == TAP)
	{
		PlayerSound(6);
	}

	if (KeyManager_GetKeyState(SPACE) == TAP)
	{
		PlayerSound(7);
		if (player->dashCount <= 0)
		{
			SoundStop(7);
		}
	}

	if (KeyManager_GetKeyState(W) == NONE &&
		KeyManager_GetKeyState(A) == NONE &&
		KeyManager_GetKeyState(S) == NONE &&
		KeyManager_GetKeyState(D) == NONE)
	{
		SoundStop(6);
	}
}

// ���콺�� �Է¹޾� ���� ���
void Mouse_Sound_Input(Player* player)
{
	if (LeftMouseManager_GetMouseState() == LMOUSEDOWN)
	{
		if (RightMouseManager_GetMouseState() == RMOUSEUP)
		{
			switch (player->nowWeapon)
			{
				case 0:
					ShootSound(11);
					break;
				case 1:
					ShootSound(12);
					break;
				case 2:
					ShootSound(13);
					break;
				case 3:
					ShootSound(14);
					break;
				default:
					break;
			}

			/*if (GameManager_GetOverHeatTime(0))
			{
				switch (GameManager_GetWeaponIndex(0))
				{
					case 0:
						SoundStop(11);
						break;
					case 1:
						SoundStop(12);
						break;
					case 2:
						SoundStop(13);
						break;
					case 3:
						SoundStop(14);
						break;
					default:
						break;
						
				}
			}*/

		}
		
	}

	if (LeftMouseManager_GetMouseState() == LMOUSEJUSTDOWN)
	{
		if (RightMouseManager_GetMouseState() == RMOUSEUP)
		{
			switch (player->nowWeapon)
			{
				case 0:
					ShootSound(25);
					break;
				case 1:
					ShootSound(26);
					break;
			/*	case 2:
					ShootSound(27);
					break;*/
			/*	case 3:
					ShootSound(28);
					break;*/
				default:
					break;
			}
		}

	}

	/*if (RightMouseManager_GetMouseState() == RMOUSEDOWN)
	{
		SoundStop(11);
		SoundStop(12);
		SoundStop(13);
		SoundStop(14);
	}*/

	if (RightMouseManager_GetMouseState() == RMOUSEDOWN)
	{
		if (LeftMouseManager_GetMouseState() == LMOUSEUP)
		{
			switch (player->nowWeapon)
			{
				case 0:
					ShootSound(11);
					break;
				case 1:
					ShootSound(12);
					break;
				case 2:
					ShootSound(13);
					break;
				case 3:
					ShootSound(14);
					break;
				default:
					//SoundStop(11);
					break;
			}

		}
	}

	else if (RightMouseManager_GetMouseState() == RMOUSEJUSTDOWN)
	{
		if (LeftMouseManager_GetMouseState() == LMOUSEUP)
		{
			switch (player->nowWeapon)
			{
				case 0:
					ShootSound(25);
					break;
				case 1:
					ShootSound(26);
					break;
				case 2:
					ShootSound(27);
					break;
				case 3:
					ShootSound(28);
					break;
				default:
					break;
			}
		}
	}

	if (LeftMouseManager_GetMouseState() == LMOUSEUP &&
		RightMouseManager_GetMouseState() == RMOUSEUP)
	{
		SoundStop(11);
		SoundStop(12);
		SoundStop(13);
		SoundStop(14);
	}

	if (LeftMouseManager_GetMouseState() == LMOUSEDOWN &&
		RightMouseManager_GetMouseState() == RMOUSEDOWN)
	{
		return;
	}
}

float GetBGMVolume(int ChannelNum)
{
	FMOD_Channel_GetVolume(ChannelName[ChannelNum], &BGMVolume);
	
	return BGMVolume;
}

float GetEffectVolume(int ChannelNum)
{
	FMOD_Channel_GetVolume(ChannelName[ChannelNum], &EffectVolume);

	return EffectVolume;
}