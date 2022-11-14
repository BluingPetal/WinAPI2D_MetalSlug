#include "framework.h"
#include "CConga.h"

#include "CRenderManager.h"

#include "CImage.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CGravity.h"
#include "CColliderObject.h"
#include "CPlayer.h"

CConga::CConga()
{
	m_pAnimator = nullptr;
	m_pTargetObj = nullptr;
	m_pFarColliderObj = nullptr;
	m_pNearColliderObj = nullptr;
	m_strName = L"Conga";
	m_congaState = CongaStatus::Idle;
	m_vecMoveDir = Vector(0, 0);
	m_vecLookDir = Vector(-1, 0);
	m_fSpeed = 200.f;
	m_fAccTime = 0.f;
	m_hp = 5;
	m_isAttack = false;
}

CConga::~CConga()
{
}

void CConga::Init()
{
	CImage* m_pCongaTotalImgL = RESOURCE->LoadImg(L"Conga", L"Image\\Monster\\CongaL.png");
	CImage* m_pCongaTotalImgR = RESOURCE->LoadImg(L"CongaR", L"Image\\Monster\\CongaR.png");
	m_pAnimator = new CAnimator;
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaIdleL", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaIdleR", m_pCongaTotalImgR, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaWalkL", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaWalkR", m_pCongaTotalImgR, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackL1", m_pCongaTotalImgL, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackR1", m_pCongaTotalImgR, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackL2", m_pCongaTotalImgL, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackR2", m_pCongaTotalImgR, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaDeathL", m_pCongaTotalImgL, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaDeathR", m_pCongaTotalImgR, 0.1f, false);
	m_pAnimator->Play(L"Monster\\Conga\\CongaIdleL");
	AddComponent(m_pAnimator);

	m_pFarColliderObj = new CColliderObject;
	m_pFarColliderObj->SetName(L"FarColliderObj");
	m_pFarColliderObj->SetOwner(this);
	//m_pFarColliderObj->SetExtension(m_fExtension);
	//m_pFarColliderObj->SetScale(m_vecScale);
	m_pFarColliderObj->SetScale(Vector(230, m_pAnimator->GetFirstAniFrame().slice.y * 1.5) * m_fExtension);
	ADDOBJECT(m_pFarColliderObj);

	m_pNearColliderObj = new CColliderObject;
	m_pNearColliderObj->SetName(L"NearColliderObj");
	m_pNearColliderObj->SetOwner(this);
	m_pNearColliderObj->SetScale(m_pAnimator->GetFirstAniFrame().slice * 0.4f * m_fExtension);
	ADDOBJECT(m_pNearColliderObj);

	AddCollider(ColliderType::Rect, Vector(90, 90), Vector(0, 0));

	m_pGravity = new CGravity;
	AddComponent(m_pGravity);
}

void CConga::Update()
{
	m_fAccTime += DT;

	if (m_congaState != CongaStatus::Death)
	{
		m_pFarColliderObj->SetPos(m_vecPos);
		m_pNearColliderObj->SetPos(m_vecPos + Vector(90 * m_vecLookDir.x, 0));

		if (m_pTargetObj != nullptr)
		{
			m_fSpeed = 250.f;
			if (m_pTargetObj->GetPos().x > m_vecPos.x)
				m_vecMoveDir.x = m_vecLookDir.x = 1;
			else if (m_pTargetObj->GetPos().x < m_vecPos.x)
				m_vecMoveDir.x = m_vecLookDir.x = -1;
			else
				m_vecMoveDir.x = 0;
		}
		else
		{
			m_fSpeed = 200.f;
		}
	}

	AnimationUpdate();
	StateUpdate();
}

void CConga::Render()
{
}

void CConga::Release()
{
}

void CConga::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CConga::OnCollisionStay(CCollider* pOtherCollider)
{
	m_pGravity->SetVelocity(0);
}

void CConga::OnCollisionExit(CCollider* pOtherCollider)
{
}

void CConga::StateUpdate()
{
	switch (m_congaState)
	{
	case CongaStatus::Idle:
		if (m_hp < 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
		}

		if (m_pTargetObj != nullptr || m_fAccTime >= 2.f)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Walk;
		}
		break;
	case CongaStatus::Walk:
		if (m_hp < 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
		}

		if (m_pTargetObj != nullptr)
		{
			m_vecPos.x += m_vecLookDir.x * m_fSpeed * DT;
		}
		else
		{
			m_vecPos.x += m_vecLookDir.x * m_fSpeed * DT;
			if (m_fAccTime >= 2.f)
			{
				m_fAccTime = 0;
				m_vecLookDir *= -1;
				m_congaState = CongaStatus::Idle;
			}
		}
		break;
	case CongaStatus::NearAttack:
		if (m_hp < 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
		}
		if (m_fAccTime < 1.3f)
		{
			if (!m_isAttack)
			{
				m_isAttack = true;
				dynamic_cast<CPlayer*>(m_pTargetObj)->SetHp(dynamic_cast<CPlayer*>(m_pTargetObj)->GetHp() - 1);
				//Logger::Debug(to_wstring(dynamic_cast<CPlayer*>(m_pTargetObj)->GetHp()));
			}
		}
		if (m_fAccTime >= 1.3f)
		{
			m_isAttack = false;
			m_fAccTime = 0;
		}
		break;
	case CongaStatus::FarAttack:
		if (m_hp < 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
		}
		break;
	case CongaStatus::Death:

		m_pTargetObj = nullptr;
		DELETEOBJECT(m_pFarColliderObj);
		DELETEOBJECT(m_pNearColliderObj);
		if (m_fAccTime >= 2.8f)
			DELETEOBJECT(this);
		break;
	}
}

void CConga::AnimationUpdate()
{
	switch (m_congaState)
	{
	case CongaStatus::Idle:
		if (m_vecLookDir.x > 0)
			m_pAnimator->Play(L"Monster\\Conga\\CongaIdleR");
		else
			m_pAnimator->Play(L"Monster\\Conga\\CongaIdleL");
		break;
	case CongaStatus::Walk:
		if (m_vecLookDir.x > 0)
			m_pAnimator->Play(L"Monster\\Conga\\CongaWalkR");
		else
			m_pAnimator->Play(L"Monster\\Conga\\CongaWalkL");
		break;
	case CongaStatus::NearAttack:
		if (m_isAttack)
		{
			if (m_vecLookDir.x > 0)
				m_pAnimator->Play(L"Monster\\Conga\\CongaAttackR1");
			else
				m_pAnimator->Play(L"Monster\\Conga\\CongaAttackL1");
			break;
		}
	case CongaStatus::FarAttack:
		if (m_vecLookDir.x > 0)
			m_pAnimator->Play(L"Monster\\Conga\\CongaAttackR2");
		else
			m_pAnimator->Play(L"Monster\\Conga\\CongaAttackL2");
		break;
	case CongaStatus::Death:
		if (m_vecLookDir.x > 0)
			m_pAnimator->Play(L"Monster\\Conga\\CongaDeathR");
		else
			m_pAnimator->Play(L"Monster\\Conga\\CongaDeathL");
		break;
	}
}
