#include "framework.h"
#include "CSceneStage01.h"

#include "WinAPI.h"
#include "CInputManager.h"
#include "CTimeManager.h"
#include "CRenderManager.h"
#include "CEventManager.h"
#include "CCameraManager.h"
#include "CPathManager.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCameraController.h"
#include "CImageObject.h"
#include "CAniObject.h"
#include "CButton.h"
#include "CPanel.h"

CSceneStage01::CSceneStage01()
{
	pPlayer = nullptr;
}

CSceneStage01::~CSceneStage01()
{
}

void CSceneStage01::Init()
{
#pragma region BACKGROUND

	// 백그라운드 Image
	pBackGround = new CImageObject;
	CImage* pBackGroundImg = RESOURCE->LoadImg(L"BackGround", L"Image\\BackGround\\Map.png");
	pFrontGround = new CImageObject;
	CImage* pFrontGroundImg = RESOURCE->LoadImg(L"FrontGround", L"Image\\BackGround\\MapPiece1.png");
	Vector backGroundOffset = Vector(-2, -68);
	Vector frontGroundOffset = Vector(-1, (70 + backGroundOffset.y));
	float extension = WINSIZEY / (frontGroundOffset.y + (pFrontGroundImg->GetHeight()));

	// ImageObject 설정
	pBackGround->SetImage(pBackGroundImg);
	pBackGround->SetPos(Vector(0,0)); // offset조절
	pBackGround->SetOffset(backGroundOffset);
	pBackGround->SetExtension(extension);
	AddGameObject(pBackGround);

	pFrontGround->SetImage(pFrontGroundImg);
	pFrontGround->SetPos(Vector(0, 0)); // offset조절
	pFrontGround->SetOffset(frontGroundOffset);
	pFrontGround->SetExtension(extension);
	AddGameObject(pFrontGround);

	pFrontOceanObj = new CAniObject;
	CImage* pFrontOceanImage = RESOURCE->LoadImg(L"FrontOcean", L"Image\\BackGround\\FrontOcean.png");
	Vector frontOceanPos = Vector(-2, 226 + frontGroundOffset.y);
	pFrontOceanObj->SetImage(pFrontOceanImage);
	//pFrontOceanObj->SetPos(frontOceanOffset);
	pFrontOceanObj->SetExtension(extension);
	AddGameObject(pFrontOceanObj);

#pragma endregion

	pPlayer = new CPlayer();
	pPlayer->SetPos(200, WINSIZEY * 0.5f);
	pPlayer->SetExtension(extension);
	AddGameObject(pPlayer);

	CMonster* pMonster = new CMonster();
	pMonster->SetPos(500, WINSIZEY * 0.5f);
	pMonster->SetExtension(extension);
	AddGameObject(pMonster);

	pFrontOceanObj->GetAnimator()->CreateAnimation(L"BackGround\\FrontOcean1", pFrontOceanImage, 0.1f);

	CCameraController* pCamController = new CCameraController;
	AddGameObject(pCamController);
}

void CSceneStage01::Enter()
{
	CAMERA->FadeIn(0.25f);
	//CAMERA->SetTargetObj(pPlayer);
}

void CSceneStage01::Update()
{
	if (BUTTONDOWN(VK_ESCAPE))
	{
		CAMERA->FadeOut(0.25f);
		DELAYCHANGESCENE(GroupScene::Title, 0.25f);
	}
	// Zoom In/Out
	if (BUTTONSTAY(VK_F1))
	{
		float zoom = CAMERA->GetZoom();
		float setZoom = zoom - 0.005;
		if (setZoom > 0)
			CAMERA->SetZoom(setZoom);
		else
			CAMERA->SetZoom(0);
	}
	if (BUTTONSTAY(VK_F2))
	{
		float zoom = CAMERA->GetZoom();
		float setZoom = zoom + 0.005;
		CAMERA->SetZoom(setZoom);
	}

	pFrontOceanObj->GetAnimator()->Play(L"BackGround\\FrontOcean1");
}

void CSceneStage01::Render()
{
}

void CSceneStage01::Exit()
{
}

void CSceneStage01::Release()
{
}
