#include "framework.h"
#include "CBoss.h"

#include "CImage.h"
#include "CBossMissile.h"
#include "CAniObject.h"
#include "CPlayer.h"

CBoss::CBoss()
{
	m_pAnimator = nullptr;
	m_strName = L"Boss";
	m_fSpeed = 200;
	m_vecMoveDir = Vector(1, 0);
	m_fAccTime = 0;
	m_fAttackAccTime = 0;
	m_hp = 120;
	m_bossStatus = BossStatus::Idle;
	m_bIsAttack = false;
	m_fFireDisappearAccTime = 0;
	m_fMissileDisappearAccTime = 0;
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
	if (m_pFireMissileAniObj != nullptr && !m_pFireMissileAniObj->GetReserveDelete())
	{
		m_fFireDisappearAccTime += DT;
		if (m_fFireDisappearAccTime >= 1.2f)
		{
			DELETEOBJECT(m_pFireMissileAniObj);
			m_fFireDisappearAccTime = 0;
		}
	}

	if (pMissile != nullptr && !pMissile->GetSafeToDelete())
	{
		if (pMissile->GetReserveDelete())
		{
			// 폭발 애니메이터 생성뒤 지움
			m_pMissileAniObj = new CAniObject;
			m_pMissileAniObj->SetImage(m_pEffectImg);
			m_pMissileAniObj->SetPos(pMissile->GetPos() + Vector(0, -200));
			m_pMissileAniObj->SetExtension(m_fExtension + 1.5);
			m_pMissileAniObj->SetLayer(Layer::Unit);
			ADDOBJECT(m_pMissileAniObj);
			m_pMissileAniObj->GetAnimator()->CreateAnimation(L"Effect\\Bomb", m_pEffectImg, 0.05f, false);
			m_pMissileAniObj->GetAnimator()->Play(L"Effect\\Bomb");
		}
	}
	if (m_pMissileAniObj != nullptr && !m_pMissileAniObj->GetReserveDelete())
	{
		m_fMissileDisappearAccTime += DT;
		if (m_fMissileDisappearAccTime >= 1.2f)
		{
			DELETEOBJECT(m_pMissileAniObj);
			m_fMissileDisappearAccTime = 0;
		}
	}

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
		if(m_fAccTime > 0.4f)
			CreateMissile();
		break;
	case BossStatus::Destroyed:
		m_vecPos.y += (m_fSpeed - 100) * DT;
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
		if (m_hp < 50)
		{
			m_bossStatus = BossStatus::DeployCannon;
			m_fAccTime = 0;
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			srand((unsigned)time(NULL));
			int ratio = rand() % 10;
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
		if (m_hp < 50)
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
			int ratio = rand() % 10;
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
		pFireMissile = new CBossMissile;
		pFireMissile->SetName(L"BossFireMissile");
		pFireMissile->SetDir(Vector(1, 0));
		pFireMissile->SetVelocity(800);
		pFireMissile->SetPos(m_vecPos + Vector(100, -300));
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
{	//m_fAttackAccTime += DT;
	if (!m_bIsAttack)
	{
		m_bIsAttack = true;

		pFireMissile = new CBossMissile;
		pFireMissile->SetName(L"BossFireMissile");
		pFireMissile->SetDir(Vector(1, 0));
		pFireMissile->SetVelocity(800);
		pFireMissile->SetPos(m_vecPos + Vector(100, -300));
		pFireMissile->SetExtension(m_fExtension);
		pFireMissile->SetOwner(this);
		ADDOBJECT(pFireMissile);

		pMissile = new CBossMissile;
		pMissile->SetName(L"BossMissile");
		pMissile->SetDir(Vector(1, 0));
		pMissile->SetVelocity(700);
		pMissile->SetPos(m_vecPos + Vector(130, -170));
		pMissile->SetExtension(m_fExtension);
		pMissile->SetOwner(this);
		ADDOBJECT(pMissile);
	}
	//CMissile* pMissile = new CMissile;
	//pMissile->SetName(L"BossMissile");
}

void CBoss::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"PlayerMissile")
	{
		m_hp--;
	}
	if (pOtherCollider->GetObjName() == L"PlayerMissile")
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pMissile->GetOwner());
		if (pPlayer->GetCurWeapon() == PlayerWeapon::Pistol)
			m_hp--;
		else if (pPlayer->GetCurWeapon() == PlayerWeapon::HeavyMachineGun)
			m_hp -= 2;
	}
	if (pOtherCollider->GetObjName() == L"Bomb")
	{
		m_hp -= 5;
	}
}

void CBoss::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBoss::OnCollisionExit(CCollider* pOtherCollider)
{
}
