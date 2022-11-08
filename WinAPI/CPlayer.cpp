#include "framework.h"
#include "CPlayer.h"

#include "WinAPI.h"
#include "CInputManager.h"
#include "CTimeManager.h"
#include "CRenderManager.h"
#include "CEventManager.h"
#include "CResourceManager.h"
#include "CCollider.h"
#include "CImage.h"
#include "CAnimator.h"

#include "CMissile.h"

CPlayer::CPlayer()
{
	m_vecPos = Vector(0, 0);
	m_vecScale = Vector(100, 100);
	m_layer = Layer::Player;
	m_strName = L"플레이어";

	m_pIdleImage = nullptr;
	m_pMoveImage = nullptr;
	m_pJumpImage = nullptr;
	m_pAttackRImage = nullptr;
	m_pAttackLImage = nullptr;
	m_pDeathImage = nullptr;

	m_vecMoveDir = Vector(0, 0);
	m_vecLookDir = Vector(0, 1);
	m_bIsMove = false; 
	m_bIsJump = false;
	m_bIsDead = false;
	m_bIsAttack = false;

	m_fSpeed = 200.0f;
	m_fAcctime = 0;
	m_fAttackAccTime = 0;

	m_curWeapon = PlayerWeapon::Pistol;
	m_status = PlayerStatus::Idle;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	m_pIdleImage = RESOURCE->LoadImg(L"PlayerIdle", L"Image\\Player\\EriIdle.png");
	m_pMoveImage = RESOURCE->LoadImg(L"PlayerMove", L"Image\\Player\\EriRun.png");
	m_pJumpImage = RESOURCE->LoadImg(L"PlayerJump", L"Image\\Player\\EriJump.png");
	m_pAttackRImage = RESOURCE->LoadImg(L"PlayerAttackR", L"Image\\Player\\EriAttackR.png");
	m_pAttackLImage = RESOURCE->LoadImg(L"PlayerAttackL", L"Image\\Player\\EriAttackL.png");
	m_pDeathImage = RESOURCE->LoadImg(L"PlayerDeath", L"Image\\Player\\EriDeath.png");

	m_pAnimator1 = new CAnimator;
	m_pAnimator2 = new CAnimator;

#pragma region Player Animation

	// Idle Animation
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleR_1", m_pIdleImage, 0.15f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumR_1", m_pIdleImage, 0.15f, false);
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleL_1", m_pIdleImage, 0.15f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumL_1", m_pIdleImage, 0.15f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleR_2", m_pIdleImage, 0.1f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleL_2", m_pIdleImage, 0.1f);

	// Move Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunL_1", m_pMoveImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunL_1", m_pMoveImage, 0.08f);
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunR_1", m_pMoveImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunR_1", m_pMoveImage, 0.08f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunL_2", m_pMoveImage, 0.08f);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunL_2", m_pMoveImage, 0.08f);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunR_2", m_pMoveImage, 0.08f);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunR_2", m_pMoveImage, 0.08f);

	// Attack Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL1_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL2_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackL_1", m_pAttackLImage, 0.1f);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL1_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL2_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownL_1", m_pAttackLImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1", m_pAttackLImage, 0.1f);

		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR1_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR2_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackR_1", m_pAttackRImage, 0.1f);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR1_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR2_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownR_1", m_pAttackRImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1", m_pAttackRImage, 0.1f);

	// Death Animation
		// L
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpL", m_pDeathImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathL", m_pDeathImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathL", m_pDeathImage, 0.1f);
		// R
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpR", m_pDeathImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathR", m_pDeathImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathR", m_pDeathImage, 0.1f);

	// Jump Animation
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpR_1", m_pJumpImage, 0.1f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpL_1", m_pJumpImage, 0.1f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpR_2", m_pJumpImage, 0.1f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpL_2", m_pJumpImage, 0.1f, false);

#pragma endregion

	AddComponent(m_pAnimator2);
	AddComponent(m_pAnimator1);

	//m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
	//m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
	AddCollider(ColliderType::Rect, Vector(50, 80), Vector(0, 0));
}

void CPlayer::Update()
{
	KeyUpdate();
	AnimatorUpdate();
}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
}

void CPlayer::KeyUpdate()
{
	if (BUTTONSTAY(VK_LEFT))
	{
		m_vecPos.x -= m_fSpeed * DT;
		m_bIsMove = true;
		m_vecLookDir.x = -1;
		m_vecMoveDir.x = -1;
	}
	else if (BUTTONSTAY(VK_RIGHT))
	{
		m_vecPos.x += m_fSpeed * DT;
		m_bIsMove = true;
		m_vecLookDir.x = +1;
		m_vecMoveDir.x = +1;
	}
	else
	{
		m_vecMoveDir.x = 0;
		m_bIsMove = false;
	}

	if (BUTTONSTAY(VK_SPACE)) // 충돌 구현 후 down으로 바꾸기
	{
		m_bIsJump = true;
	}
	else
	{
		m_bIsJump = false;
	}

	if (BUTTONDOWN('A'))
	{
		m_bIsAttack = true;
	}



	if (BUTTONDOWN('Q'))
	{
		m_bIsDead = true;
	}

	/*
	if (BUTTONSTAY(VK_UP))
	{
		m_vecPos.y -= m_fSpeed * DT;
		m_bIsMove = true;
		m_vecMoveDir.y = +1;
	}
	if (BUTTONSTAY(VK_DOWN))
	{
		m_vecPos.y += m_fSpeed * DT;
		m_bIsMove = true;
		m_vecMoveDir.y = -1;
	}
	else
	{
		m_bIsMove = false;
		m_vecMoveDir.y = 0;
	}

	if (BUTTONDOWN(VK_SPACE))
	{
		CreateMissile();
	}
	*/
}

