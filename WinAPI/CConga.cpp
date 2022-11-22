#include "framework.h"
#include "CConga.h"

#include "CRenderManager.h"

#include "CImage.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CGravity.h"
#include "CColliderObject.h"
#include "CPlayer.h"
#include "CMissile.h"
#include "CPlayerMissile.h"
#include "CBomb.h"
#include "CScene.h"
#include "CSceneStage01.h"

#define PI 3.141592

CConga::CConga()
{
	m_pAnimator = nullptr;
	m_pTargetObj = nullptr;
	m_pFarColliderObj = nullptr;
	m_pNearColliderObj = nullptr;
	m_strName = L"Conga";
	m_congaState = CongaStatus::Idle;
	m_fAccTime = 0.f;
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
	m_pFarColliderObj->SetScale(Vector(230, m_pAnimator->GetFirstAniFrame().slice.y * 1.5) * m_fExtension);

	m_pNearColliderObj = new CColliderObject;
	m_pNearColliderObj->SetName(L"NearColliderObj");
	m_pNearColliderObj->SetOwner(this);
	m_pNearColliderObj->SetScale(m_pAnimator->GetFirstAniFrame().slice * 0.4f * m_fExtension);

	AddCollider(ColliderType::Rect, Vector(90, 90), Vector(0, 0));

	m_congaDeathSound = RESOURCE->LoadSound(L"congaDeath", L"Sound\\congaDeath.mp3");

	m_pGravity = new CGravity;
	AddComponent(m_pGravity);
}

void CConga::Update()
{
	m_fAccTime += DT;

	//Logger::Debug(to_wstring(dynamic_cast<CPlayer*>(m_pTargetObj)->GetHp()));
	//Logger::Debug(to_wstring(m_hp));

	if (m_congaState != CongaStatus::Death)
	{
		m_pFarColliderObj->SetPos(m_vecPos);
		m_pNearColliderObj->SetPos(m_vecPos + Vector(90 * m_vecLookDir.x, 0));

		if (m_pTargetObj != nullptr)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTargetObj);
			if (!pPlayer->GetIsDead())
			{
				m_fSpeed = 250.f;
				if (m_pTargetObj->GetPos().x > m_vecPos.x)
					m_vecMoveDir.x = m_vecLookDir.x = 1;
				else if (m_pTargetObj->GetPos().x < m_vecPos.x)
					m_vecMoveDir.x = m_vecLookDir.x = -1;
				else
					m_vecMoveDir.x = 0;
			}
			//else
			//{
			//	m_pTargetObj = nullptr;
			//}
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
	dynamic_cast<CSceneStage01*>(SCENE->GetCurScene())->CongaDecrease();
}

void CConga::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"obstacle")
	{
		if(m_congaState != CongaStatus::Death)
			m_vecLookDir.x *= -1;
	}
	if(pOtherCollider->GetObjName() == L"ground")
	{
		m_pGravity->SetVelocity(0);
	}
	if (pOtherCollider->GetObjName() == L"slopeGround")
	{
		m_pGravity->SetVelocity(0);
	}
	if (pOtherCollider->GetObjName() == L"PlayerMissile")
	{
		CPlayerMissile* pMissile = dynamic_cast<CPlayerMissile*>(pOtherCollider->GetOwner());
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pMissile->GetOwner());
		SetTarget(pPlayer);
		if (pPlayer->GetCurWeapon() == PlayerWeapon::Pistol)
			m_hp--;
		else if (pPlayer->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
			m_hp -= 2;
	}
	if (pOtherCollider->GetObjName() == L"Bomb")
	{
		CBomb* pMissile = dynamic_cast<CBomb*>(pOtherCollider->GetOwner());
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pMissile->GetOwner());
		SetTarget(pPlayer);
		m_hp -= 5;
	}
}

void CConga::OnCollisionStay(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"ground")
	{
		if (m_congaState == CongaStatus::Death)
			m_fSpeed = 0;
		m_pGravity->SetVelocity(0);
	}
	if (pOtherCollider->GetObjName() == L"slopeGround")
	{
		m_pGravity->SetVelocity(0);
		if (m_congaState == CongaStatus::Death)
			m_fSpeed = 0;
		if (pOtherCollider->GetRotation() < 0)
		{
			if (m_vecMoveDir.x > 0)
				m_vecPos.y += m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
			else if (m_vecMoveDir.x < 0)
				m_vecPos.y -= m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
		}
		else
		{
			if (m_vecMoveDir.x > 0)
				m_vecPos.y -= m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
			else if (m_vecMoveDir.x < 0)
				m_vecPos.y -= m_fSpeed * tanf(pOtherCollider->GetRotation() / 180 * PI) * DT;
		}
	}
}

void CConga::OnCollisionExit(CCollider* pOtherCollider)
{
}

void CConga::StateUpdate()
{
	switch (m_congaState)
	{
	case CongaStatus::Idle:
		if (m_hp <= 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
			break;
		}
		if (m_pTargetObj != nullptr || m_fAccTime >= 2.f)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Walk;
		}
		break;
	case CongaStatus::Walk:
		if (m_hp <= 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
			break;
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
		if (m_hp <= 0)
		{
			m_fAccTime = 0;
			m_congaState = CongaStatus::Death;
			break;
		}
		Logger::Debug(to_wstring(m_fAccTime));
		if (m_fAccTime < 1.3f)
		{
			//if (dynamic_cast<CPlayer*>(m_pTargetObj)->GetIsDead())
			//{
			//	m_pTargetObj = nullptr;
			//	m_congaState = CongaStatus::Idle;
			//}
			if (!m_isAttack) // player가 near attack 중에 death state로 갔을 경우도 제외
			{
				//Logger::Debug(to_wstring(m_fAccTime));
				//Logger::Debug(L"Here");
				
  				m_isAttack = true;
	 			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTargetObj);
				pPlayer->SetHp(dynamic_cast<CPlayer*>(m_pTargetObj)->GetHp() - 1);
				Logger::Debug(to_wstring(pPlayer->GetHp()));
			}
		}
		else//if (m_fAccTime >= 1.3f)
		{
			m_isAttack = false;
			m_fAccTime = 0;
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTargetObj);
			if (pPlayer->GetHp() == 0)
			{
				m_fAccTime = 0;
				m_pTargetObj = nullptr;
				m_congaState = CongaStatus::Idle;
			}
		}
		break;
	case CongaStatus::Death:
		m_pTargetObj = nullptr;
		DELETEOBJECT(m_pFarColliderObj);
		DELETEOBJECT(m_pNearColliderObj);
		if (m_fAccTime >= 2.8f)
		{
			DELETEOBJECT(this);
			SOUND->Play(m_congaDeathSound, 0.5f);
		}
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
		}
		else
		{
			m_pAnimator->Stop();
		}
		break;
	case CongaStatus::Death:
		if (m_vecLookDir.x > 0)
			m_pAnimator->Play(L"Monster\\Conga\\CongaDeathR");
		else
			m_pAnimator->Play(L"Monster\\Conga\\CongaDeathL");
		break;
	}
}

void CConga::CongaAddObject()
{
	ADDOBJECT(m_pFarColliderObj);
	ADDOBJECT(m_pNearColliderObj);
}
