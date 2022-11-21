#include "framework.h"
#include "CBomb.h"

#include "CImage.h"
#include "CAnimator.h"
#include "CAniObject.h"
#include "CGravity.h"
#include "CPlayer.h"
#include "CConga.h"
#include "CNPC.h"

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
	m_reserveDelete = false;
	m_bCreatedAni = false;
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

	m_pEffectImage = new CImage;
	m_pEffectImage = RESOURCE->LoadImg(L"BombEffectImg", L"Image\\Effect\\Explosion.png");

	AddCollider(ColliderType::Rect, Vector(50, 50), Vector(0, 0));
}

void CBomb::Update()
{
	m_pAnimator->Play(L"Item\\Bomb");
	m_vecPos.x +=  m_vecDir.x *300.f * DT;

	if (m_reserveDelete && !m_bCreatedAni)
	{
		CPlayer* owner = dynamic_cast<CPlayer*>(m_pOwner);
		m_pAnimator->GetCurAni()->SetAlpha(0);
		m_pAnimator->Stop();
		m_bCreatedAni = true;
		m_pBombAniObj = new CAniObject;
		m_pBombAniObj->SetImage(m_pEffectImage);
		m_pBombAniObj->GetAnimator()->CreateAnimation(L"Effect\\Bomb", m_pEffectImage, 0.05f, false);
		m_pBombAniObj->GetAnimator()->Play(L"Effect\\Bomb");
		m_pBombAniObj->SetPos(m_vecPos + Vector(0, -100));
		m_pBombAniObj->SetExtension(m_fExtension);
		m_pBombAniObj->SetLayer(Layer::Effect);
		ADDOBJECT(m_pBombAniObj);
	}

	// 화면밖으로 나갈경우 삭제
	Vector cameraLookAt = CAMERA->GetLookAt();
	if (m_vecPos.x < CAMERA->ScreenToWorldPoint(Vector(0, 0)).x - 100 ||
		m_vecPos.x > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).x + 100 ||
		m_vecPos.y < CAMERA->ScreenToWorldPoint(Vector(0, 0)).y - 100 ||
		m_vecPos.y > CAMERA->ScreenToWorldPoint(Vector(WINSIZEX, WINSIZEY)).y + 100)
		m_reserveDelete = true;

	if (m_reserveDelete && !this->GetReserveDelete())
	{
		m_fDisappearAccTime += DT;
		if (m_fDisappearAccTime > 1.3f)
		{
			RemoveCollider();
			DELETEOBJECT(this);
			DELETEOBJECT(m_pBombAniObj);
		}
	}
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
			m_fVelocity = 0;
			m_pGravity->SetVelocity(0);
			m_reserveDelete = true;
		}
	}
	if (pOtherCollider->GetObjName() == L"Conga")
	{
		CConga* pOtherObj = dynamic_cast<CConga*>(pOtherCollider->GetOwner());
		if (pOtherObj->GetCongaState() != CongaStatus::Death)
		{
			m_fVelocity = 0;
			m_reserveDelete = true;
		}
	}
	if (pOtherCollider->GetObjName() == L"Boss" || pOtherCollider->GetObjName() == L"obstacleCastle")
	{
		m_fVelocity = 0;
		m_reserveDelete = true;
	}
	if (pOtherCollider->GetObjName() == L"BombNPC" || pOtherCollider->GetObjName() == L"HeavyGunNPC")
	{
		CNPC* pOtherObj = dynamic_cast<CNPC*>(pOtherCollider->GetOwner());
		if (pOtherObj->GetStatus() == NPCStatus::Trapped)
		{
			m_fVelocity = 0;
			m_reserveDelete = true;
		}
	}
}

void CBomb::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBomb::OnCollisionExit(CCollider* pOtherCollider)
{
}
