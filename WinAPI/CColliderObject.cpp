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
        CConga* pOwner = dynamic_cast<CConga*>(m_pOwner);
        if (pOtherCollider->GetObjName() == L"Player" && (pOwner->GetCongaState() !=CongaStatus::Death))
            pOwner->SetTarget(pOtherCollider->GetOwner());
    }
    if (m_strName == L"NearColliderObj")
    {
        CConga* pOwner = dynamic_cast<CConga*>(m_pOwner);
        if (pOtherCollider->GetObjName() == L"Player" && (pOwner->GetCongaState() != CongaStatus::Death))
            pOwner->SetCongaState(CongaStatus::NearAttack);
    }
}

void CColliderObject::OnCollisionStay(CCollider* pOtherCollider)
{
    //if (m_strName == L"NearColliderObj")
    //{
    //    if (pOtherCollider->GetObjName() == L"Player")
    //        if (dynamic_cast<CConga*>(m_pOwner)->GetAccTime() > 1.3f)
    //        {
    //            dynamic_cast<CConga*>(m_pOwner)->SetAccTime(0);
    //            dynamic_cast<CConga*>(m_pOwner)->SetCongaState(CongaStatus::Idle);
    //        }
    //}
}

void CColliderObject::OnCollisionExit(CCollider* pOtherCollider)
{
    if (m_strName == L"FarColliderObj")
    {
        CConga* pOwner = dynamic_cast<CConga*>(m_pOwner);
        if (pOtherCollider->GetObjName() == L"Player" && (pOwner->GetCongaState() != CongaStatus::Death))
            pOwner->SetTarget(nullptr);
    }
    if (m_strName == L"NearColliderObj")
    {
        CConga* pOwner = dynamic_cast<CConga*>(m_pOwner);
        if (pOtherCollider->GetObjName() == L"Player" && (pOwner->GetCongaState() != CongaStatus::Death))
            pOwner->SetCongaState(CongaStatus::Walk);
    }
}
