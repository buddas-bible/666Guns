#pragma once
#include <windows.h>
#include "Player.h"

typedef struct CAMERA
{
	int width;
	int height;
	POINT LeftTopPOINT;
	POINT centerPosition;
	Player* player;

	void (*MoveCamera)(Player* player);
	void (*Update)(struct CAMERA* camera, double deltaTime);

} Camera;

void Camera_Init(Camera* camera, Player* player);
void Camera_Update(Camera* camera, double deltaTime);

void Camera_FollowPlayer(Camera* camera);
BOOL Camera_IsInRenderDistance(Camera* camera, POINT objectPosition);

POINT Camera_WorldToCameraPOINT(Camera* camera, POINT* worldPoint);
RECT Camera_WorldToCameraRECT(Camera* camera, RECT* rect);

POINT Camera_RelativeSpawnPOINT(Camera* camera, POINT* spawnPOINT);

void Camera_DEVMOVE(Camera* camera, POINT moveAt);

void Camera_SetShake(POINT newShake);