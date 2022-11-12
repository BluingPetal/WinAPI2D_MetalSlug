#include "framework.h"
#include "CSceneChooseChar.h"

#include "CImage.h"
#include "CImageObject.h"
#include "CFontImageObj.h"
#include "CAniObject.h"

#include "CCameraController.h"

CSceneChooseChar::CSceneChooseChar()
{
	m_pEriDoor = nullptr;
	m_pEriSelect = nullptr;
	m_fAccTime = 0;
	m_fExtension = 1;
	m_bSelectedChar = false;
}

CSceneChooseChar::~CSceneChooseChar()
{
}

void CSceneChooseChar::Init()
{
	CImage* pBlackImg = RESOURCE->LoadImg(L"pBlackImg", L"Image\\Start\\BlackImg.png");
	CImageObject* pBlackObj = new CImageObject;
	pBlackObj->SetImage(pBlackImg);

	CImage* pSelectCharImg = RESOURCE->LoadImg(L"ChooseCharBackImg", L"Image\\Start\\SelectChar.png");
	CImageObject* pBackObj = new CImageObject;
	pBackObj->SetImage(pSelectCharImg);
	pBackObj->SetRenderAsFrame(true);
	float sourceBackInfo[4] = { 3, 116, 304, 216 };
	pBackObj->SetSourceInfo(sourceBackInfo[0], sourceBackInfo[1], sourceBackInfo[2], sourceBackInfo[3]);
	float extension = WINSIZEY / (float)sourceBackInfo[3];
	pBackObj->SetPos((WINSIZEX - sourceBackInfo[2] * extension) * 0.5, 0);
	pBackObj->SetExtension(extension);
	Logger::Debug(to_wstring(sourceBackInfo[2] * extension));

	CImageObject* pSoldierSelect = new CImageObject;
	pSoldierSelect->SetImage(pSelectCharImg);
	pSoldierSelect->SetRenderAsFrame(true);
	float sourceSoldierSelectInfo[4] = { 2, 2, 247, 34 };
	pSoldierSelect->SetSourceInfo(sourceSoldierSelectInfo[0], sourceSoldierSelectInfo[1], sourceSoldierSelectInfo[2], sourceSoldierSelectInfo[3]);
	Vector SoldierSelectOffset = Vector(29, 1);
	pSoldierSelect->SetPos(pBackObj->GetPos());
	pSoldierSelect->SetOffset(SoldierSelectOffset);
	pSoldierSelect->SetExtension(extension);

	CImageObject* pDoor1 = new CImageObject;
	pDoor1->SetImage(pSelectCharImg);
	pDoor1->SetRenderAsFrame(true);
	float sourceDoorInfo[4] = { 70, 335, 64, 134 };
	pDoor1->SetSourceInfo(sourceDoorInfo[0], sourceDoorInfo[1], sourceDoorInfo[2], sourceDoorInfo[3]);
	Vector Door1Offset = Vector(19, 65 - 9);
	pDoor1->SetPos(pBackObj->GetPos());
	pDoor1->SetOffset(Door1Offset);
	pDoor1->SetExtension(extension);

	CImageObject* pDoor2 = new CImageObject;
	pDoor2->SetImage(pSelectCharImg);
	pDoor2->SetRenderAsFrame(true);
	pDoor2->SetSourceInfo(sourceDoorInfo[0], sourceDoorInfo[1], sourceDoorInfo[2], sourceDoorInfo[3]);
	Vector Door2Offset = Vector(155, 65 - 9);
	pDoor2->SetPos(pBackObj->GetPos());
	pDoor2->SetOffset(Door2Offset);
	pDoor2->SetExtension(extension);

	CImageObject* pDoor3 = new CImageObject;
	pDoor3->SetImage(pSelectCharImg);
	pDoor3->SetRenderAsFrame(true);
	pDoor3->SetSourceInfo(sourceDoorInfo[0], sourceDoorInfo[1], sourceDoorInfo[2], sourceDoorInfo[3]);
	Vector Door3Offset = Vector(223, 65 - 9);
	pDoor3->SetPos(pBackObj->GetPos());
	pDoor3->SetOffset(Door3Offset);
	pDoor3->SetExtension(extension);

	CImageObject* pPlayer1 = new CImageObject;
	pPlayer1->SetImage(pSelectCharImg);
	pPlayer1->SetRenderAsFrame(true);
	float sourcePlayer1Info[4] = { 54, 38, 48, 32 };
	pPlayer1->SetSourceInfo(sourcePlayer1Info[0], sourcePlayer1Info[1], sourcePlayer1Info[2], sourcePlayer1Info[3]);
	Vector Player1Offset = Vector(89, 31);
	pPlayer1->SetPos(pBackObj->GetPos());
	pPlayer1->SetOffset(Player1Offset);
	pPlayer1->SetExtension(extension);

	m_pEriDoor = new CImageObject;
	m_pEriDoor->SetImage(pSelectCharImg);
	m_pEriDoor->SetRenderAsFrame(true);
	m_pEriDoor->SetSourceInfo(sourceDoorInfo[0], sourceDoorInfo[1], sourceDoorInfo[2], sourceDoorInfo[3]);
	Vector EriDoorOffset = Vector(87, 65 - 9);
	m_pEriDoor->SetPos(pBackObj->GetPos());
	m_pEriDoor->SetOffset(EriDoorOffset);
	m_pEriDoor->SetExtension(extension);

	m_pEriSelect = new CImageObject;
	m_pEriSelect->SetImage(pSelectCharImg);
	m_pEriSelect->SetRenderAsFrame(true);
	float sourceEriInfo[4] = { 377, 3, 64, 120 };
	m_pEriSelect->SetSourceInfo(sourceEriInfo[0], sourceEriInfo[1], sourceEriInfo[2], sourceEriInfo[3]);
	m_pEriSelect->SetPos(pBackObj->GetPos());
	m_pEriSelect->SetOffset(EriDoorOffset + Vector(0, 9));
	m_pEriSelect->SetExtension(extension);

	selectSoldierAniObj = new CAniObject;
	selectSoldierAniObj->SetImage(pSelectCharImg);
	// offset 조절하기
	Vector selectSoldierOffset = m_pEriSelect->GetOffset() + Vector(33, 120);
	selectSoldierAniObj->SetOffset(pBackObj->GetPos() - selectSoldierOffset);
	selectSoldierAniObj->SetExtension(extension);
	selectSoldierAniObj->GetAnimator()->CreateAnimation(L"BackGround\\SelectSoldier", pSelectCharImg, 0.1f);

	m_fExtension = extension;

	// AddObject
	AddGameObject(pBlackObj);
	AddGameObject(m_pEriSelect);
	AddGameObject(m_pEriDoor);
	AddGameObject(pDoor1);
	AddGameObject(pDoor2);
	AddGameObject(pDoor3);
	AddGameObject(pBackObj);
	AddGameObject(pPlayer1);
	AddGameObject(selectSoldierAniObj);
	AddGameObject(pSoldierSelect);

	//CCameraController* pCamController = new CCameraController;
	//AddGameObject(pCamController);
}

