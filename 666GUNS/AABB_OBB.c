#include <math.h>
#include <windows.h>

#include "GameDefine.h"
#include "Rotation.h"
#include "AABB_OBB.h"


/// <summary>
/// 박스와 점 비교
/// </summary>
/// <param name="A">박스</param>
/// <param name="B">점</param>
/// <returns>박스 안에 점이 있으면 true</returns>
BOOL CollisionCheck_RtoP(RECT A, Vector2D B)
{
	if (((A.left <= B.x) && (B.x <= A.right)) && 
		((A.top <= B.y) && (B.y <= A.bottom)))
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 원과 점 비교
/// </summary>
/// <param name="A">원</param>
/// <param name="B">점</param>
/// <returns>원 안에 점이 있으면 true</returns>
BOOL CollisionCheck_CtoP(Bullet A, Vector2D B)
{
	double x = pow(fabs(A.centerPosition.x - B.x), 2.0);
	double y = pow(fabs(A.centerPosition.y - B.y), 2.0);

	double d = sqrt(x + y);

	int r = A.radius / 2;

	if (r < d)
	{
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// 두 박스 비교
/// </summary>
/// <param name="A">박스1 (좌상,우하 점을 가지고 있는 구조체)</param>
/// <param name="B">박스2 (좌상,우하 점을 가지고 있는 구조체)</param>
/// <returns>겹치면 true</returns>
BOOL CollisionCheck_AABB(RECT A, RECT B)
{
	if (A.right < B.left || A.left > B.right)
	{
		return FALSE;
	}

	if (A.top > B.bottom || A.bottom < B.top)
	{
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// 축이 분리된 박스 2개 비교
/// 
/// 정석대로는 각각의 변 벡터를 구해야하지만
/// 사각형끼리의 충돌이기 때문에 직사각형에서 두 변은 평행하기 때문에
/// 너비와 높이 벡터를 이용했다.
/// </summary>
/// <param name="A">중점과, 4개의 모서리, 높이 벡터와 너비 벡터를 가진 사각형</param>
/// <param name="B">중점과, 4개의 모서리, 높이 벡터와 너비 벡터를 가진 사각형</param>
/// <returns>겹치면 true</returns>
BOOL CollisionCheck_OBB(Player A, Player B)
{
	// 두 물체 중점 벡터 (B -> A)
	Vector2D d = Vector2D_Sub(A.centerPosition, B.centerPosition);
	Vector2D aVec[2] = { A.widthVector, A.heightVector};
	Vector2D bVec[2] = { B.widthVector, B.heightVector };

	Vector2D aVertex1 = Vector2D_Sub(A.plgVertex[0], A.centerPosition);
	Vector2D bVertex1 = Vector2D_Sub(B.plgVertex[0], B.centerPosition);

	for (int i = 0; i < 2; ++i)
	{
		double sum = 0;

		// 물체 중점에서 모서리를 가리키는 벡터를 투영시킨다.
		double vertexScalar = Vector2D_ScalarProjection(aVertex1, aVec[i]);

		sum = sum + vertexScalar;

		// 모서리 벡터를 가지고 물체의 높이 벡터, 너비 벡터에 투영시키고,
		// 벡터화 해서 다른 물체의 베이스 벡터에 투영시킨다.
		for (int j = 0; j < 2; ++j)
		{
			// 모서리 벡터를 자신의 너비, 높이 벡터에 투영시킨다.
			double Scalar = Vector2D_ScalarProjection(bVertex1, bVec[j]);
			
			Scalar = floor(Scalar * 100000000000) / 100000000000;

			// 투영시켜 나온 스칼라 값을 베이스 벡터와 곱해서 벡터를 구한다.
			Vector2D vector = Vector2D_Multiple(bVec[j], Scalar);

			// 구한 벡터를 베이스 벡터에 투영시킨다.
			double baseTovector = Vector2D_ScalarProjection(vector, aVec[i]);

			sum = sum + baseTovector;
		}

		double dScalar = Vector2D_ScalarProjection(d, aVec[i]);
		if (sum < dScalar)
		{
			return FALSE;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		double sum = 0;

		// 물체 중점에서 모서리를 가리키는 벡터를 투영시킨다
		double vertexScalar = Vector2D_ScalarProjection(bVertex1, bVec[i]);

		sum = sum + vertexScalar;

		// 모서리 벡터를 가지고 물체의 높이 벡터, 너비 벡터에 투영시키고,
		// 벡터화 해서 다른 물체의 베이스 벡터에 투영시킨다.
		for (int j = 0; j < 2; ++j)
		{
			// 모서리 벡터를 자신의 너비, 높이 벡터에 투영시킨다.
			double Scalar = Vector2D_ScalarProjection(aVertex1, aVec[j]);

			Scalar = floor(Scalar * 100000000000) / 100000000000;

			// 투영시켜 나온 스칼라 값을 베이스 벡터와 곱해서 벡터를 구한다.
			Vector2D vector = Vector2D_Multiple(aVec[j], Scalar);

			// 구한 벡터를 베이스 벡터에 투영시킨다.
			double baseTovector = Vector2D_ScalarProjection(vector, bVec[i]);

			sum = sum + baseTovector;
		}

		double dScalar = Vector2D_ScalarProjection(d, bVec[i]);
		if (sum < dScalar)
		{
			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
/// 축이 분리된 박스와 원 비교
/// </summary>
/// <param name="A"></param>
/// <param name="B"></param>
/// <returns></returns>
BOOL CollisionCheck_OBB2(Player A, Bullet B)
{
	// 두 물체 중점 벡터 (B -> A)
	Vector2D d = Vector2D_Sub(A.centerPosition, B.centerPosition);
	Vector2D aVec[2] = { A.widthVector, A.heightVector };
	Vector2D bVec[2] = { { 1 , 0 }, { 0 , -1 } };

	Vector2D circleRadius = {0, B.radius};

	Vector2D aVertex1 = Vector2D_Sub(A.plgVertex[0], A.centerPosition);
	// Vector2D bVertex1 = Vector2D_Sub(Vector2D_Add(), B.centerPosition);

	for (int i = 0; i < 2; ++i)
	{
		double sum = 0;

		// 물체 중점에서 모서리를 가리키는 벡터를 투영시킨다.
		double vertexScalar = Vector2D_ScalarProjection(aVertex1, aVec[i]);

		sum = sum + vertexScalar;

		sum = sum + B.radius;

		double dScalar = Vector2D_ScalarProjection(d, aVec[i]);
		if (sum < dScalar)
		{
			return FALSE;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		double sum = 0;

		sum = sum + B.radius;

		// 모서리 벡터를 가지고 물체의 높이 벡터, 너비 벡터에 투영시키고,
		// 벡터화 해서 다른 물체의 베이스 벡터에 투영시킨다.
		for (int j = 0; j < 2; ++j)
		{
			// 모서리 벡터를 자신의 너비, 높이 벡터에 투영시킨다.
			double Scalar = Vector2D_ScalarProjection(aVertex1, aVec[j]);

			Scalar = floor(Scalar * 100000000000) / 100000000000;

			// 투영시켜 나온 스칼라 값을 베이스 벡터와 곱해서 벡터를 구한다.
			Vector2D vector = Vector2D_Multiple(aVec[j], Scalar);

			// 구한 벡터를 베이스 벡터에 투영시킨다.
			double baseTovector = Vector2D_ScalarProjection(vector, bVec[i]);

			sum = sum + baseTovector;
		}

		double dScalar = Vector2D_ScalarProjection(d, bVec[i]);
		if (sum < dScalar)
		{
			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
/// 두 원 충돌
/// 충돌 처리가 제대로 이뤄지지 않는다면
/// math.h를 확인하자
/// </summary>
/// <param name="A">원1 (중점과 지름을 가지고 있는 구조체)</param>
/// <param name="B">원2 (중점과 지름을 가지고 있는 구조체)</param>
/// <returns>겹치면 true</returns>
BOOL CollisionCheck_CtoC(Bullet A, Bullet B)
{
	// 제곱 함수 (절댓값 함수)
	double x = pow(fabs(A.centerPosition.x - B.centerPosition.x), 2.0);
	double y = pow(fabs(A.centerPosition.y - B.centerPosition.y), 2.0);

	// 두 점 사이의 거리
	double d = sqrt(x + y);

	// 두 원의 반지름 합
	double r = ((A.radius / 2) + (B.radius / 2));

	// 거리가 반지름의 합보다 크다면 FALSE
	if (d > r)
	{
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// 원과 박스의 충돌
/// </summary>
/// <param name="A">박스 (좌상, 우하 점을 가지고 있는 구조체)</param>
/// <param name="B">원 (중점과 지름을 가지고 있는 구조체)</param>
/// <returns>겹치면 true</returns>
BOOL CollisionCheck_BtoC(Bullet A, RECT B)
{
	Vector2D Left_Top = { B.left, B.top };
	Vector2D Left_Bottom = {B.left, B.bottom};
	Vector2D Right_Top = {B.right, B.top};
	Vector2D Right_Bottom = { B.right, B.bottom };

	int r = A.radius / 2;

	RECT expendRECT1 = { B.left - r, B.top,
		B.right + r, B.bottom };
	RECT expendRECT2 = { B.left, B.top - r,
		B.right, B.bottom + r };

	// 반지름 만큼 확장된 박스 안에 원의 중점이 있다면?
	if (CollisionCheck_RtoP(expendRECT1, A.centerPosition) == TRUE)
	{
		return TRUE;
	}

	if (CollisionCheck_RtoP(expendRECT2, A.centerPosition) == TRUE)
	{
		return TRUE;
	}

	// 박스의 모서리가 원 안에 있다면?
	if (CollisionCheck_CtoP(A, Left_Top) == TRUE)
	{
		return TRUE;
	}
	if (CollisionCheck_CtoP(A, Left_Bottom) == TRUE)
	{
		return TRUE;
	}
	if (CollisionCheck_CtoP(A, Right_Top) == TRUE)
	{
		return TRUE;
	}
	if (CollisionCheck_CtoP(A, Right_Bottom) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}