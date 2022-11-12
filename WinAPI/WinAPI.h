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

	TileTool,
	AniTool,

	SelectChar,
	Stage01,

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

	Monster,
	Player,// 잠시동안

	Missile,
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
	Idle,
	Move,
	Jump,
	Attack,

	Dead,
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