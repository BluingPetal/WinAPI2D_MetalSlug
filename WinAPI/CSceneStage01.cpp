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
	m_pInsertCoinImgObj = nullptr;
	m_pMissionImgObj1 = nullptr;

	m_bIsStarted = true;
	
	m_fAccTime = 0;
	m_fMissionAccTime = 0;
	m_fPlayerMaxPosX = 0;
	m_iTimeCount = 1;

	m_fTime = 60;
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

	pFrontGround->SetImage(pFrontGroundImg);
	pFrontGround->SetPos(Vector(0, 0));
	pFrontGround->SetOffset(frontGroundOffset);
	pFrontGround->SetExtension(extension);

	CAniObject* pFrontOceanObj1 = new CAniObject;
	CImage* pFrontOceanImage = RESOURCE->LoadImg(L"FrontOcean", L"Image\\BackGround\\FrontOcean.png");
	Vector frontOceanPos1 = Vector(0, 226 + backGroundOffset.y);
	pFrontOceanObj1->SetImage(pFrontOceanImage);
	pFrontOceanObj1->SetOffset(frontOceanPos1);
	pFrontOceanObj1->SetExtension(extension);

	CAniObject* pFrontOceanObj2 = new CAniObject;
	Vector frontOceanPos2 = Vector(0, frontOceanPos1.y + 24);
	pFrontOceanObj2->SetImage(pFrontOceanImage);
	pFrontOceanObj2->SetOffset(frontOceanPos2);
	pFrontOceanObj2->SetLayer(Layer::ForeGround);
	pFrontOceanObj2->SetExtension(extension);

	pFrontOceanObj1->GetAnimator()->CreateAnimation(L"BackGround\\FrontOcean1", pFrontOceanImage, 0.1f);
	pFrontOceanObj2->GetAnimator()->CreateAnimation(L"BackGround\\FrontOcean2", pFrontOceanImage, 0.1f);
	pFrontOceanObj1->GetAnimator()->Play(L"BackGround\\FrontOcean1");
	pFrontOceanObj2->GetAnimator()->Play(L"BackGround\\FrontOcean2");
	pFrontOceanObj1->SetPosWithFirstLt();
	pFrontOceanObj2->SetPosWithFirstLt();

	CImageObject* pPlayerStatusObj = new CImageObject;
	CImage* pStatusImg = RESOURCE->LoadImg(L"PlayerStatus", L"Image\\Font\\PlayerStatus.png");
	pPlayerStatusObj->SetImage(pStatusImg);
	pPlayerStatusObj->SetExtension(extension+1);
	pPlayerStatusObj->SetFixed(true);
	pPlayerStatusObj->SetLayer(Layer::ForeGround);
	pPlayerStatusObj->SetPos(WINSIZEX * 0.25f, 20);
	float sourcePlayerStatusInfo[4] = { 0, 0, 64, 19 };
	pPlayerStatusObj->SetRenderAsFrame(true);
	pPlayerStatusObj->SetSourceInfo(sourcePlayerStatusInfo[0], sourcePlayerStatusInfo[1], sourcePlayerStatusInfo[2], sourcePlayerStatusInfo[3]);

	CImageObject* pPlayerBarObj = new CImageObject;
	pPlayerBarObj->SetImage(pStatusImg);
	pPlayerBarObj->SetExtension(extension + 1);
	pPlayerBarObj->SetFixed(true);
	pPlayerBarObj->SetLayer(Layer::ForeGround);
	pPlayerBarObj->SetPos(20, 20);
	float sourcePlayerBarInfo[4] = { 0, 19, 63, 10 };
	pPlayerBarObj->SetRenderAsFrame(true);
	pPlayerBarObj->SetSourceInfo(sourcePlayerBarInfo[0], sourcePlayerBarInfo[1], sourcePlayerBarInfo[2], sourcePlayerBarInfo[3]);

#pragma endregion

