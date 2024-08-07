#pragma once
#include <Windows.h>
#include "Vector2D.h"
#include "Weapon.h"

/// <summary>
/// �÷��̾� ��ü ����
/// �� ���Ͽ����� �Ʒ��� PLAYER ����ü�� ���� �Լ����� ����Ǿ� ����.
/// ���Ǵ� ������ .c ���Ͽ� ������.
/// </summary>
typedef struct PLAYER
{
	int radius;					// ��Ʈ�ʿ� ��� �� ���� �ݰ�..
	Vector2D centerPosition;	// ����
	Vector2D plgVertex[4];		// ȸ���� ����� LT,RT,LB,RB ��ǥ. 
	POINT plgPOINT[4];			// ���� plgVertex�� POINT������ ������ ��. plgBlt() ����� ���� 3���� POINT
	Vector2D heightVector;		// ��� ����  // ��Ʈ���� �ٶ� ����. �ʱⰪ�� (0,-1)
	Vector2D widthVector;		// OBB�� ���� ��������
	Vector2D prePosition;		// ���� ��ǥ ����

	int nowStatus;				// ���� ���� (Idle, RightMoving, LeftMoving, Attack)
	BOOL statusChanged;			// ���°� ��������� TRUE�� �ٲ�. Update�� ���� �׻� FALSE�� �ٲ����
	int nowWeapon;				// ���� �߻����� ���� (Pistol, Rifle, Shotgun, Laser)
	Weapon* selectedWeapon[2];	// 0���� ��Ŭ������, 1���� ��Ŭ������. ������ GameManager�� ����.
	int* playerHP;				// �÷��̾� ü��. ������ GameManager�� ����.

	BOOL isDead;				// �׾�����
	double deathTime;			// ��� �ִϸ��̼� ó���� �ʿ��� �ð�

	BOOL isRight;				// ���� ���콺�� ĳ���ͺ��� �����ʿ� �ִ���
	BOOL isShooting;			// ���� ���������
	double shootingTime;		// ���� ������� �ð�

	int dashCount;				// �뽬 ���� üũ
	double dashCoolTimeCount;	// �뽬 ��Ÿ�� üũ
	double dashTimeCount;		// �뽬�����϶� ���� �뽬 �ð�	
	Vector2D dashAt;			// �뽬 ����

	double gracePeriod;			// ���� �ð�
	BOOL isCheckHit;				// �ǰ����� üũ ����
	RECT hitbox;					// ��Ʈ�ڽ�
	int hitboxHeightRadius;			// ��Ʈ�ڽ� ����
	int hitboxWidthRadius;			// ��Ʈ�ڽ� �ʺ�

	int tag;					// � ������Ʈ���� ���ϱ� ���� �±�

	void (*Move)(struct PLAYER*, Vector2D, double);	// Player�� ������ plgThree�� �Ű��ִ� �̵� �Լ�
	void (*Rotate)(struct PLAYER*, double);	// Player�� headVector�� plgThree�� ȸ�������ִ� ȸ�� �Լ�
	void (*SetPOINT)(struct PLAYER*);		// plgPOINT�� plgVertex�� ���� �־��ִ� �Լ�
	void (*LookAt)(struct PLAYER*, Vector2D mouse);	// ������ ��ġ�� ��庤�͸� �Ű� ȸ���ϴ� �Լ�
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
