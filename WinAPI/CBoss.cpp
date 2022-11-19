#include "framework.h"
#include "CBoss.h"

#include "CImage.h"

CBoss::CBoss()
{
	m_pAnimator = nullptr;
	m_strName = L"Boss";
	m_fSpeed = 200;
	m_vecMoveDir = Vector(1, 0);
	m_fAccTime = 0;
	m_hp = 200;
	m_bossStatus = BossStatus::Idle;
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
		break;
	case BossStatus::DeployCannon:
		break;
	case BossStatus::CannonWalk:
		break;
	case BossStatus::CannonFastWalk:
		break;
	case BossStatus::CannonFire:
		break;
	case BossStatus::Destroyed:
		break;
	default:
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
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			m_bossStatus = BossStatus::Walk;
			m_fAccTime = 0;
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
		if (m_fAccTime > 2.f) // 달려오는 시간 끝나면 walk state로
		{
			m_bossStatus = BossStatus::Walk;
			m_fAccTime = 0;
		}
		break;
	case BossStatus::CannonFire: // 불이랑 미사일
		if (m_hp <= 0)
		{
			m_bossStatus = BossStatus::Destroyed;
			m_fAccTime = 0;
			break;
		}
		if (m_fAccTime > 1.2f)
		{
			m_bossStatus = BossStatus::CannonWalk;
			m_fAccTime = 0;
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

void CBoss::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CBoss::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBoss::OnCollisionExit(CCollider* pOtherCollider)
{
}
