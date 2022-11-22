#include "framework.h"
#include "CPlayer.h"

#include "WinAPI.h"
#include "CInputManager.h"
#include "CTimeManager.h"
#include "CRenderManager.h"
#include "CEventManager.h"
#include "CResourceManager.h"
#include "CCollider.h"
#include "CImage.h"
#include "CAnimator.h"
#include "CGravity.h"
#include "CConga.h"
#include "CScene.h"
#include "CSceneStage01.h"
#include "CSceneBoss.h"
#include "CAniObject.h"
#include "CBomb.h"

#include "CPlayerMissile.h"

#define PI 3.141592

CPlayer::CPlayer()
{
	m_vecPos = Vector(0, 0);
	m_vecScale = Vector(100, 100);
	m_layer = Layer::Player;
	m_strName = L"Player";

	m_pIdleImage = nullptr;
	m_pMoveImage = nullptr;
	m_pJumpImage = nullptr;
	m_pAttackRImage = nullptr;
	m_pAttackLImage = nullptr;
	m_pDeathImage = nullptr;

	m_vecMoveDir = Vector(0, 0);
	m_vecLookDir = m_vecPrevLookDir = tempLookDir = Vector(1, 0);
	m_bIsMove = false; 
	m_bIsJump = true;
	m_bIsDead = false;
	m_bIsAttack = false;
	m_bIsShoot = false; 

	m_bIsPrevCurLookSame = true;

	m_iBomb = 10;
	m_iBullet = 0;
	m_bBombDiff = false;
	m_bBulletDiff = false;

	m_hp = 5;
	m_fSpeed = 300.0f;
	m_fAcctime = 0;

	m_curWeapon = PlayerWeapon::Pistol;
	m_status = PlayerStatus::Idle;
	m_prevStatus = PlayerStatus::Idle;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
#pragma region Player Image

	m_pIdleImage = RESOURCE->LoadImg(L"PlayerIdle", L"Image\\Player\\EriIdle.png");
	m_pMoveImage = RESOURCE->LoadImg(L"PlayerMove", L"Image\\Player\\EriRun.png");
	m_pJumpImage = RESOURCE->LoadImg(L"PlayerJump", L"Image\\Player\\EriJump.png");
	m_pAttackRImage = RESOURCE->LoadImg(L"PlayerAttackR", L"Image\\Player\\EriAttackR.png");
	m_pAttackLImage = RESOURCE->LoadImg(L"PlayerAttackL", L"Image\\Player\\EriAttackL.png");
	m_pDeathImage = RESOURCE->LoadImg(L"PlayerDeath", L"Image\\Player\\EriDeath.png");
	CImage* pVictoryImage = RESOURCE->LoadImg(L"PlayerVictory", L"Image\\Player\\EriVictory.png");
	m_pEffectImage = RESOURCE->LoadImg(L"MissileEffectImg", L"Image\\Effect\\MissileEffect.png");
	CImage* m_pEffectImg = RESOURCE->LoadImg(L"Effect", L"Image\\Effect\\Explosion.png");

#pragma endregion

	m_pAnimator1 = new CAnimator;
	m_pAnimator2 = new CAnimator;

#pragma region Player Animation

	// Idle Animation
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleR_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumR_1", m_pIdleImage, 0.1f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunR_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunColdR", m_pIdleImage, 0.1f, false);
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleL_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumL_1", m_pIdleImage, 0.1f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunL_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunColdL", m_pIdleImage, 0.1f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleR_2", m_pIdleImage, 0.1f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleL_2", m_pIdleImage, 0.1f);

	// Move Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunL_1", m_pMoveImage, 0.05f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunL_1", m_pMoveImage, 0.05f);
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunR_1", m_pMoveImage, 0.05f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunR_1", m_pMoveImage, 0.05f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunL_2", m_pMoveImage, 0.05f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunL_2", m_pMoveImage, 0.05f);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunR_2", m_pMoveImage, 0.05f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunR_2", m_pMoveImage, 0.05f);

	// Attack Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL1_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL2_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackL_1", m_pAttackLImage, 0.05f, false);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL1_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL2_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1", m_pAttackLImage, 0.03f, false);

		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR1_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR2_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackR_1", m_pAttackRImage, 0.05f, false);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR1_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR2_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1", m_pAttackRImage, 0.03f, false);

		// SitAttack
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitAttackL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitHeavyGunL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitBombL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitKnifeL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitAttackR", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitHeavyGunR", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitBombR", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitKnifeR", m_pAttackRImage, 0.05f, false);

	// Death Animation
		// L
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpL", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathL", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathL", m_pDeathImage, 0.08f, false);
		// R
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpR", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathR", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathR", m_pDeathImage, 0.08f, false);

	// Jump Animation
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpR_1", m_pJumpImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpL_1", m_pJumpImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpMoveR_1", m_pJumpImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpMoveL_1", m_pJumpImage, 0.08f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpR_2", m_pJumpImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpL_2", m_pJumpImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpMoveR_2", m_pJumpImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpMoveL_2", m_pJumpImage, 0.08f, false);

	// Victory Animation
	m_pAnimator1->CreateAnimation(L"Player\\Victory\\EriVictory", pVictoryImage, 0.2f, true);

