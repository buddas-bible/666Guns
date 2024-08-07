#include <stdlib.h>
#include <time.h>

#include "Player.h"
#include "Rotation.h"
#include "Bullet.h"
#include "Core.h"
#include "SoundManager.h"
#include "Camera.h"

#define DASH_SPEED 1

extern RECT buildPosition[44];

/// <summary>
/// Player 구조체 초기화. 함수포인터에 필요 함수를 연결.
/// </summary>
/// <param name="player"></param>
void Player_Init(Player* player)
{
	player->centerPosition.x = 4500;
	player->centerPosition.y = 1000;
	player->radius = 128;

	player->heightVector.x = 0;
	player->heightVector.y = -1;

	//player->widthVector.x = 1;
	//player->widthVector.y = 0;

	player->nowStatus = Anim_IdleRight;
	player->nowWeapon = Weapon_Pistol;
	player->statusChanged = FALSE;

	player->isCheckHit = TRUE;
	player->isDead = FALSE;
	player->isShooting = FALSE;
	player->deathTime = 0;

	player->shootingTime = 0;

	player->gracePeriod = 0;

	player->dashCount = 2;				// 대쉬 갯수 체크
	player->dashCoolTimeCount = 0;	// 대쉬 쿨타임 체크
	player->dashTimeCount = 0;

	player->hitboxHeightRadius = 50;
	player->hitboxWidthRadius = 25;

	player->tag = Tag_Player;

	player->plgVertex[0].x = player->centerPosition.x - player->radius;
	player->plgVertex[0].y = player->centerPosition.y - player->radius;

	player->plgVertex[1].x = player->centerPosition.x + player->radius;
	player->plgVertex[1].y = player->centerPosition.y - player->radius;

	player->plgVertex[2].x = player->centerPosition.x - player->radius;
	player->plgVertex[2].y = player->centerPosition.y + player->radius;

	player->plgVertex[3].x = player->centerPosition.x + player->radius;
	player->plgVertex[3].y = player->centerPosition.y + player->radius;

	player->Move = Player_Move;
	player->Rotate = Player_Rotate;
	player->SetPOINT = Player_SetPOINT;
	player->LookAt = Player_LookAt;
	player->FireWeapon = Player_FireWeapon;
	player->Update = Player_Update;			// Core.h 에 있는 Player_Update 함수다.

	player->SetPOINT(player);
	Player_SetHitbox(player);
}

/// <summary>
/// 플레이어 이동
/// </summary>
/// <param name="player"></param>
/// <param name="moveAt"></param>
void Player_Move(Player* player, Vector2D moveDirection, double speed)
{
	Vector2D moveAt;
	moveAt.x = moveDirection.x * speed * Engine_GetDeltaTime();
	moveAt.y = moveDirection.y * speed * Engine_GetDeltaTime();

	player->prePosition = player->centerPosition;

	player->centerPosition.x += moveAt.x;
	player->centerPosition.y += moveAt.y;

	Player_SetHitbox(player);
	
	for (int i = 0; i < 44; ++i)
	{
		if (i == 6)
		{
			continue;
		}

		if (CollisionCheck_AABB(player->hitbox, buildPosition[i]))
		{
			//player->centerPosition = player->prePosition;
			
			player->centerPosition.x -= moveAt.x;
			player->centerPosition.y -= moveAt.y;

			Player_SetHitbox(player);

			return;
		}
	}

	player->plgVertex[0].x += moveAt.x;
	player->plgVertex[0].y += moveAt.y;

	player->plgVertex[1].x += moveAt.x;
	player->plgVertex[1].y += moveAt.y;

	player->plgVertex[2].x += moveAt.x;
	player->plgVertex[2].y += moveAt.y;

	player->SetPOINT(player);
}

/// <summary>
/// 플레이어 회전
/// </summary>
/// <param name="player"></param>
/// <param name="theta"></param>
void Player_Rotate(Player* player, double theta)
{
	Rotation_PlayerRotation(player, theta);
	player->SetPOINT(player);
}

/// <summary>
/// plgVertex의 값을 plgPOINT로 복사하면서 캐릭터의 로컬형태로 변경(카메라를 위해)
/// </summary>
/// <param name="player"></param>
void Player_SetPOINT(Player* player)
{
	player->plgPOINT[0].x = (int)(player->plgVertex[0].x - player->centerPosition.x +player->radius);
	player->plgPOINT[0].y = (int)(player->plgVertex[0].y - player->centerPosition.y +player->radius);

	player->plgPOINT[1].x = (int)(player->plgVertex[1].x - player->centerPosition.x + player->radius);
	player->plgPOINT[1].y = (int)(player->plgVertex[1].y - player->centerPosition.y + player->radius);

	player->plgPOINT[2].x = (int)(player->plgVertex[2].x - player->centerPosition.x + player->radius);
	player->plgPOINT[2].y = (int)(player->plgVertex[2].y - player->centerPosition.y + player->radius);

	player->plgPOINT[3].x = (int)(player->plgVertex[3].x - player->centerPosition.x + player->radius);
	player->plgPOINT[3].y = (int)(player->plgVertex[3].y - player->centerPosition.y + player->radius);
}

