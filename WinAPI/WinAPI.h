#pragma once

#include "resource.h"

//========================================
//##    ��������(�ν��Ͻ�, ������ �ڵ�)	##
//========================================

extern HINSTANCE hInst;
extern HWND hWnd;

//========================================
//##			������ ����				##
//========================================

#define WINSTARTX   100
#define WINSTARTY   100
#define WINSIZEX	1280
#define	WINSIZEY	720
#define WINSTYLE	WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX

//========================================
//##			���Ӿ� �׷�				##
//========================================

enum class GroupScene
{
	Title,

	SelectChar,
	Stage01,
	Boss,

	TileTool,
	AniTool,

	Size,
};

//========================================
//##				���̾�				##
//========================================

enum class Layer
{
	Default,
	Tile,
	BackGround,
	Unit,
	NPC,
	Item,
	Monster,
	Player,// ��õ���

	PlayerMissile,
	BossMissile,
	Effect,
	ForeGround,

	Collider,
	Ui,
	Size,
};

//========================================
//##				Ÿ��					##
//========================================

enum class TypeTile
{
	None,
	Ground,
	Platform,
	Wall,

	Size,
};

//========================================
//##				����					##
//========================================

enum class PlayerStatus
{
	Prepare,
	Idle,
	Move,
	Jump,
	Attack,
	SitAttack,

	Dead,
	Victory,
};

//========================================
//##				����					##
//========================================

enum class PlayerWeapon
{
	Knife,
	Pistol,
	HeavyMachineGun,
	Bomb,
};

static bool bShow;