#pragma endregion

	AddComponent(m_pAnimator2);
	AddComponent(m_pAnimator1);

	m_gravity = new CGravity;
	AddComponent(m_gravity);
	AddCollider(ColliderType::Rect, Vector(50, 80), Vector(0, 0));

	m_gunSound = RESOURCE->LoadSound(L"pistol", L"Sound\\gun.mp3");
	m_okaySound = RESOURCE->LoadSound(L"okay", L"Sound\\getBomb.mp3");
	m_getHeavyGunSound = RESOURCE->LoadSound(L"getHeavyGun", L"Sound\\getHeavyGun.mp3");
	m_deathSound = RESOURCE->LoadSound(L"playerDeathSound", L"Sound\\eriDeath.mp3");
}

void CPlayer::Update()
{
	//list<CPlayerMissile*>::iterator listIter = m_listMissile.begin();
	//for (; listIter != m_listMissile.end(); listIter++)
	//{ 
	//	auto pMissile = *listIter;
	//	if (pMissile->GetReserveDelete() && !pMissile->GetSafeToDelete())
	//	{
	//		// 폭발 애니메이터 생성뒤 지움
	//		CAniObject* m_pMissileAniObj = new CAniObject;
	//		m_pMissileAniObj->SetImage(m_pEffectImage);
	//		m_pMissileAniObj->SetPos(pMissile->GetPos());
	//		m_pMissileAniObj->SetExtension(m_fExtension);
	//		m_pMissileAniObj->SetLayer(Layer::Unit);
	//		ADDOBJECT(m_pMissileAniObj);
	//		m_pMissileAniObj->GetAnimator()->CreateAnimation(L"Effect\\PlayerMissileEffect", m_pEffectImage, 0.05f, false);
	//		m_pMissileAniObj->GetAnimator()->Play(L"Effect\\PlayerMissileEffect");
	//		float m_duration = 0;
	//		m_listMissileAniObj.push_back(make_pair(&m_duration, m_pMissileAniObj));
	//		m_listMissile.erase(listIter);
	//		break;
	//	}
	//}
	//
	//for (auto pMissileAni : m_listMissileAniObj)
	//{
	//	*(pMissileAni.first) += DT;
	//}
	//
	//list<pair<float*, CAniObject*>>::iterator listEraseAniIter = m_listMissileAniObj.begin();
	//for (; listEraseAniIter != m_listMissileAniObj.end(); listEraseAniIter++)//auto pMissileAni : m_listMissileAniObj)
	//{
	//	auto pMissileAni = *listEraseAniIter;
	//	if (*(pMissileAni.first) > 0.4f)
	//	{
	//		DELETEOBJECT(pMissileAni.second);
	//		m_listMissileAniObj.erase(listEraseAniIter); // 자료구조 크기가 줄어들면서 오류 일어날 수 있음
	//		break;
	//	}
	//}


	//if (m_pMissile != nullptr && !m_pMissile->GetSafeToDelete())
	//{
	//	if (m_pMissile->GetReserveDelete())
	//	{
	//		// 폭발 애니메이터 생성뒤 지움
	//		m_pMissileAniObj = new CAniObject;
	//		m_pMissileAniObj->SetImage(m_pEffectImage);
	//		m_pMissileAniObj->SetPos(m_pMissile->GetPos());
	//		m_pMissileAniObj->SetExtension(m_fExtension);
	//		m_pMissileAniObj->SetLayer(Layer::Unit);
	//		ADDOBJECT(m_pMissileAniObj);
	//		m_pMissileAniObj->GetAnimator()->CreateAnimation(L"Effect\\PlayerMissileEffect", m_pEffectImage, 0.1f, false);
	//		m_pMissileAniObj->GetAnimator()->Play(L"Effect\\PlayerMissileEffect");
	//	}
	//}
	//if (m_pMissileAniObj != nullptr && !m_pMissileAniObj->GetReserv eDelete())
	//{
	//	m_fMissileDisappearAccTime += DT;
	//	if (m_fMissileDisappearAccTime >= 0.8f)
	//	{
	//		DELETEOBJECT(m_pMissileAniObj);
	//		m_fMissileDisappearAccTime = 0;
	//	}
	//}

	if (m_status != PlayerStatus::Victory)
	{
		if (m_curWeapon == PlayerWeapon::Pistol || m_curWeapon == PlayerWeapon::HeavyMachineGun)
			m_curGun = m_curWeapon;
		if(m_status!= PlayerStatus::Prepare)
			KeyUpdate();
		BehaviorUpdate();
		StatusUpdate();
	}
	AnimatorUpdate();
	if (m_curWeapon == PlayerWeapon::Bomb || m_curWeapon == PlayerWeapon::Knife)
		m_curWeapon = m_curGun;
}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
}

