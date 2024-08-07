#pragma once
#include <Windows.h>
#include "Vector2D.h"
#include "Player.h"
#include "Bullet.h"

/// <summary>
/// 플레이어 객체 파일
/// 이 파일에서는 아래의 PLAYER 구조체를 위한 함수들이 선언되어 있음.
/// 정의는 동명의 .c 파일에 존재함.
/// </summary>
typedef struct ENEMY
{
	int radius;					// 비트맵에 출력 할 때의 반경..
	Vector2D centerPosition;	// 중점
	Vector2D plgVertex[4];		// 회전에 사용할 LT,RT,LB,RB 좌표. 
	POINT plgPOINT[4];			// 위의 plgVertex를 POINT형으로 변경한 것. plgBlt() 사용을 위한 3개의 POINT
	Vector2D heightVector;		// 비트맵이 바라볼 벡터. 초기값은 (0,-1)
	Vector2D widthVector;		// OBB를 위한 수직벡터
	Vector2D prePosition;		// 이전 좌표 저장
	double speed;				// 이동속도

	int nowWeapon;				// 현재 들고 있는 무기
	int nowStatus;				// 현재 상태 (Idle, RightMoving, LeftMoving, Attack)
	BOOL statusChanged;			// 상태가 변경됐을때 TRUE로 바뀜. Update를 통해 항상 FALSE로 바뀔거임

	int nowType;				// Melee 타입인지 Range 타입인지
	BOOL isShooting;			// 현재 사격중인지
	double fireDelay;			// 연사속도
	double animTime;			// 공격 모션 시간

	BOOL isRight;				// 현재 우측을 바라보고 있는지
	int frame;					// 출력을 위한 각자의 프레임
	double frameDelta;			// 출력을 위한 각자의 프레임인터벌시간
	int weaponFrame;			// 무기출력을 위한 각자의 프레임
	double weaponFrameDelta;	// 무기출력을 위한 각자의 프레임인터벌시간

	int detectionRadius;		// 감지 범위
	BOOL isDetection;			// 플레이어 감지 여부

	BOOL isCheckHit;			// 피격판정 체크 여부
	RECT hitbox;				// 히트박스
	int hitboxHeightRadius;		// 히트박스 높이
	int hitboxWidthRadius;		// 히트박스 너비

	int tag;

	BOOL isDead;				// 죽었는지
	double deathTime;			// 사망 애니메이션 처리에 필요한 시간
	BOOL isActive;

	void (*Move)(struct ENEMY*, Vector2D);	// Player의 중점과 plgThree를 옮겨주는 이동 함수
	void (*Rotate)(struct ENEMY*, double);	// Player의 headVector와 plgThree를 회전시켜주는 회전 함수
	void (*SetPOINT)(struct ENEMY*);		// plgPOINT에 plgVertex의 값을 넣어주는 함수
	void (*LookAt)(struct ENEMY*, Vector2D lookPos);	// 지정된 위치로 헤드벡터를 옮겨 회전하는 함수
}Enemy;

void Enemy_Init(Enemy* enemy, int enemyType);
void EnemyPool_Init();

void Enemy_Move(Enemy* enemy, Vector2D moveDirection, double speed);
void Enemy_Rotate(Enemy* enemy, double theta);
void Enemy_SetPOINT(Enemy* enemy);
void Enemy_SetHitbox(Enemy* enemy);
void Enemy_LookAt(Enemy* enemy, Vector2D lookPos);
void Enemy_SetDirection(Enemy* enemy, Vector2D* playerPos);

void Enemy_BulletInit(Bullet* bullet, Enemy* enemy);
void Enemy_BulletShot(Enemy* enemy);
void Enemy_Attack(Enemy* enemy);

void Enemy_Attack(Enemy* enemy);
void Enemy_Follow(Enemy* enemy, Player* player);
double Enemy_PlayerDistance(Enemy* enemy, Player* player);

Enemy* Enemy_GetEnemy(int enemyType);
void Enemy_StartEnemy(Enemy* enemy, POINT position);

void Enemy_Death(Enemy* enemy);
