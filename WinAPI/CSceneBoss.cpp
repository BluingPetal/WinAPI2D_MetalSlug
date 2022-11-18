#include "framework.h"
#include "CSceneBoss.h"

#include "CBridge.h"
#include "CImage.h"
#include "CImageObject.h"
#include "CPlayer.h"
#include "CAniObject.h"

#include "CCameraController.h"

CSceneBoss::CSceneBoss()
{
	m_fAccTime = 0;
	m_bStart = false;
	m_pWarpAni = nullptr;
	m_fExtension = 0;
}

CSceneBoss::~CSceneBoss()
{
}

void CSceneBoss::Init()
{
	CImage* pBossBackGround = RESOURCE->LoadImg(L"BossBackground", L"Image\\BackGround\\BossMap.png");
	float extension = WINSIZEY / (float)pBossBackGround->GetHeight();
	m_fExtension = extension;
	pBossBackgroundObj1 = new CImageObject;
	pBossBackgroundObj2 = new CImageObject;
	pBossBackgroundObj1->SetImage(pBossBackGround);
	pBossBackgroundObj1->SetExtension(extension);
	pBossBackgroundObj1->SetScale(Vector(pBossBackGround->GetWidth(), pBossBackGround->GetHeight()) * extension);
	AddGameObject(pBossBackgroundObj1);
	pBossBackgroundObj2->SetImage(pBossBackGround);
	pBossBackgroundObj2->SetExtension(extension);
	pBossBackgroundObj2->SetPos(pBossBackGround->GetWidth() * extension, 0);
	pBossBackgroundObj2->SetScale(Vector(pBossBackGround->GetWidth(), pBossBackGround->GetHeight()) * extension);
	AddGameObject(pBossBackgroundObj2);
	m_queueBackGround.push(pBossBackgroundObj1);
	m_queueBackGround.push(pBossBackgroundObj2);
	pCurBackgroundObj = pBossBackgroundObj1;

	m_pPlayer = new CPlayer;
	m_pPlayer->SetExtension(extension);
	//m_fPlayerMaxPosX = m_pPlayer->GetPos().x;

	m_pWarpAni = new CAniObject;
	CImage* pWarpImage = RESOURCE->LoadImg(L"Warp", L"Image\\BackGround\\Warp.png");
	m_pWarpAni->SetImage(pWarpImage);
	m_pWarpAni->SetExtension(extension);
	m_pWarpAni->SetPos(280, 470);
	m_pWarpAni->GetAnimator()->CreateAnimation(L"BackGround\\Warp", pWarpImage, 0.1f, false);
	m_pWarpAni->SetLayer(Layer::Unit);
	AddGameObject(m_pWarpAni);

	CCameraController* pCamController = new CCameraController;
	AddGameObject(pCamController);
}

void CSceneBoss::Enter()
{
	CBridge* pBridge = new CBridge;
	pBridge->SetExtension(m_fExtension);
	pBridge->SetLayer(Layer::BackGround);
	AddGameObject(pBridge);
}

void CSceneBoss::Update()
{

#pragma region BackGround นÝบน
	if (pCurBackgroundObj == pBossBackgroundObj1 && (pBossBackgroundObj1->GetPos().x + pBossBackgroundObj1->GetScale().x) < CAMERA->GetLookAt().x - WINSIZEX * 0.5f)
	{
		pBossBackgroundObj1->SetPos(pBossBackgroundObj2->GetPos().x + pBossBackgroundObj2->GetScale().x, 0);
		pCurBackgroundObj = pBossBackgroundObj2;
	}
	else if(pCurBackgroundObj == pBossBackgroundObj2 && (pBossBackgroundObj2->GetPos().x + pBossBackgroundObj2->GetScale().x) < CAMERA->GetLookAt().x - WINSIZEX * 0.5f)
	{
		pBossBackgroundObj2->SetPos(pBossBackgroundObj1->GetPos().x + pBossBackgroundObj1->GetScale().x, 0);
		pCurBackgroundObj = pBossBackgroundObj1;
	}
#pragma endregion

	if (m_pWarpAni != nullptr && !m_bStart)
	{
		m_fAccTime += DT;

		if (m_fAccTime < 0.1 * 20)
			m_pWarpAni->GetAnimator()->Play(L"BackGround\\Warp");
		else
		{
			m_bStart = true;
			m_pPlayer->SetPos(m_pWarpAni->GetPos() + Vector(0, 20));
			AddGameObject(m_pPlayer);
		}
	}

	



	Logger::Debug(to_wstring(MOUSEWORLDPOS.x) + L", " + to_wstring(MOUSEWORLDPOS.y));
}

void CSceneBoss::Render()
{
}

void CSceneBoss::Exit()
{
}

void CSceneBoss::Release()
{
}
