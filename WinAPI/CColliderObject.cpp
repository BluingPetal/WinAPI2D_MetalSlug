#include "framework.h"
#include "CColliderObject.h"

#include "CCollider.h"
#include "CConga.h"
#include "CPlayer.h"
#include "CAniObject.h"
#include "CSceneStage01.h"

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
        if (pOtherCollider->GetObjName() == L"Player" && (pOwner->GetCongaState() != CongaStatus::Death))
        {
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
            if(!pPlayer->GetIsDead())
                pOwner->SetTarget(pOtherCollider->GetOwner());
        }
    }
    if (m_strName == L"NearColliderObj")
    {
        CConga* pOwner = dynamic_cast<CConga*>(m_pOwner);
        if (pOtherCollider->GetObjName() == L"Player" && (pOwner->GetCongaState() != CongaStatus::Death))
        {
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
            if (!pPlayer->GetIsDead())
            {
               // pOwner->SetAccTime(0);
                pOwner->SetCongaState(CongaStatus::NearAttack);
            }
        }
    }
    //if (m_strName == L"frontOcean")
    //{
    //    if (pOtherCollider->GetObjName() == L"Player")
    //    {
    //        CAniObject* pWaterAniObj = new CAniObject;
    //        CImage* pWaterAniImg = RESOURCE->LoadImg(L"WaterAni", L"Image\\BackGround\\WaterAni.png");
    //        Vector waterAniPos = pOtherCollider->GetOwner()->GetPos();
    //        pWaterAniObj->SetImage(pWaterAniImg);
    //        pWaterAniObj->SetPos(waterAniPos + Vector(0, 30));
    //        pWaterAniObj->SetExtension(m_fExtension -2);
    //        pWaterAniObj->GetAnimator()->CreateAnimation(L"BackGround\\WaterAni", pWaterAniImg, 0.1f);
    //        pWaterAniObj->GetAnimator()->Play(L"BackGround\\WaterAni");
    //        pWaterAniObj->SetLayer(Layer::ForeGround);
    //        ADDOBJECT(pWaterAniObj);
    //    }
    //}
    if (m_strName == L"frontOcean")
    {
        if (pOtherCollider->GetObjName() == L"Player")
        {
            // TODO : 바다 나갔을 때 위치 계산해서 예외처리해주기
            CAniObject* aniObj = dynamic_cast<CSceneStage01*>(SCENE->GetCurScene())->GetWaterAni();
            aniObj->SetAlpha(1);
        }
    }
}

void CColliderObject::OnCollisionStay(CCollider* pOtherCollider)
{
    /* 근접 공격 추가 시 실행
    if (m_strName == L"NearColliderObj")
    {
        if (pOtherCollider->GetObjName() == L"Player")
        {
            Logger::Debug(L"here");
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOtherCollider->GetOwner());
            pPlayer->SetCurWeapon(PlayerWeapon::Knife);
        }
    }
    */
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
        {
            pOwner->SetAccTime(0);
            pOwner->SetCongaState(CongaStatus::Walk);
        }
    }
    if (m_strName == L"frontOcean")
    {
        if (pOtherCollider->GetObjName() == L"Player")
        {
            CAniObject* aniObj = dynamic_cast<CSceneStage01*>(SCENE->GetCurScene())->GetWaterAni();
            aniObj->SetAlpha(0);
        }
    }
}
