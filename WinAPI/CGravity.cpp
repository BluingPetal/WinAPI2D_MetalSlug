#include "framework.h"
#include "CGravity.h"

#include "CPlayer.h"

CGravity::CGravity()
{
	m_fVelY = 0;
	m_fGravity = 550;
}

CGravity::~CGravity()
{
}

void CGravity::Init()
{ 
}

void CGravity::Update()
{
}

void CGravity::PhysicsUpdate()
{
	CGameObject* pOwner = GetOwner();

	if (pOwner->GetName() == L"Player")
	{
		if (dynamic_cast<CPlayer*>(pOwner)->GetIsJump())
		{
			Vector vecOwnerPos;
			vecOwnerPos.x = pOwner->GetPos().x;
			vecOwnerPos.y = pOwner->GetPos().y + m_fVelY * DT;
			pOwner->SetPos(vecOwnerPos);
			m_fVelY += m_fGravity * DT;
			if (m_fVelY > 1000)
				m_fVelY = 1000;
		}
	}
	if (pOwner->GetName() == L"Conga")
	{
		Vector vecOwnerPos;
		vecOwnerPos.x = pOwner->GetPos().x;
		vecOwnerPos.y = pOwner->GetPos().y + m_fVelY * DT;
		pOwner->SetPos(vecOwnerPos);
		m_fVelY += m_fGravity * DT;
		if (m_fVelY > 1000)
			m_fVelY = 1000;
	}
}

void CGravity::Render()
{
}

void CGravity::Release()
{
}
