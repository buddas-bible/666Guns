#pragma once
#include "Player.h"
#include<windows.h>

/// �ش� ���� ������ ���� �� ä�� ����

/// ���� ���� ���(BGM)
#define GAME_START "Resources/Music/Maintheme_666.ogg"
#define IN_GAME "Resources/Music/Ingame_BGM.ogg"
#define END_GAME "Resources/Music/GameOver_BGM.ogg"
#define IN_GAME_POPUP "Resources/Music/Ingame_pop_Sound.ogg"

/// ���� ���� ���(Effect)
#define TITLE_BUTTON_CLICK "Resources/Music/Click_Sound.ogg"
#define POPUP_BUTTON_CLICK "Resources/Music/Edit_Sound.ogg"

#define PLAYER_WALK "Resources/Music/Character_Walk.ogg"
#define PLAYER_DASH "Resources/Music/Character_Dash.ogg"
#define PLAYER_DAMAGED "Resources/Music/Character_Damage.ogg"
#define OVERHEAT "Resources/Music/Overheat_Warning3.ogg"

#define WEAPON_CHANGE "Resources/Music/Key_Change.ogg"
#define GUN_HANDGUN "Resources/Music/Gun_Handgun.ogg"
#define GUN_RIFLE "Resources/Music/Gun_Rifle.ogg"
#define GUN_LASER "Resources/Music/Gun_Laser2.ogg"
#define GUN_SHOTGUN "Resources/Music/Gun_Shotgun.ogg"

#define ENEMY_DAMAGED "Resources/Music/Enemy_Damage.ogg"
#define ENEMY_SHOOT "Resources/Music/Enemy_Shot.ogg"
#define ENEMY_SWING "Resources/Music/Enemy_Swing.ogg"

#define OBJECT_WALL "Resources/Music/Object_Wall.ogg"
#define OBJECT_BOX1 "Resources/Music/Object_Box.ogg"
#define OBJECT_BOX2 "Resources/Music/Object_Box2.ogg"
#define OBJECT_CAR "Resources/Music/Object_Car.ogg"

#define OCCUPATION "Resources/Music/666_Capture2.ogg"
#define ENDING_LINE "Resources/Music/666_Line.ogg"
#define ENDING_BGM "Resources/Music/End_BGM.ogg"





/// ���� �ý��� �ʱ�ȭ & ���� �Է�
void SoundSystemInput();
void LoadSoundLoopFile(const char* filePath, int num);
void LoadSoundDefaultFile(const char* filePath, int num);

/// ���� ���
void Play_Sound(int num);
void LoadBGMSound();
void LoadEffectSound();

/// ��Ȳ�� ����
void BGMSound(int ChannelNum);
void EndingBGMSound();
void PlayerSound(int ChannelNum);
void EnemySound(int ChannelNum);
void ObjectSound(int ChannelNum);
void ClickSound(int BtnNum);
void ShootSound(int GunNum);
void InGameSound(int ChannelNum);

/// ���带 �����ϴ� ���
void SoundPaused();
void SoundStop(int ChannelNum);
void SetBGMVolume(int ChannelNum);
void SetEffectVolume(int ChannelNum);
void BGMVolumeUp();
void BGMVolumeDown();
void EffectVolumeUp();
void EffectVolumeDown();

/// ���� �ý��� ���� �� ����
void SoundUpdate();
void SoundDestroy();
void KeyBoard_Sound_Input(Player* player);
void Mouse_Sound_Input(Player* player);

/// ���� ��������
float GetBGMVolume(int ChannelNum);
float GetEffectVolume(int ChannelNum);

void HoldSound();
void ShotGunSound();