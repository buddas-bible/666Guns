#include "Bullet.h"

Bullet bulletPool[6][BULLETPOOL_MAX];

/// <summary>
/// Bullet 풀 초기화 함수
/// </summary>
void BulletPool_Init()
{
	// 먼저 투사체들의 데이터를 전부 0으로 초기화 시켜준다.
	// 사용 가능 여부도 TRUE로 초기화 시켜둔다.
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
/// 투사체를 초기화하는 함수
/// </summary>
/// <param name=""></param>
/// <param name="player"></param>
void Bullet_Init(Bullet* bullet, Player* player)
{
	// 투사체의 위치를 옮긴다
	bullet->centerPosition = Vector2D_Add(player->centerPosition, Vector2D_Multiple(player->heightVector, player->radius / 2));
	
	// 투사체의 속도를 초기화
	bullet->speed = Bullet_GetBulletSpeed(player->nowWeapon);

	// 투사체의 크기를 초기화
	bullet->radius = Bullet_GetBulletRadius(player->nowWeapon);

	// 투사체 머리 방향
	bullet->headVector = player->heightVector;

	// 투사체 속력
	bullet->velocity = Vector2D_Multiple(player->heightVector, bullet->speed);

	// 투사체 종류
	bullet->bulletType = player->nowWeapon;

	bullet->lifeTime = 0;
}


/// <summary>
/// 플레이어의 무기에 따라 속도를 반환하는 함수
/// </summary>
/// <param name="weaponNumber">무기 번호</param>
/// <returns>투사체 속도</returns>
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
/// 플레이어의 무기에 따라 크기를 반환하는 함수
/// </summary>
/// <param name="weaponNumber">무기 번호</param>
/// <returns>투사체 크기</returns>
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
/// 총을 쏘는 함수
/// </summary>
/// <param name="player"></param>
void Bullet_Shot(Player* player)
{
	// 오브젝트 풀에서 사용가능한 투사체를 찾음
	Bullet* bullet = Bullet_GetBullet(player->nowWeapon);

	// 투사체를 초기화
	Bullet_Init(bullet, player);
}

/// <summary>
/// 델타타임마다 투사체가 이동하는 함수
/// </summary>
/// <param name="">투사체 포인터</param>
/// <param name="dt">델타타임</param>
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
/// 오브젝트 풀에서 사용 가능한 투사체를 반환하는 함수
/// </summary>
/// <returns>투사체의 포인터</returns>
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
	// 새로운 친구 생성 해야함.
}