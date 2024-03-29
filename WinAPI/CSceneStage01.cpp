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
#include "CItem.h"
#include "CNPC.h"
#include "CSound.h"

CSceneStage01::CSceneStage01()
{
	m_pPlayer = nullptr;
	m_pBackGround = nullptr;
	m_pObstacle = nullptr;
	m_pInsertCoinImgObj = nullptr;
	m_pMissionImgObj1 = nullptr;
	m_pMissionImgObj2 = nullptr;
	m_pTimeImgObj = nullptr;
	m_pBarFontObj = nullptr;
	m_pBulletObj = nullptr;
	m_pBombObj = nullptr;
	pBoat = nullptr;
	pBoatCastle = nullptr;
	pWaterAniObj = nullptr;
	m_pBoatCastleCollider = nullptr;
	m_pWarp = nullptr;
	m_pConga = nullptr;

	m_bIsStarted = true;
	m_bCallObj = false;
	m_bBoatDisappear = false;
	m_bIsOpen = false;
	m_bFontDisappear = false;
	
	m_fAccTime = 0;
	m_fMissionAccTime = 0;
	m_fPlayerMaxPosX = 0;
	m_iTimeCount = 1;
	countBullet = 0;
	congaCount = 0;

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
	m_pBackGround->SetOffset(backGroundOffset);
	m_pBackGround->SetExtension(extension);

	pFrontGround->SetImage(pFrontGroundImg);
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

	CImageObject* pFishBack = new CImageObject;
	CImageObject* pFishFront = new CImageObject;
	CImage* pFishImg = RESOURCE->LoadImg(L"fish", L"Image\\BackGround\\Fish.png");
	pFishBack->SetImage(pFishImg);
	pFishBack->SetExtension(extension);
	pFishBack->SetPos(459 * extension, 146 * extension);
	float sourceFishBackInfo[4] = { 16, 24, 86, 43 };
	pFishBack->SetRenderAsFrame(true);
	pFishBack->SetSourceInfo(sourceFishBackInfo[0], sourceFishBackInfo[1], sourceFishBackInfo[2], sourceFishBackInfo[3]);

	pFishFront->SetImage(pFishImg);
	pFishFront->SetExtension(extension);
	pFishFront->SetLayer(Layer::ForeGround);
	pFishFront->SetPos(pFishBack->GetPos());
	pFishFront->SetOffset(Vector(30, 0));
	float sourceFishFrontInfo[4] = { 102, 24, 57, 43 };
	pFishFront->SetRenderAsFrame(true);
	pFishFront->SetSourceInfo(sourceFishFrontInfo[0], sourceFishFrontInfo[1], sourceFishFrontInfo[2], sourceFishFrontInfo[3]);

	pBoatCastle = new CImageObject;
	CImage* pBoatImg = RESOURCE->LoadImg(L"castle", L"Image\\BackGround\\Boat.png");
	pBoatCastle->SetImage(pBoatImg);
	pBoatCastle->SetExtension(extension);
	pBoatCastle->SetPos(2600, 40);
	float sourceCastleInfo[4] = { 0, 0, 120, 139 };
	pBoatCastle->SetRenderAsFrame(true);
	pBoatCastle->SetSourceInfo(sourceCastleInfo[0], sourceCastleInfo[1], sourceCastleInfo[2], sourceCastleInfo[3]);

	pBoat = new CImageObject;
	pBoat->SetImage(pBoatImg);
	pBoat->SetExtension(extension);
	pBoat->SetPos(2500,450);
	pBoat->SetLayer(Layer::ForeGround);
	float sourceBoatInfo[4] = { 74, 150, 89, 85};
	pBoat->SetRenderAsFrame(true);
	pBoat->SetSourceInfo(sourceBoatInfo[0], sourceBoatInfo[1], sourceBoatInfo[2], sourceBoatInfo[3]);
	pBoat->SetAlpha(0);

#pragma endregion

#pragma region OBJECT

	m_pPlayer = new CPlayer;
	m_pPlayer->SetPos(200, 0);//WINSIZEY * 0.3f);
	m_pPlayer->SetExtension(extension);
	m_pPlayer->SetStatus(PlayerStatus::Prepare);
	m_fPlayerMaxPosX = m_pPlayer->GetPos().x;
	

	//m_pConga = new CConga;
	//m_pConga->SetPos(WINSIZEX * 0.8f, WINSIZEY * 0.5f);
	//m_pConga->SetExtension(extension);

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

	m_pMissionImgObj1 = new CFontImageObj;
	m_pMissionImgObj1->SetExtension(extension);

	m_pMissionImgObj2 = new CFontImageObj;
	m_pMissionImgObj2->SetExtension(extension);

	m_pInsertCoinImgObj = new CFontImageObj;
	m_pInsertCoinImgObj->SetFixed(true);
	m_pInsertCoinImgObj->SetExtension(extension + 1);

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

	m_pWarp = new CAniObject;
	CImage* warpImg = RESOURCE->LoadImg(L"InWarpImg", L"Image\\BackGround\\Warp.png");
	Vector warpPos = Vector(3350, 400);
	m_pWarp->SetImage(warpImg);
	m_pWarp->SetPos(warpPos);
	m_pWarp->SetExtension(extension);
	m_pWarp->GetAnimator()->CreateAnimation(L"BackGround\\InWarp", warpImg, 0.1f, false);
	//m_pWarp->GetAnimator()->Play(L"BackGround\\InWarp");
	m_pWarp->SetLayer(Layer::Unit);

#pragma endregion

#pragma region COLLIDER

	CColliderObject* pFrontOceanCollider = new CColliderObject;
	pFrontOceanCollider->SetName(L"ground");
	pFrontOceanCollider->SetExtension(extension);
	Vector frontOceanOffset = pFrontOceanObj2->GetAnimator()->GetFirstAniFrame().offset;
	pFrontOceanCollider->SetOffset(frontOceanOffset);
	pFrontOceanCollider->SetPos(pFrontOceanObj2->GetPos() + Vector(-300, -30));
	pFrontOceanCollider->SetScale(pFrontOceanObj2->GetAnimator()->GetFirstAniFrame().slice.x - 200, 10);

	m_pObstacle = new CColliderObject;
	m_pObstacle->SetName(L"obstacle");
	m_pObstacle->SetExtension(extension);
	m_pObstacle->SetPos(0, WINSIZEY * 0.5f);
	m_pObstacle->SetScale(5, WINSIZEY / extension);

	CColliderObject* pLastObstacle = new CColliderObject;
	pLastObstacle->SetName(L"obstacle");
	pLastObstacle->SetExtension(extension);
	pLastObstacle->SetPos(3700 + WINSIZEX * 0.5f + m_pPlayer->GetScale().x * 0.5f, WINSIZEY);
	pLastObstacle->SetScale(5, WINSIZEY);

	CColliderObject* pGround1 = new CColliderObject;
	pGround1->SetName(L"slopeGround");
	pGround1->SetExtension(extension);
	pGround1->SetPos(pFrontOceanCollider->GetPos().x + pFrontOceanCollider->GetScale().x + 350, pFrontOceanCollider->GetPos().y + 20);
	pGround1->SetScale(90, 10);
	pGround1->SetType(ColliderType::Obb);
	pGround1->SetRot(-15);

	CColliderObject* pGround2 = new CColliderObject;
	pGround2->SetName(L"slopeGround");
	pGround2->SetExtension(extension);
	pGround2->SetPos(2000, WINSIZEY - 180);
	pGround2->SetScale(330, 10);
	pGround2->SetType(ColliderType::Obb);
	pGround2->SetRot(-2);

	CColliderObject* pGround3 = new CColliderObject;
	pGround3->SetName(L"slopeGround");
	pGround3->SetExtension(extension);
	pGround3->SetPos(2890, 610);
	pGround3->SetScale(230, 14);
	pGround3->SetType(ColliderType::Obb);
	pGround3->SetRot(13);

	CColliderObject* pBrokenBoat = new CColliderObject;
	pBrokenBoat->SetName(L"obstacle");
	pBrokenBoat->SetExtension(extension);
	pBrokenBoat->SetPos(3200, 560);
	pBrokenBoat->SetScale(10, 40);

	CColliderObject* pGround4 = new CColliderObject;
	pGround4->SetName(L"ground");
	pGround4->SetExtension(extension);
	pGround4->SetPos(pBrokenBoat->GetPos().x + 150, pBrokenBoat->GetPos().y - 55);
	pGround4->SetScale(100, 10);

	CColliderObject* pGround5 = new CColliderObject;
	pGround5->SetName(L"slopeGround");
	pGround5->SetExtension(extension);
	pGround5->SetPos(pGround4->GetPos().x + 630, pGround4->GetPos().y + 120);
	pGround5->SetScale(300, 10);
	pGround5->SetType(ColliderType::Obb);
	pGround5->SetRot(15);

	m_pBoatCastleCollider = new CColliderObject;
	m_pBoatCastleCollider->SetName(L"obstacleCastle");
	m_pBoatCastleCollider->SetExtension(extension);
	m_pBoatCastleCollider->SetPos(pBoatCastle->GetPos() + Vector(sourceCastleInfo[2] * 0.5f, 0) * extension);
	m_pBoatCastleCollider->SetScale(Vector(sourceCastleInfo[2], sourceCastleInfo[3]) * 0.8f + Vector(0, 200));

	CNPC* pHeavyGunNPC = new CNPC;
	pHeavyGunNPC->SetExtension(extension);
	pHeavyGunNPC->SetPos(WINSIZEX * 1.3f, WINSIZEY * 0.5f);
	pHeavyGunNPC->SetName(L"HeavyGunNPC");
	AddGameObject(pHeavyGunNPC);

	//
	//CImageObject* pWarpImg = new CImageObject;
	//CImage* pBoatImg = RESOURCE->LoadImg(L"castle", L"Image\\BackGround\\Boat.png");
	//pBoatCastle->SetImage(pBoatImg);
	//pBoatCastle->SetExtension(extension);
	//pBoatCastle->SetPos(2600, 40);
	//float sourceCastleInfo[4] = { 0, 0, 120, 139 };
	//pBoatCastle->SetRenderAsFrame(true);
	//pBoatCastle->SetSourceInfo(sourceCastleInfo[0], sourceCastleInfo[1], sourceCastleInfo[2], sourceCastleInfo[3]);


	//Logger::Debug(to_wstring(pFrontOceanCollider->GetPos().x) + L", !" + to_wstring(pGround1->GetPos().x));

#pragma endregion

	pBackGroundSound = RESOURCE->LoadSound(L"BackGroundSound", L"Sound\\Mission1.mp3");
	pMissionStartSound = RESOURCE->LoadSound(L"MissionStartSound", L"Sound\\mission1start.mp3");


#pragma region ADDOBJECT

	AddGameObject(m_pBackGround);
	AddGameObject(pFrontGround);
	AddGameObject(pFrontOceanObj1);
	AddGameObject(pFrontOceanObj2);
	AddGameObject(pGround1);
	AddGameObject(pGround2);
	AddGameObject(pGround3);
	AddGameObject(pGround4);
	AddGameObject(pGround5);
	AddGameObject(pFishBack);
	AddGameObject(pFishFront);
	AddGameObject(pBoat);
	AddGameObject(pBoatCastle);
	AddGameObject(m_pWarp);

	AddGameObject(m_pPlayer);
	//AddGameObject(m_pConga);
	AddGameObject(pFrontOceanCollider);
	AddGameObject(m_pBoatCastleCollider);
	AddGameObject(pWaterAniObj);
	AddGameObject(m_pObstacle);
	AddGameObject(pLastObstacle);
	AddGameObject(pBrokenBoat);

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

	SOUND->Play(pBackGroundSound, true);
	SOUND->Play(pMissionStartSound);

	//m_pConga->CongaAddObject();

	//CColliderObject* temp = new CColliderObject;
	//temp->SetPos(300, 300);
	//temp->SetScale(200, 200);
	//temp->SetType(ColliderType::Obb);
	//temp->SetName(L"OBB");
	//temp->SetRot(45);
	////temp->SetLayer(Layer::ForeGround);
	//AddGameObject(temp);
}

