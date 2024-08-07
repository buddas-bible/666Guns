#pragma once
#include <windows.h>

typedef struct
{
	double x;
	double y;

}Vector2D;

double Vector2D_Size(Vector2D);

Vector2D Vector2D_Add(Vector2D, Vector2D);
Vector2D Vector2D_Sub(Vector2D, Vector2D);
Vector2D Vector2D_Multiple(Vector2D, double);

double Vector2D_Dot(Vector2D, Vector2D);
double Vector2D_Cross(Vector2D, Vector2D);

double DegToRadian(double);

double Vector2D_ScalarProjection(Vector2D, Vector2D);

double Vector2D_FindTheta(Vector2D, Vector2D);
Vector2D Vector2D_Normalization(Vector2D);

double Vector2D_BiggerThen(Vector2D, Vector2D);
double Vector2D_Equality(Vector2D, Vector2D);