void Player_SetHitbox(Player* player)
{
	player->hitbox.left = player->centerPosition.x - player->hitboxWidthRadius;
	player->hitbox.right = player->centerPosition.x + player->hitboxWidthRadius;
	player->hitbox.top = player->centerPosition.y - player->hitboxHeightRadius;
	player->hitbox.bottom = player->centerPosition.y + player->hitboxHeightRadius;
}


/// <summary>
/// 주어진 좌표를 쳐다보는 함수. (총기의 방향을 설정)
/// </summary>
/// <param name="player"></param>
/// <param name="mouse"></param>
void Player_LookAt(Player* player, Vector2D lookDestination)
{
	Rotation_LookAtMouse(player, lookDestination);
	player->SetPOINT(player);
}

void Player_FireWeapon(Player* player, int weaponside)
{
	POINT shakePOINT;

	if (player->selectedWeapon[0]->explodeTime + player->selectedWeapon[1]->explodeTime > 0)
	{
		shakePOINT.x = SHAKE_EXPLODE - rand() % SHAKE_EXPLODE;
		shakePOINT.y = SHAKE_EXPLODE - rand() % SHAKE_EXPLODE;
		Camera_SetShake(shakePOINT);
	}

	if (player->selectedWeapon[weaponside]->weaponIndex == -1)
	{
		return;
	}

	if (player->selectedWeapon[weaponside]->isOverHeat)
	{
		return;
	}

	if (player->selectedWeapon[weaponside]->fireDelay > 0)
	{
		return;
	}

	player->nowWeapon = player->selectedWeapon[weaponside]->weaponIndex;

	

	switch (player->selectedWeapon[weaponside]->weaponIndex)
	{
		case Weapon_Pistol:
		{
			player->selectedWeapon[weaponside]->fireDelay = FIREDELAY_PISTOL;
			player->selectedWeapon[weaponside]->heatGauge += HEATINCREASE_PISTOL;
			player->shootingTime = SHOOTINGTIME_PISTOL;
			srand(time(NULL));
			shakePOINT.x = SHAKE_PISTOL - rand() % SHAKE_PISTOL;
			shakePOINT.y = SHAKE_PISTOL - rand() % SHAKE_PISTOL;
			Camera_SetShake(shakePOINT);
			// 오브젝트 풀에서 사용가능한 투사체를 찾음
			Bullet* bullet = Bullet_GetBullet(player->nowWeapon);
			if (bullet == NULL)
			{
				return;
			}

			// 투사체를 초기화
			Bullet_Init(bullet, player);
		}
		break;

		case Weapon_Rifle:
		{
			player->selectedWeapon[weaponside]->fireDelay = FIREDELAY_RIFLE;
			player->selectedWeapon[weaponside]->heatGauge += HEATINCREASE_RIFLE;
			player->shootingTime = SHOOTINGTIME_RIFLE;
			shakePOINT.x = SHAKE_RILFE - rand() % SHAKE_RILFE;
			shakePOINT.y = SHAKE_RILFE - rand() % SHAKE_RILFE;
			Camera_SetShake(shakePOINT);
			// 오브젝트 풀에서 사용가능한 투사체를 찾음
			Bullet* bullet = Bullet_GetBullet(player->nowWeapon);
			if (bullet == NULL)
			{
				return;
			}

			// 투사체를 초기화
			Bullet_Init(bullet, player);
		}
		break;

		case Weapon_Railgun:
		{
			player->selectedWeapon[weaponside]->fireDelay = FIREDELAY_RAILGUN;
			player->selectedWeapon[weaponside]->heatGauge += HEATINCREASE_RAILGUN;
			player->shootingTime = SHOOTINGTIME_RAILGUN;
			shakePOINT.x = SHAKE_RAILGUN - rand() % SHAKE_RAILGUN;
			shakePOINT.y = SHAKE_RAILGUN - rand() % SHAKE_RAILGUN;
			Camera_SetShake(shakePOINT);
			// 오브젝트 풀에서 사용가능한 투사체를 찾음
			Bullet* bullet1 = Bullet_GetBullet(player->nowWeapon);
			Bullet* bullet2 = Bullet_GetBullet(player->nowWeapon);
			Bullet* bullet3 = Bullet_GetBullet(player->nowWeapon);
			Bullet* bullet4 = Bullet_GetBullet(player->nowWeapon);
			Bullet* bullet5 = Bullet_GetBullet(player->nowWeapon);

			if (bullet1 == NULL)
			{
				return;
			}

			if (bullet2 == NULL)
			{
				return;
			}

			if (bullet3 == NULL)
			{
				return;
			}

			if (bullet4 == NULL)
			{
				return;
			}

			if (bullet5 == NULL)
			{
				return;
			}

			// 투사체를 초기화
			Bullet_Init(bullet1, player);
			Bullet_Init(bullet2, player);
			Bullet_Init(bullet3, player);
			Bullet_Init(bullet4, player);
			Bullet_Init(bullet5, player);
		}
		break;

		case Weapon_Shotgun:
		{
			player->selectedWeapon[weaponside]->fireDelay = FIREDELAY_SHOTGUN;
			player->selectedWeapon[weaponside]->heatGauge += HEATINCREASE_SHOTGUN;
			player->shootingTime = SHOOTINGTIME_SHOTGUN;
			shakePOINT.x = SHAKE_SHOTGUN - rand() % SHAKE_SHOTGUN;
			shakePOINT.y = SHAKE_SHOTGUN - rand() % SHAKE_SHOTGUN;
			Camera_SetShake(shakePOINT);
			// 오브젝트 풀에서 사용가능한 투사체를 찾음
			Bullet* bullet1 = Bullet_GetBullet(player->nowWeapon);
			Bullet* bullet2 = Bullet_GetBullet(player->nowWeapon);
			Bullet* bullet3 = Bullet_GetBullet(player->nowWeapon);

			if (bullet1 == NULL)
			{
				return;
			}

			if (bullet2 == NULL)
			{
				return;
			}

			if (bullet3 == NULL)
			{
				return;
			}

			Vector2D vertical;
			vertical.x = player->heightVector.y;
			vertical.y = -player->heightVector.x;

			// 투사체를 초기화
			Bullet_Init(bullet1, player);
			Bullet_Init(bullet2, player);
			Bullet_Init(bullet3, player);

			bullet1->lifeTime = 200;
			bullet2->lifeTime = 200;
			bullet3->lifeTime = 200;

			bullet1->headVector.x += vertical.x/6;
			bullet1->headVector.y += vertical.y/6;
			bullet1->headVector = Vector2D_Normalization(bullet1->headVector);
			bullet1->velocity = Vector2D_Multiple(bullet1->headVector, bullet1->speed);

			bullet2->headVector.x -= vertical.x/6;
			bullet2->headVector.y -= vertical.y/6;
			bullet2->headVector = Vector2D_Normalization(bullet2->headVector);
			bullet2->velocity = Vector2D_Multiple(bullet2->headVector, bullet2->speed);

		}
		break;

		default:
			break;
	}

	player->isShooting = TRUE;

	if (player->selectedWeapon[weaponside]->heatGauge >= 100)
	{
		PlayerSound(9);
		player->selectedWeapon[weaponside]->heatGauge = 100;
		player->selectedWeapon[weaponside]->overHeatTime = OVERHEATWAITTIME;
		player->selectedWeapon[weaponside]->isOverHeat = TRUE;
		player->selectedWeapon[weaponside]->explodeTime = 650;

		shakePOINT.x = SHAKE_EXPLODE - rand() % SHAKE_EXPLODE;
		shakePOINT.y = SHAKE_EXPLODE - rand() % SHAKE_EXPLODE;
		Camera_SetShake(shakePOINT);
	}
}

