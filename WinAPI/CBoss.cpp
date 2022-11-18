#include "framework.h"
#include "CBoss.h"

#include "CImage.h"

CBoss::CBoss()
{
	m_pAnimator = nullptr;
}

CBoss::~CBoss()
{
}

void CBoss::Init()
{
	m_vecPos = Vector(WINSIZEX * 0.5f, WINSIZEY * 0.5f);
	CImage* bossImg = RESOURCE->LoadImg(L"Boss", L"Image\\Boss\\Boss.png");
	m_pAnimator = new CAnimator;
	m_pAnimator->CreateAnimation(L"Boss\\Boss", bossImg, 0.1f);
	m_pAnimator->Play(L"Boss\\Boss");
	AddComponent(m_pAnimator);
}

void CBoss::Update()
{
}

void CBoss::Render()
{
}

void CBoss::Release()
{
}

void CBoss::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CBoss::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBoss::OnCollisionExit(CCollider* pOtherCollider)
{
}
