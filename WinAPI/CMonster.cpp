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
	m_pCongaTotalImg = RESOURCE->LoadImg(L"Conga", L"Image\\Monster\\Conga.png");

	m_pAnimator = new CAnimator;

	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaIdleL", m_pCongaTotalImg, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaWalkL", m_pCongaTotalImg, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackL1", m_pCongaTotalImg, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaAttackL2", m_pCongaTotalImg, 0.1f);
	m_pAnimator->CreateAnimation(L"Monster\\Conga\\CongaDeathL", m_pCongaTotalImg, 0.1f);

	AddComponent(m_pAnimator);

	AddCollider(ColliderType::Rect, Vector(90, 90), Vector(0, 0));
}

void CMonster::Update()
{
	m_pAnimator->Play(L"Monster\\Conga\\CongaDeathL");
}

void CMonster::Render()
{
}

void CMonster::Release()
{
}

void CMonster::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"플레이어")
	{
		Logger::Debug(L"몬스터가 플레이어와 충돌진입");
	}
	else if (pOtherCollider->GetObjName() == L"미사일")
	{
		Logger::Debug(L"몬스터가 미사일과 충돌진입");
	}
}

void CMonster::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CMonster::OnCollisionExit(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"플레이어")
	{
		Logger::Debug(L"몬스터가 플레이어와 충돌해제");
	}
	else if (pOtherCollider->GetObjName() == L"미사일")
	{
		Logger::Debug(L"몬스터가 미사일과 충돌해제");
	}
}
