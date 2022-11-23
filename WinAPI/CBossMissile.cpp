#include "framework.h"
#include "CBossMissile.h"

#include "CGravity.h"
#include "CPlayer.h"
#include "CAniObject.h"

CBossMissile::CBossMissile()
{
	m_layer = Layer::BossMissile;
	m_fDisappearAccTime = 0;
	m_reserveDelete = false;
	m_bCreatedAni = false;
}

CBossMissile::~CBossMissile()
{
}

void CBossMissile::Init()
{
	AddCollider(ColliderType::Rect, Vector(40, 40), Vector(0, 0));

	if (m_strName == L"BossFireMissile" && m_pOwner->GetName() == L"Boss")
	{
		m_pAnimator = new CAnimator;
		CImage* pBossFireMissile = RESOURCE->LoadImg(L"BossFireMissile", L"Image\\Boss\\BossAttack.png");
		m_pAnimator->CreateAnimation(L"Boss\\BossFireMissile", pBossFireMissile, 0.1f, false);
		m_pGravity = new CGravity;
		AddComponent(m_pGravity);
		m_pGravity->SetVelocity(10);
		AddComponent(m_pAnimator);
	}
	else if (m_strName == L"BossMissile" && m_pOwner->GetName() == L"Boss")
	{
		m_pAnimator = new CAnimator;
		CImage* pBossMissile = RESOURCE->LoadImg(L"BossMissile", L"Image\\Boss\\BossAttack.png");
		m_pAnimator->CreateAnimation(L"Boss\\BossMissile", pBossMissile, 0.1f, false);
		m_pGravity = new CGravity;
		AddComponent(m_pGravity);
		m_pGravity->SetVelocity(-10);
		m_pGravity->SetGravity(1500);
		AddComponent(m_pAnimator);
	}

	m_pEffectImage = new CImage;
	m_pEffectImage = RESOURCE->LoadImg(L"MissileEffectImg", L"Image\\Effect\\MissileEffect.png");
	
	m_pExplode = RESOURCE->LoadSound(L"BossMissileExplode", L"Sound\\bossExplode.mp3");
}

void CBossMissile::Update()
{
	m_vecPos += m_vecDir * m_fVelocity * DT;

	if (m_reserveDelete && !m_bCreatedAni)
	{
		m_pAnimator->GetCurAni()->SetAlpha(0);
		m_pAnimator->Stop();
		m_bCreatedAni = true;
		//if (m_strName == L"BossFireMissile")
		//{
		//	m_pFireMissileAniObj = new CAniObject;
		//	m_pFireMissileAniObj->SetImage(m_pEffectImage);
		//	m_pFireMissileAniObj->GetAnimator()->CreateAnimation(L"Effect\\PlayerMissileEffect", m_pEffectImage, 0.05f, false);
		//	m_pFireMissileAniObj->GetAnimator()->Play(L"Effect\\PlayerMissileEffect");
		//	m_pFireMissileAniObj->SetExtension(m_fExtension);
		//	m_pFireMissileAniObj->SetLayer(Layer::Effect);
		//	ADDOBJECT(m_pFireMissileAniObj);
		//}
		//else if (m_strName == L"BossMissile")
		//{
			m_pMissileAniObj = new CAniObject;
			m_pMissileAniObj->SetImage(m_pEffectImage);
			m_pMissileAniObj->GetAnimator()->CreateAnimation(L"Effect\\PlayerMissileEffect", m_pEffectImage, 0.05f, false);
			m_pMissileAniObj->GetAnimator()->Play(L"Effect\\PlayerMissileEffect");
			m_pMissileAniObj->SetExtension(m_fExtension);
			m_pMissileAniObj->SetLayer(Layer::Effect);
			ADDOBJECT(m_pMissileAniObj);
		//}

		/*if (owner->GetCurWeapon() == PlayerWeapon::Pistol)
			m_pMissileAniObj->SetPos(m_vecPos);
		else if (owner->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
		{
			if (m_vecDir.x > 0)
				m_pMissileAniObj->SetPos(m_vecPos + Vector(55, 0));
			else if (m_vecDir.x < 0)
				m_pMissileAniObj->SetPos(m_vecPos + Vector(-55, 0));

			if (m_vecDir.y > 0 || m_vecDir.y < 0)
			{
				GetCollider()->SetScale(Vector(30, 110));
				m_pMissileAniObj->SetPos(m_vecPos);
			}*/
		//}
	}

	// 화면밖으로 나갈경우 삭제
	Vector cameraLookAt = CAMERA->GetLookAt();
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).x ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(0, 0)).y ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).y)
		m_reserveDelete = true;

	if (m_strName == L"BossFireMissile" && m_pOwner->GetName() == L"Boss")
	{
		m_fAttackAccTime += DT;

		if (m_fAttackAccTime <= 0.4f)
		{
			m_pAnimator->Play(L"Boss\\BossFireMissile", true);
			m_fAttackAccTime = 0;
		}
	}
	else if (m_strName == L"BossMissile" && m_pOwner->GetName() == L"Boss")
	{
		m_fAttackAccTime += DT;

		m_pAnimator->ReversePlay(L"Boss\\BossMissile");
		m_fAttackAccTime = 0;
	}

	if (m_reserveDelete && !this->GetReserveDelete())
	{
		m_fDisappearAccTime += DT;
		if (m_fDisappearAccTime > 0.38f)
		{
			if (!this->GetReserveDelete())
			{
				DELETEOBJECT(this);
				DELETEOBJECT(m_pMissileAniObj);
			}
		}
	}
}