void CPlayer::KeyUpdate()
{
	tempLookDir.y = m_vecLookDir.y;
	m_prevStatus = m_status;
	m_bBombDiff = false;
	m_bBulletDiff = false;

	if (!(m_status == PlayerStatus::Dead))
	{
		if (BUTTONSTAY(VK_LEFT))
		{
			m_vecLookDir.x = -1;
			m_vecMoveDir.x = -1;
		}
		else if (BUTTONSTAY(VK_RIGHT))
		{
			m_vecLookDir.x = +1;
			m_vecMoveDir.x = +1;
		}
		else
		{
			m_vecMoveDir.x = 0;
		}

		if (BUTTONSTAY(VK_UP))
		{
			m_vecLookDir.y = -1;
		}
		else if (BUTTONSTAY(VK_DOWN))
		{
			m_vecLookDir.y = +1;
		}
		else
		{
			m_vecLookDir.y = 0;
		}

		if (BUTTONDOWN(VK_SPACE))
		{
			if (!m_bIsJump && m_status != PlayerStatus::SitAttack)
			{
				m_bIsJump = true;
				m_vecMoveDir.y = -1;
				m_status = PlayerStatus::Jump;
				m_gravity->SetVelocity(-400);
			}
		}

		if (BUTTONDOWN('A'))
		{
			m_bBulletDiff = true;
			SOUND->Play(m_gunSound);
			if (m_bIsPrevCurLookSame)
			{
				if (tempLookDir.y != m_vecPrevLookDir.y)
					m_bIsPrevCurLookSame = false;
			}
			else
			{
				m_vecPrevLookDir.y = tempLookDir.y;
				m_bIsPrevCurLookSame = true;
			}

			CreateMissile();
			// TODO : 총알에 따른 무기 정해주기
			//m_curWeapon = PlayerWeapon::HeavyMachineGun;
			m_fAcctime = 0;
			m_bIsAttack = true;
			m_bIsShoot = true;
		}
		if (BUTTONDOWN('F') && m_iBomb > 0)
		{
			m_bBombDiff = true;
			if (m_iBomb > 0)
				m_iBomb--;
			else m_iBomb == 0;
			m_curWeapon = PlayerWeapon::Bomb;
			CBomb* pBomb = new CBomb;
			ADDOBJECT(pBomb);
			pBomb->SetPos(m_vecPos);
			pBomb->SetExtension(m_fExtension);
			pBomb->SetDir(Vector(m_vecLookDir.x, 0));
			pBomb->SetOwner(this);
			m_fAcctime = 0;
			m_bIsAttack = true;
			m_bIsShoot = true;
		}
		// PrevLookDir.y 정의
		if (tempLookDir.y != m_vecLookDir.y)
		{
			m_vecPrevLookDir.y = tempLookDir.y;
		}
	}
	if (BUTTONDOWN('Q'))
	{
		m_bIsDead = false;
		m_hp = 5;
		m_bIsJump = true;
		m_fSpeed = 300;
		m_status = PlayerStatus::Idle;
		if(dynamic_cast<CSceneStage01*>(SCENE->GetCurScene()))
			m_vecPos = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.2, WINSIZEY * 0.5));
		else if (dynamic_cast<CSceneBoss*>(SCENE->GetCurScene()))
			m_vecPos = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.8, WINSIZEY * 0.5));
	}
}

