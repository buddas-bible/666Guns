#include "GameDefine.h"
#include "Enemy.h"
#include "Rotation.h"
#include "Core.h"
#include "SoundManager.h"

extern Player player;

Enemy enemyPool[2][ENEMYPOOL_MAX];

/// <summary>
/// Enemy 구조체 초기화. 함수포인터에 필요 함수를 연결.
/// 추후에 매개변수를 늘려서 사용할 예정
/// </summary>
/// <param name="player"></param>
void Enemy_Init(Enemy* enemy, int enemyType)
{
	enemy->centerPosition.x = 4500;
	enemy->centerPosition.y = 800;
	enemy->radius = 128;

	enemy->heightVector.x = 0;
	enemy->heightVector.y = -1;

	if (enemyType == EnemyType_Range)
	{
		enemy->nowWeapon = Weapon_EnemyRifle;
		enemy->speed = EMENYRANGESPEED;
	}
	else if (enemyType == EnemyType_Melee)
	{
		enemy->nowWeapon = Weapon_EnemyRifle;
		enemy->speed = ENEMYMELEESPEED;
	}

	enemy->nowStatus = EAnim_IdleRight;
	enemy->nowType = enemyType;

	enemy->frame = 0;
	enemy->frameDelta = 0;
	enemy->weaponFrame = 0;
	enemy->weaponFrameDelta = 0;

	enemy->hitboxHeightRadius = 60;
	enemy->hitboxWidthRadius = 35;

	enemy->tag = Tag_Enemy;
	
	enemy->isDead = FALSE;
	enemy->isActive = FALSE;
	enemy->statusChanged = FALSE;

	enemy->plgVertex[0].x = enemy->centerPosition.x - enemy->radius;
	enemy->plgVertex[0].y = enemy->centerPosition.y - enemy->radius;

	enemy->plgVertex[1].x = enemy->centerPosition.x + enemy->radius;
	enemy->plgVertex[1].y = enemy->centerPosition.y - enemy->radius;

	enemy->plgVertex[2].x = enemy->centerPosition.x - enemy->radius;
	enemy->plgVertex[2].y = enemy->centerPosition.y + enemy->radius;

	enemy->plgVertex[3].x = enemy->centerPosition.x + enemy->radius;
	enemy->plgVertex[3].y = enemy->centerPosition.y + enemy->radius;

	enemy->isCheckHit = TRUE;

	enemy->Move = Enemy_Move;
	enemy->Rotate = Enemy_Rotate;
	enemy->SetPOINT = Enemy_SetPOINT;
	enemy->LookAt = Enemy_LookAt;

	enemy->SetPOINT(enemy);
	Enemy_SetHitbox(enemy);
}

/// <summary>
/// Enemy 풀 초기화 함수
/// </summary>
void EnemyPool_Init()
{
	for (int type = 0; type < 2; ++type)
	{
		for (int index = 0; index < ENEMYPOOL_MAX; ++index)
		{
			Enemy_Init(&enemyPool[type][index], type);
		}
	}
}

/// <summary>
/// 적 이동
/// </summary>
/// <param name="player"></param>
/// <param name="moveAt"></param>
void Enemy_Move(Enemy* enemy, Vector2D moveDirection, double speed)
{
	Vector2D moveAt;
	moveAt.x = moveDirection.x * speed * Engine_GetDeltaTime();
	moveAt.y = moveDirection.y * speed * Engine_GetDeltaTime();

	enemy->prePosition = enemy->centerPosition;

	enemy->centerPosition.x += moveAt.x;
	enemy->centerPosition.y += moveAt.y;

	enemy->plgVertex[0].x += moveAt.x;
	enemy->plgVertex[0].y += moveAt.y;

	enemy->plgVertex[1].x += moveAt.x;
	enemy->plgVertex[1].y += moveAt.y;

	enemy->plgVertex[2].x += moveAt.x;
	enemy->plgVertex[2].y += moveAt.y;

	enemy->SetPOINT(enemy);
	Enemy_SetHitbox(enemy);
}

