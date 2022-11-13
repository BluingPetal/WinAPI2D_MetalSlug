#include "framework.h"
#include "CColliderObject.h"

#include "CCollider.h"
#include "CConga.h"

CColliderObject::CColliderObject()
{
    m_strName = L"ColliderObj";
    m_layer = Layer::Collider;
    m_pCollider = new CCollider;
    m_vecOffset = Vector(0, 0);
    m_pOwner = nullptr;
}

CColliderObject::~CColliderObject()
{
}

CCollider* CColliderObject::GetCollider()
{
    return m_pCollider;
}

CGameObject* CColliderObject::GetColliderObjOwner()
{
    return m_pOwner;
}

void CColliderObject::Init()
{
    m_pCollider->SetPos(m_vecPos + m_vecOffset * m_fExtension);
    m_pCollider->SetScale(m_vecScale);
    AddCollider(ColliderType::Rect, m_vecScale * m_fExtension, m_vecOffset * m_fExtension);
}

void CColliderObject::Update()
{
}

void CColliderObject::Render()
{
}

void CColliderObject::Release()
{
}

void CColliderObject::OnCollisionEnter(CCollider* pOtherCollider)
{
    if (m_strName == L"FarColliderObj")
    {
        if (pOtherCollider->GetObjName() == L"Player")
            dynamic_cast<CConga*>(m_pOwner)->SetTarget(pOtherCollider->GetOwner());
    }
    if (m_strName == L"NearColliderObj")
    {
        if (pOtherCollider->GetObjName() == L"Player")
            dynamic_cast<CConga*>(m_pOwner)->SetCongaState(CongaStatus::NearAttack);
    }
}

void CColliderObject::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CColliderObject::OnCollisionExit(CCollider* pOtherCollider)
{
    if (m_strName == L"FarColliderObj")
    {
        if (pOtherCollider->GetObjName() == L"Player")
            dynamic_cast<CConga*>(m_pOwner)->SetTarget(nullptr);
    }
    if (m_strName == L"NearColliderObj")
    {
        if (pOtherCollider->GetObjName() == L"Player")
            dynamic_cast<CConga*>(m_pOwner)->SetCongaState(CongaStatus::Walk);
    }
}