void CPlayer::AnimatorUpdate()
{
	m_fAcctime += DT;

	switch (m_status)
	{
	case PlayerStatus::Prepare:
		m_pAnimator1->Play(L"Player\\Jump\\EriJumpR_1");
		m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
		break;
	case PlayerStatus::Idle:
	{
		if (m_vecLookDir.x > 0) // m_vecLookDir.x == 0인 경우는 없음
		{
			m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
			if (m_fAcctime >= 5.f)
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleGumR_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator2->Stop();
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunColdR");
				}
			}
			else
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleR_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunR_1");
				}
			}
		}
		else
		{
			m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
			if (m_fAcctime >= 5.f)
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleGumL_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator2->Stop();
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunColdL");
				}
			}
			else
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleL_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunL_1");
				}
			}
		}
		if (m_fAcctime > 7.5f)
			m_fAcctime = 0;
	}
		break;
	case PlayerStatus::Move:
	{
		if (m_vecMoveDir.x > 0)
		{
			if (m_fAcctime >= 0.05 * 4)
				m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");

			if (m_curWeapon == PlayerWeapon::Pistol)
				m_pAnimator1->Play(L"Player\\Move\\EriRunR_1");
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				m_pAnimator1->Play(L"Player\\Move\\EriHeavyGunRunR_1");
		}
		else
		{
			if (m_fAcctime >= 0.05 * 4)
				m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");

			if (m_curWeapon == PlayerWeapon::Pistol)
				m_pAnimator1->Play(L"Player\\Move\\EriRunL_1");
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				m_pAnimator1->Play(L"Player\\Move\\EriHeavyGunRunL_1");
		}
	}
		break;
	case PlayerStatus::Jump:
	{
		if (m_vecLookDir.x > 0)
		{
			if (m_vecMoveDir.x == 0)
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpR_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpMoveR_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
			}
		}
		else
		{
			if (m_vecMoveDir.x == 0)
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpL_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpMoveL_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
			}
		}
	}
		break;
	case PlayerStatus::Attack:
	{
		if (m_vecMoveDir.x > 0) // 오른쪽으로 움직이는 경우
		{
			if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if(m_vecPrevLookDir.y == 0)
						HeavyGunGoingUp(L"R");
					else if (m_vecPrevLookDir.y < 0)
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else if (m_vecLookDir.y == 0)
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime > m_fShootTime)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if (m_vecPrevLookDir.y < 0)
					{
						HeavyGunGoingUpPlace(L"R");
					}
					else if (m_vecPrevLookDir.y > 0)
					{
						HeavyGunGoingDownPlace(L"R");
					}
					else
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else // else if (m_vecLookDir.y > 0)
			{
				if (m_bIsJump)
				{
					if (m_bIsShoot)
					{
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingDown(L"R");
						else if (m_vecPrevLookDir.y > 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
			}
		}
		else if (m_vecMoveDir.x < 0) // 왼쪽으로 움직이는 경우
		{
			if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if (m_vecPrevLookDir.y == 0)
						HeavyGunGoingUp(L"L");
					else if (m_vecPrevLookDir.y < 0)
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else if (m_vecLookDir.y == 0)
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime > m_fShootTime)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if (m_vecPrevLookDir.y < 0)
					{
						HeavyGunGoingUpPlace(L"L");
					}
					else if (m_vecPrevLookDir.y > 0)
					{
						HeavyGunGoingDownPlace(L"L");
					}
					else
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else // else if (m_vecLookDir.y > 0)
			{
				if (m_bIsJump)
				{
					if (m_bIsShoot)
					{
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingDown(L"L");
						else if (m_vecPrevLookDir.y > 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
			}
		}
		else // 아무 쪽으로도 가지 않고 있을 경우 (m_vecMoveDir.x == 0)
		{
			if (m_vecLookDir.x > 0)
			{
				if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingUp(L"R");
						else if (m_vecPrevLookDir.y < 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y == 0)
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y < 0)
						{
							HeavyGunGoingUpPlace(L"R");
						}
						else if (m_vecPrevLookDir.y > 0)
						{
							HeavyGunGoingDownPlace(L"R");
						}
						else
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime > m_fShootTime)
							{
								m_bIsAttack = false;
							}
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else // else if (m_vecLookDir.y > 0)
				{
					if (m_bIsJump)
					{
						if (m_bIsShoot)
						{
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
						}

						if (m_curWeapon == PlayerWeapon::Pistol)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
						{
							if (m_vecPrevLookDir.y == 0)
								HeavyGunGoingDown(L"R");
							else if (m_vecPrevLookDir.y > 0)
							{
								m_fShootTime = 0.05f * 4;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownR_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
						else if (m_curWeapon == PlayerWeapon::Bomb)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::Knife)
						{
							srand((unsigned)time(NULL));
							int playAnimatorNum = rand() % 2;
							if (playAnimatorNum == 0)
							{
								m_fShootTime = 0.05f * 6;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
							else
							{
								m_fShootTime = 0.05f * 9;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
					}
				}
			}
			else //(m_vecLookDir.x < 0)
			{
				if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingUp(L"L");
						else if (m_vecPrevLookDir.y < 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y == 0)
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y < 0)
						{
							HeavyGunGoingUpPlace(L"L");
						}
						else if (m_vecPrevLookDir.y > 0)
						{
							HeavyGunGoingDownPlace(L"L");
						}
						else
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime > m_fShootTime)
							{
								m_bIsAttack = false;
							}
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else // else if (m_vecLookDir.y > 0)
				{
					if (m_bIsJump)
					{
						if (m_bIsShoot)
						{
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
						}

						if (m_curWeapon == PlayerWeapon::Pistol)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
						{
							if (m_vecPrevLookDir.y == 0)
								HeavyGunGoingDown(L"L");
							else if (m_vecPrevLookDir.y > 0)
							{
								m_fShootTime = 0.05f * 4;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownL_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
						else if (m_curWeapon == PlayerWeapon::Bomb)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::Knife)
						{
							srand((unsigned)time(NULL));
							int playAnimatorNum = rand() % 2;
							if (playAnimatorNum == 0)
							{
								m_fShootTime = 0.05f * 6;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
							else
							{
								m_fShootTime = 0.05f * 9;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
					}
				}
			}
		}
	}
		break;
	case PlayerStatus::SitAttack:
	{
		if (m_vecLookDir.x > 0) // 오른쪽을 보고 있을 경우
		{
			if (m_bIsShoot)
			{
				m_pAnimator2->Stop();
			}
			if (m_curWeapon == PlayerWeapon::Pistol)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitAttackR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
			{
				m_fShootTime = 0.05f * 7;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitHeavyGunR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Bomb)
			{
				m_fShootTime = 0.05f * 8;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitBombR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Knife)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitKnifeR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
		}
		else if (m_vecLookDir.x < 0) // 왼쪽을 보고 있을 경우
		{
			if (m_bIsShoot)
			{
				m_pAnimator2->Stop();
			}
			if (m_curWeapon == PlayerWeapon::Pistol)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitAttackL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
			{
				m_fShootTime = 0.05f * 7;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitHeavyGunL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Bomb)
			{
				m_fShootTime = 0.05f * 8;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitBombL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Knife)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitKnifeL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
		}
	}
		break;
	case PlayerStatus::Dead:
	{
		m_pAnimator2->Stop();

		if (m_vecLookDir.x > 0)
		{
			if (m_bIsJump)
			{
				m_pAnimator1->Play(L"Player\\Death\\EriDeathJumpL");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Death\\EriNearDeathL");
			}
		}
		else
		{
			if (m_bIsJump)
			{
				m_pAnimator1->Play(L"Player\\Death\\EriDeathJumpR");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Death\\EriNearDeathR");
			}
		}
	}
		break;

	case PlayerStatus::Victory:
	{
		m_pAnimator2->Stop();
		m_pAnimator1->Play(L"Player\\Victory\\EriVictory");
	}
		break;	
	}
}

void CPlayer::BehaviorUpdate()
{
	switch (m_status)
	{
	case PlayerStatus::Prepare:
		m_bIsAttack = false;
		m_bIsJump = false;	
		m_bIsMove = false;
		m_bIsSit = false;
		m_bIsShoot = false;
		m_gravity->SetGravity(45);
		break;
	case PlayerStatus::Idle:
		break;
	case PlayerStatus::Move:
		m_vecPos.x += m_vecMoveDir.x * m_fSpeed * DT;
		break;
	case PlayerStatus::Jump:
		m_vecPos.x += m_vecMoveDir.x * m_fSpeed * DT;
		break;
	case PlayerStatus::Attack:
		m_vecPos.x += m_vecMoveDir.x * m_fSpeed * DT;
		break;
	case PlayerStatus::SitAttack:
		m_bIsSit = true;
		break;
	case PlayerStatus::Dead:
		if (!m_bIsDead)
		{
			m_bIsDead = true;
			m_gravity->SetVelocity(-300);
			SOUND->Play(m_deathSound);
		}
		m_vecPos.x -= m_vecMoveDir.x * m_fSpeed * DT;
		break;
	case PlayerStatus::Victory:
		break;
	}

}

void CPlayer::HeavyGunGoingDownPlace(wstring RorL)
{
	float goingDownPlaceTime = 0.03f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_bIsJump)
	{
		if (m_fAcctime < goingDownPlaceTime)
			m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlace" + RorL + L"_1");
		else
		{
			if (m_fAcctime >= goingDownPlaceTime && m_fAcctime < goingDownPlaceTime + m_fShootTime)
			{
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttack" + RorL + L"_1", true);
					m_bIsShoot = false;
				}
			}
			if (m_fAcctime >= goingDownPlaceTime + m_fShootTime)
			{
				m_bIsAttack = false;
			}
		}
	}
}

void CPlayer::HeavyGunGoingDown(wstring RorL)
{
	float goingDownTime = 0.03f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_bIsJump)
	{
		if (m_fAcctime < goingDownTime)
			m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDown" + RorL + L"_1");
		else
		{
			if (m_fAcctime >= goingDownTime && m_fAcctime < goingDownTime + m_fShootTime)
			{
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDown" + RorL + L"_1", true);
					m_bIsShoot = false;
				}
			}
			if (m_fAcctime >= goingDownTime + m_fShootTime)
			{
				m_bIsAttack = false;
			}
		}
	}
}

void CPlayer::HeavyGunGoingUpPlace(wstring RorL)
{
	float goingUpPlaceTime = 0.02f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_fAcctime < goingUpPlaceTime)
		m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlace" + RorL + L"_1");
	else
	{
		if (m_fAcctime >= goingUpPlaceTime && m_fAcctime < goingUpPlaceTime + m_fShootTime)
		{
			if (m_bIsShoot)
			{
				m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttack" + RorL + L"_1", true);
				m_bIsShoot = false;
			}
		}
		if (m_fAcctime >= goingUpPlaceTime + m_fShootTime)
		{
			m_bIsAttack = false;
		}
	}
}

void CPlayer::HeavyGunGoingUp(wstring RorL)
{
	float goingUpTime = 0.03f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_fAcctime < goingUpTime)
		m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUp" + RorL + L"_1");
	else
	{
		if (m_fAcctime >= goingUpTime && m_fAcctime < goingUpTime + m_fShootTime)
		{
			if (m_bIsShoot)
			{
				m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUp" + RorL + L"_1", true);
				m_bIsShoot = false;
			}
		}
		if (m_fAcctime >= goingUpTime + m_fShootTime)
		{
			m_bIsAttack = false;
		}
	}
}

