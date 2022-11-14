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

CMissile::CMissile()
{
	m_vecScale = Vector(10, 10);
	m_vecDir = Vector(0, 0);
	m_fVelocity = 300;
	m_layer = Layer::Missile;
	m_strName = L"Missile";
	m_pOwner = nullptr;
	m_pAnimator = nullptr;
	m_fAccTime = 0;
	m_bIsEntered = false;
}

CMissile::~CMissile()
{
}

void CMissile::Init()
{
	AddCollider(ColliderType::Circle, Vector(8, 8), Vector(0, 0));
	CImage* pPlayerMissileImg = RESOURCE->LoadImg(L"PlayerMissile", L"Image\\Player\\EriIdle.png"); // 미사일 이미지 넣기
	CImage* pPlayerHitCongaImgL = RESOURCE->LoadImg(L"CongaHurtL", L"Image\\Monster\\CongaL.png");
	CImage* pPlayerHitCongaImgR = RESOURCE->LoadImg(L"CongaHurtR", L"Image\\Monster\\CongaR.png"); 
	//m_pAnimator->CreateAnimation(L"PlayerMissile", pPlayerMissileImg, 0.1f, false);
	m_pAnimator = new CAnimator;
	m_pAnimator->CreateAnimation(L"Missile\\CongaHurtL", pPlayerHitCongaImgL, 0.05f, false);
	m_pAnimator->CreateAnimation(L"Missile\\CongaHurtR", pPlayerHitCongaImgR, 0.05f, false);
	AddComponent(m_pAnimator);
}

void CMissile::Update()
{
	if(!m_bIsEntered)
		m_vecPos += m_vecDir * m_fVelocity * DT;

	// 화면밖으로 나갈경우 삭제
	Vector cameraLookAt = CAMERA->GetLookAt();
	/*
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(cameraLookAt.x - WINSIZEX * 0.5f, cameraLookAt.y - WINSIZEY * 0.5f)).x ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(cameraLookAt.x + WINSIZEX * 0.5f, cameraLookAt.y + WINSIZEY * 0.5f)).x ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(cameraLookAt.x - WINSIZEX * 0.5f, cameraLookAt.y - WINSIZEY * 0.5f)).y ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(cameraLookAt.x + WINSIZEX * 0.5f, cameraLookAt.y + WINSIZEY * 0.5f)).y)
		DELETEOBJECT(this);
	*/
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).x ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(0, 0)).y ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).y)
		DELETEOBJECT(this);
}

void CMissile::Render()
{
	RENDER->FrameCircle(
		m_vecPos.x,
		m_vecPos.y,
		m_vecScale.x);
}

void CMissile::Release()
{
}

void CMissile::OnCollisionEnter(CCollider* pOtherCollider)
{
	m_bIsEntered = true;
	m_fAccTime = 0;

	// Player 미사일에 Conga가 맞았을 경우 Conga 애니메이션 실행, Conga Hp 감소
	if (m_pOwner->GetName() == L"Player")
	{
		if (pOtherCollider->GetObjName() == L"Conga")
		{
			CConga* pOtherObj = dynamic_cast<CConga*>(pOtherCollider->GetOwner());

			if (pOtherObj->GetCongaState() != CongaStatus::Death && !pOtherObj->GetReserveDelete())
			{
				pOtherObj->SetHp(pOtherObj->GetHp() - 1);
				if (m_vecDir.x > 0)
					m_pAnimator->Play(L"Missile\\CongaHurtL");
				else
					m_pAnimator->Play(L"Missile\\CongaHurtR");
			}
			else if (pOtherObj->GetCongaState() == CongaStatus::Death)
				m_bIsEntered = false;
		}
	}
}

void CMissile::OnCollisionStay(CCollider* pOtherCollider)
{
	m_fAccTime += DT;

	// Player 미사일에 Conga가 맞았을 경우 미사일 없어지는 시간만큼 대기
	if (m_pOwner->GetName() == L"Player")
	{
		if (pOtherCollider->GetObjName() == L"Conga")
		{
			if (m_fAccTime >= 0.05f * 9)
			{
				DELETEOBJECT(this);
				m_fAccTime = 0;
			}
		}
	}
}

void CMissile::OnCollisionExit(CCollider* pOtherCollider)
{
	m_bIsEntered = false;
}

void CMissile::SetDir(Vector dir)
{
	m_vecDir = dir.Normalized();
}

void CMissile::SetVelocity(float velocity)
{
	m_fVelocity = velocity;
}
