#include <math.h>
#include "Vector2D.h"
#include "GameDefine.h"


/// <summary>
/// 벡터 크기 구하기
/// </summary>
/// <param name="v1">벡터</param>
/// <returns>벡터 크기</returns>
double Vector2D_Size(Vector2D v1)
{
	double x = pow(v1.x, 2.0);
	double y = pow(v1.y, 2.0);
	double d = pow(x + y, 0.5);

	return d;
	//return floor(d * 100000000000) / 100000000000;
}

/// <summary>
/// 벡터 더하기
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>두 벡터 합한 결과</returns>
Vector2D Vector2D_Add(Vector2D v1, Vector2D v2)
{
	Vector2D AddResult = { v1.x + v2.x , v1.y + v2.y };

	return AddResult;
}

/// <summary>
/// 벡터 빼기
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>두 벡터 뺀 결과</returns>
Vector2D Vector2D_Sub(Vector2D v1, Vector2D v2)
{
	Vector2D SubResult = { v1.x - v2.x , v1.y - v2.y };

	return SubResult;
}

/// <summary>
/// 벡터 실수배
/// </summary>
/// <param name="v1"></param>
/// <param name="R"></param>
/// <returns>실수배 벡터</returns>
Vector2D Vector2D_Multiple(Vector2D v1, double R)
{
	v1.x = v1.x * R;
	v1.y = v1.y * R;

	return v1;
}

/// <summary>
/// 벡터 내적
/// </summary>
/// <param name="v1">벡터</param>
/// <param name="v2">벡터</param>
/// <returns>-1 ~ 1 사이 cos theta 값</returns>
double Vector2D_Dot(Vector2D v1, Vector2D v2)
{
	double DotResult = (v1.x * v2.x) + (v1.y * v2.y);

	return DotResult;
}

/// <summary>
/// 벡터 외적
/// 2D 평면 상에서 외적은 x,y값이 0. Z값만 존재
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>Z값</returns>
double Vector2D_Cross(Vector2D v1, Vector2D v2)
{
	double CrossResult = (v1.x * v2.y) - (v2.x * v1.y);

	return CrossResult;
}

/// <summary>
/// 육십분법을 호도법으로 변환해주는 함수
/// </summary>
/// <param name="deg">육십분법</param>
/// <returns>호도법</returns>
double DegToRadian(double deg)
{
	return deg / 180 * PI;
}

/// <summary>
/// 두 벡터 사이 각도를 구하는 함수
/// </summary>
/// <param name="v1">벡터</param>
/// <param name="v2">벡터</param>
/// <returns>theta</returns>
double Vector2D_FindTheta(Vector2D v1, Vector2D v2)
{
	v1 = Vector2D_Normalization(v1);
	v2 = Vector2D_Normalization(v2);

	// 내적을 구한다.
	double dot = Vector2D_Dot(v1, v2);

	if (dot > 1.0)
	{
		dot = 1.0;
	}
	if (dot < -1.0)
	{
		dot = -1.0;
	}
	// 내적을 이용한 각도 계산
	// V1, V2 내적값 = |V1||V2|cos(theta)
	// double vector1 = Vector2D_Size(v1);
	// double vector2 = Vector2D_Size(v2);
	// cos(theta) = (V1 dot V2) / |V1| |V2|
	// theta = acos((V1 dot V2) / |V1| |V2|)
	return acos(dot);
}

/// <summary>
/// v1을 정규화된 벡터 v2에 투영시키는 함수.
/// </summary>
/// <param name="v1">벡터 1</param>
/// <param name="v2">벡터 2</param>
/// <returns>투영된 벡터 크기</returns>
double Vector2D_ScalarProjection(Vector2D v1, Vector2D v2)
{
	double result;
	v2 = Vector2D_Normalization(v2);

	result = Vector2D_Dot(v1, v2);

	return fabs(result);
}



/// <summary>
/// 벡터 정규화
/// </summary>
/// <param name="v1">벡터</param>
/// <returns>정규화된 벡터</returns>
Vector2D Vector2D_Normalization(Vector2D v1)
{
	Vector2D result = { 0,0 };

	if (Vector2D_Size(v1) == 0)
	{
		return result;
	}

	result.x = v1.x / Vector2D_Size(v1);
	result.y = v1.y / Vector2D_Size(v1);

	return result;
}

/// <summary>
/// 두 벡터 크기 비교
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>v2가 크면 1, 아니면 0</returns>
double Vector2D_BiggerThen(Vector2D v1, Vector2D v2)
{
	if ((v1.x < v2.x) && (v1.y < v2.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// 두 벡터가 같은지 확인하고 1또는 0을 리턴
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>같으면 1, 아니면 0</returns>
double Vector2D_Equality(Vector2D v1, Vector2D v2)
{
	if ((v1.x == v2.x) && (v1.y == v2.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}