void CSceneChooseChar::Enter()
{
	CAMERA->FadeIn(0.25f);
}

void CSceneChooseChar::Update()
{
	m_fAccTime += DT;

	if (!m_bSelectedChar && m_fAccTime <= 1.5f)
	{
		int doorLength = 126 * m_fExtension;

		if (m_fAccTime < 1.f)
			m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y - doorLength / 1.f * DT);
	}
	else
	{
		float sourceEriInfo[4] = { 377, 126, 64, 120 };

		m_pEriSelect->SetSourceInfo(sourceEriInfo[0], sourceEriInfo[1], sourceEriInfo[2], sourceEriInfo[3]);

		if (BUTTONDOWN(VK_SPACE))
		{
			m_bSelectedChar = true;
			m_fAccTime = 0;
		}
	}

	if (m_bSelectedChar)
	{
		float sourceEriInfo[4] = { 377, 249, 64, 120 };
		m_pEriSelect->SetSourceInfo(sourceEriInfo[0], sourceEriInfo[1], sourceEriInfo[2], sourceEriInfo[3]);

		if (m_fAccTime > 0.5f && m_fAccTime <= 3.0f)
		{
			int doorLength = 126 * m_fExtension;

			if (m_fAccTime < 1.5f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y + doorLength / 1.f * DT);
			else if (m_fAccTime < 1.6f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y - 5 / 0.1f * DT);
			else if (m_fAccTime < 1.7f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y + 5 / 0.1f * DT);
			else if (m_fAccTime < 1.8f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y - 5 / 0.1f * DT);
			else if (m_fAccTime < 1.9f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y + 5 / 0.1f * DT);
			else if (m_fAccTime < 100.0f) //2.0f)
				selectSoldierAniObj->GetAnimator()->Play(L"BackGround\\SelectSoldier");
		}
		else if (m_fAccTime > 100.0f) // 3.0f)
		{
			CAMERA->FadeOut(0.25f);
			DELAYCHANGESCENE(GroupScene::Stage01, 0.25f);
		}
	}


	//int doorLength = 120;
	//Vector eriDoorPos += 
}

void CSceneChooseChar::Render()
{
	if (m_bSelectedChar)
	{
		float sourceEriInfo[4] = { 377, 249, 64, 120 };
		if (m_fAccTime < 0.03f)
		{
			RENDER->FillRect(m_pEriSelect->GetPos().x + m_pEriSelect->GetOffset().x * m_fExtension,
				m_pEriSelect->GetPos().y + m_pEriSelect->GetOffset().y * m_fExtension,
				m_pEriSelect->GetPos().x + (m_pEriSelect->GetOffset().x + sourceEriInfo[2]) * m_fExtension,
				m_pEriSelect->GetPos().y + (m_pEriSelect->GetOffset().y + sourceEriInfo[3]) * m_fExtension,
				Color(255, 255, 255, 1.f));
		}
	}
}

void CSceneChooseChar::Exit()
{
}

void CSceneChooseChar::Release()
{
}
