#include "framework.h"
#include "CNPC.h"

#include "CGravity.h"
#include "CAnimator.h"
#include "CItem.h"

CNPC::CNPC()
{
	m_pNPCAni = nullptr;
	m_pGravity = nullptr;
	m_layer = Layer::NPC;
	m_status = NPCStatus::Trapped;
	m_fAccTime = 0;
	m_fSpeed = 50.f;
	m_isFree = false;
	m_isMeet = false;
	m_isGave = false;
	m_strName = L"NPC";
}

CNPC::~CNPC()
{
}

void CNPC::Init()
{
	CImage* m_pNPCImg = new CImage;
	m_pNPCImg = RESOURCE->LoadImg(L"NPC", L"Image\\NPC\\NPCs.png");
	m_pGravity = new CGravity;
	m_pGravity->SetOwner(this);
	AddComponent(m_pGravity);
	
	m_pNPCAni = new CAnimator;
	m_pNPCAni->CreateAnimation(L"NPC\\NPCtrapped", m_pNPCImg, 0.1f);
	m_pNPCAni->CreateAnimation(L"NPC\\NPCfree", m_pNPCImg, 0.2f, false);
	m_pNPCAni->CreateAnimation(L"NPC\\NPCwalkL", m_pNPCImg, 0.3f, false);
	m_pNPCAni->CreateAnimation(L"NPC\\NPCwalkR", m_pNPCImg, 0.3f, false);
	m_pNPCAni->CreateAnimation(L"NPC\\NPCgive", m_pNPCImg, 0.1f, false);
	m_pNPCAni->CreateAnimation(L"NPC\\NPCrun", m_pNPCImg, 0.1f);
	AddCollider(ColliderType::Rect, Vector(50, 80), Vector(0, 0));
	m_pNPCAni->SetOwner(this);
	AddComponent(m_pNPCAni);
}

void CNPC::Update()
{
	m_fAccTime += DT;

	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x)
		DELETEOBJECT(this);
	BehaviorUpdate();
	StateUpdate();
	AniUpdate();
}

void CNPC::Render()
{
}

void CNPC::Release()
{
}

void CNPC::BehaviorUpdate()
{
	switch (m_status)
	{
	case NPCStatus::Trapped:
		break;
	case NPCStatus::Free:
		break;
	case NPCStatus::Walk:
		if (m_fAccTime > 1.8f)
		{
			m_vecPos.x += m_fSpeed * DT;
			if (m_fAccTime > 3.6f)
				m_fAccTime = 0;
		}
		else
		{
			m_vecPos.x -= m_fSpeed * DT;
		}
		break;
	case NPCStatus::Give:
		if (m_fAccTime > 0.5f)
		{
			if (!m_isGave)
			{
				GiveItem();
				m_isGave = true;
			}
		}
		break;
	case NPCStatus::Run:
		m_vecPos.x -= (m_fSpeed + 200) * DT;
		break;
	default:
		break;
	}
}

void CNPC::StateUpdate()
{
	switch (m_status)
	{
	case NPCStatus::Trapped:
		if (m_isFree)
		{
			m_status = NPCStatus::Free;
			m_fAccTime = 0;
		}
		break;
	case NPCStatus::Free:
		if (m_fAccTime > 0.8f)
		{
			m_status = NPCStatus::Walk;
			m_fAccTime = 0;
		}
		break;
	case NPCStatus::Walk:
		if (m_isMeet)
		{
			m_status = NPCStatus::Give;
			m_fAccTime = 0;
		}
		break;
	case NPCStatus::Give:
		if (m_fAccTime > 1.5f)
		{
			m_status = NPCStatus::Run;
			m_fAccTime = 0;
		}
		break;
	case NPCStatus::Run:
		break;
	}
}

void CNPC::AniUpdate()
{
	switch (m_status)
	{
	case NPCStatus::Trapped:
		m_pNPCAni->Play(L"NPC\\NPCtrapped");
		break;
	case NPCStatus::Free:
		m_pNPCAni->Play(L"NPC\\NPCfree");
		break;
	case NPCStatus::Walk:
		if(m_fAccTime < 1.8f)
			m_pNPCAni->Play(L"NPC\\NPCwalkL");
		else
			m_pNPCAni->Play(L"NPC\\NPCwalkR");
		break;
	case NPCStatus::Give:
		m_pNPCAni->Play(L"NPC\\NPCgive");
		break;
	case NPCStatus::Run:
		m_pNPCAni->Play(L"NPC\\NPCrun");
		break;
	}
}

void CNPC::GiveItem()
{
	if (m_strName == L"BombNPC")
	{
		CItem* pItemBomb = new CItem;
		pItemBomb->SetName(L"ItemBomb");
		pItemBomb->SetPos(m_vecPos + Vector(-30, -50));
		pItemBomb->SetExtension(m_fExtension - 0.5);
		ADDOBJECT(pItemBomb);
	}
	else if (m_strName == L"HeavyGunNPC")
	{
		CItem* pItemHeavyGun = new CItem;
		pItemHeavyGun->SetName(L"ItemHeavyGun");
		pItemHeavyGun->SetPos(m_vecPos + Vector(-30, -50));
		pItemHeavyGun->SetExtension(m_fExtension - 0.5);
		ADDOBJECT(pItemHeavyGun);
	}
}

void CNPC::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"PlayerMissile")
	{
		m_isFree = true;
	}
	if (pOtherCollider->GetObjName() == L"Player")
	{
		m_isMeet = true;
	}
}

void CNPC::OnCollisionStay(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"ground" || pOtherCollider->GetObjName() == L"slopeGround")
	{
		m_pGravity->SetVelocity(0);
	}
}

void CNPC::OnCollisionExit(CCollider* pOtherCollider)
{
}