/// <summary>
/// 플레이어 회전
/// </summary>
/// <param name="player"></param>
/// <param name="theta"></param>
void Enemy_Rotate(Enemy* enemy, double theta)
{
	Rotation_PlayerRotation(enemy, theta);
	enemy->SetPOINT(enemy);
}

/// <summary>
/// plgVertex의 값을 plgPOINT로 복사하면서 캐릭터의 로컬형태로 변경(카메라를 위해)
/// </summary>
/// <param name="player"></param>
void Enemy_SetPOINT(Enemy* enemy)
{
	enemy->plgPOINT[0].x = (int)(enemy->plgVertex[0].x - enemy->centerPosition.x + enemy->radius);
	enemy->plgPOINT[0].y = (int)(enemy->plgVertex[0].y - enemy->centerPosition.y + enemy->radius);

	enemy->plgPOINT[1].x = (int)(enemy->plgVertex[1].x - enemy->centerPosition.x + enemy->radius);
	enemy->plgPOINT[1].y = (int)(enemy->plgVertex[1].y - enemy->centerPosition.y + enemy->radius);

	enemy->plgPOINT[2].x = (int)(enemy->plgVertex[2].x - enemy->centerPosition.x + enemy->radius);
	enemy->plgPOINT[2].y = (int)(enemy->plgVertex[2].y - enemy->centerPosition.y + enemy->radius);

	enemy->plgPOINT[3].x = (int)(enemy->plgVertex[3].x - enemy->centerPosition.x + enemy->radius);
	enemy->plgPOINT[3].y = (int)(enemy->plgVertex[3].y - enemy->centerPosition.y + enemy->radius);
}

void Enemy_SetHitbox(Enemy* enemy)
{
	enemy->hitbox.left = enemy->centerPosition.x - enemy->hitboxWidthRadius;
	enemy->hitbox.right = enemy->centerPosition.x + enemy->hitboxWidthRadius;
	enemy->hitbox.top = enemy->centerPosition.y - enemy->hitboxHeightRadius;
	enemy->hitbox.bottom = enemy->centerPosition.y + enemy->hitboxHeightRadius;
}

/// <summary>
/// 주어진 좌표를 쳐다보는 함수. (총기의 방향을 설정)
/// </summary>
/// <param name="player"></param>
/// <param name="mouse"></param>
void Enemy_LookAt(Enemy* enemy, Vector2D lookDestination)
{
	Rotation_LookAtMouse(enemy, lookDestination);
	enemy->SetPOINT(enemy);
}

void Enemy_SetDirection(Enemy* enemy, Vector2D* playerPos)
{
	if (enemy->centerPosition.x < playerPos->x)
	{
		enemy->isRight = TRUE;
	}
	else if (enemy->centerPosition.x > playerPos->x)
	{
		enemy->isRight = FALSE;
	}
}

void Enemy_BulletInit(Bullet* bullet, Enemy* enemy)
{
	bullet->centerPosition = Vector2D_Add(enemy->centerPosition, Vector2D_Multiple(enemy->heightVector, enemy->radius));

	bullet->speed = Bullet_GetBulletSpeed(enemy->nowWeapon);

	bullet->radius = Bullet_GetBulletRadius(enemy->nowWeapon);

	bullet->velocity = Vector2D_Multiple(enemy->heightVector, bullet->speed);

	bullet->bulletType = Weapon_EnemyRifle;
}

void Enemy_BulletShot(Enemy* enemy)
{
	if (enemy->isRight)
	{
		enemy->nowStatus = EAnim_IdleRight;
	}
	else
	{
		enemy->nowStatus = EAnim_IdleLeft;
	}

	Bullet* bullet = Bullet_GetBullet(enemy->nowWeapon);
	if (bullet == NULL)
	{
		return;
	}

	Enemy_BulletInit(bullet, enemy);
}

void Enemy_BatonAttack(Enemy* enemy)
{
	if (enemy->isRight)
	{
		enemy->nowStatus = EAnim_AttackRight;
	}
	else
	{
		enemy->nowStatus = EAnim_AttackLeft;
	}

	Vector2D rectCenter = Vector2D_Add(enemy->centerPosition, Vector2D_Multiple(enemy->heightVector, enemy->radius));

	RECT BatonCollision = { rectCenter.x - 15, rectCenter.y - 15, rectCenter.x + 15, rectCenter.y + 15};

	if (enemy->frame == 7 || enemy->frame == 8)
	{
		if (CollisionCheck_AABB(BatonCollision, player.hitbox) == TRUE)
		{
			// 유저 피격.
			if (player.gracePeriod <= 0)
			{
				Player_Hit(&player);
			}
		}
	}
}