void CPlayer::AnimatorUpdate()
{
	m_fAcctime += DT; 
	/*
	if (m_vecMoveDir.Length() > 0)
		m_vecLookDir = m_vecMoveDir;

	wstring str = L"";

	if (m_bIsMove)	str += L"Move";
	else			str += L"Idle";

	if (m_vecLookDir.x > 0) str += L"Right";
	else if (m_vecLookDir.x < 0) str += L"Left";

	if (m_vecLookDir.y > 0) str += L"Up";
	else if (m_vecLookDir.y < 0) str += L"Down";

	m_pAnimator1->Play(str, false);
	*/

	switch (m_status)
	{
	case PlayerStatus::Idle:
		if (m_fAcctime >= 5)
		{
			if (m_vecLookDir.x >= 0)
			{
				m_pAnimator1->Play(L"Player\\Idle\\EriIdleGumR_1");
				m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
			}
			else if (m_vecLookDir.x < 0)
			{
				m_pAnimator1->Play(L"Player\\Idle\\EriIdleGumL_1");
				m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
			}

			if (m_fAcctime >= 8.f)
				m_fAcctime = 0;
		}
		else
		{
			if (m_vecLookDir.x >= 0)
			{
				m_pAnimator1->Play(L"Player\\Idle\\EriIdleR_1");
				m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
			}
			else if (m_vecLookDir.x < 0)
			{
				m_pAnimator1->Play(L"Player\\Idle\\EriIdleL_1");
				m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
			}
		}

		if(m_bIsDead)
			m_status = PlayerStatus::Dead;
		else if (m_bIsMove)
			m_status = PlayerStatus::Move;
		else if (m_bIsJump)
			m_status = PlayerStatus::Jump;

		break;

	case PlayerStatus::Move:
		if (m_vecMoveDir.x < 0)
		{
			if (m_bIsAttack)
			{
				m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1");
			}
			else
				m_pAnimator1->Play(L"Player\\Move\\EriRunL_1");

			if(m_fAcctime >= (0.1 * 4))
				m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");
		}
		else if (m_vecMoveDir.x > 0)
		{
			m_pAnimator1->Play(L"Player\\Move\\EriRunR_1");
			if (m_fAcctime >= (0.1 * 4))
				m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");
		}
		else
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}

		if (m_bIsDead)
			m_status = PlayerStatus::Dead;
		break;

	case PlayerStatus::Jump:
		// 여기서 m_fAcctime 생각
		if (m_vecLookDir.x >= 0)
		{
			m_pAnimator1->Play(L"Player\\Jump\\EriJumpR_1");
			m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
		}
		else if (m_vecLookDir.x < 0)
		{
			m_pAnimator1->Play(L"Player\\Jump\\EriJumpL_1");
			m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
		}

		if(m_bIsJump == false)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
			break;
		}
		break;
	case PlayerStatus::Sit:
		break;
	case PlayerStatus::Attack:
		if (m_bIsMove)
		{
			if (m_bIsJump)
			{
				if (m_vecLookDir.x >= 0)
				{
					//
				}
				else if (m_vecLookDir.x < 0)
				{
					//
				}
			}
			else
			{
				if (m_vecLookDir.x >= 0)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
				}
				else if (m_vecLookDir.x < 0)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
				}
			}
		}
		else
		{

		}




		break;
	case PlayerStatus::Dead:
		break;
	default:
		m_status = PlayerStatus::Idle;
		m_fAcctime = 0;
		break;
	}
}

void CPlayer::CreateMissile()
{
	Logger::Debug(L"미사일 생성");

	CMissile* pMissile = new CMissile();
	pMissile->SetPos(m_vecPos);
	pMissile->SetDir(Vector(1, 0));
	ADDOBJECT(pMissile);

	CMissile* pMissile1 = new CMissile();
	pMissile1->SetPos(m_vecPos);
	pMissile1->SetDir(Vector(1, -1));
	ADDOBJECT(pMissile1);

	CMissile* pMissile2 = new CMissile();
	pMissile2->SetPos(m_vecPos);
	pMissile2->SetDir(Vector(1, 1));
	ADDOBJECT(pMissile2);

	CMissile* pMissile3 = new CMissile();
	pMissile3->SetPos(m_vecPos);
	pMissile3->SetDir(Vector(3, 1));
	ADDOBJECT(pMissile3);

	CMissile* pMissile4 = new CMissile();
	pMissile4->SetPos(m_vecPos);
	pMissile4->SetDir(Vector(3, -1));
	ADDOBJECT(pMissile4);
}

void CPlayer::Attack()
{
	switch (m_status)
	{
	case PlayerStatus::Idle:
		m_pAnimator1->Play(L"Player\\Attack\\EriJumpR_1");
		break;
	case PlayerStatus::Move:
		break;
	case PlayerStatus::Jump:
		break;
	case PlayerStatus::Sit:
		break;
	case PlayerStatus::Attack:
		break;
	case PlayerStatus::Dead:
		break;
	default:
		break;
	}
}

void CPlayer::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CPlayer::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CPlayer::OnCollisionExit(CCollider* pOtherCollider)
{
}