void CSceneStage01::Update()
{
	m_fAccTime += DT;
	
	//Logger::Debug(to_wstring(MOUSEWORLDPOS.x) + L", " + to_wstring(MOUSEWORLDPOS.y));

	//prevBomb = m_pPlayer->GetBomb();
	//prevBullet = m_pPlayer->GetBullet();
	
	if (m_fPlayerMaxPosX > 900 && m_fPlayerMaxPosX < 1000 && !m_bCallObj)
	{
		m_bCallObj = true;
		CConga* pConga1 = new CConga;
		pConga1->SetExtension(m_pPlayer->GetExtension());
		pConga1->SetPos(2300, WINSIZEY * 0.5f);
		pConga1->SetLookDir(Vector(1, 0));
		AddGameObject(pConga1);
		pConga1->CongaAddObject();

		CConga* pConga2 = new CConga;
		pConga2->SetAccTime(0.15f);
		pConga2->SetExtension(m_pPlayer->GetExtension());
		pConga2->SetPos(2400, WINSIZEY * 0.5f);
		AddGameObject(pConga2);
		pConga2->CongaAddObject();

		CConga* pConga3 = new CConga;
		pConga3->SetExtension(m_pPlayer->GetExtension());
		pConga3->SetAccTime(0.23f);
		pConga3->SetPos(2500, WINSIZEY * 0.5f);
		AddGameObject(pConga3);
		pConga3->CongaAddObject();
		congaCount += 3;
	}
	if (m_fPlayerMaxPosX > 1900 && m_fPlayerMaxPosX < 2000 && m_bCallObj)
	{
		m_bCallObj = false;
		CNPC* pBombNPC = new CNPC;
		pBombNPC->SetExtension(m_pPlayer->GetExtension());
		pBombNPC->SetPos(3000, WINSIZEY * 0.5f);
		pBombNPC->SetName(L"BombNPC");
		AddGameObject(pBombNPC);
	}

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

	// mission start
	if (!m_pMissionImgObj1->GetReserveDelete() && !m_pMissionImgObj2->GetReserveDelete() && !m_bFontDisappear)
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
			//if(m_pMissionImgObj1 ->GetPos().x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX,0)).x + 100)
				DELETEOBJECT(m_pMissionImgObj1);
			//if (m_pMissionImgObj2->GetPos().x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, 0)).x + 100)
				DELETEOBJECT(m_pMissionImgObj2);
				m_bFontDisappear = true;
				//m_pMissionImgObj1 = nullptr;
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

	if (m_fPlayerMaxPosX >= WINSIZEX * 0.4f && m_fPlayerMaxPosX < 3700)// * m_pPlayer->GetExtension())
	{
		//m_fPlayerMaxPosX = 3700;
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
	else if (m_fPlayerMaxPosX >= 3700)
		m_fPlayerMaxPosX = 3700;

	// Player MaxPosition 정의
	if (m_pPlayer->GetPos().x > m_fPlayerMaxPosX)
	{
		m_fPlayerMaxPosX = m_pPlayer->GetPos().x;
	}

	//if (m_pPlayer->GetBombDiff())
	//{
		Vector startBombObjPos = Vector(WINSIZEX * 0.37, 60);
		wstring bomb = to_wstring(m_pPlayer->GetBomb());
		m_pBombObj->DeleteObj();
		m_pBombObj->CreateImgObj(bomb, startBombObjPos, bomb.size(), FontType::Score);
		m_pBombObj->Show();
	//}
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
	if (m_pBoatCastleCollider->GetReserveDelete())
	{
		DELETEOBJECT(pBoatCastle);
		pBoat->SetAlpha(1);
	}
	if (m_fPlayerMaxPosX > 3200 && m_fPlayerMaxPosX < 3300 && !m_bCallObj)
	{
		m_bCallObj = true;
		for (int i = 0; i < 5; i++)
		{
			float posX = 4200 - i * 100;
			CConga* pConga = new CConga;
			pConga->SetExtension(m_pPlayer->GetExtension());
			pConga->SetAccTime(0.23f);
			pConga->SetPos(posX, WINSIZEY * 0.5f);
			AddGameObject(pConga);
			pConga->CongaAddObject();
			congaCount++;
		}
	}
	if (m_fPlayerMaxPosX > 3200 && m_bCallObj && congaCount == 0)
	{
		m_pWarp->GetAnimator()->Play(L"BackGround\\InWarp");
		if (m_pPlayer->GetPos().x > 3300 && m_pPlayer->GetPos().x < 3400)
		{
			CAMERA->FadeOut(1.f);
			m_pPlayer->SetStatus(PlayerStatus::Victory);
			DELAYCHANGESCENE(GroupScene::Boss, 2.f);
		}
	}
	//if (m_pPlayer->GetPos().x > 3300 && m_pPlayer->GetPos().x < 3400 && m_bCallObj && congaCount == 0)
	//{
	//	CAMERA->FadeOut(2.0f);
	//}
}

void CSceneStage01::Render()
{
}

void CSceneStage01::Exit()
{
	SOUND->Stop(pBackGroundSound);
	SOUND->Stop(pMissionStartSound);
}

void CSceneStage01::Release()
{
}

CAniObject* CSceneStage01::GetWaterAni()
{
	return pWaterAniObj;
}