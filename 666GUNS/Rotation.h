#pragma once
#include <math.h>
#include "Vector2D.h"
#include "Player.h"

Vector2D Rotation_FindCenter(RECT);

//Vector2D Rotation_FindLeftTop(Player);
//Vector2D Rotation_FindRightTop(Player);
//Vector2D Rotation_FindLeftBottom(Player);
//Vector2D Rotation_FindRightBottom(Player);

Vector2D Rotation_VectorRotation(Vector2D, double);

void Rotation_PlayerRotation(Player*, double);

void Rotation_LookAtMouse(Player*, Vector2D);
