#include "windows.h"

#include "LineSegment.h"



double CollisionCheck_CCW2(Vector2D v1, Vector2D v2)
{
	return Vector2D_Cross(v1, v2);
}

/// <summary>
/// v1이 v3를 기준으로 본 v2는 시계인가 반시계인가?
/// 반시계 = 양수
/// 시계 = 음수
/// </summary>
double CollisionCheck_CCW(Vector2D v1, Vector2D v2, Vector2D v3)
{
	Vector2D vectorA = Vector2D_Sub(v1, v3);
	Vector2D vecterB = Vector2D_Sub(v2, v3);
	return CollisionCheck_CCW2(vectorA, vecterB);
}

/// <summary>
/// 점과 선분 간의 충돌 확인
/// </summary>
/// <returns></returns>
double CollisionCheck_LineSegment(Player player, Bullet bullet)
{
	// 점의 입장에서 선분을 확인
	double bTp1 = CollisionCheck_CCW(player.centerPosition, player.prePosition, bullet.centerPosition);
	double bTp2 = CollisionCheck_CCW(player.centerPosition, player.prePosition, bullet.prePosition);


	// 선분의 입장에서 점 or 점의 이전 좌표를 확인
	double pTb1 = CollisionCheck_CCW(bullet.centerPosition, bullet.prePosition, player.centerPosition);
	double pTb2 = CollisionCheck_CCW(bullet.centerPosition, bullet.prePosition, player.prePosition);

	if (bTp1 * bTp2 == 0 && pTb1 * pTb2 == 0)
	{
		return FALSE;
	}
	else if (bTp1* bTp2 < 0 && pTb1 * pTb2 == 0)
	{
		return TRUE;
	}
	else if (bTp1* bTp2 == 0 && pTb1 * pTb2 < 0)
	{
		return TRUE;
	}
	else if (bTp1 * bTp2 < 0 && pTb1 * pTb2 < 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// 선분의 교차점을 찾는 함수
/// 만약 시간이 남아 총알이 도탄하는 기믹까지 넣는다면
/// 써도 나쁘지 않을듯함 
/// </summary>
/// <param name="player"></param>
/// <param name="bullet"></param>
Vector2D CollisionCheck_IntersectVector2D(Player player, Bullet bullet)
{
	double under = (player.centerPosition.x - player.prePosition.x) * (bullet.centerPosition.y - bullet.prePosition.y)
		- (player.centerPosition.y - player.prePosition.y) * (bullet.centerPosition.x - bullet.prePosition.x);

	double x = (((player.centerPosition.x * player.prePosition.y) - (player.centerPosition.y * player.prePosition.x)) * (bullet.centerPosition.x - bullet.prePosition.x))
		- ((player.centerPosition.x - player.prePosition.x) * ((bullet.centerPosition.x * bullet.prePosition.y) - (bullet.centerPosition.y * bullet.prePosition.x)));

	double y = (((player.centerPosition.x * player.prePosition.y) - (player.centerPosition.y * player.prePosition.x)) * (bullet.centerPosition.y - bullet.prePosition.y))
		- ((player.centerPosition.y - player.prePosition.y) * ((bullet.centerPosition.x * bullet.prePosition.y) - (bullet.centerPosition.y * bullet.prePosition.x)));

	// 교차점
	x = x / under;
	y = y / under;

	Vector2D intersectVector2D = { x, y };
	return intersectVector2D;

	/*
	if (bullet.nVector.x == - 1)
	{
		if (player.precenterPosition.x <= x)
		{
			player.centerPosition.x = x - abs(player.centerPosition.x - x);
			// player.centerPosition.x = obstruction.centerPosition.x - EPSILON;
		}
		else
		{
			player.centerPosition.x = x + abs(player.centerPosition.x - x);
			// player.centerPosition.x = obstruction.centerPosition.x + EPSILON;
		}
	}
	else if (bullet.nVector.y == - 1)
	{
		if (player.precenterPosition.y <= y)
		{
			player.centerPosition.y = y - abs(player.centerPosition.y - y);
		}
		else
		{
			player.centerPosition.y = y + abs(player.centerPosition.y - y);
		}
	}

	player.velocity.x = player.velocity.x * obstruction.nVector.x * 0.8;
	player.velocity.y = player.velocity.y * obstruction.nVector.y * 0.8;
	*/

}