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
	m_bVictorySoundPlay = false;
	m_fTime = 60;
	m_fAccTime = 0;
	m_iTimeCount = 1;
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
	pBossBackgroundObj1->SetPos(Vector(CAMERA->GetLookAt().x, 0) - Vector(pBossBackgroundObj1->GetScale().x * 0.5f, 0));
	pBossBackgroundObj2->SetImage(pBossBackGround);
	pBossBackgroundObj2->SetExtension(extension);
	pBossBackgroundObj2->SetPos(pBossBackgroundObj1->GetPos() + Vector(pBossBackgroundObj1->GetPos().x, 0));
	pBossBackgroundObj2->SetScale(Vector(pBossBackGround->GetWidth(), pBossBackGround->GetHeight()) * extension);
	m_queueBackGround.push(pBossBackgroundObj1);
	m_queueBackGround.push(pBossBackgroundObj2);
	AddGameObject(pBossBackgroundObj1);
	AddGameObject(pBossBackgroundObj2);
	pCurBackgroundObj = pBossBackgroundObj1;

	CImageObject* pPlayerStatusObj = new CImageObject;
	CImage* pStatusImg = RESOURCE->LoadImg(L"PlayerStatus", L"Image\\Font\\PlayerStatus.png");
	pPlayerStatusObj->SetImage(pStatusImg);
	pPlayerStatusObj->SetExtension(4.25);
	pPlayerStatusObj->SetFixed(true);
	pPlayerStatusObj->SetLayer(Layer::ForeGround);
	pPlayerStatusObj->SetPos(WINSIZEX * 0.25f, 20);
	float sourcePlayerStatusInfo[4] = { 0, 0, 64, 19 };
	pPlayerStatusObj->SetRenderAsFrame(true);
	pPlayerStatusObj->SetSourceInfo(sourcePlayerStatusInfo[0], sourcePlayerStatusInfo[1], sourcePlayerStatusInfo[2], sourcePlayerStatusInfo[3]);
	AddGameObject(pPlayerStatusObj);

	CImageObject* pPlayerBarObj = new CImageObject;
	pPlayerBarObj->SetImage(pStatusImg);
	pPlayerBarObj->SetExtension(4.25);
	pPlayerBarObj->SetFixed(true);
	pPlayerBarObj->SetLayer(Layer::ForeGround);
	pPlayerBarObj->SetPos(20, 20);
	float sourcePlayerBarInfo[4] = { 0, 19, 63, 10 };
	pPlayerBarObj->SetRenderAsFrame(true);
	pPlayerBarObj->SetSourceInfo(sourcePlayerBarInfo[0], sourcePlayerBarInfo[1], sourcePlayerBarInfo[2], sourcePlayerBarInfo[3]);
	AddGameObject(pPlayerBarObj);

	m_pInsertCoinImgObj = new CFontImageObj;
	m_pInsertCoinImgObj->SetFixed(true);
	m_pInsertCoinImgObj->SetExtension(4.25);

	m_pTimeImgObj = new CFontImageObj;
	m_pTimeImgObj->SetFixed(true);
	m_pTimeImgObj->SetExtension(3.75);

	m_pBarFontObj = new CFontImageObj;
	m_pBarFontObj->SetFixed(true);
	m_pBarFontObj->SetExtension(2.25);

	m_pBulletObj = new CFontImageObj;
	m_pBulletObj->SetFixed(true);
	m_pBulletObj->SetExtension(3.25);

	m_pBombObj = new CFontImageObj;
	m_pBombObj->SetFixed(true);
	m_pBombObj->SetExtension(3.25);

	AddGameObject(m_pInsertCoinImgObj);
	AddGameObject(m_pTimeImgObj);
	AddGameObject(m_pBarFontObj);
	AddGameObject(m_pBulletObj);
	AddGameObject(m_pBombObj);

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

	pBackGroundSound = RESOURCE->LoadSound(L"bossBackGroundSound", L"Sound\\Boss.mp3");
	pMissionCompleteSound = RESOURCE->LoadSound(L"missionCompleteSound", L"Sound\\missionComplete.mp3");
	pMissionClearSound = RESOURCE->LoadSound(L"missionClearSound", L"Sound\\missionClear.mp3");
	pBossAppearSound = RESOURCE->LoadSound(L"bossAppear", L"Sound\\BossAppear.mp3");
}

