#include "framework.h"
#include "CColliderObject.h"

#include "CCollider.h"

CColliderObject::CColliderObject()
{
    m_strName = L"ColliderObj";
    m_layer = Layer::Collider;
    m_pCollider = new CCollider;
    m_vecOffset = Vector(0, 0);
}

CColliderObject::~CColliderObject()
{
}

CCollider* CColliderObject::GetCollider()
{
    return m_pCollider;
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
