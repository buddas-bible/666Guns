#pragma once
#include <Windows.h>
#include "Vector2D.h"
#include "Weapon.h"

/// <summary>
/// 플레이어 객체 파일
/// 이 파일에서는 아래의 PLAYER 구조체를 위한 함수들이 선언되어 있음.
/// 정의는 동명의 .c 파일에 존재함.
/// </summary>
typedef struct PLAYER
{
	int radius;					// 비트맵에 출력 할 때의 반경..
	Vector2D centerPosition;	// 중점
	Vector2D plgVertex[4];		// 회전에 사용할 LT,RT,LB,RB 좌표. 
	POINT plgPOINT[4];			// 위의 plgVertex를 POINT형으로 변경한 것. plgBlt() 사용을 위한 3개의 POINT
	Vector2D heightVector;		// 헤드 벡터  // 비트맵이 바라볼 벡터. 초기값은 (0,-1)
	Vector2D widthVector;		// OBB를 위한 수직벡터
	Vector2D prePosition;		// 이전 좌표 저장

	int nowStatus;				// 현재 상태 (Idle, RightMoving, LeftMoving, Attack)
	BOOL statusChanged;			// 상태가 변경됐을때 TRUE로 바뀜. Update를 통해 항상 FALSE로 바뀔거임
	int nowWeapon;				// 현재 발사중인 무기 (Pistol, Rifle, Shotgun, Laser)
	Weapon* selectedWeapon[2];	// 0번은 좌클릭무기, 1번은 우클릭무기. 원본은 GameManager에 있음.
	int* playerHP;				// 플레이어 체력. 원본은 GameManager에 있음.

	BOOL isDead;				// 죽었는지
	double deathTime;			// 사망 애니메이션 처리에 필요한 시간

	BOOL isRight;				// 현재 마우스가 캐릭터보다 오른쪽에 있는지
	BOOL isShooting;			// 현재 사격중인지
	double shootingTime;		// 현재 사격중인 시간

	int dashCount;				// 대쉬 갯수 체크
	double dashCoolTimeCount;	// 대쉬 쿨타임 체크
	double dashTimeCount;		// 대쉬상태일때 남은 대쉬 시간	
	Vector2D dashAt;			// 대쉬 방향

	double gracePeriod;			// 무적 시간
	BOOL isCheckHit;				// 피격판정 체크 여부
	RECT hitbox;					// 히트박스
	int hitboxHeightRadius;			// 히트박스 높이
	int hitboxWidthRadius;			// 히트박스 너비

	int tag;					// 어떤 오브젝트인지 비교하기 위한 태그

	void (*Move)(struct PLAYER*, Vector2D, double);	// Player의 중점과 plgThree를 옮겨주는 이동 함수
	void (*Rotate)(struct PLAYER*, double);	// Player의 headVector와 plgThree를 회전시켜주는 회전 함수
	void (*SetPOINT)(struct PLAYER*);		// plgPOINT에 plgVertex의 값을 넣어주는 함수
	void (*LookAt)(struct PLAYER*, Vector2D mouse);	// 지정된 위치로 헤드벡터를 옮겨 회전하는 함수
	void (*FireWeapon)(struct PLAYER*, int weaponSide);
	void (*Update)();
}Player;

void Player_Init(Player* player);

void Player_Move(Player* player, Vector2D moveAt, double speed);
void Player_Rotate(Player* player, double theta);
void Player_SetPOINT(Player* player);
void Player_SetHitbox(Player* player);
void Player_LookAt(Player* player, Vector2D lookDestination);
void Player_FireWeapon(Player* player, int weaponside);
void Player_SetDirection(Player* player, Vector2D* mousePos);
void Player_DashStart(Player* player, Vector2D dashAt);
void Player_Dash(Player* player);

void Player_Hit(Player* player);

void Player_Death(Player* player);
