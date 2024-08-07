#pragma once
#include <Windows.h>
#include "Vector2D.h"
#include "Player.h"
#include "Bullet.h"

/// <summary>
/// �÷��̾� ��ü ����
/// �� ���Ͽ����� �Ʒ��� PLAYER ����ü�� ���� �Լ����� ����Ǿ� ����.
/// ���Ǵ� ������ .c ���Ͽ� ������.
/// </summary>
typedef struct ENEMY
{
	int radius;					// ��Ʈ�ʿ� ��� �� ���� �ݰ�..
	Vector2D centerPosition;	// ����
	Vector2D plgVertex[4];		// ȸ���� ����� LT,RT,LB,RB ��ǥ. 
	POINT plgPOINT[4];			// ���� plgVertex�� POINT������ ������ ��. plgBlt() ����� ���� 3���� POINT
	Vector2D heightVector;		// ��Ʈ���� �ٶ� ����. �ʱⰪ�� (0,-1)
	Vector2D widthVector;		// OBB�� ���� ��������
	Vector2D prePosition;		// ���� ��ǥ ����
	double speed;				// �̵��ӵ�

	int nowWeapon;				// ���� ��� �ִ� ����
	int nowStatus;				// ���� ���� (Idle, RightMoving, LeftMoving, Attack)
	BOOL statusChanged;			// ���°� ��������� TRUE�� �ٲ�. Update�� ���� �׻� FALSE�� �ٲ����

	int nowType;				// Melee Ÿ������ Range Ÿ������
	BOOL isShooting;			// ���� ���������
	double fireDelay;			// ����ӵ�
	double animTime;			// ���� ��� �ð�

	BOOL isRight;				// ���� ������ �ٶ󺸰� �ִ���
	int frame;					// ����� ���� ������ ������
	double frameDelta;			// ����� ���� ������ ���������͹��ð�
	int weaponFrame;			// ��������� ���� ������ ������
	double weaponFrameDelta;	// ��������� ���� ������ ���������͹��ð�

	int detectionRadius;		// ���� ����
	BOOL isDetection;			// �÷��̾� ���� ����

	BOOL isCheckHit;			// �ǰ����� üũ ����
	RECT hitbox;				// ��Ʈ�ڽ�
	int hitboxHeightRadius;		// ��Ʈ�ڽ� ����
	int hitboxWidthRadius;		// ��Ʈ�ڽ� �ʺ�

	int tag;

	BOOL isDead;				// �׾�����
	double deathTime;			// ��� �ִϸ��̼� ó���� �ʿ��� �ð�
	BOOL isActive;

	void (*Move)(struct ENEMY*, Vector2D);	// Player�� ������ plgThree�� �Ű��ִ� �̵� �Լ�
	void (*Rotate)(struct ENEMY*, double);	// Player�� headVector�� plgThree�� ȸ�������ִ� ȸ�� �Լ�
	void (*SetPOINT)(struct ENEMY*);		// plgPOINT�� plgVertex�� ���� �־��ִ� �Լ�
	void (*LookAt)(struct ENEMY*, Vector2D lookPos);	// ������ ��ġ�� ��庤�͸� �Ű� ȸ���ϴ� �Լ�
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
