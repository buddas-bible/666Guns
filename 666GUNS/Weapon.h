#pragma once
#include <windows.h>

typedef struct WEAPON
{
	int weaponIndex;
	double heatGauge;
	double fireDelay;
	double overHeatTime;
	BOOL isOverHeat;
	double explodeTime;
	int frame;
	double frameDelta;
}Weapon;