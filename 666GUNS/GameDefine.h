#pragma once

#define PI 3.141592653589793

#define WALKSPEED 0.5
#define MAXDASHCOUNT 2
#define DASHCOOLTIME 7000
#define DASHTIME 500
#define GRACEPERIOD 3000
#define PLAYERDEATHTIME 3500
#define OVERHEATWAITTIME 3000

#define WEAPONDROPRATIO 30

#define ENEMYMELEESPEED 0.45
#define EMENYRANGESPEED 0.3

#define ENEMYDEATHTIME 2000
#define ENEMYFIREDELAY 4000
#define ENEMYMELEEDELAY 550

#define ENEMYPOOL_MAX 20
#define ENEMYRANGECOUNT_MAX 10

#define WEAPONTYPECOUNT 6
#define BULLETPOOL_MAX 30

#define FIREDELAY_PISTOL 500
#define FIREDELAY_RIFLE 50
#define FIREDELAY_RAILGUN 700
#define FIREDELAY_SHOTGUN 600

#define HEATINCREASE_PISTOL 15
#define HEATINCREASE_RIFLE 5
#define HEATINCREASE_RAILGUN 30
#define HEATINCREASE_SHOTGUN 20

#define SHOOTINGTIME_PISTOL 100
#define SHOOTINGTIME_RIFLE 100
#define SHOOTINGTIME_RAILGUN 100
#define SHOOTINGTIME_SHOTGUN 200

#define SHAKE_PISTOL 8
#define SHAKE_RILFE 5
#define SHAKE_RAILGUN 20
#define SHAKE_SHOTGUN 15
#define SHAKE_EXPLODE 22

#define HEAT_DECREASE 10.0

#define BULLETSPEED_PISTOL 1.5
#define BULLETSPEED_RIFLE 2.5
#define BULLETSPEED_SHOTGUN 2
#define BULLETSPEED_RAILGUN 3.5
#define BULLETSPEED_ENEMYRIFLE 0.5

#define BULLETRADIUS_PISTOL 20
#define BULLETRADIUS_RIFLE 20
#define BULLETRADIUS_SHOTGUN 20
#define BULLETRADIUS_RAILGUN 20
#define BULLETRADIUS_ENEMYRIFLE 10

enum GameStatus
{
	GameStatus_Playing,
	GameStatus_Win,
	GameStatus_Lose
};

enum Tag
{
	Tag_Player,
	Tag_Enemy,
	Tag_Wall
};

enum Scene
{
	Scene_Menu,
	Scene_Game,
	Scene_Ending,
	Scene_Credit
};

enum WeaponSide
{
	WeaponSide_Left,
	WeaponSide_Right
};

enum Weapon
{
	Weapon_Pistol,
	Weapon_Rifle,
	Weapon_Railgun,
	Weapon_Shotgun,
	Weapon_Dummy,
	Weapon_EnemyRifle,
	Weapon_EnemyBaton
};

enum Anim
{
	Anim_IdleLeft,
	Anim_IdleRight,
	Anim_DashLeft,
	Anim_DashRight,
	Anim_MoveLeft,
	Anim_MoveRight,
	Anim_DeadLeft,
	Anim_DeadRight
};

enum EnemyAnim
{
	EAnim_IdleLeft,
	EAnim_IdleRight,
	EAnim_MoveLeft,
	EAnim_MoveRight,
	EAnim_DeadLeft,
	EAnim_DeadRight,
	EAnim_AttackLeft,
	EAnim_AttackRight
};

enum EnemyType
{
	EnemyType_Range,
	EnemyType_Melee
};

enum Gun
{
	a,
	b,
	c,
	d,
	e,
	f,
	g,
	h
};