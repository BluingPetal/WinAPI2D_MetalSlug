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
}

void CMissile::Update()
{
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
}

void CMissile::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CMissile::OnCollisionExit(CCollider* pOtherCollider)
{
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
