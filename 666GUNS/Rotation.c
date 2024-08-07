#define _USE_MATH_DEFINES
#include <math.h>
#include "Rotation.h"
#include "LineSegment.h"


/// <summary>
/// 사각형의 중점을 찾는 함수
/// </summary>
/// <param name="rect">죄상우하 좌표를 가지는 사각형</param>
/// <returns>중점 좌표</returns>
Vector2D Rotation_FindCenter(RECT rect)
{
	int x = (rect.right - rect.left) / 2;
	int y = (rect.bottom - rect.top) / 2;
	Vector2D center = { x, y };

	return center;
}

/*
Vector2D Rotation_FindLeftTop(Player player)
{
	int x = player.centerPosition.x - player.radius;
	int y = player.centerPosition.y - player.radius;
	Vector2D point = { x, y };
	return point;
}

Vector2D Rotation_FindRightTop(Player player)
{
	int x = player.centerPosition.x + player.radius;
	int y = player.centerPosition.y - player.radius;
	Vector2D point = { x, y };
	return point;
}

Vector2D Rotation_FindLeftBottom(Player player)
{
	int x = player.centerPosition.x - player.radius;
	int y = player.centerPosition.y + player.radius;
	Vector2D point = { x, y };
	return point;
}

Vector2D Rotation_FindRightBottom(Player player)
{
	int x = player.centerPosition.x + player.radius;
	int y = player.centerPosition.y + player.radius;
	Vector2D point = { x, y };
	return point;
}*/

/// <summary>
/// rarian theta만큼 회전하는 함수
/// </summary>
/// <param name="p">x,y좌표를 가지는 구조체</param>
/// <param name="theta">radian</param>
/// <returns>변환된</returns>
Vector2D Rotation_VectorRotation(Vector2D vector, double theta)
{
	// [ cos -sin]  [x]
	// [ sin cos ]  [y]
	// cos세타 x - sin세타 y, sin세타 x + cos세타 y
	double tempX = vector.x;
	double tempY = vector.y;

	vector.x = (cos(theta) * tempX) - (sin(theta) * tempY);
	vector.y = (sin(theta) * tempX) + (cos(theta) * tempY);

	return vector;
}

/// <summary>
/// 중점을 기준으로 사각형을 회전하는 함수
/// </summary>
/// <param name="rect">좌상우하 좌표를 가지는 사각형</param>
/// <param name="theta">radian 각도</param>
void Rotation_PlayerRotation(Player* player, double theta)
{
	// 방향 벡터를 변환함
	player->heightVector = Rotation_VectorRotation(player->heightVector, theta);
	player->widthVector = Rotation_VectorRotation(player->widthVector, theta);

	//센터에서 각 점의 벡터를 구한다.
	Vector2D OtoLT = Vector2D_Sub(player->plgVertex[0], player->centerPosition);
	Vector2D OtoRT = Vector2D_Sub(player->plgVertex[1], player->centerPosition);
	Vector2D OtoLB = Vector2D_Sub(player->plgVertex[2], player->centerPosition);
	Vector2D OtoRB = Vector2D_Sub(player->plgVertex[3], player->centerPosition);

	// 센터->점 벡터를 회전시킨다.
	OtoLT = Rotation_VectorRotation(OtoLT, theta);
	OtoRT = Rotation_VectorRotation(OtoRT, theta);
	OtoLB = Rotation_VectorRotation(OtoLB, theta);
	OtoRB = Rotation_VectorRotation(OtoRB, theta);

	// 회전시킨 벡터에 센터를 더한다.
	player->plgVertex[0] = Vector2D_Add(OtoLT, player->centerPosition);
	player->plgVertex[1] = Vector2D_Add(OtoRT, player->centerPosition);
	player->plgVertex[2] = Vector2D_Add(OtoLB, player->centerPosition);
	player->plgVertex[3] = Vector2D_Add(OtoRB, player->centerPosition);
}

/// <summary>
/// 플레이어 캐릭터가 마우스를 바라보게 하는 함수
/// </summary>
/// <param name="player">플레이어 구조체</param>
/// <param name="Mouse">마우스 좌표를 가진 구조체</param>
void Rotation_LookAtMouse(Player* player, Vector2D Mouse)
{
	// 캐릭터->마우스 벡터를 구한다.
	// 벡터를 정규화 시켜서 연산을 용이하게 한다.
	Vector2D playertoMouse = Vector2D_Normalization(Vector2D_Sub(Mouse, player->centerPosition));
	Vector2D playerHead = Vector2D_Add(player->centerPosition, player->heightVector);
	
	// 플레이어->마우스 벡터와 헤드 벡터와의 각도를 구함
	double theta = Vector2D_FindTheta(playertoMouse, player->heightVector);
	if (theta < 0.015)
	{
		return;
	}

	//theta = floor(theta * 10000000000000) / 10000000000000;

	// 시계인지 반시계인지 벡터의 방향성을 구함.
	double ccw = CollisionCheck_CCW(playerHead, Mouse, player->centerPosition);
	
	// 반시계
	if (ccw > 0)
	{
		Rotation_PlayerRotation(player, theta);
	}
	// 시계
	else if (ccw < 0)
	{
		Rotation_PlayerRotation(player, theta * -1.0);
	}
	else
	{
		return;
	}
}