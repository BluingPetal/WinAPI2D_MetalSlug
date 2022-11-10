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
#include "CColliderObject.h"

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
	CImageObject* pFrontGround = new CImageObject;
	CImage* pFrontGroundImg = RESOURCE->LoadImg(L"FrontGround", L"Image\\BackGround\\MapPiece1.png");
	Vector backGroundOffset = Vector(-2, -68);
	Vector frontGroundOffset = Vector(-1, (70 + backGroundOffset.y));
	float extension = WINSIZEY / (frontGroundOffset.y + (pFrontGroundImg->GetHeight()));

	// ImageObject 설정
	pBackGround->SetImage(pBackGroundImg);
	pBackGround->SetPos(Vector(0,0));
	pBackGround->SetOffset(backGroundOffset);
	pBackGround->SetExtension(extension);
	AddGameObject(pBackGround);

	pFrontGround->SetImage(pFrontGroundImg);
	pFrontGround->SetPos(Vector(0, 0));
	pFrontGround->SetOffset(frontGroundOffset);
	pFrontGround->SetExtension(extension);
	AddGameObject(pFrontGround);

	CAniObject* pFrontOceanObj1 = new CAniObject;
	CImage* pFrontOceanImage = RESOURCE->LoadImg(L"FrontOcean", L"Image\\BackGround\\FrontOcean.png");
	Vector frontOceanPos1 = Vector(0, 226 + backGroundOffset.y);
	pFrontOceanObj1->SetImage(pFrontOceanImage);
	pFrontOceanObj1->SetOffset(frontOceanPos1);
	pFrontOceanObj1->SetExtension(extension);
	AddGameObject(pFrontOceanObj1);

	CAniObject* pFrontOceanObj2 = new CAniObject;
	Vector frontOceanPos2 = Vector(0, frontOceanPos1.y + 24);
	pFrontOceanObj2->SetImage(pFrontOceanImage);
	pFrontOceanObj2->SetOffset(frontOceanPos2);
	pFrontOceanObj2->SetExtension(extension);
	AddGameObject(pFrontOceanObj2);

#pragma endregion

	pPlayer = new CPlayer;
	pPlayer->SetPos(200, WINSIZEY * 0.5f);
	pPlayer->SetExtension(extension);
	AddGameObject(pPlayer);

	CMonster* pMonster = new CMonster();
	pMonster->SetPos(500, WINSIZEY * 0.5f);
	pMonster->SetExtension(extension);
	AddGameObject(pMonster);

	pFrontOceanObj1->GetAnimator()->CreateAnimation(L"BackGround\\FrontOcean1", pFrontOceanImage, 0.1f);
	pFrontOceanObj2->GetAnimator()->CreateAnimation(L"BackGround\\FrontOcean2", pFrontOceanImage, 0.1f);
	pFrontOceanObj1->GetAnimator()->Play(L"BackGround\\FrontOcean1");
	pFrontOceanObj2->GetAnimator()->Play(L"BackGround\\FrontOcean2");

	pFrontOceanObj1->SetPosWithFirstLt();
	pFrontOceanObj2->SetPosWithFirstLt();

	CColliderObject* pFrontOceanCollider = new CColliderObject;
	pFrontOceanCollider->SetName(L"frontOcean");
	pFrontOceanCollider->SetExtension(extension);
	Vector frontOceanOffset = pFrontOceanObj2->GetAnimator()->GetFirstAniFrame().offset;
	pFrontOceanCollider->SetOffset(frontOceanOffset);
	pFrontOceanCollider->SetPos(pFrontOceanObj2->GetPos());
	pFrontOceanCollider->SetScale(pFrontOceanObj2->GetAnimator()->GetFirstAniFrame().slice);
	pFrontOceanCollider->SetLayer(Layer::ForeGround);
	AddGameObject(pFrontOceanCollider);

	// CCameraController* pCamController = new CCameraController;
	// AddGameObject(pCamController);
}

void CSceneStage01::Enter()
{
	CAMERA->FadeIn(0.25f);
}

void CSceneStage01::Update()
{
	if (BUTTONDOWN(VK_ESCAPE))
	{
		CAMERA->FadeOut(0.25f);
		DELAYCHANGESCENE(GroupScene::Title, 0.25f);
	}

	if (BUTTONSTAY(VK_RIGHT))
	{
		//Vector prevPos = pBackGround->GetPos();
		//pBackGround->SetPos(prevPos + Vector(50 * DT, 0));
	}
	if (BUTTONSTAY(VK_LEFT))
	{
		//Vector prevPos = pBackGround->GetPos();
		//pBackGround->SetPos(prevPos + Vector(-50 * DT, 0));
	}

	//pFrontOceanObj1->GetAnimator()->Play(L"BackGround\\FrontOcean1");
	//pFrontOceanObj2->GetAnimator()->Play(L"BackGround\\FrontOcean2");
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
