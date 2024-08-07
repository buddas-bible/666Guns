#pragma once
#include "GameDefine.h"
#include "Vector2D.h"
#include "Bullet.h"
#include "Player.h"

double CollisionCheck_CCW2(Vector2D, Vector2D);
double CollisionCheck_CCW(Vector2D, Vector2D, Vector2D);

double CollisionCheck_LineSegment(Player, Bullet);
Vector2D CollisionCheck_IntersectVector2D(Player, Bullet);