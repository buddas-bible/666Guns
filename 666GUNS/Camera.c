#include "Camera.h"

#define RENDER_DISTANCE 2000
#define WIDTH_COUNT 5
#define HEIGHT_COUNT 6

BOOL isCameraShake = FALSE;
double shakeDelta = 0;
POINT shakePOINT;

void Camera_Init(Camera* camera, Player* player)
{
	camera->player = player;
	//camera->MoveCamera = Camera_FollowPlayer;
	camera->Update = Camera_Update;

	camera->width = 1920;
	camera->height = 1080;

	camera->centerPosition.x = 960;
	camera->centerPosition.y = 540;

	camera->LeftTopPOINT.x = camera->centerPosition.x - camera->width / 2;
	camera->LeftTopPOINT.y = camera->centerPosition.y - camera->height / 2;
}

void Camera_Update(Camera* camera, double deltaTime)
{
	Camera_FollowPlayer(camera);
	if (shakeDelta > 0)
	{
		shakeDelta -= deltaTime;
		Camera_DEVMOVE(camera, shakePOINT);
	}
}

/// 캐릭터를 따라가는 카메라.
/// 캐릭터를 중심에 두고 따라간다.
/// 카메라가 월드 좌표를 넘어가려고 하면 안넘어가도록 고정시켜준다.
void Camera_FollowPlayer(Camera* camera)
{
	camera->centerPosition.x = (int)camera->player->centerPosition.x;
	camera->centerPosition.y = (int)camera->player->centerPosition.y;

	if (camera->centerPosition.x <= camera->width / 2)
	{
		camera->centerPosition.x = camera->width / 2;
	}

	if (camera->centerPosition.x >= WIDTH_COUNT * camera->width - camera->width / 2)
	{
		camera->centerPosition.x = WIDTH_COUNT * camera->width - camera->width / 2;
	}

	if (camera->centerPosition.y <= camera->height / 2)
	{
		camera->centerPosition.y = camera->height / 2;
	}

	if (camera->centerPosition.y >= HEIGHT_COUNT * camera->height - camera->height / 2)
	{
		camera->centerPosition.y = HEIGHT_COUNT * camera->height - camera->height / 2;
	}
	
	camera->LeftTopPOINT.x = camera->centerPosition.x - camera->width / 2;
	camera->LeftTopPOINT.y = camera->centerPosition.y - camera->height / 2;
}

BOOL Camera_IsInRenderDistance(Camera* camera, POINT objectPosition)
{
	int x = camera->centerPosition.x - objectPosition.x;
	int y = camera->centerPosition.y - objectPosition.y;

	if ((x * x) + (y * y) <= RENDER_DISTANCE * RENDER_DISTANCE)
	{
		return TRUE;
	}
	return FALSE;
}

POINT Camera_WorldToCameraPOINT(Camera* camera, POINT* worldPOINT)
{
	POINT localPOINT;
	localPOINT.x = worldPOINT->x - camera->LeftTopPOINT.x;
	localPOINT.y = worldPOINT->y - camera->LeftTopPOINT.y;

	return localPOINT;
}

RECT Camera_WorldToCameraRECT(Camera* camera, RECT* worldRECT)
{
	RECT localRECT;
	localRECT.left = worldRECT->left - camera->LeftTopPOINT.x;
	localRECT.right = worldRECT->right - camera->LeftTopPOINT.x;
	localRECT.top = worldRECT->top - camera->LeftTopPOINT.y;
	localRECT.bottom = worldRECT->bottom - camera->LeftTopPOINT.y;
	return localRECT;
}

POINT Camera_RelativeSpawnPOINT(Camera* camera, POINT* spawnPOINT)
{
	POINT localPOINT;
	localPOINT.x = camera->LeftTopPOINT.x + spawnPOINT->x;
	localPOINT.y = camera->LeftTopPOINT.y + spawnPOINT->y;

	return localPOINT;
}

void Camera_DEVMOVE(Camera* camera, POINT moveAt)
{
	camera->centerPosition.x += moveAt.x;
	camera->centerPosition.y += moveAt.y;

	if (camera->centerPosition.x <= camera->width / 2)
	{
		camera->centerPosition.x = camera->width / 2;
	}

	if (camera->centerPosition.x >= WIDTH_COUNT * camera->width - camera->width / 2)
	{
		camera->centerPosition.x = WIDTH_COUNT * camera->width - camera->width / 2;
	}

	if (camera->centerPosition.y <= camera->height / 2)
	{
		camera->centerPosition.y = camera->height / 2;
	}

	if (camera->centerPosition.y >= HEIGHT_COUNT * camera->height - camera->height / 2)
	{
		camera->centerPosition.y = HEIGHT_COUNT * camera->height - camera->height / 2;
	}

	camera->LeftTopPOINT.x = camera->centerPosition.x - camera->width / 2;
	camera->LeftTopPOINT.y = camera->centerPosition.y - camera->height / 2;
}

void Camera_SetShake(POINT newShake)
{
	shakeDelta = 50;
	shakePOINT = newShake;
}
