#include "framework.h"
#include "CBomb.h"

#include "CImage.h"
#include "CAnimator.h"

CBomb::CBomb()
{
	m_vecDir = Vector(0, 0);
	m_fVelocity = 100;

	m_pAnimator = nullptr;
	m_pOwner = nullptr;
}

CBomb::~CBomb()
{
}

void CBomb::Init()
{
	m_pAnimator = new CAnimator;
	CImage* pImg = RESOURCE->LoadImg(L"PlayerBomb", L"Image\\Item\\Weapon.png");
	//m_pAnimator->CreateAnimation(L"PlayerBomb", )
}

void CBomb::Update()
{
}

void CBomb::Render()
{
}

void CBomb::Release()
{
}

void CBomb::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CBomb::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBomb::OnCollisionExit(CCollider* pOtherCollider)
{
}
