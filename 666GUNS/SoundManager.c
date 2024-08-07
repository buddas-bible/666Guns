#include <conio.h>
#include <fmod.h>
#include "GameManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "MainMenu.h"

#define MAXSOUND 100

//버튼 길이: 가로 450 세로 130
extern RECT Credit_Button; // 사각형 영역 지정
extern RECT Start_Button; // 사각형 영역 지정
extern RECT Exit_Button; // 사각형 영역 지정

//버튼 길이: 가로 128 세로 128
extern RECT Option_Button; // 사각형 영역 지정
extern RECT BGMVolUp_Button; // 사각형 영역 지정
extern RECT BGMVolDown_Button; // 사각형 영역 지정

FMOD_SYSTEM* g_FMODSystem = NULL; //사운드 시스템 포인터 변수
/// 하나의 FMOD_SOUND를 다수의 FMOD_CHANNEL들을 통해 출력 가능
FMOD_SOUND* pFMODSound[MAXSOUND]; //사운드 객체 포인터 변수 (하나의 인덱스당 하나의 파일 저장)
FMOD_CHANNEL* ChannelName[MAXSOUND]; //사운드를 출력하는 채널 생성 변수
FMOD_BOOL isPaused = 0;
FMOD_BOOL isPlaying = 0;
float BGMVolume = 0.5f;
float EffectVolume = 0.5f;

void SoundSystemInput()
{
	/// <summary>
	/// 한번만 호출해야함
	FMOD_System_Create(&g_FMODSystem); //사운드 시스템을 생성
	FMOD_System_Init(g_FMODSystem, 32, FMOD_INIT_NORMAL, NULL); //사운드 시스템 초기화
	/// </summary>
}


// 사운드 파일 입력 (채널도 동일)
/// 메뉴브금 0
/// 플레이어 이동 1
/// 버튼 클릭 2
/// 인게임 브금 3
/// 총 발사 4
/// 총 발사2 5

//FMOD_DEFAULT -> 사운드 1번 출력 (효과음)
//FMOD_LOOP_NORMAL -> 사운드 반복 출력 (브금)

// 루프 사운드 파일 입력
void LoadSoundLoopFile(const char* filePath, int num)
{
	//if (pFMODSound != 0)
	//	FMOD_Sound_Release(pFMODSound[0]); //사운드 객체를 해제
	FMOD_System_CreateSound(g_FMODSystem, filePath, FMOD_LOOP_NORMAL, 0, &(pFMODSound[num]));
}

// 한번 재생 사운드 파일 입력
void LoadSoundDefaultFile(const char* filePath, int num)
{
	//if (pFMODSound != 0)
	//	FMOD_Sound_Release(pFMODSound[0]); //사운드 객체를 해제
	FMOD_System_CreateSound(g_FMODSystem, filePath, FMOD_DEFAULT, 0, &(pFMODSound[num]));
}

// 사운드 출력
void Play_Sound(int num)
{
	if (ChannelName[num] != 0)
		FMOD_Channel_IsPlaying(ChannelName[num], &isPlaying);
	if (isPlaying != 0)
		FMOD_Channel_Stop(ChannelName[num]); //사운드 출력을 정지
	FMOD_System_PlaySound(g_FMODSystem, pFMODSound[num], NULL, isPaused, &ChannelName[num]);
}

// 브금 사운드들
void LoadBGMSound()
{
	LoadSoundLoopFile(GAME_START, 0);
	LoadSoundLoopFile(IN_GAME, 1);
	LoadSoundLoopFile(END_GAME, 2);
	LoadSoundDefaultFile(ENDING_BGM, 18);
}

// 효과음 사운드들
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

// 상황에 따른 BGM (0 ~ 2) 
void BGMSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetBGMVolume(ChannelNum);
}

// 엔딩 사운드 29
void EndingBGMSound()
{
	FMOD_System_PlaySound(g_FMODSystem, pFMODSound[18], NULL, isPaused, &(ChannelName[18]));
	FMOD_Channel_SetVolume(ChannelName[18], 1.0);
}

// 플레이어 상태에 따른 사운드 (6 ~ 9)
void PlayerSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}

// 적 상태에 따른 사운드(15 ~ 18)
void EnemySound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}

// 오브젝트 상태에 따른 사운드 (19 ~ 22)
void ObjectSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}

// 상황에 따른 클릭 사운드 (4 ~ 5)
void ClickSound(int BtnNum)
{
	Play_Sound(BtnNum);
	SetEffectVolume(BtnNum);
}

// 무기 상황에 따른 사운드 (루프 10 ~ 14 / 단일 25 ~ 28) 
// 10번 파일은 무기교체 사운드
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

// 인게임 상태에 따른 사운드 (3, 23 ~ 24)
// 인게임 팝업 3 점령 23 엔딩선 24
void InGameSound(int ChannelNum)
{
	Play_Sound(ChannelNum);
	SetEffectVolume(ChannelNum);
}


//사운드 출력을 일시정지
void SoundPaused()
{
	isPaused = !isPaused;
	FMOD_Channel_SetPaused(ChannelName[0], isPaused); 
}

//사운드 출력을 정지
void SoundStop(int ChannelNum)
{
	FMOD_Channel_Stop(ChannelName[ChannelNum]); 
}

// 브금 볼륨 설정
void SetBGMVolume(int ChannelNum)
{
	FMOD_Channel_SetVolume(ChannelName[ChannelNum], BGMVolume);
}

// 효과음 볼륨 설정
void SetEffectVolume(int ChannelNum)
{
	FMOD_Channel_SetVolume(ChannelName[ChannelNum], EffectVolume);
}

//브금의 볼륨을 조절
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


//효과음의 볼륨을 조절
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


// 사운드 갱신해주기.
void SoundUpdate()
{
	FMOD_System_Update(g_FMODSystem);
}

//사운드 시스템 해제
void SoundDestroy()
{
	FMOD_Sound_Release(pFMODSound);
	FMOD_System_Close(g_FMODSystem); //시스템을 닫기
	FMOD_System_Release(g_FMODSystem); //시스템 해제
}

// 키보드를 입력받아 사운드 재생
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

// 마우스를 입력받아 사운드 재생
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