#include "framework.h"
#include "CMonster.h"

#include "CRenderManager.h"
#include "CCollider.h"
#include "CAnimator.h"

CMonster::CMonster()
{
	m_vecScale = Vector(100, 100);
	m_layer = Layer::Monster;
}

CMonster::~CMonster()
{
}

void CMonster::Init()
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
}

void CMonster::Update()
{
	if(BUTTONSTAY(VK_LEFT))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackL1");
	if (BUTTONSTAY(VK_RIGHT))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackR1");
	if (BUTTONSTAY(VK_UP))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackL2");
	if (BUTTONSTAY(VK_DOWN))
		m_pAnimator->Play(L"Monster\\Conga\\CongaAttackR2");
}

void CMonster::Render()
{
}

void CMonster::Release()
{
}

void CMonster::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"�÷��̾�")
	{
		Logger::Debug(L"���Ͱ� �÷��̾�� �浹����");
	}
	else if (pOtherCollider->GetObjName() == L"�̻���")
	{
		Logger::Debug(L"���Ͱ� �̻��ϰ� �浹����");
	}
}

void CMonster::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CMonster::OnCollisionExit(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"�÷��̾�")
	{
		Logger::Debug(L"���Ͱ� �÷��̾�� �浹����");
	}
	else if (pOtherCollider->GetObjName() == L"�̻���")
	{
		Logger::Debug(L"���Ͱ� �̻��ϰ� �浹����");
	}
}
