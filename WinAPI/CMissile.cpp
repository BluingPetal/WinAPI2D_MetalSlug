#include "framework.h"
#include "CMissile.h"

#include "CRenderManager.h"
#include "CTimeManager.h"
#include "CEventManager.h"
#include "CCollider.h"
#include "CColliderObject.h"
#include "CAnimator.h"
#include "CImage.h"
#include "CConga.h"
#include "CGravity.h"
#include "CPlayer.h"
#include "CAnimation.h"

CMissile::CMissile()
{
	m_vecScale = Vector(10, 10);
	m_vecDir = Vector(0, 0);
	m_fVelocity = 500;
	m_layer = Layer::Missile;
	m_strName = L"Missile";
	m_pOwner = nullptr;
	m_pAnimator = nullptr;
	m_fDisappearAccTime = 0;
	m_fAttackAccTime = 0;
	m_bIsEntered = false;
}

CMissile::~CMissile()
{
}

void CMissile::Init()
{
	if(m_strName == L"PlayerMissile")
		AddCollider(ColliderType::Rect, Vector(8, 8), Vector(0, 0));
	else if (m_strName == L"BossFireMissile")
		AddCollider(ColliderType::Rect, Vector(40, 40), Vector(0, 0));
	else if (m_strName == L"BossMissile")
		AddCollider(ColliderType::Rect, Vector(40, 40), Vector(0, 0));

	// Animator 정의
	m_pAnimator = new CAnimator;
	if (m_strName == L"PlayerMissile")
	{
		CImage* pPlayerMissileImg = RESOURCE->LoadImg(L"PlayerMissile", L"Image\\Player\\EriIdle.png"); // 미사일 이미지 넣기
		CImage* pPlayerHitCongaImgL = RESOURCE->LoadImg(L"CongaHurtL", L"Image\\Monster\\CongaL.png");
		CImage* pPlayerHitCongaImgR = RESOURCE->LoadImg(L"CongaHurtR", L"Image\\Monster\\CongaR.png");
		m_pAnimator->CreateAnimation(L"Missile\\CongaHurtL", pPlayerHitCongaImgL, 0.05f, false);
		m_pAnimator->CreateAnimation(L"Missile\\CongaHurtR", pPlayerHitCongaImgR, 0.05f, false);
	}
	else if (m_strName == L"BossFireMissile")
	{
		CImage* pBossFireMissile = RESOURCE->LoadImg(L"BossFireMissile", L"Image\\Boss\\BossAttack.png");
		m_pAnimator->CreateAnimation(L"Boss\\BossFireMissile", pBossFireMissile, 0.1f, false);
		m_pGravity = new CGravity;
		AddComponent(m_pGravity);
		m_pGravity->SetVelocity(10);
	}
	else if (m_strName == L"BossMissile")
	{
		CImage* pBossMissile = RESOURCE->LoadImg(L"BossMissile", L"Image\\Boss\\BossAttack.png");
		m_pAnimator->CreateAnimation(L"Boss\\BossMissile", pBossMissile, 0.1f, false);
		m_pGravity = new CGravity;
		AddComponent(m_pGravity);
		m_pGravity->SetVelocity(-10);
		m_pGravity->SetGravity(1500);
	}
	//m_pAnimator->CreateAnimation(L"PlayerMissile", pPlayerMissileImg, 0.1f, false);
	AddComponent(m_pAnimator);
}

void CMissile::Update()
{
	if(!m_bIsEntered)
		m_vecPos += m_vecDir * m_fVelocity * DT;

	// 화면밖으로 나갈경우 삭제
	Vector cameraLookAt = CAMERA->GetLookAt();
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).x ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(0, 0)).y ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).y)
		DELETEOBJECT(this);




	if (m_strName == L"BossFireMissile")
	{
		m_fAttackAccTime += DT;

		if (m_fAttackAccTime <= 0.4f)
		{
			m_pAnimator->Play(L"Boss\\BossFireMissile", true);
			m_fAttackAccTime = 0;
		}
	}
	if (m_strName == L"BossMissile")
	{
		m_fAttackAccTime += DT;

		m_pAnimator->ReversePlay(L"Boss\\BossMissile");
		m_fAttackAccTime = 0;
	}
}

void CMissile::Render()
{
	//RENDER->FrameCircle(
	//	m_vecPos.x,
	//	m_vecPos.y,
	//	m_vecScale.x);
	RENDER->FrameRect(
		m_vecPos.x - m_vecScale.x * 0.5f,
		m_vecPos.y - m_vecScale.y * 0.5f,
		m_vecPos.x + m_vecScale.x * 0.5f,
		m_vecPos.y + m_vecScale.y * 0.5f);
}

void CMissile::Release()
{
}

void CMissile::OnCollisionEnter(CCollider* pOtherCollider)
{

	// Player 미사일에 Conga가 맞았을 경우 Conga 애니메이션 실행, Conga Hp 감소
	if (m_pOwner->GetName() == L"Player")
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
	}

	if (m_strName == L"BossFireMissile")
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
	if (m_strName == L"BossMissile")
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

void CMissile::OnCollisionStay(CCollider* pOtherCollider)
{

	// Player 미사일에 Conga가 맞았을 경우 미사일 없어지는 시간만큼 대기
	if (m_pOwner->GetName() == L"Player")
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
}

void CMissile::OnCollisionExit(CCollider* pOtherCollider)
{
	if (m_pOwner->GetName() == L"Player")
	{
		if (pOtherCollider->GetObjName() == L"Conga")
		{
			m_bIsEntered = false;
		}
	}
}

void CMissile::SetDir(Vector dir)
{
	m_vecDir = dir.Normalized();
}

void CMissile::SetVelocity(float velocity)
{
	m_fVelocity = velocity;
}

CAnimator* CMissile::GetAnimator()
{
	return m_pAnimator;
}
