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

	TileTool,
	AniTool,

	SelectChar,
	Stage01,

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

	Monster,
	Player,// ��õ���

	Missile,
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
	Idle,
	Move,
	Jump,
	Attack,

	Dead,
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