#include "framework.h"
#include "CSceneBoss.h"

#include "CBridge.h"
#include "CImage.h"
#include "CImageObject.h"
#include "CPlayer.h"

#include "CCameraController.h"

CSceneBoss::CSceneBoss()
{
}

CSceneBoss::~CSceneBoss()
{
}

void CSceneBoss::Init()
{
	CImage* pBossBackGround = RESOURCE->LoadImg(L"BossBackground", L"Image\\BackGround\\BossMap.png");
	float extension = WINSIZEY / (float)pBossBackGround->GetHeight();
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

	CBridge* pBridge = new CBridge;
	pBridge->SetLayer(Layer::BackGround);
	AddGameObject(pBridge);

	m_pPlayer = new CPlayer;
	m_pPlayer->SetPos(WINSIZEX * 0.5, WINSIZEY * 0.5f);
	m_pPlayer->SetExtension(extension);
	AddGameObject(m_pPlayer);
	//m_fPlayerMaxPosX = m_pPlayer->GetPos().x;


	CCameraController* pCamController = new CCameraController;
	AddGameObject(pCamController);
}

void CSceneBoss::Enter()
{
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
