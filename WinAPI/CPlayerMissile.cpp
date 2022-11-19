#include "framework.h"
#include "CPlayerMissile.h"

#include "CPlayer.h"
#include "CConga.h"

CPlayerMissile::CPlayerMissile()
{
}

CPlayerMissile::~CPlayerMissile()
{
}

void CPlayerMissile::Init()
{
	m_strName = L"PlayerMissile";
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pOwner);
	if (pPlayer->GetCurWeapon() == PlayerWeapon::Pistol)
		AddCollider(ColliderType::Rect, Vector(20, 20), Vector(0, 0));
	else if (pPlayer->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
		AddCollider(ColliderType::Rect, Vector(110, 30), Vector(0, 0));

	m_pAnimator = new CAnimator;
	CImage* pPlayerMissileImg = RESOURCE->LoadImg(L"PlayerMissile", L"Image\\Player\\Weapon.png"); // 미사일 이미지 넣기
	CImage* pPlayerHitCongaImgL = RESOURCE->LoadImg(L"CongaHurtL", L"Image\\Monster\\CongaL.png");
	CImage* pPlayerHitCongaImgR = RESOURCE->LoadImg(L"CongaHurtR", L"Image\\Monster\\CongaR.png");
	m_pAnimator->CreateAnimation(L"Missile\\Pistol", pPlayerMissileImg, 0.05f);
	m_pAnimator->CreateAnimation(L"Missile\\HeavyGunUp", pPlayerMissileImg, 0.05f);
	m_pAnimator->CreateAnimation(L"Missile\\HeavyGunDown", pPlayerMissileImg, 0.05f);
	m_pAnimator->CreateAnimation(L"Missile\\HeavyGunRight", pPlayerMissileImg, 0.05f);
	m_pAnimator->CreateAnimation(L"Missile\\HeavyGunLeft", pPlayerMissileImg, 0.05f);
	m_pAnimator->CreateAnimation(L"Missile\\CongaHurtL", pPlayerHitCongaImgL, 0.05f, false);
	m_pAnimator->CreateAnimation(L"Missile\\CongaHurtR", pPlayerHitCongaImgR, 0.05f, false);
	AddComponent(m_pAnimator);
}

void CPlayerMissile::Update()
{
	//if (!m_bIsEntered)
	//	m_vecPos += m_vecDir * m_fVelocity * DT;
	if (!m_bIsEntered)
		m_vecPos += m_vecDir * m_fVelocity * DT;

	// 화면밖으로 나갈경우 삭제
	Vector cameraLookAt = CAMERA->GetLookAt();
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).x ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(0, 0)).y ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).y)
		DELETEOBJECT(this);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pOwner);
	if (pPlayer->GetCurWeapon() == PlayerWeapon::Pistol)
		m_pAnimator->Play(L"Missile\\Pistol");
	else if (pPlayer->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
	{
		if (m_vecDir.y > 0)
			m_pAnimator->Play(L"Missile\\HeavyGunDown");
		else if (m_vecDir.y < 0)
			m_pAnimator->Play(L"Missile\\HeavyGunUp");
		else if (m_vecDir.x < 0)
			m_pAnimator->Play(L"Missile\\HeavyGunLeft");
		else if (m_vecDir.x > 0)
			m_pAnimator->Play(L"Missile\\HeavyGunRight");
	}
}

void CPlayerMissile::Render()
{
}

void CPlayerMissile::Release()
{
}

void CPlayerMissile::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"Conga")
	{
		m_bIsEntered = true;
		m_fDisappearAccTime = 0;
		CConga* pOtherObj = dynamic_cast<CConga*>(pOtherCollider->GetOwner());

		if (pOtherObj->GetCongaState() != CongaStatus::Death && !pOtherObj->GetReserveDelete())
		{
			pOtherObj->SetHp(pOtherObj->GetHp() - 1);
			m_fVelocity = 0;
			if (m_vecDir.x > 0)
				m_pAnimator->Play(L"Missile\\CongaHurtL");
			else
				m_pAnimator->Play(L"Missile\\CongaHurtR");
		}
		else if (pOtherObj->GetCongaState() == CongaStatus::Death)
			m_bIsEntered = false;
	}
	else if (pOtherCollider->GetObjName() == L"Boss" || pOtherCollider->GetObjName() == L"slopeGround" || pOtherCollider->GetObjName() == L"ground")
	{
		//6DELETEOBJECT(this);
	}
}

void CPlayerMissile::OnCollisionStay(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"Conga")
	{
		m_fDisappearAccTime += DT;
		if (m_fDisappearAccTime >= 0.05f * 8)
		{
			DELETEOBJECT(this);
			//m_fAccTime = 0;
		}
	}
}

void CPlayerMissile::OnCollisionExit(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"Conga")
	{
		m_bIsEntered = false;
	}
}
