#include "framework.h"
#include "CItem.h"

#include "CImage.h"
#include "CAnimator.h"
#include "CGravity.h"

CItem::CItem()
{
	m_strName = L"Item";
	m_layer = Layer::Item;
	m_pItemAni = nullptr;
	m_pGravity = nullptr;
}

CItem::~CItem()
{
}

void CItem::Init()
{
	CImage* m_pItemImg = new CImage;
	m_pItemImg = RESOURCE->LoadImg(L"Item", L"Image\\Player\\Weapon.png");
	//m_pItemObj = new CImageObject;
	//m_pItemObj->SetImage(m_pItemImg);
	//m_pItemObj->SetExtension(m_fExtension);
	//m_pItemObj->SetLayer(m_layer);
	//m_pItemObj->SetName(m_strName);
	//m_pItemObj->SetPos(m_vecPos);
	//m_pItemObj->SetRenderAsFrame(m_bRenderAsFrame);
	//if(m_bRenderAsFrame)
	//	m_pItemObj->SetSourceInfo(m_vecSourceInfo[0], m_vecSourceInfo[1], m_vecSourceInfo[2], m_vecSourceInfo[3]);
	
	m_pGravity = new CGravity;
	m_pGravity->SetOwner(this);
	AddComponent(m_pGravity);

	m_pItemAni = new CAnimator;
	if (m_strName == L"ItemHeavyGun")
	{
		m_pItemAni->CreateAnimation(L"Item\\ItemHeavyGun", m_pItemImg, 0.1f);
		AddCollider(ColliderType::Rect, Vector(50, 50), Vector(0, 0));
	}
	else if (m_strName == L"ItemBomb")
	{
		m_pItemAni->CreateAnimation(L"Item\\ItemBomb", m_pItemImg, 0.1f);
		AddCollider(ColliderType::Rect, Vector(50, 50), Vector(0, 0));
	}
	m_pItemAni->SetOwner(this);
	AddComponent(m_pItemAni);
}

void CItem::Update()
{
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x)
		DELETEOBJECT(this);
	if (m_strName == L"ItemHeavyGun")
	{
		m_pItemAni->Play(L"Item\\ItemHeavyGun");
	}
	else if (m_strName == L"ItemBomb")
	{
		m_pItemAni->Play(L"Item\\ItemBomb");
	}
}

void CItem::Render()
{
}

void CItem::Release()
{
}

void CItem::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CItem::OnCollisionStay(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"Player")
		DELETEOBJECT(this);
	if (pOtherCollider->GetObjName() == L"slopeGround" || pOtherCollider->GetObjName() == L"ground")
		m_pGravity->SetVelocity(0);
}

void CItem::OnCollisionExit(CCollider* pOtherCollider)
{
}