void CSceneBoss::Enter()
{
	CAMERA->FadeIn(1.0f);
	SOUND->Play(pBackGroundSound, true);

	CAMERA->SetTargetPos(Vector(WINSIZEX * 0.5f, WINSIZEY * 0.5f));
	CBridge* pBridge = new CBridge;
	pBridge->SetExtension(m_fExtension);
	pBridge->SetLayer(Layer::BackGround);
	AddGameObject(pBridge);

	m_pInsertCoinImgObj->SetInterval(1.1f);
	Vector startPos = Vector(WINSIZEX * 0.35f, WINSIZEY * 0.03f);
	m_pInsertCoinImgObj->SetPos(startPos);
	m_pInsertCoinImgObj->CreateImgObj(L"insert coin", startPos, 11, FontType::Coin);

	m_pTimeImgObj->SetInterval(1.f);
	Vector timeStartPos = Vector(WINSIZEX * 0.45f, 20);
	m_pTimeImgObj->SetPos(timeStartPos);
	wstring time = to_wstring(m_fTime);
	m_pTimeImgObj->CreateImgObj(time, timeStartPos, time.size(), FontType::Time);

	m_pBarFontObj->SetInterval(1.1f);
	Vector startbarFontPos = Vector(-35, 65);
	m_pBarFontObj->SetPos(startbarFontPos);
	m_pBarFontObj->CreateImgObj(L"1up2", startbarFontPos, 4, FontType::Default);

	m_pBulletObj->SetInterval(1.f);
	Vector startBulletObjPos = Vector(WINSIZEX * 0.3, 60);
	m_pBulletObj->SetPos(startBulletObjPos);
	wstring bullet;
	if (m_pPlayer->GetCurWeapon() == PlayerWeapon::Pistol)
		bullet = L"z";
	else if (m_pPlayer->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
	{
		bullet = to_wstring(m_pPlayer->GetBullet());
		if (bullet == L"0")
			bullet = L"z";
	}
	m_pBulletObj->CreateImgObj(bullet, startBulletObjPos, bullet.size(), FontType::Score);

	m_pBombObj->SetInterval(1.f);
	Vector startBombObjPos = Vector(WINSIZEX * 0.37, 60);
	m_pBombObj->SetPos(startBombObjPos);
	wstring bomb = to_wstring(m_pPlayer->GetBomb());
	m_pBombObj->CreateImgObj(bomb, startBombObjPos, bomb.size(), FontType::Score);

	m_pInsertCoinImgObj->Show();
	m_pTimeImgObj->Show();
	m_pBarFontObj->Show();
	m_pBulletObj->Show();
	m_pBombObj->Show();
}

void CSceneBoss::Update()
{
	m_fAccTime += DT;
	// insert coin 깜박거리는 효과
	if (m_fAccTime >= 1.f)
	{
		vector<CImageObject*> vecInsertCoinImgObj = m_pInsertCoinImgObj->GetImageObj();
		for (int i = 0; i < vecInsertCoinImgObj.size(); i++)
		{
			vecInsertCoinImgObj[i]->SetAlpha(!(vecInsertCoinImgObj[i]->GetAlpha()));
		}
		m_fAccTime = 0; m_iTimeCount++; // timeout 되면 gameover창

		if (m_iTimeCount % 4 == 0)
		{
			m_fTime--;

			wstring timeStr = to_wstring(m_fTime);
			Vector timeStartPos = Vector(WINSIZEX * 0.45f, 20);
			m_pTimeImgObj->DeleteObj();
			m_pTimeImgObj->CreateImgObj(timeStr, timeStartPos, timeStr.size(), FontType::Time);
			m_pTimeImgObj->Show();
		}
	}

	Vector startBombObjPos = Vector(WINSIZEX * 0.37, 60);
	wstring bomb = to_wstring(m_pPlayer->GetBomb());
	m_pBombObj->DeleteObj();
	m_pBombObj->CreateImgObj(bomb, startBombObjPos, bomb.size(), FontType::Score);
	m_pBombObj->Show();

	if (m_pPlayer->GetBulletDiff())
	{
		Vector startBulletObjPos = Vector(WINSIZEX * 0.3, 60);
		wstring bullet;
		if (m_pPlayer->GetCurWeapon() == PlayerWeapon::Pistol)
			bullet = L"z";
		else if (m_pPlayer->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
		{
			bullet = to_wstring(m_pPlayer->GetBullet());
			if (bullet == L"0")
				bullet = L"z";
		}
		m_pBulletObj->DeleteObj();
		m_pBulletObj->CreateImgObj(bullet, startBulletObjPos, bullet.size(), FontType::Score);
		m_pBulletObj->Show();
	}

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
		m_fVictoryAccTime += DT;
		Vector m_vecTargetPos1, m_vecTargetPos2;
		m_vecTargetPos1 = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.28f, WINSIZEY * 0.3f));
		m_vecTargetPos2 = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.2f, WINSIZEY * 0.5f));
		m_vecMissionCompleteStartPos1 += (m_vecTargetPos1 - m_vecMissionCompleteStartPos1) * 1.f * DT; // 소리에 따라서는 3초 조절 필요
		m_vecMissionCompleteStartPos2 += (m_vecTargetPos2 - m_vecMissionCompleteStartPos2) * 1.f * DT;
		m_pMissionComplete1->SetPos(m_vecMissionCompleteStartPos1);
		m_pMissionComplete2->SetPos(m_vecMissionCompleteStartPos2);
		if (!m_bVictorySoundPlay)
		{
			m_bVictorySoundPlay = true;
			SOUND->Play(pMissionCompleteSound);
			SOUND->Play(pMissionClearSound);
			SOUND->Stop(pBackGroundSound);
		}
	}

#pragma region BackGround 반복
	if (pCurBackgroundObj == pBossBackgroundObj1 && ((pBossBackgroundObj1->GetPos().x + pBossBackgroundObj1->GetScale().x) < CAMERA->GetLookAt().x - WINSIZEX * 0.5f))
	{
		pBossBackgroundObj1->SetPos(pBossBackgroundObj2->GetPos().x + pBossBackgroundObj2->GetScale().x, 0);
		pCurBackgroundObj = pBossBackgroundObj2;
	}
	else if(pCurBackgroundObj == pBossBackgroundObj2 && ((pBossBackgroundObj2->GetPos().x + pBossBackgroundObj2->GetScale().x) < CAMERA->GetLookAt().x - WINSIZEX * 0.5f))
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
			SOUND->Play(pBossAppearSound);
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

	//Logger::Debug(to_wstring(MOUSEWORLDPOS.x) + L", " + to_wstring(MOUSEWORLDPOS.y));
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
