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

CSceneStage01::CSceneStage01()
{
	pPlayer = nullptr;
	pBackGround = nullptr;
	pObstacle = nullptr;

	m_fPlayerMaxPosX = 0;
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

#pragma region OBJECT

	pPlayer = new CPlayer;
	pPlayer->SetPos(200, WINSIZEY * 0.5f);
	pPlayer->SetExtension(extension);
	m_fPlayerMaxPosX = pPlayer->GetPos().x;
	AddGameObject(pPlayer);

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

	pObstacle = new CColliderObject;
	pObstacle->SetName(L"obstacle");
	pObstacle->SetExtension(extension);
	pObstacle->SetPos(0, WINSIZEY * 0.5f);
	pObstacle->SetScale(5, WINSIZEY / extension);
	pObstacle->SetLayer(Layer::ForeGround);
	AddGameObject(pObstacle);

#pragma endregion

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

	if (m_fPlayerMaxPosX >= WINSIZEX * 0.4f)
	{
		// 카메라 이동
		Vector targetPos = Vector(m_fPlayerMaxPosX + WINSIZEX * 0.1f, CAMERA->GetLookAt().y);
		CAMERA->SetTargetPos(targetPos);
		// 뒤로 이동을 막는 장애물 위치 이동
		pObstacle->SetPos(CAMERA->ScreenToWorldPoint(Vector(0, WINSIZEY * 0.5f)));

		if (pPlayer->GetPos().x >= m_fPlayerMaxPosX)
		{
			float posDiffX = pPlayer->GetPos().x - m_fPlayerMaxPosX;
			pBackGround->SetPos(pBackGround->GetPos().x + posDiffX * 0.5f, pBackGround->GetPos().y);
		}
	}

	// Player MaxPosition 정의
	if (pPlayer->GetPos().x > m_fPlayerMaxPosX)
	{
		m_fPlayerMaxPosX = pPlayer->GetPos().x;
	}
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