void Player_SetDirection(Player* player, Vector2D* mousePos)
{
	if (player->centerPosition.x < mousePos->x)
	{
		player->isRight = TRUE;
	}
	else if (player->centerPosition.x > mousePos->x)
	{
		player->isRight = FALSE;
	}
}

void Player_DashStart(Player* player, Vector2D _dashAt)
{
	if (player->dashCount <= 0)
	{
		return;
	}

	if (player->dashTimeCount > 0)
	{
		return;
	}

	if (Vector2D_Size(_dashAt) == 0)
	{
		return;
	}

	player->dashTimeCount = DASHTIME;
	player->statusChanged = TRUE;
	player->dashCount--;
	player->dashAt = Vector2D_Normalization(_dashAt);
	player->isCheckHit = FALSE;

	if (player->dashAt.x > 0)
	{
		player->nowStatus = Anim_DashRight;
	}
	else
	{
		player->nowStatus = Anim_DashLeft;
	}
}

void Player_Dash(Player* player)
{
	Player_Move(player, player->dashAt, DASH_SPEED);
}

void Player_Hit(Player* player)
{
	if (player->isDead)
	{
		return;
	}

	*(player->playerHP) -= 1;
	player->gracePeriod = GRACEPERIOD;
	Play_Sound(8);
	
	if (*(player->playerHP) <= 0)
	{
		Player_Death(player);
	}
}

void Player_Death(Player* player)
{
	if (player->isDead)
	{
		return;
	}

	if (player->deathTime > 0)
	{
		return;
	}

	SoundStop(1);
	player->deathTime = PLAYERDEATHTIME;
	player->isDead = TRUE;
	player->isCheckHit = FALSE;
	player->statusChanged = TRUE;
	player->gracePeriod = 0;

	if (player->isRight)
	{
		player->nowStatus = Anim_DeadRight;
	}
	else
	{
		player->nowStatus = Anim_DeadLeft;
	}
}

