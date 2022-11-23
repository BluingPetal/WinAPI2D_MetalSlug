#pragma once

#include "resource.h"

//========================================
//##    전역변수(인스턴스, 윈도우 핸들)	##
//========================================

extern HINSTANCE hInst;
extern HWND hWnd;

//========================================
//##			윈도우 설정				##
//========================================

#define WINSTARTX   100
#define WINSTARTY   100
#define WINSIZEX	1280
#define	WINSIZEY	720
#define WINSTYLE	WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX

//========================================
//##			게임씬 그룹				##
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
//##				레이어				##
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
	Player,// 잠시동안

	PlayerMissile,
	BossMissile,
	Effect,
	ForeGround,

	Collider,
	Ui,
	Size,
};

//========================================
//##				타일					##
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
//##				상태					##
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
//##				무기					##
//========================================

enum class PlayerWeapon
{
	Knife,
	Pistol,
	HeavyMachineGun,
	Bomb,
};

static bool bShow;