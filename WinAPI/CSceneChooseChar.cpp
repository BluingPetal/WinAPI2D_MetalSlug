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
	m_pEriBox = nullptr;
	m_pSelectSoldierAniObj = nullptr;
	m_pEri1 = nullptr;
	m_pEri2 = nullptr;
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
	Vector selectCharOffset = EriDoorOffset + Vector(0, 9);
	m_pEriSelect->SetOffset(selectCharOffset);
	m_pEriSelect->SetExtension(extension);

	m_pSelectSoldierAniObj = new CAniObject;
	m_pSelectSoldierAniObj->SetImage(pSelectCharImg);
	Vector selectSoldierOffset = selectCharOffset + Vector(33, 120);
	m_pSelectSoldierAniObj->SetPos(pBackObj->GetPos() + selectSoldierOffset * extension);
	m_pSelectSoldierAniObj->SetExtension(extension);
	m_pSelectSoldierAniObj->GetAnimator()->CreateAnimation(L"BackGround\\SelectSoldier", pSelectCharImg, 0.15f, false);

	m_pEriBox = new CImageObject;
	m_pEriBox->SetImage(pSelectCharImg);
	m_pEriBox->SetRenderAsFrame(true);
	float sourceEriBoxInfo[4] = { 137, 335, 48, 52 };
	m_pEriBox->SetSourceInfo(sourceEriBoxInfo[0], sourceEriBoxInfo[1], sourceEriBoxInfo[2], sourceEriBoxInfo[3]);
	Vector eriBoxOffset = Vector(9, 52);
	m_pEriBox->SetPos(m_pEriDoor->GetPos() + EriDoorOffset * extension);
	m_pEriBox->SetOffset(eriBoxOffset);
	m_pEriBox->SetExtension(extension);
	m_pEriBox->SetAlpha(0);

	m_pEri1 = new CAniObject;
	m_pEri2 = new CAniObject;
	CImage* m_pEriIdle = RESOURCE->LoadImg(L"PlayerIdle", L"Image\\Player\\EriIdle.png");
	m_pEri1->SetImage(m_pEriIdle);
	m_pEri2->SetImage(m_pEriIdle);
	Vector eriOffset = Vector(23, 24);
	m_pEri1->SetPos(m_pEriBox->GetPos() + (eriBoxOffset + eriOffset) * extension);
	m_pEri2->SetPos(m_pEriBox->GetPos() + (eriBoxOffset + eriOffset) * extension);
	m_pEri1->SetExtension(extension);
	m_pEri2->SetExtension(extension);
	m_pEri1->GetAnimator()->CreateAnimation(L"Player\\Idle\\EriIdleR_1", m_pEriIdle, 0.15f);
	m_pEri2->GetAnimator()->CreateAnimation(L"Player\\Idle\\EriIdleR_2", m_pEriIdle, 0.15f);

	m_fExtension = extension;

	// AddObject
	AddGameObject(pBlackObj);
	AddGameObject(m_pEriSelect);
	AddGameObject(m_pEriDoor);
	AddGameObject(m_pEriBox);
	AddGameObject(m_pEri2);
	AddGameObject(m_pEri1);
	AddGameObject(pDoor1);
	AddGameObject(pDoor2);
	AddGameObject(pDoor3);
	AddGameObject(pBackObj);
	AddGameObject(pPlayer1);
	AddGameObject(m_pSelectSoldierAniObj);
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

	if (!m_bSelectedChar)
	{
		if (!m_bSelectedChar && m_fAccTime <= 1.f)
		{
			int doorLength = 126 * m_fExtension;

			if (m_fAccTime < 0.5f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y - doorLength / 0.5f * DT);
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
	}
	else
	{
		float sourceEriInfo[4] = { 377, 249, 64, 120 };
		m_pEriSelect->SetSourceInfo(sourceEriInfo[0], sourceEriInfo[1], sourceEriInfo[2], sourceEriInfo[3]);

		if (m_fAccTime > 0.5f && m_fAccTime <= 3.0f)
		{
			int doorLength = 126 * m_fExtension;

			if (m_fAccTime < 1.f)
			{
				m_pEriBox->SetAlpha(1);
				m_pEri1->GetAnimator()->Play(L"Player\\Idle\\EriIdleR_1");
				m_pEri2->GetAnimator()->Play(L"Player\\Idle\\EriIdleR_2");
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y + doorLength / 0.5f * DT);
			}
			else if (m_fAccTime < 1.1f)
			{
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y - 20 / 0.1f * DT);
				m_pSelectSoldierAniObj->GetAnimator()->Play(L"BackGround\\SelectSoldier");
			}
			else if (m_fAccTime < 1.2f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y + 20 / 0.1f * DT);
			else if (m_fAccTime < 1.3f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y - 20 / 0.1f * DT);
			else if (m_fAccTime < 1.4f)
				m_pEriDoor->SetPos(m_pEriDoor->GetPos().x, m_pEriDoor->GetPos().y + 20 / 0.1f * DT);
			else if (m_fAccTime >= 1.8f)
				m_pSelectSoldierAniObj->GetAnimator()->Stop();
		}
		else if (m_fAccTime > 3.5f)
		{
			CAMERA->FadeOut(1.f);
			DELAYCHANGESCENE(GroupScene::Stage01, 1.f);
		}
	}

	Vector eriOffset = Vector(23, 24);
	m_pEriBox->SetPos(m_pEriDoor->GetPos() + m_pEriDoor->GetOffset() * m_fExtension);
	m_pEri1->SetPos(m_pEriBox->GetPos() + (m_pEriBox->GetOffset() + eriOffset) * m_fExtension);
	m_pEri2->SetPos(m_pEriBox->GetPos() + (m_pEriBox->GetOffset() + eriOffset) * m_fExtension);
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
