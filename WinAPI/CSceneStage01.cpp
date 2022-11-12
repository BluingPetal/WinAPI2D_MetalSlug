#include "framework.h"
#include "CSceneStage01.h"

#include "CPlayer.h"
#include "CCameraController.h"
#include "CImageObject.h"
#include "CAniObject.h"
#include "CButton.h"
#include "CPanel.h"
#include "CColliderObject.h"
#include "CConga.h"
#include "CFontImageObj.h"

CSceneStage01::CSceneStage01()
{
	m_pPlayer = nullptr;
	m_pBackGround = nullptr;
	m_pObstacle = nullptr;
	m_pFontImgObj = nullptr;
	
	m_fAccTime = 0;
	m_fPlayerMaxPosX = 0;
}

CSceneStage01::~CSceneStage01()
{
}

void CSceneStage01::Init()
{
#pragma region BACKGROUND

	// 백그라운드 Image
	m_pBackGround = new CImageObject;
	CImage* pBackGroundImg = RESOURCE->LoadImg(L"BackGround", L"Image\\BackGround\\Map.png");
	CImageObject* pFrontGround = new CImageObject;
	CImage* pFrontGroundImg = RESOURCE->LoadImg(L"FrontGround", L"Image\\BackGround\\MapPiece1.png");
	Vector backGroundOffset = Vector(-2, -68);
	Vector frontGroundOffset = Vector(-1, (70 + backGroundOffset.y));
	float extension = WINSIZEY / (frontGroundOffset.y + (pFrontGroundImg->GetHeight()));

	// ImageObject 설정
	m_pBackGround->SetImage(pBackGroundImg);
	m_pBackGround->SetPos(Vector(0,0));
	m_pBackGround->SetOffset(backGroundOffset);
	m_pBackGround->SetExtension(extension);
	AddGameObject(m_pBackGround);

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

#pragma region OBJECT

	m_pPlayer = new CPlayer;
	m_pPlayer->SetPos(200, WINSIZEY * 0.5f);
	m_pPlayer->SetExtension(extension);
	m_fPlayerMaxPosX = m_pPlayer->GetPos().x;
	AddGameObject(m_pPlayer);

	CConga* pConga = new CConga;
	pConga->SetPos(500, WINSIZEY * 0.5f);
	pConga->SetExtension(extension);
	AddGameObject(pConga);

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

	m_pObstacle = new CColliderObject;
	m_pObstacle->SetName(L"obstacle");
	m_pObstacle->SetExtension(extension);
	m_pObstacle->SetPos(0, WINSIZEY * 0.5f);
	m_pObstacle->SetScale(5, WINSIZEY / extension);
	m_pObstacle->SetLayer(Layer::ForeGround);
	AddGameObject(m_pObstacle);

#pragma endregion

	m_pFontImgObj = new CFontImageObj;
	m_pFontImgObj->SetExtension(extension + 1);
	AddGameObject(m_pFontImgObj);

	// CCameraController* pCamController = new CCameraController;
	// AddGameObject(pCamController);
}

void CSceneStage01::Enter()
{
	CAMERA->FadeIn(0.25f);

	m_pFontImgObj->SetInterval(1.1f);
	m_pFontImgObj->CreateImg(L"insert coin", Vector(WINSIZEX * 0.65f, WINSIZEY * 0.05f), 11, FontType::Coin);
}

void CSceneStage01::Update()
{
	m_fAccTime += DT;

	// insert coin 깜박거리는 효과
	if (m_fAccTime >= 1.f)
	{
		for (int i = 0; i < m_pFontImgObj->GetImageObj().size(); i++)
		{
			CImageObject* imgObj = m_pFontImgObj->GetImageObj()[i];
			imgObj->SetAlpha(!(imgObj->GetAlpha()));
		}
		m_fAccTime = 0;
	}
	for (int i = 0; i < m_pFontImgObj->GetImageObj().size(); i++)
	{
		CImageObject* imgObj = m_pFontImgObj->GetImageObj()[i];
		if (i != 0)
		{
			CImageObject* prevObj = m_pFontImgObj->GetImageObj()[i-1];
			Vector posDiff = imgObj->GetPos() - prevObj->GetPos();
			imgObj->SetPos(prevObj->GetPos() + CAMERA->ScreenToWorldPoint(posDiff * i));
		}
		else
		{
			imgObj->SetPos(CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.65f, WINSIZEY * 0.05f)));
		}
	}

	if (BUTTONDOWN(VK_ESCAPE))
	{
		CAMERA->FadeOut(0.25f);
		DELAYCHANGESCENE(GroupScene::Title, 0.25f);
	}

	if (m_fPlayerMaxPosX >= WINSIZEX * 0.4f)
	{
		// 카메라 이동
		Vector targetPos = Vector(m_fPlayerMaxPosX + WINSIZEX * 0.1f, CAMERA->GetLookAt().y);
		CAMERA->SetTargetPos(targetPos);
		// 뒤로 이동을 막는 장애물 위치 이동
		m_pObstacle->SetPos(CAMERA->ScreenToWorldPoint(Vector(0, WINSIZEY * 0.5f)));

		if (m_pPlayer->GetPos().x >= m_fPlayerMaxPosX)
		{
			float posDiffX = m_pPlayer->GetPos().x - m_fPlayerMaxPosX;
			m_pBackGround->SetPos(m_pBackGround->GetPos().x + posDiffX * 0.5f, m_pBackGround->GetPos().y);
		}
	}

	// Player MaxPosition 정의
	if (m_pPlayer->GetPos().x > m_fPlayerMaxPosX)
	{
		m_fPlayerMaxPosX = m_pPlayer->GetPos().x;
	}
	/*
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
	*/
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
