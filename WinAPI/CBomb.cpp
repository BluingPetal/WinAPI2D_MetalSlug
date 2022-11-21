#include "framework.h"
#include "CBomb.h"

#include "CImage.h"
#include "CAnimator.h"
#include "CGravity.h"

CBomb::CBomb()
{
	m_vecDir = Vector(0, 0);
	m_fVelocity = 100;
	count = 0;
	m_pGravity = nullptr;
	m_strName = L"Bomb";
	m_layer = Layer::PlayerMissile;

	m_pAnimator = nullptr;
	m_pOwner = nullptr;
}

CBomb::~CBomb()
{
}

CGameObject* CBomb::GetOwner()
{
	return m_pOwner;
}

void CBomb::Init()
{
	m_pAnimator = new CAnimator;
	CImage* pImg = RESOURCE->LoadImg(L"PlayerBomb", L"Image\\Player\\Weapon.png");
	m_pAnimator->CreateAnimation(L"Item\\Bomb", pImg, 0.05f);
	AddComponent(m_pAnimator);

	m_pGravity = new CGravity;
	m_pGravity->SetVelocity(-400);
	m_pGravity->SetGravity(800);
	AddComponent(m_pGravity);

	AddCollider(ColliderType::Rect, Vector(50, 50), Vector(0, 0));
}

void CBomb::Update()
{
	m_pAnimator->Play(L"Item\\Bomb");
	m_vecPos.x +=  m_vecDir.x *300.f * DT;
}

void CBomb::Render()
{
}

void CBomb::Release()
{
}

void CBomb::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"ground" || pOtherCollider->GetObjName() == L"slopeGround")
	{
		count++;
		if (count <= 1)
		{
			m_pGravity->SetVelocity(-300);
		}
		else
		{
			if (!this->GetReserveDelete())
				DELETEOBJECT(this);
		}
	}
	else if (pOtherCollider->GetObjName() == L"Conga")
	{
		if(!pOtherCollider->GetOwner()->GetReserveDelete())
			DELETEOBJECT(this);
	}
}

void CBomb::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBomb::OnCollisionExit(CCollider* pOtherCollider)
{
}
