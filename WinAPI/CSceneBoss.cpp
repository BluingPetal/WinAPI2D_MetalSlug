#include "framework.h"
#include "CSceneBoss.h"

#include "CBridge.h"
#include "CImage.h"
#include "CImageObject.h"
#include "CPlayer.h"
#include "CAniObject.h"
#include "CBoss.h"
#include "CColliderObject.h"
#include "CFontImageObj.h"

CSceneBoss::CSceneBoss()
{
	m_fStartAccTime = 0;
	m_fBossAppearAccTime = 0;
	m_fExtension = 0;
	m_pWarpAni = nullptr;
	m_bStart = false;
	m_bBossAppear = false;
	m_bVictory = false;
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
	pBossBackgroundObj2->SetImage(pBossBackGround);
	pBossBackgroundObj2->SetExtension(extension);
	pBossBackgroundObj2->SetPos(pBossBackGround->GetWidth() * extension, 0);
	pBossBackgroundObj2->SetScale(Vector(pBossBackGround->GetWidth(), pBossBackGround->GetHeight()) * extension);
	m_queueBackGround.push(pBossBackgroundObj1);
	m_queueBackGround.push(pBossBackgroundObj2);
	pCurBackgroundObj = pBossBackgroundObj1;

	m_pPlayer = new CPlayer;
	m_pPlayer->SetExtension(extension);
	//m_fPlayerMaxPosX = m_pPlayer->GetPos().x;

	pBoss = new CBoss;
	pBoss->SetExtension(extension);

	m_pWarpAni = new CAniObject;
	CImage* pWarpImage = RESOURCE->LoadImg(L"Warp", L"Image\\BackGround\\Warp.png");
	m_pWarpAni->SetImage(pWarpImage);
	m_pWarpAni->SetExtension(extension);
	m_pWarpAni->SetPos(WINSIZEX * 0.7f, 470);
	m_pWarpAni->GetAnimator()->CreateAnimation(L"BackGround\\Warp", pWarpImage, 0.1f, false);
	m_pWarpAni->SetLayer(Layer::Unit);
	AddGameObject(m_pWarpAni);
	
	m_pObstacle = new CColliderObject;
	m_pObstacle->SetName(L"obstacle");
	m_pObstacle->SetExtension(extension);
	m_pObstacle->SetPos(WINSIZEX, WINSIZEY * 0.5f);
	m_pObstacle->SetScale(5, WINSIZEY / extension);
	AddGameObject(m_pObstacle);

	m_pDeathObj = new CColliderObject;
	m_pDeathObj->SetName(L"deathObstacle");
	m_pDeathObj->SetPos(WINSIZEX * 0.5f, WINSIZEY * 0.95f);
	m_pDeathObj->SetScale(WINSIZEX, WINSIZEY * 0.1f);
	AddGameObject(m_pDeathObj);

	m_pWater = new CAniObject;
	CImage* pBossEffectImage = RESOURCE->LoadImg(L"BossWater", L"Image\\Boss\\BossBackground.png");
	m_pWater->SetImage(pBossEffectImage);
	m_pWater->SetExtension(extension);
	m_pWater->SetPos(pBoss->GetPos());//.x, WINSIZEY * 0.8f);
	m_pWater->GetAnimator()->CreateAnimation(L"Boss\\BossWater", pBossEffectImage, 0.1f);
	m_pWater->SetLayer(Layer::ForeGround);
	//AddGameObject(m_pWater);

	m_pMissionComplete1 = new CFontImageObj;
	m_pMissionComplete2 = new CFontImageObj;
	m_pMissionComplete1->SetExtension(extension);
	m_pMissionComplete2->SetExtension(extension);
}

void CSceneBoss::Enter()
{
	CBridge* pBridge = new CBridge;
	pBridge->SetExtension(m_fExtension);
	pBridge->SetLayer(Layer::BackGround);
	AddGameObject(pBridge);
	AddGameObject(pBossBackgroundObj1);
	AddGameObject(pBossBackgroundObj2);

	CAMERA->FadeIn(1.0f);
}