#pragma region OBJECT

	m_pPlayer = new CPlayer;
	m_pPlayer->SetPos(200, WINSIZEY * 0.5f);
	m_pPlayer->SetExtension(extension);
	m_fPlayerMaxPosX = m_pPlayer->GetPos().x;

	m_pConga = new CConga;
	m_pConga->SetPos(WINSIZEX * 0.8f, WINSIZEY * 0.5f);
	m_pConga->SetExtension(extension);

	pWaterAniObj = new CAniObject;
	CImage* pWaterAniImg = RESOURCE->LoadImg(L"WaterAni", L"Image\\BackGround\\WaterAni.png");
	Vector waterAniPos = m_pPlayer->GetPos();
	pWaterAniObj->SetImage(pWaterAniImg);
	pWaterAniObj->SetPos(waterAniPos + Vector(0, 30));
	pWaterAniObj->SetExtension(extension - 2);
	pWaterAniObj->SetAlpha(0);
	pWaterAniObj->GetAnimator()->CreateAnimation(L"BackGround\\WaterAni", pWaterAniImg, 0.1f);
	pWaterAniObj->GetAnimator()->Play(L"BackGround\\WaterAni");
	pWaterAniObj->SetLayer(Layer::ForeGround);

	m_pInsertCoinImgObj = new CFontImageObj;
	m_pInsertCoinImgObj->SetFixed(true);
	m_pInsertCoinImgObj->SetExtension(extension + 1);

	m_pMissionImgObj1 = new CFontImageObj;
	m_pMissionImgObj1->SetExtension(extension);

	m_pMissionImgObj2 = new CFontImageObj;
	m_pMissionImgObj2->SetExtension(extension);

	m_pTimeImgObj = new CFontImageObj;
	m_pTimeImgObj->SetFixed(true);
	m_pTimeImgObj->SetExtension(extension + 0.5);

	m_pBarFontObj = new CFontImageObj;
	m_pBarFontObj->SetFixed(true);
	m_pBarFontObj->SetExtension(extension-1);

	m_pBulletObj = new CFontImageObj;
	m_pBulletObj->SetFixed(true);
	m_pBulletObj->SetExtension(extension);

	m_pBombObj = new CFontImageObj;
	m_pBombObj->SetFixed(true);
	m_pBombObj->SetExtension(extension);

#pragma endregion

#pragma region COLLIDER

	CColliderObject* pFrontOceanCollider = new CColliderObject;
	pFrontOceanCollider->SetName(L"frontOcean");
	pFrontOceanCollider->SetExtension(extension);
	Vector frontOceanOffset = pFrontOceanObj2->GetAnimator()->GetFirstAniFrame().offset;
	pFrontOceanCollider->SetOffset(frontOceanOffset);
	pFrontOceanCollider->SetPos(pFrontOceanObj2->GetPos());
	pFrontOceanCollider->SetScale(pFrontOceanObj2->GetAnimator()->GetFirstAniFrame().slice);
	pFrontOceanCollider->SetLayer(Layer::ForeGround);

	m_pObstacle = new CColliderObject;
	m_pObstacle->SetName(L"obstacle");
	m_pObstacle->SetExtension(extension);
	m_pObstacle->SetPos(0, WINSIZEY * 0.5f);
	m_pObstacle->SetScale(5, WINSIZEY / extension);
	m_pObstacle->SetLayer(Layer::ForeGround);

#pragma endregion

#pragma region ADDOBJECT

	AddGameObject(m_pBackGround);
	AddGameObject(pFrontGround);
	AddGameObject(pFrontOceanObj1);
	AddGameObject(pFrontOceanObj2);

	AddGameObject(m_pPlayer);
	AddGameObject(m_pConga);
	AddGameObject(pFrontOceanCollider);
	AddGameObject(pWaterAniObj);
	AddGameObject(m_pObstacle);

	AddGameObject(m_pInsertCoinImgObj);
	AddGameObject(m_pMissionImgObj1);
	AddGameObject(m_pMissionImgObj2);
	AddGameObject(m_pTimeImgObj);
	AddGameObject(pPlayerStatusObj);
	AddGameObject(pPlayerBarObj);
	AddGameObject(m_pBarFontObj);
	AddGameObject(m_pBulletObj);
	AddGameObject(m_pBombObj);

#pragma endregion
	// CCameraController* pCamController = new CCameraController;
	// AddGameObject(pCamController);
}

void CSceneStage01::Enter()
{
	CAMERA->FadeIn(1.f);

	m_pInsertCoinImgObj->SetInterval(1.1f);
	Vector startPos = Vector(WINSIZEX * 0.35f, WINSIZEY * 0.03f);
	m_pInsertCoinImgObj->SetPos(startPos);
	m_pInsertCoinImgObj->CreateImgObj(L"insert coin", startPos, 11, FontType::Coin);

	m_vecStartPos1 = Vector(-WINSIZEX * 0.3f, WINSIZEY * 0.3f);
	m_pMissionImgObj1->SetInterval(1.1f);
	m_pMissionImgObj1->CreateImgObj(L"mission", m_vecStartPos1, 7, FontType::Mission); // Vector(WINSIZEX * 0.3f, WINSIZEY * 0.3f)

	m_vecStartPos2 = Vector(-WINSIZEX * 0.25f, WINSIZEY * 0.5f);
	m_pMissionImgObj2->SetInterval(1.f);
	m_pMissionImgObj2->CreateImgObj(L"start", m_vecStartPos2, 5, FontType::Mission); //, Vector(WINSIZEX * 0.35f, WINSIZEY * 0.5f)

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
		if(bullet==L"0")
			bullet = L"z";
	}
	m_pBulletObj->CreateImgObj(bullet, startBulletObjPos, bullet.size(), FontType::Score);

	m_pBombObj->SetInterval(1.f);
	Vector startBombObjPos = Vector(WINSIZEX * 0.37, 60);
	m_pBombObj->SetPos(startBombObjPos);
	wstring bomb = to_wstring(m_pPlayer->GetBomb());
	m_pBombObj->CreateImgObj(bomb, startBombObjPos, bomb.size(), FontType::Score);
	
	m_pInsertCoinImgObj->Show();
	m_pMissionImgObj1->Show();
	m_pMissionImgObj2->Show();
	m_pTimeImgObj->Show();
	m_pBarFontObj->Show();
	m_pBulletObj->Show();
	m_pBombObj->Show();

	m_pConga->CongaAddObject();
}