void CPlayer::StatusUpdate()
{
	switch (m_status)
	{
	case PlayerStatus::Prepare:
		if (m_fAcctime > 5.0f)
		{
			m_gravity->SetGravity(600);
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Idle:
		if (m_hp <=0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_vecMoveDir.y > 0)
		{
			m_status = PlayerStatus::Jump;
			m_fAcctime = 0;
		}
		//else if (m_bIsAttack && !m_bIsJump && m_vecLookDir.y > 0)
		//{
		//	m_status = PlayerStatus::SitAttack;
		//	m_fAcctime = 0;
		//}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (m_vecMoveDir.x != 0)
		{
			m_status = PlayerStatus::Move;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Move:
		if(m_hp <= 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if(m_vecMoveDir.x == 0)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		//else if (m_bIsAttack && !m_bIsJump && m_vecLookDir.y > 0)
		//{
		//	m_status = PlayerStatus::SitAttack;
		//	m_fAcctime = 0;
		//}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (m_vecMoveDir.y >0)
		{
			m_status = PlayerStatus::Jump;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Jump:
		if (m_hp <= 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (!m_bIsJump)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Attack:
		if (m_hp <= 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
			break;
		}
		else if (!m_bIsJump && m_vecLookDir.y > 0)
		{
			m_status = PlayerStatus::SitAttack;
			m_fAcctime = 0;
			break;
		}
		if (!m_bIsAttack)
		{
			m_vecPrevLookDir.y = 0;
			if (m_vecMoveDir.y < 0)
				m_status = PlayerStatus::Jump;
			else if (m_vecMoveDir.x != 0)// && !(m_vecLookDir.y > 0) && !(m_vecPrevLookDir.y > 0))
				m_status = PlayerStatus::Move;
			else
				m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::SitAttack:
		if (m_hp == 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
			m_bIsSit = false;
			break;
		}
		//if (m_vecMoveDir.y < 0)
		//{
		//	m_status = PlayerStatus::Jump;
		//	m_fAcctime = 0;
		//	m_bIsAttack = false;
		//	break;
		//}
		if (!m_bIsAttack)
		{
			m_vecPrevLookDir.y = 0;
			if (m_bIsJump || m_vecLookDir.y <= 0)
				m_status = PlayerStatus::Attack;
			else if (m_vecMoveDir.x != 0)// && !(m_vecLookDir.y > 0) && !(m_vecPrevLookDir.y > 0))
				m_status = PlayerStatus::Move;
			else
				m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
			m_bIsSit = false;
			//else if (m_vecMoveDir.x == 0 && m_vecMoveDir.y <= 0)
			//	m_status = PlayerStatus::Idle;
		}
	case PlayerStatus::Dead:
		// 죽었으면 새로 부활
		break;
	case PlayerStatus::Victory:
		m_bIsJump = true;
		m_gravity->SetVelocity(-10);
		m_bIsDead = false;
		m_bIsMove = false;
		break;
	default:
		break;
	}
}

void CPlayer::CreateMissile()
{
	Logger::Debug(L"미사일 생성");

	CPlayerMissile* m_pMissile = new CPlayerMissile;
	m_pMissile->SetName(L"PlayerMissile");
	m_pMissile->SetVelocity(1000);

	if (m_vecLookDir.y < 0)
	{
		if (m_bIsSit)
		{
			m_pMissile->SetDir(Vector(m_vecLookDir.x, 0));
			m_pMissile->SetPos(m_vecPos + Vector(m_vecLookDir.x * 100, 0));
		}
		else
		{
			m_pMissile->SetDir(Vector(0, m_vecLookDir.y));
			m_pMissile->SetPos(m_vecPos + Vector(0, m_vecLookDir.y * 100));
		}
	}
	else if (m_vecLookDir.y > 0)
	{
		if (m_bIsJump)
		{
			m_pMissile->SetDir(Vector(0, m_vecLookDir.y));
			m_pMissile->SetPos(m_vecPos + Vector(0, m_vecLookDir.y * 100));
		}
		else
		{
			m_pMissile->SetDir(Vector(m_vecLookDir.x, 0));
			m_pMissile->SetPos(m_vecPos + Vector(m_vecLookDir.x * 100, 0));
		}
	}
	else
	{
		m_pMissile->SetDir(Vector(m_vecLookDir.x, 0));
		m_pMissile->SetPos(m_vecPos + Vector(m_vecLookDir.x * 100, 0));
	}
	/*
	if (m_vecLookDir.y < 0)
		m_pMissile->SetDir(Vector(0, m_vecLookDir.y));
	else if (m_bIsJump && m_vecLookDir.y > 0)
		m_pMissile->SetDir(Vector(0, m_vecLookDir.y));
	else if(m_vecLookDir.x > 0)
		m_pMissile->SetDir(Vector(m_vecLookDir.x, 0));
	else if (m_vecLookDir.x < 0)
		m_pMissile->SetDir(Vector(m_vecLookDir.x, 0));
		*/
	if (m_curWeapon == PlayerWeapon::Pistol)
	{
		m_pMissile->SetPos(m_pMissile->GetPos() + Vector(0, -20));
	}

	m_pMissile->SetOwner(this);
	m_pMissile->SetExtension(m_fExtension);
	//m_listMissile.push_back(m_pMissile);
	ADDOBJECT(m_pMissile);
}

CGravity* CPlayer::GetGravity()
{
	return m_gravity;
}

void CPlayer::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"ground")
	{
		m_bIsJump = false;
		m_bIsAttack = false;
		m_vecMoveDir.y = 0;
		m_vecLookDir.y = 0;
		m_gravity->SetVelocity(0);
	}
	else if (pOtherCollider->GetObjName() == L"slopeGround")
	{
		if (m_bIsJump)
		{
			m_bIsJump = false;
			m_bIsAttack = false;
		}
		m_vecMoveDir.y = 0;
		m_vecLookDir.y = 0;
		m_gravity->SetVelocity(0);
	}
	if (pOtherCollider->GetObjName() == L"obstacle" || pOtherCollider->GetObjName() == L"obstacleCastle")
	{
		//m_gravity->SetVelocity(0); // 벽에 붙음
		if(m_vecPos.x > pOtherCollider->GetPos().x)
			m_vecPos.x += m_fSpeed * DT + 3;
		else
			m_vecPos.x -= m_fSpeed * DT + 3;
		if(!m_bIsJump)
			m_gravity->SetVelocity(0);
		m_fSpeed = 0;
	}
	if (pOtherCollider->GetObjName() == L"deathObstacle")
	{
		m_bIsJump = true;
		//m_bIsAttack = false;
		//m_vecMoveDir.y = 0;
		//m_vecLookDir.y = 0;
		//m_gravity->SetVelocity(0);
		m_status = PlayerStatus::Dead;
	}
	if (pOtherCollider->GetObjName() == L"ItemBomb")
	{
		SOUND->Play(m_okaySound);
		m_iBomb += 10;
	}
	if (pOtherCollider->GetObjName() == L"ItemHeavyGun")
	{
		SOUND->Play(m_getHeavyGunSound);
		m_curWeapon = PlayerWeapon::HeavyMachineGun;
	}
}

void CPlayer::OnCollisionStay(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"ground")
	{
		if (m_status == PlayerStatus::Dead)
		{
			m_fSpeed = 0;
			m_gravity->SetVelocity(0);
		}
		if (!m_bIsJump)
			m_gravity->SetVelocity(0);
	}
	if (pOtherCollider->GetObjName() == L"slopeGround")
	{
		if (m_status == PlayerStatus::Dead)
		{
			m_fSpeed = 0;
			m_gravity->SetVelocity(0);
		}
		if (pOtherCollider->GetRotation() < 0)
		{
			if (m_vecMoveDir.x > 0)
			{
				if (!m_bIsJump)
					m_gravity->SetVelocity(0);
				m_vecPos.y += m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
			}
			else if (m_vecMoveDir.x == 0)
			{
				if (!m_bIsJump)
					m_gravity->SetVelocity(0);
			}
			else if (m_vecMoveDir.x < 0)
			{
				if (!m_bIsJump)
					m_gravity->SetVelocity(0);
				if(!m_bIsSit)
					m_vecPos.y -= m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
			}

			if (m_bIsSit)
				m_gravity->SetVelocity(0);
		}
		else
		{
			if (m_vecMoveDir.x < 0)
			{
				if (!m_bIsJump)
					m_gravity->SetVelocity(0);
				m_vecPos.y -= m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
			}
			else if (m_vecMoveDir.x == 0)
			{
				if (!m_bIsJump)
					m_gravity->SetVelocity(0);
			}
			else if (m_vecMoveDir.x > 0)
			{
				if (!m_bIsJump)
					m_gravity->SetVelocity(0);
				if (!m_bIsSit)
					m_vecPos.y -= m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
			}

			if (m_bIsSit)
				m_gravity->SetVelocity(0);
		}
	}
}

void CPlayer::OnCollisionExit(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"obstacle" || pOtherCollider->GetObjName() == L"obstacleCastle")
	{
		m_fSpeed = 300;
	}
	//if (pOtherCollider->GetObjName() == L"slopeGround")
	//{
	//	if (m_status == PlayerStatus::Dead)
	//	{
	//		m_fSpeed = 0;
	//		m_gravity->SetVelocity(0);
	//	}
	//	if (pOtherCollider->GetRotation() < 0)
	//	{
	//		if (m_vecMoveDir.x > 0)
	//		{
	//			if (!m_bIsJump)
	//				m_gravity->SetVelocity(0);
	//			m_vecPos.y += m_fSpeed * sinf(pOtherCollider->GetRotation() / 180 * PI) * DT;
	//		}
	//		else if (m_vecMoveDir.x == 0)
	//		{
	//			if (!m_bIsJump)
	//				m_gravity->SetVelocity(0);
	//		}
	//
	//		if (m_bIsSit)
	//			m_gravity->SetVelocity(0);
	//	}
	//	else
	//	{
	//		if (m_vecMoveDir.x < 0)
	//		{
	//			if (!m_bIsJump)
	//				m_gravity->SetVelocity(0);
	//			m_vecPos.y -= m_fSpeed * sinf(pOtherCollider->GetRotation() / 180 * PI) * DT;
	//		}
	//		else if (m_vecMoveDir.x == 0)
	//		{
	//			if (!m_bIsJump)
	//				m_gravity->SetVelocity(0);
	//		}
	//
	//		if (m_bIsSit)
	//			m_gravity->SetVelocity(0);
	//	}
	//}
}
