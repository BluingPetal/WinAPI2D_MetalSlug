#include "framework.h"
#include "CConga.h"

#include "CImage.h"
#include "CRenderManager.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CGravity.h"

CConga::CConga()
{
	m_pAnimator = nullptr;
	m_strName = L"Conga";
}

CConga::~CConga()
{
}

void CConga::Init()
{
	m_pCongaTotalImgL = RESOURCE->LoadImg(L"Conga", L"Image\\Monster\\Conga.png");
	m_pCongaTotalImgR = RESOURCE->LoadImg(L"CongaR", L"Image\\Monster\\CongaR.png");

	m_pAnimator = new CAnimator;

	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaIdleL", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaIdleR", m_pCongaTotalImgR, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaWalkL", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaWalkR", m_pCongaTotalImgR, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackL1", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackR1", m_pCongaTotalImgR, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackL2", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackR2", m_pCongaTotalImgR, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaDeathL", m_pCongaTotalImgL, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaDeathR", m_pCongaTotalImgR, 0.1f);

	AddComponent(m_pAnimator);

	AddCollider(ColliderType::Rect, Vector(90, 90), Vector(0, 0));

	m_pGravity = new CGravity;
	AddComponent(m_pGravity);
}

void CConga::Update()
{
	if (BUTTONSTAY(VK_LEFT))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackL1");
	if (BUTTONSTAY(VK_RIGHT))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackR1");
	if (BUTTONSTAY(VK_UP))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackL2");
	if (BUTTONSTAY(VK_DOWN))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackR2");
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