void CSceneBoss::Update()
{
	if (pBoss->GetStatus() == BossStatus::Destroyed && !m_bVictory)
	{
		m_bVictory = true;
		m_pPlayer->SetStatus(PlayerStatus::Victory);
		AddGameObject(m_pMissionComplete1);
		AddGameObject(m_pMissionComplete2);
		m_vecMissionCompleteStartPos1 = CAMERA->ScreenToWorldPoint(Vector( - WINSIZEX * 0.4f, WINSIZEY * 0.3f));
		m_vecMissionCompleteStartPos2 = CAMERA->ScreenToWorldPoint(Vector(-WINSIZEX * 0.5f, WINSIZEY * 0.5f));
		m_pMissionComplete1->SetInterval(1.1f);
		m_pMissionComplete2->SetInterval(1.1f);
		m_pMissionComplete1->CreateImgObj(L"mission", m_vecMissionCompleteStartPos1, 7, FontType::Mission);
		m_pMissionComplete2->CreateImgObj(L"complete", m_vecMissionCompleteStartPos1, 8, FontType::Mission);
		m_pMissionComplete1->Show();
		m_pMissionComplete2->Show();
	}
	else if (m_bVictory)
	{
		//m_fVictoryAccTime += DT;
		Vector m_vecTargetPos1, m_vecTargetPos2;
		m_vecTargetPos1 = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.28f, WINSIZEY * 0.3f));
		m_vecTargetPos2 = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.2f, WINSIZEY * 0.5f));
		m_vecMissionCompleteStartPos1 += (m_vecTargetPos1 - m_vecMissionCompleteStartPos1) * 1.f * DT; // 소리에 따라서는 3초 조절 필요
		m_vecMissionCompleteStartPos2 += (m_vecTargetPos2 - m_vecMissionCompleteStartPos2) * 1.f * DT;
		m_pMissionComplete1->SetPos(m_vecMissionCompleteStartPos1);
		m_pMissionComplete2->SetPos(m_vecMissionCompleteStartPos2);

	}

#pragma region BackGround 반복
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
		m_fStartAccTime += DT;

		if (m_fStartAccTime < 0.1 * 20)
			m_pWarpAni->GetAnimator()->Play(L"BackGround\\Warp");
		else
		{
			m_bStart = true;
			m_pPlayer->SetPos(m_pWarpAni->GetPos() + Vector(0, 20));
			AddGameObject(m_pPlayer);
		}
	}

	//if (!m_bBossAppear)
	//{
	m_pWater->SetPos(pBoss->GetPos().x, WINSIZEY * 0.93f);
	if (pBoss->GetStatus() != BossStatus::Destroyed)
	{
		m_fBossAppearAccTime += DT;

		if (m_fBossAppearAccTime > 5.f && !m_bBossAppear)
		{
			m_bBossAppear = true;

			//m_vecScale = Vector(bossImg->GetWidth() * m_fExtension, WINSIZEY - bossImg->GetHeight() * m_fExtension)
			//pBoss->SetPos(pBoss->GetScale().x, WINSIZEY - pBoss->GetScale().y);
			AddGameObject(pBoss);
			AddGameObject(m_pWater);
			m_pWater->GetAnimator()->Play(L"Boss\\BossWater");
		}
		else if (m_fBossAppearAccTime > 5.f && m_fBossAppearAccTime < 12.0f)
		{
			Vector bossUpdatePos = Vector(pBoss->GetPos().x, pBoss->GetPos().y + ((WINSIZEY - pBoss->GetScale().y * 0.5f) - pBoss->GetPos().y) * DT);
			//position 정해주기
			pBoss->SetPos(bossUpdatePos);
		}
		else if (m_fBossAppearAccTime > 12.0f)
		{
			Vector targetPos = CAMERA->GetLookAt() + Vector(200 * DT, 0);
			CAMERA->SetTargetPos(targetPos);
		}
	}

	m_pObstacle->SetPos(CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY * 0.5f)));
	m_pDeathObj->SetPos(CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.5f, WINSIZEY * 0.95f)));

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
