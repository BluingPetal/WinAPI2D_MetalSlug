#include "framework.h"
#include "CBossMissile.h"

#include "CGravity.h"
#include "CPlayer.h"

CBossMissile::CBossMissile()
{
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
}

void CBossMissile::Update()
{
	m_vecPos += m_vecDir * m_fVelocity * DT;

	// 화면밖으로 나갈경우 삭제
	Vector cameraLookAt = CAMERA->GetLookAt();
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).x ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(0, 0)).y ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).y)
		DELETEOBJECT(this);

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
			CPlayer* pOtherObj = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
			pOtherObj->SetHp(pOtherObj->GetHp() - 5);
			DELETEOBJECT(this);
		}
		else if (pOtherCollider->GetObjName() == L"slopeGround" || pOtherCollider->GetObjName() == L"ground")
		{
			m_pGravity->SetVelocity(0);
			DELETEOBJECT(this);
		}
		//if (m_fAttackAccTime <= 0.4f)
		//{
		//	m_pAnimator->Play(L"Boss\\BossFireMissile", true);
		//	m_fAttackAccTime = 0;
		//}
	}
	else if (m_strName == L"BossMissile" && m_pOwner->GetName() == L"Boss")
	{
		//m_bIsEntered = true;
		//m_fDisappearAccTime = 0;

		if (pOtherCollider->GetObjName() == L"Player")
		{
			m_pGravity->SetVelocity(0);
			CPlayer* pOtherObj = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
			pOtherObj->SetHp(pOtherObj->GetHp() - 10);
			DELETEOBJECT(this);
		}
		else if (pOtherCollider->GetObjName() == L"slopeGround" || pOtherCollider->GetObjName() == L"ground")
		{
			m_pGravity->SetVelocity(0);
			DELETEOBJECT(this);
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
