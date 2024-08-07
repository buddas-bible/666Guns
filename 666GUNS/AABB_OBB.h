#pragma once

#include "Vector2D.h"

#include "Player.h"
#include "Circle.h"
#include "Bullet.h"


BOOL CollisionCheck_RtoP(RECT A, Vector2D B);
BOOL CollisionCheck_CtoP(Bullet A, Vector2D B);

BOOL CollisionCheck_AABB(RECT A, RECT B);

BOOL CollisionCheck_OBB(Player A, Player B);

BOOL CollisionCheck_OBB2(Player A, Bullet B);

BOOL CollisionCheck_CtoC(Bullet A, Bullet B);
BOOL CollisionCheck_BtoC(Bullet A, RECT B);