void CSceneStage01::Update()
{
	m_fAccTime += DT;
	
	prevBomb = m_pPlayer->GetBomb();
	prevBullet = m_pPlayer->GetBullet();

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

	if (!m_pMissionImgObj1->GetReserveDelete() && !m_pMissionImgObj2->GetReserveDelete())
	{
		Vector m_vecTargetPos1, m_vecTargetPos2;
		if (m_bIsStarted)
		{
			m_vecTargetPos1 = Vector(WINSIZEX * 0.3f, WINSIZEY * 0.3f);
			m_vecTargetPos2 = Vector(WINSIZEX * 0.35f, WINSIZEY * 0.5f);
		}
		else
		{
			m_vecTargetPos1 = Vector(WINSIZEX * 1.3f, WINSIZEY * 0.3f);
			m_vecTargetPos2 = Vector(WINSIZEX * 1.35f, WINSIZEY * 0.5f);
		}

		m_vecStartPos1 += (m_vecTargetPos1 - m_vecStartPos1) * 3.f * DT; // 소리에 따라서는 3초 조절 필요
		m_vecStartPos2 += (m_vecTargetPos2 - m_vecStartPos2) * 3.f * DT;
		m_pMissionImgObj1->SetPos(m_vecStartPos1);
		m_pMissionImgObj2->SetPos(m_vecStartPos2);
		//m_pMissionImgObj1->DeleteObj();
		//m_pMissionImgObj2->DeleteObj();

		if ((m_vecTargetPos1.x - m_vecStartPos1.x) < 1.f && (m_vecTargetPos2.x - m_vecStartPos2.x) < 1.f && m_bIsStarted)
		{
			m_fMissionAccTime += DT;
			if (m_fMissionAccTime >= 2.0f)
				m_bIsStarted = false;
		}
		else if ((m_vecTargetPos1.x - m_vecStartPos1.x) < 1.f && (m_vecTargetPos1.x - m_vecStartPos2.x) < 1.f && !m_bIsStarted)
		{
			for (int i = 0; i < m_pMissionImgObj1->GetImageObj().size(); i++)
				m_pMissionImgObj1->DeleteObj();
			for (int i = 0; i < m_pMissionImgObj2->GetImageObj().size(); i++)
				m_pMissionImgObj2->DeleteObj();

			//if (!m_pMissionImgObj1->GetReserveDelete() && !m_pMissionImgObj2->GetReserveDelete())
			//{
				DELETEOBJECT(m_pMissionImgObj1);
				//m_pMissionImgObj1 = nullptr;
				DELETEOBJECT(m_pMissionImgObj1);
				//m_pMissionImgObj2 = nullptr;
			//}
			//while (!m_pMissionImgObj1->GetImageObj(L"insert coin").empty())
			//{
			//	m_pMissionImgObj1->FindImgObjQueue(L"insert coin").front()->SetAlpha(0);
			//	m_pMissionImgObj1->FindImgObjQueue(L"insert coin").pop();
			//}
			//while (!m_pMissionImgObj2->FindImgObjQueue(L"start").empty())
			//{
			//	m_pMissionImgObj2->FindImgObjQueue(L"start").front()->SetAlpha(0);
			//	m_pMissionImgObj2->FindImgObjQueue(L"start").pop();
			//}
		}
		//m_pMissionImgObj1->Show();
		//m_pMissionImgObj2->Show();
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

	if (m_pPlayer->GetBombDiff())
	{
		Vector startBombObjPos = Vector(WINSIZEX * 0.37, 60);
		wstring bomb = to_wstring(m_pPlayer->GetBomb());
		m_pBombObj->DeleteObj();
		m_pBombObj->CreateImgObj(bomb, startBombObjPos, bomb.size(), FontType::Score);
		m_pBombObj->Show();
	}
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

	Vector waterAniPos = m_pPlayer->GetPos();
	pWaterAniObj->SetPos(waterAniPos + Vector(30, 30));
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

CAniObject* CSceneStage01::GetWaterAni()
{
	return pWaterAniObj;
}