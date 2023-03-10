#include "framework.h"
#include "CMonster.h"

#include "CRenderManager.h"
#include "CCollider.h"
#include "CAnimator.h"

CMonster::CMonster()
{
	m_vecScale = Vector(100, 100);
	m_layer = Layer::Monster;
	m_vecMoveDir = Vector(0, 0);
	m_vecLookDir = Vector(-1, 0);
	m_fSpeed = 200.f;
	m_hp = 10;
	m_pGravity = nullptr;
}

CMonster::~CMonster()
{
}

void CMonster::Init()
{
}

void CMonster::Update()
{
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
		//Logger::Debug(L"���Ͱ� �÷��̾�� �浹����");
	}
	else if (pOtherCollider->GetObjName() == L"�̻���")
	{
		//Logger::Debug(L"���Ͱ� �̻��ϰ� �浹����");
	}
}

void CMonster::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CMonster::OnCollisionExit(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"�÷��̾�")
	{
		//Logger::Debug(L"���Ͱ� �÷��̾�� �浹����");
	}
	else if (pOtherCollider->GetObjName() == L"�̻���")
	{
		//Logger::Debug(L"���Ͱ� �̻��ϰ� �浹����");
	}
}