void CBossMissile::Render()
{
}

void CBossMissile::Release()
{
}

void CBossMissile::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (m_strName == L"BossFireMissile" && m_pOwner->GetName() == L"Boss")
	{
		//m_bIsEntered = true;
		//m_fDisappearAccTime = 0;

		if (pOtherCollider->GetObjName() == L"Player")
		{
			m_pGravity->SetVelocity(0);
			m_pGravity->SetGravity(0);
			CPlayer* pOtherObj = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
			pOtherObj->SetHp(pOtherObj->GetHp() - 5);
			m_fVelocity = 0;
			m_reserveDelete = true;
		}
		else if (pOtherCollider->GetObjName() == L"slopeGround" || pOtherCollider->GetObjName() == L"ground")
		{
			m_pGravity->SetVelocity(0);
			m_pGravity->SetGravity(0);
			m_fVelocity = 0;
			m_reserveDelete = true;
		}
		//if (m_fAttackAccTime <= 0.4f)
		//{
		//	m_pAnimator->Play(L"Boss\\BossFireMissile", true);
		//	m_fAttackAccTime = 0;
		//}
		SOUND->Play(m_pExplode);
	}
	else if (m_strName == L"BossMissile" && m_pOwner->GetName() == L"Boss")
	{
		//m_bIsEntered = true;
		//m_fDisappearAccTime = 0;

		if (pOtherCollider->GetObjName() == L"Player")
		{
			m_pGravity->SetVelocity(0);
			m_pGravity->SetGravity(0);
			CPlayer* pOtherObj = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
			pOtherObj->SetHp(pOtherObj->GetHp() - 10);
			m_fVelocity = 0;
			m_reserveDelete = true;
		}
		else if (pOtherCollider->GetObjName() == L"slopeGround" || pOtherCollider->GetObjName() == L"ground")
		{
			m_pGravity->SetVelocity(0);
			m_pGravity->SetGravity(0);
			m_fVelocity = 0;
			m_reserveDelete = true;
		}
		//if (m_fAttackAccTime <= 0.4f)
		//{
		//	m_pAnimator->Play(L"Boss\\BossFireMissile", true);
		//	m_fAttackAccTime = 0;
		//}
	}
}

void CBossMissile::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBossMissile::OnCollisionExit(CCollider* pOtherCollider)
{
}
