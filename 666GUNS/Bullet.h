#pragma once
#include "GameDefine.h"
#include "Vector2D.h"
#include "Player.h"

typedef struct
{
	// �ҷ��� ��ġ
	Vector2D centerPosition;
	
	// Ȥ�� �� ���� �浹 ����� ���� ���� ��ǥ�� ����
	Vector2D prePosition;

	int radius;

	RECT hitbox;

	int bulletType;

	// �Ѿ��� �⺻ �ӷ�
	double speed;

	Vector2D headVector;

	// ��庤��(���� ����) ��ġ���� �߻��Ѵٰ� �ϸ�.
	// ������Ʈ ������ �÷��̾� �Ǵ� ������ ��庤�� ��ġ���� ������ �ǰ�
	// �ӵ� = (��庤�� * �ӷ�)���� �ӵ� ���͸� ���ϸ� �� �Ͱ���. 
	Vector2D velocity;

	// ���� Ȱ��ȭ �Ǿ��ִ� �����ΰ�?
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

