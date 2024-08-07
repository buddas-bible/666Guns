#pragma once
#include "GameDefine.h"
#include "Vector2D.h"
#include "Player.h"

typedef struct
{
	// 불렛의 위치
	Vector2D centerPosition;
	
	// 혹시 모를 선분 충돌 계산을 위해 이전 좌표를 저장
	Vector2D prePosition;

	int radius;

	RECT hitbox;

	int bulletType;

	// 총알의 기본 속력
	double speed;

	Vector2D headVector;

	// 헤드벡터(높이 벡터) 위치에서 발사한다고 하면.
	// 오브젝트 생성을 플레이어 또는 적군의 헤드벡터 위치에서 생성이 되고
	// 속도 = (헤드벡터 * 속력)으로 속도 벡터를 구하면 될 것같다. 
	Vector2D velocity;

	// 현재 활성화 되어있는 상태인가?
	BOOL isActive;

	double lifeTime;

}Bullet;


void BulletPool_Init();
void Bullet_Init(Bullet* bullet, Player* player);

double Bullet_GetBulletSpeed(int weaponNumber);
int Bullet_GetBulletRadius(int weaponNumber);

void Bullet_Shot(Player* player);
void Bullet_Flying(Bullet* bullet, double deltaTime);
Bullet* Bullet_GetBullet(int bulletType);

