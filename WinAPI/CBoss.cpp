#include "framework.h"
#include "CBoss.h"

#include "CImage.h"
#include "CMissile.h"
#include "CAniObject.h"

CBoss::CBoss()
{
	m_pAnimator = nullptr;
	m_strName = L"Boss";
	m_fSpeed = 200;
	m_vecMoveDir = Vector(1, 0);
	m_fAccTime = 0;
	m_fAttackAccTime = 0;
	m_hp = 200;
	m_bossStatus = BossStatus::Idle;
	m_bIsAttack = false;
	m_fDisappearAccTime = 0;
}

CBoss::~CBoss()
{
}

void CBoss::Init()
{
	CImage* bossImg = RESOURCE->LoadImg(L"Boss", L"Image\\Boss\\Boss.png");
	m_pAnimator = new CAnimator;
	m_pAnimator->CreateAnimation(L"Boss\\BossWalk", bossImg, 0.1f);
	m_pAnimator->CreateAnimation(L"Boss\\BossFire", bossImg, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Boss\\BossCannon", bossImg, 0.1f, false); // cannon 나올때
	m_pAnimator->CreateAnimation(L"Boss\\BossWithCannonFire", bossImg, 0.1f, false);
	m_pAnimator->CreateAnimation(L"Boss\\BossWithCannon", bossImg, 0.1f);
	m_pAnimator->CreateAnimation(L"Boss\\BossDestroyed", bossImg, 0.1f);
	m_pAnimator->Play(L"Boss\\BossWalk");
	AddComponent(m_pAnimator);

	m_pEffectImg = RESOURCE->LoadImg(L"Effect", L"Image\\Effect\\Explosion.png");
	
	//m_pFireMissileAni = new CAnimator;
	//m_pMissileAni = new CAnimator;

	Vector size = m_pAnimator->GetFirstAniFrame().slice;
	m_vecScale = size * m_fExtension;
	m_vecPos = Vector(m_vecScale.x * 0.5f - 80, WINSIZEY + m_vecScale.y * 0.5f);

	AddCollider(ColliderType::Rect, m_vecScale * 0.5f + Vector(100, 0), Vector(-20, 180));
}

void CBoss::Update()
{
	BehaviorUpdate();
	StateUpdate();
	AniUpdate();

	if (pFireMissile != nullptr && !pFireMissile->GetSafeToDelete())
	{
		if (pFireMissile->GetReserveDelete())
		{
			// 폭발 애니메이터 생성뒤 지움
			m_pFireMissileAniObj = new CAniObject;
			m_pFireMissileAniObj->SetImage(m_pEffectImg);
			m_pFireMissileAniObj->SetPos(pFireMissile->GetPos() + Vector(0, -200));
			m_pFireMissileAniObj->SetExtension(m_fExtension + 1.5);
			m_pFireMissileAniObj->SetLayer(Layer::Unit);
			ADDOBJECT(m_pFireMissileAniObj);
			m_pFireMissileAniObj->GetAnimator()->CreateAnimation(L"Effect\\Bomb", m_pEffectImg, 0.05f, false);
			m_pFireMissileAniObj->GetAnimator()->Play(L"Effect\\Bomb");
		}
	}
	//else
	//{
		if (m_pFireMissileAniObj != nullptr && !m_pFireMissileAniObj->GetReserveDelete())
		{
			m_fDisappearAccTime += DT;
			if (m_fDisappearAccTime >= 1.3f)
			{
				DELETEOBJECT(m_pFireMissileAniObj);
				m_fDisappearAccTime = 0;
			}
		}
	//}
	//if (pMissile != nullptr && pMissile->GetReserveDelete())
	//{
	//
	//}

}

void CBoss::Render()
{
}

void CBoss::Release()
{
}

void CBoss::BehaviorUpdate()
{
	switch (m_bossStatus)
	{
	case BossStatus::Idle:
		break;
	case BossStatus::Walk:
		m_vecPos += m_vecMoveDir * m_fSpeed * DT;
		break;
	case BossStatus::FastWalk:
		if(m_fAccTime > 3.0f)
			m_vecPos += m_vecMoveDir * (m_fSpeed + 300) * DT;
		break;
	case BossStatus::Fire:
		m_vecPos += m_vecMoveDir * m_fSpeed * DT;
		CreateFireMissile();
		break;
	case BossStatus::DeployCannon:
		m_vecPos += m_vecMoveDir * m_fSpeed * DT;
		break;
	case BossStatus::CannonWalk:
		m_vecPos += m_vecMoveDir * m_fSpeed * DT;
		break;
	case BossStatus::CannonFastWalk:
		if (m_fAccTime > 3.0f)
			m_vecPos += m_vecMoveDir * (m_fSpeed + 300) * DT;
		break;
	case BossStatus::CannonFire:
		m_vecPos += m_vecMoveDir * m_fSpeed * DT;
		break;
	case BossStatus::Destroyed:
		break;
	}
}

void CBoss::StateUpdate()
{
	m_fAccTime += DT;

	switch (m_bossStatus)
	{
	case BossStatus::Idle:
		if (m_fAccTime > 7.f)
		{
			m_bossStatus = BossStatus::Walk;
			m_fAccTime = 0;
		}
		break;
	case BossStatus::Walk:
		if (m_hp < 100)
		{
			m_bossStatus = BossStatus::DeployCannon;
			m_fAccTime = 0;
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			srand((unsigned)time(NULL));
			int ratio = rand() % 5;
			if (ratio == 0)
			{
				m_bossStatus = BossStatus::FastWalk;
				m_fAccTime = 0;
			}
			else
			{
				m_bossStatus = BossStatus::Fire;
				m_fAccTime = 0;
			}
		}
		break;
	case BossStatus::FastWalk:
		if (m_fAccTime > 5.f) // 달려오는 시간 끝나면 walk state로
		{
			m_bossStatus = BossStatus::Walk;
			m_fAccTime = 0;
		}
		break;
	case BossStatus::Fire:
		if (m_hp < 100)
		{
			m_bossStatus = BossStatus::DeployCannon;
			m_fAccTime = 0;
			m_bIsAttack = false;
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			m_bossStatus = BossStatus::Walk;
			m_fAccTime = 0;
			m_bIsAttack = false;
		}
		break;
	case BossStatus::DeployCannon:
		if (m_fAccTime > 1.2f)
		{
			m_bossStatus = BossStatus::CannonWalk;
			m_fAccTime = 0;
		}
		break;
	case BossStatus::CannonWalk:
		if (m_hp <= 0)
		{
			m_bossStatus = BossStatus::Destroyed;
			m_fAccTime = 0;
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			srand((unsigned)time(NULL));
			int ratio = rand() % 5;
			if (ratio == 0)
			{
				m_bossStatus = BossStatus::CannonFastWalk;
				m_fAccTime = 0;
			}
			else
			{
				m_bossStatus = BossStatus::CannonFire;
				m_fAccTime = 0;
			}
		}
		break;
	case BossStatus::CannonFastWalk:
		if (m_fAccTime > 5.f) // 달려오는 시간 끝나면 walk state로
		{
			m_bossStatus = BossStatus::CannonWalk;
			m_fAccTime = 0;
		}
		break;
	case BossStatus::CannonFire: // 불이랑 미사일
		if (m_hp <= 0)
		{
			m_bossStatus = BossStatus::Destroyed;
			m_fAccTime = 0;
			m_bIsAttack = false;
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			m_bossStatus = BossStatus::CannonWalk;
			m_fAccTime = 0;
			m_bIsAttack = false;
		}
		break;
	case BossStatus::Destroyed:
		break;
	default:
		break;
	}
}

void CBoss::AniUpdate()
{
	switch (m_bossStatus)
	{
	case BossStatus::Idle:
		m_pAnimator->Play(L"Boss\\BossWalk");
		break;
	case BossStatus::Walk:
		m_pAnimator->Play(L"Boss\\BossWalk");
		break;
	case BossStatus::FastWalk:
		m_pAnimator->Play(L"Boss\\BossWalk");
		break;
	case BossStatus::Fire:
		m_pAnimator->Play(L"Boss\\BossFire");
		break;
	case BossStatus::DeployCannon:
		m_pAnimator->Play(L"Boss\\BossCannon");
		break;
	case BossStatus::CannonWalk:
		m_pAnimator->Play(L"Boss\\BossWithCannon");
		break;
	case BossStatus::CannonFastWalk:
		m_pAnimator->Play(L"Boss\\BossWithCannon");
		break;
	case BossStatus::CannonFire:
		m_pAnimator->Play(L"Boss\\BossWithCannonFire");
		break;
	case BossStatus::Destroyed:
		m_pAnimator->Play(L"Boss\\BossDestroyed");
		break;
	}
}

void CBoss::CreateFireMissile()
{
	//m_fAttackAccTime += DT;
	if (!m_bIsAttack)
	{
		m_bIsAttack = true;
		pFireMissile = new CMissile;
		pFireMissile->SetName(L"BossFireMissile");
		pFireMissile->SetDir(Vector(1, 0));
		pFireMissile->SetVelocity(800);
		pFireMissile->SetPos(m_vecPos + Vector(0, -300));
		pFireMissile->SetExtension(m_fExtension);
		pFireMissile->SetOwner(this);
		ADDOBJECT(pFireMissile);
	}
	//else if (m_fAttackAccTime <= 0.2f)
	//{
	//	pFireMissile->GetAnimator()->Play(L"Boss\\BossFireMissile", false);
	//	m_fAttackAccTime = 0;
	//}
}

void CBoss::CreateMissile()
{
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"BossMissile");
}

void CBoss::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"PlayerMissile")
	{
		m_hp--;
	}
}

void CBoss::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBoss::OnCollisionExit(CCollider* pOtherCollider)
{
}