void Enemy_Attack(Enemy* enemy)
{
	if (enemy->nowType == EnemyType_Range)			// 원거리
	{
		if (enemy->fireDelay > 0)
		{
			return;
		}
		EnemySound(16);
		enemy->isShooting = TRUE;
		enemy->fireDelay = ENEMYFIREDELAY;
		enemy->animTime = 100;
		Enemy_BulletShot(enemy);
	}
	else if (enemy->nowType == EnemyType_Melee)		// 근거리
	{
		EnemySound(17);
		enemy->isShooting = TRUE;
		Enemy_BatonAttack(enemy);
	}
}

void Enemy_Follow(Enemy* enemy, Player* player)
{
	enemy->Move(enemy, enemy->heightVector, enemy->speed);

	if (enemy->isRight)
	{
		enemy->nowStatus = EAnim_MoveRight;
	}
	else
	{
		enemy->nowStatus = EAnim_MoveLeft;
	}
}

double Enemy_PlayerDistance(Enemy* enemy, Player* player)
{
	Vector2D temp = Vector2D_Sub(enemy->centerPosition, player->centerPosition);
	double distance = Vector2D_Size(temp);

	return distance;
}

/// <summary>
/// 오브젝트 풀에서 사용 가능한 투사체를 반환하는 함수
/// </summary>
/// <returns>투사체의 포인터</returns>
Enemy* Enemy_GetEnemy(int enemyType)
{
	for (int i = 0; i < ENEMYPOOL_MAX; ++i)
	{
		if (enemyPool[enemyType][i].isActive == FALSE)
		{
			enemyPool[enemyType][i].isActive = TRUE;
			Enemy* enemy = &enemyPool[enemyType][i];

			return enemy;
		}
	}

	return NULL;
	// 새로운 친구 생성 해야함.
}

void Enemy_StartEnemy(Enemy* enemy, POINT position)
{
	enemy->centerPosition.x = position.x;
	enemy->centerPosition.y = position.y;

	enemy->isDead = FALSE;
	enemy->isActive = TRUE;
	enemy->isCheckHit = TRUE;
	enemy->statusChanged = FALSE;

	enemy->heightVector.x = 0;
	enemy->heightVector.y = -1;

	enemy->frame = 0;
	enemy->frameDelta = 0;
	enemy->weaponFrame = 0;
	enemy->weaponFrameDelta = 0;

	enemy->fireDelay = 0;
	enemy->animTime = 0;

	enemy->plgVertex[0].x = enemy->centerPosition.x - enemy->radius;
	enemy->plgVertex[0].y = enemy->centerPosition.y - enemy->radius;

	enemy->plgVertex[1].x = enemy->centerPosition.x + enemy->radius;
	enemy->plgVertex[1].y = enemy->centerPosition.y - enemy->radius;

	enemy->plgVertex[2].x = enemy->centerPosition.x - enemy->radius;
	enemy->plgVertex[2].y = enemy->centerPosition.y + enemy->radius;

	enemy->plgVertex[3].x = enemy->centerPosition.x + enemy->radius;
	enemy->plgVertex[3].y = enemy->centerPosition.y + enemy->radius;

	Enemy_SetPOINT(enemy);
	Enemy_SetHitbox(enemy);
}

void Enemy_Death(Enemy* enemy)
{
	if (enemy->isDead)
	{
		return;
	}

	if (enemy->deathTime > 0)
	{
		return;
	}

	EnemySound(15);
	enemy->deathTime = ENEMYDEATHTIME;
	enemy->isDead = TRUE;
	enemy->isCheckHit = FALSE;
	enemy->statusChanged = TRUE;

	if (enemy->isRight)
	{
		enemy->nowStatus = EAnim_DeadRight;
	}
	else
	{
		enemy->nowStatus = EAnim_DeadLeft;
	}
}

