#include "Bullet.h"

Bullet bulletPool[6][BULLETPOOL_MAX];

/// <summary>
/// Bullet Ǯ �ʱ�ȭ �Լ�
/// </summary>
void BulletPool_Init()
{
	// ���� ����ü���� �����͸� ���� 0���� �ʱ�ȭ �����ش�.
	// ��� ���� ���ε� TRUE�� �ʱ�ȭ ���ѵд�.
	for (int i = 0; i < WEAPONTYPECOUNT; ++i)
	{
		for (int j = 0; j < BULLETPOOL_MAX; j++)
		{
			bulletPool[i][j].centerPosition.x = 0;
			bulletPool[i][j].centerPosition.y = 0;
			bulletPool[i][j].prePosition.x = 0;
			bulletPool[i][j].prePosition.y = 0;
			bulletPool[i][j].speed = 0;
			bulletPool[i][j].radius = 0;
			bulletPool[i][j].isActive = FALSE;
		}

	}
}

/// <summary>
/// ����ü�� �ʱ�ȭ�ϴ� �Լ�
/// </summary>
/// <param name=""></param>
/// <param name="player"></param>
void Bullet_Init(Bullet* bullet, Player* player)
{
	// ����ü�� ��ġ�� �ű��
	bullet->centerPosition = Vector2D_Add(player->centerPosition, Vector2D_Multiple(player->heightVector, player->radius / 2));
	
	// ����ü�� �ӵ��� �ʱ�ȭ
	bullet->speed = Bullet_GetBulletSpeed(player->nowWeapon);

	// ����ü�� ũ�⸦ �ʱ�ȭ
	bullet->radius = Bullet_GetBulletRadius(player->nowWeapon);

	// ����ü �Ӹ� ����
	bullet->headVector = player->heightVector;

	// ����ü �ӷ�
	bullet->velocity = Vector2D_Multiple(player->heightVector, bullet->speed);

	// ����ü ����
	bullet->bulletType = player->nowWeapon;

	bullet->lifeTime = 0;
}


/// <summary>
/// �÷��̾��� ���⿡ ���� �ӵ��� ��ȯ�ϴ� �Լ�
/// </summary>
/// <param name="weaponNumber">���� ��ȣ</param>
/// <returns>����ü �ӵ�</returns>
double Bullet_GetBulletSpeed(int weaponNumber)
{
	switch (weaponNumber)
	{
	case Weapon_Pistol:
		return BULLETSPEED_PISTOL;

	case Weapon_Rifle:
		return BULLETSPEED_RIFLE;

	case Weapon_Shotgun:
		return BULLETSPEED_SHOTGUN;

	case Weapon_Railgun:
		return BULLETSPEED_RAILGUN;

	case Weapon_EnemyRifle:
		return BULLETSPEED_ENEMYRIFLE;

	default:
		break;
	}
}

/// <summary>
/// �÷��̾��� ���⿡ ���� ũ�⸦ ��ȯ�ϴ� �Լ�
/// </summary>
/// <param name="weaponNumber">���� ��ȣ</param>
/// <returns>����ü ũ��</returns>
int Bullet_GetBulletRadius(int weaponNumber)
{
	switch (weaponNumber)
	{
	case Weapon_Pistol:
		return BULLETRADIUS_PISTOL;

	case Weapon_Rifle:
		return BULLETRADIUS_RIFLE;

	case Weapon_Shotgun:
		return BULLETRADIUS_SHOTGUN;

	case Weapon_Railgun:
		return BULLETRADIUS_RAILGUN;

	case Weapon_EnemyRifle:
		return BULLETRADIUS_ENEMYRIFLE;

	default:
		break;
	}
}


/// <summary>
/// ���� ��� �Լ�
/// </summary>
/// <param name="player"></param>
void Bullet_Shot(Player* player)
{
	// ������Ʈ Ǯ���� ��밡���� ����ü�� ã��
	Bullet* bullet = Bullet_GetBullet(player->nowWeapon);

	// ����ü�� �ʱ�ȭ
	Bullet_Init(bullet, player);
}

/// <summary>
/// ��ŸŸ�Ӹ��� ����ü�� �̵��ϴ� �Լ�
/// </summary>
/// <param name="">����ü ������</param>
/// <param name="dt">��ŸŸ��</param>
void Bullet_Flying(Bullet* bullet, double deltaTime)
{
	if (bullet->bulletType == Weapon_Shotgun || bullet->lifeTime > 0)
	{
		if (bullet->lifeTime <= 0)
		{
			bullet->isActive = FALSE;
			return;
		}

		bullet->lifeTime -= deltaTime;
	}

	bullet->centerPosition.x = bullet->centerPosition.x + (bullet->velocity.x * (deltaTime));
	bullet->centerPosition.y = bullet->centerPosition.y + (bullet->velocity.y * (deltaTime));
}


/// <summary>
/// ������Ʈ Ǯ���� ��� ������ ����ü�� ��ȯ�ϴ� �Լ�
/// </summary>
/// <returns>����ü�� ������</returns>
Bullet* Bullet_GetBullet(int bulletType)
{
	for (int i = 0; i < BULLETPOOL_MAX; ++i)
	{
		if (bulletPool[bulletType][i].isActive == FALSE)
		{
			bulletPool[bulletType][i].isActive = TRUE;
			Bullet* bullet = &bulletPool[bulletType][i];

			return bullet;
		}
	}

	return NULL;
	// ���ο� ģ�� ���� �ؾ���.
}