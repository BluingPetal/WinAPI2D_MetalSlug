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
#include "CGravity.h"
#include "CConga.h"

#include "CMissile.h"

CPlayer::CPlayer()
{
	m_vecPos = Vector(0, 0);
	m_vecScale = Vector(100, 100);
	m_layer = Layer::Player;
	m_strName = L"Player";

	m_pIdleImage = nullptr;
	m_pMoveImage = nullptr;
	m_pJumpImage = nullptr;
	m_pAttackRImage = nullptr;
	m_pAttackLImage = nullptr;
	m_pDeathImage = nullptr;

	m_vecMoveDir = Vector(0, 0);
	m_vecLookDir = m_vecPrevLookDir = tempLookDir = Vector(1, 0);
	m_bIsMove = false; 
	m_bIsJump = true;
	m_bIsDead = false;
	m_bIsAttack = false;
	m_bIsShoot = false; 

	m_bIsPrevCurLookSame = true;

	m_hp = 3;
	m_fSpeed = 300.0f;
	m_fAcctime = 0;
	m_fAttackAccTime = 0;

	m_curWeapon = PlayerWeapon::HeavyMachineGun;
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
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleR_1", m_pIdleImage, 0.085f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumR_1", m_pIdleImage, 0.085f, false);
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleL_1", m_pIdleImage, 0.085f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumL_1", m_pIdleImage, 0.085f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleR_2", m_pIdleImage, 0.08f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleL_2", m_pIdleImage, 0.08f);

	// Move Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunL_1", m_pMoveImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunL_1", m_pMoveImage, 0.08f);
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunR_1", m_pMoveImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunR_1", m_pMoveImage, 0.08f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunL_2", m_pMoveImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunL_2", m_pMoveImage, 0.08f);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunR_2", m_pMoveImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunR_2", m_pMoveImage, 0.08f);

	// Attack Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackL_1", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL1_1", m_pAttackLImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL2_1", m_pAttackLImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombL_1", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackL_1", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackL_1", m_pAttackLImage, 0.08f, false);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackL_1", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL1_1", m_pAttackLImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL2_1", m_pAttackLImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpL_1", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownL_1", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1", m_pAttackLImage, 0.03f, false);

		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackR_1", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR1_1", m_pAttackRImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR2_1", m_pAttackRImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombR_1", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackR_1", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackR_1", m_pAttackRImage, 0.08f, false);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackR_1", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR1_1", m_pAttackRImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR2_1", m_pAttackRImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpR_1", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownR_1", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1", m_pAttackRImage, 0.03f, false);

		// SitAttack
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitAttackL", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitHeavyGunL", m_pAttackLImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitAttackR", m_pAttackRImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitHeavyGunR", m_pAttackRImage, 0.08f, false);

	// Death Animation
		// L
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpL", m_pDeathImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathL", m_pDeathImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathL", m_pDeathImage, 0.08f);
		// R
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpR", m_pDeathImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathR", m_pDeathImage, 0.08f);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathR", m_pDeathImage, 0.08f);

	// Jump Animation
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpR_1", m_pJumpImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpL_1", m_pJumpImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpMoveR_1", m_pJumpImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Jump\\EriJumpMoveL_1", m_pJumpImage, 0.08f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpR_2", m_pJumpImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpL_2", m_pJumpImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpMoveR_2", m_pJumpImage, 0.08f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Jump\\EriJumpMoveL_2", m_pJumpImage, 0.08f, false);

#pragma endregion

	AddComponent(m_pAnimator2);
	AddComponent(m_pAnimator1);

	m_gravity = new CGravity;
	AddComponent(m_gravity);
	//m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
	//m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
	AddCollider(ColliderType::Rect, Vector(50, 80), Vector(0, 0));
}

void CPlayer::Update()
{
	KeyUpdate();
	AnimatorUpdate();
	BehaviorUpdate();
}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
}

void CPlayer::KeyUpdate()
{
	/*
	if (temp.y != m_vecLookDir.y)
	{
		m_vecPrevLookDir.y = temp.y;
		temp.y = m_vecLookDir.y;
	}
	*/
	tempLookDir.y = m_vecLookDir.y;

	if (BUTTONSTAY(VK_LEFT))
	{
		m_bIsMove = true;
		if(m_bIsJump == false) m_vecLookDir.x = -1;
		m_vecMoveDir.x = -1;
	}
	else if (BUTTONSTAY(VK_RIGHT))
	{
		m_bIsMove = true;
		if (m_bIsJump == false) m_vecLookDir.x = +1;
		m_vecMoveDir.x = +1;
	}
	else
	{
		m_vecMoveDir.x = 0;
		m_bIsMove = false;
	}

	if (BUTTONSTAY(VK_UP))
	{
		m_vecLookDir.y = +1;
	}
	else if (BUTTONSTAY(VK_DOWN))
	{
		m_vecLookDir.y = -1;
	}
	else
	{
		m_vecLookDir.y = 0;
	}
	/*
	if (BUTTONSTAY(VK_SPACE)) // 충돌 구현 후 down으로 바꾸기
	{
		m_bIsJump = true;
	}
	else
	{
		m_bIsJump = false;
	}
	*/

	if (BUTTONDOWN(VK_SPACE))
	{
		if (!m_bIsJump)
		{
			m_status = PlayerStatus::Jump;
			m_gravity->SetVelocity(-400);
			m_bIsJump = true;
		}
	}

	if (BUTTONDOWN('A'))
	{
		if (m_bIsPrevCurLookSame)
		{
			if (tempLookDir.y != m_vecPrevLookDir.y)
				m_bIsPrevCurLookSame = false;
		}
		else
		{
			m_vecPrevLookDir.y = tempLookDir.y;
			m_bIsPrevCurLookSame = true;
		}

		CreateMissile();
		// TODO : 총알에 따른 무기 정해주기
		m_curWeapon = PlayerWeapon::HeavyMachineGun;
		m_fAttackAccTime = 0;
		m_bIsAttack = true;
		m_bIsShoot = true;
		//Logger::Debug(L"prev : " + to_wstring(m_vecPrevLookDir.y) + L"cur : " + to_wstring(m_vecLookDir.y));
	}
	if (BUTTONDOWN('F'))
	{
		m_curWeapon = PlayerWeapon::Bomb;
		m_fAttackAccTime = 0;
		m_bIsAttack = true;
		m_bIsShoot = true;
	}

	//if (BUTTONDOWN('Q'))
	//{
	//	m_bIsDead = true;
	//}

	
	// PrevLookDir.y 정의
	if (tempLookDir.y != m_vecLookDir.y)
	{
		m_vecPrevLookDir.y = tempLookDir.y;
	}
	

	/*

	if (BUTTONDOWN(VK_SPACE))
	{
		CreateMissile();
	}
	*/
}

void CPlayer::AnimatorUpdate()
{
	m_fAcctime += DT;
	//Logger::Debug(L"m_fAttackAccTime : " + to_wstring(m_fAttackAccTime));
	
	switch (m_status)
	{
#pragma region Player status IDLE
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
		break;
#pragma endregion
#pragma region Player status MOVE
	case PlayerStatus::Move:
		if (m_vecMoveDir.x < 0)
		{
			m_vecPos.x -= m_fSpeed * DT;
			if(m_curWeapon == PlayerWeapon::HeavyMachineGun)
				m_pAnimator1->Play(L"Player\\Move\\EriHeavyGunRunL_1");
			else
				m_pAnimator1->Play(L"Player\\Move\\EriRunL_1");

			if (m_fAcctime >= (0.08 * 4))
				m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");
		}
		else if (m_vecMoveDir.x > 0)
		{
			m_vecPos.x += m_fSpeed * DT;
			if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				m_pAnimator1->Play(L"Player\\Move\\EriHeavyGunRunR_1");
			else
				m_pAnimator1->Play(L"Player\\Move\\EriRunR_1");

			if (m_fAcctime >= (0.08 * 4))
				m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");
		}
		break;
#pragma endregion
#pragma region Player status JUMP
	case PlayerStatus::Jump:
		// 여기서 m_fAcctime 생각
		if (m_vecLookDir.x >= 0)
		{
			if (m_bIsMove)
			{
				// 뛰면서 move animator
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpR_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
			}
		}
		else if (m_vecLookDir.x < 0)
		{
			if (m_bIsMove)   
			{
				// 뛰면서 move animator
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpL_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
			}
		}

		if(m_bIsJump == false)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		break;
#pragma endregion
	case PlayerStatus::Attack:

		m_fAttackAccTime += DT;

#pragma region 오른쪽으로 걷고 있을 경우 공격
		
		if (m_vecMoveDir.x > 0) // 오른쪽으로 걸을 때
		{
			// 상체 정의
			if (m_vecLookDir.y > 0) // 위쪽을 바라보며 공격
			{
				m_vecPos.x += m_fSpeed * DT;

				// 하체 정의
				if (m_bIsJump)
				{
					// TODO : gravity 구현 후 점프 다리 애니메이션 정의
					m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
				}
				else
				{
					if (m_fAcctime >= (0.08 * 4))
						m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAttackAccTime > 0.08f * 10)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun) // Heavy Machine Gun은 탄을 흩뿌림
				{
					float WaitedTime = 0.f;
					float needToWaitTime = 0.f;
				
					if (m_vecPrevLookDir.y < 0)
					{
						if (m_bIsJump)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.02 * 2;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpR_1");
					}
					else if (m_vecPrevLookDir.y == 0)
					{
						needToWaitTime += 0.02 * 2;
						if (m_fAttackAccTime < needToWaitTime)
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpR_1");
					}

					WaitedTime = needToWaitTime;
					needToWaitTime += 0.08 * 4;
					if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpR_1", true);
							m_bIsShoot = false;
						}
					}
					if (m_fAttackAccTime >= needToWaitTime)
					{
						m_bIsAttack = false;
					}
				}
			}
			else if (m_vecLookDir.y == 0)
			{
				m_vecPos.x += m_fSpeed * DT;

				// 하체 정의
				if (m_bIsJump)
				{
					// TODO : gravity 구현 후 점프 다리 애니메이션 정의
					m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
				}
				else
				{
					if (m_fAcctime >= (0.08 * 4))
						m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAttackAccTime > 0.08f * 10)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					float WaitedTime = 0.f;
					float needToWaitTime = 0.f;

					if (m_vecPrevLookDir.y < 0)
					{
						if (m_bIsJump)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1");
						}
					}
					else if (m_vecPrevLookDir.y > 0)
					{
						needToWaitTime += 0.02 * 2;
						if (m_fAttackAccTime < needToWaitTime)
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1");
					}

					WaitedTime = needToWaitTime;
					needToWaitTime += 0.08 * 7;
					if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackR_1", true);
							m_bIsShoot = false;
						}
					}
					if (m_fAttackAccTime >= needToWaitTime)
					{
						m_bIsAttack = false;
					}
				}
			}
			else // 아래쪽 (m_vecLookDir.y < 0)
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					if (m_bIsJump)
					{
						m_vecPos.x += m_fSpeed * DT;

						// 하체 정의
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");

						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAttackAccTime > 0.08f * 6)
						{
							m_bIsAttack = false;
						}
					}
					else
					{
						if (m_bIsShoot)
						{
							m_pAnimator2->Stop();
							m_pAnimator1->Play(L"Player\\Attack\\EriSitAttackR", true);
							m_bIsShoot = false;
						}
						if (m_fAttackAccTime > 0.08f * 9)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					float WaitedTime = 0.f;
					float needToWaitTime = 0.f;
					if (m_bIsJump)
					{
						m_vecPos.x += m_fSpeed * DT;

						// 하체 정의
						if (m_bIsJump)
						{
							// TODO : gravity 구현 후 점프 다리 애니메이션 정의
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
						}
						else
						{
							if (m_fAcctime >= (0.08 * 4))
								m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
							else
								m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");
						}

						if (m_vecPrevLookDir.y > 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1");

							WaitedTime = needToWaitTime;
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownR_1");
						}
						else if (m_vecPrevLookDir.y == 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownR_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 4;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownR_1", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime >= needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
					else // 전 프레임에 위에서 쐈던 것만 고려
					{
						if (m_vecPrevLookDir.y > 0)
						{
							m_vecPos.x += m_fSpeed * DT;

							// 하체 정의
							if (m_bIsJump)
							{
								// TODO : gravity 구현 후 점프 다리 애니메이션 정의
								m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
							}
							else
							{
								if (m_fAcctime >= (0.08 * 4))
									m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
								else
									m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");
							}

							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 7;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator2->Stop();
								m_pAnimator1->Play(L"Player\\Attack\\EriSitHeavyGunR", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime > needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
				}
			}

			if (m_curWeapon == PlayerWeapon::Bomb)
			{
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
					m_bIsShoot = false;
				}
				if (m_fAttackAccTime > 0.08f * 6)
				{
					m_bIsAttack = false;
				}
			}
			else if (m_curWeapon == PlayerWeapon::Knife)
			{
				srand((unsigned)time(NULL));
				int randnum = rand() % 2;
				if (randnum == 0)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1");

					if (m_fAttackAccTime > 0.08f * 6)
					{
						m_bIsAttack = false;
					}
				}
				else
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1");

					if (m_fAttackAccTime > 0.08f * 9)
					{
						m_bIsAttack = false;
					}
				}
			}
		}
#pragma endregion
#pragma region  멈춰있을 경우 공격
		else if (m_vecMoveDir.x == 0) // 멈춰있을 경우
		{
			if (m_vecLookDir.x >= 0) // 오른쪽 보고 있을 경우
			{
				if (m_vecLookDir.y > 0)
				{
					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAttackAccTime > 0.08f * 10)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						float WaitedTime = 0.f;
						float needToWaitTime = 0.f;

						if (m_vecPrevLookDir.y < 0)
						{
							if (m_bIsJump)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1");
							}

							WaitedTime = needToWaitTime;
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpR_1");
						}
						else if (m_vecPrevLookDir.y == 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpR_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 4;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpR_1", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime >= needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y == 0)
				{
					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
							m_bIsShoot = false;
						}

						if (m_fAttackAccTime > 0.08f * 10)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						float WaitedTime = 0.f;
						float needToWaitTime = 0.f;

						if (m_vecPrevLookDir.y < 0)
						{
							if (m_bIsJump)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1");
							}
						}
						else if (m_vecPrevLookDir.y > 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 7;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackR_1", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime >= needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y < 0)
				{
					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						if (m_bIsJump)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAttackAccTime > 0.08f * 6)
							{
								m_bIsAttack = false;
							}
						}
						else // TODO : pistol 앉아서 쏘는 애니메이션
						{
							/*
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAttackAccTime > 0.08f * 6)
							{
								m_bIsAttack = false;
							}
							*/
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						float WaitedTime = 0.f;
						float needToWaitTime = 0.f;
						if (m_bIsJump)
						{
							if (m_vecPrevLookDir.y > 0)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1");

								WaitedTime = needToWaitTime;
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownR_1");
							}
							else if (m_vecPrevLookDir.y == 0)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownR_1");
							}

							WaitedTime = needToWaitTime;
							needToWaitTime += 0.08 * 4;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
							{
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownR_1", true);
									m_bIsShoot = false;
								}
							}
							if (m_fAttackAccTime >= needToWaitTime)
							{
								m_bIsAttack = false;
							}
						}
						else // 전 프레임에 위에서 쐈던 것만 고려
						{
							if (m_vecPrevLookDir.y > 0)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1");

								WaitedTime = needToWaitTime;
								// TODO : 앉아서 Heavy Gun 오른쪽으로 쏘는 애니메이션
								/*
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownR_1");
								*/
							}
						}
					}
				}

				if (m_curWeapon == PlayerWeapon::Bomb)
				{
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAttackAccTime > 0.08f * 6)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int randnum = rand() % 2;
					if (randnum == 0)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1");

						if (m_fAttackAccTime > 0.08f * 6)
						{
							m_bIsAttack = false;
						}
					}
					else
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1");

						if (m_fAttackAccTime > 0.08f * 9)
						{
							m_bIsAttack = false;
						}
					}
				}

				// 하체 정의
				if (m_bIsJump)
				{
					// TODO : gravity 구현 후 점프 다리 애니메이션 정의
					m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
				}
				else
				{
					m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
				}
			}
			else // 왼쪽 보고 있을 경우
			{
				if (m_vecLookDir.y > 0)
				{
					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAttackAccTime > 0.08f * 10)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						float WaitedTime = 0.f;
						float needToWaitTime = 0.f;

						if (m_vecPrevLookDir.y < 0)
						{
							if (m_bIsJump)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1");
							}

							WaitedTime = needToWaitTime;
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpL_1");
						}
						else if (m_vecPrevLookDir.y == 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpL_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 4;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpL_1", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime >= needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
					// TODO : bomb공격, knife 공격 구현
				}
				else if (m_vecLookDir.y == 0)
				{
					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
							m_bIsShoot = false;
						}

						if (m_fAttackAccTime > 0.08f * 10)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						float WaitedTime = 0.f;
						float needToWaitTime = 0.f;

						if (m_vecPrevLookDir.y < 0)
						{
							if (m_bIsJump)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1");
							}
						}
						else if (m_vecPrevLookDir.y > 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 7;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackL_1", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime >= needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y < 0)
				{
					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						if (m_bIsJump)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAttackAccTime > 0.08f * 6)
							{
								m_bIsAttack = false;
							}
						}
						else // TODO : pistol 앉아서 쏘는 애니메이션
						{
							/*
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAttackAccTime > 0.08f * 6)
							{
								m_bIsAttack = false;
							}
							*/
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						float WaitedTime = 0.f;
						float needToWaitTime = 0.f;
						if (m_bIsJump)
						{
							if (m_vecPrevLookDir.y > 0)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1");

								WaitedTime = needToWaitTime;
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
							}
							else if (m_vecPrevLookDir.y == 0)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
							}

							WaitedTime = needToWaitTime;
							needToWaitTime += 0.08 * 4;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
							{
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownL_1", true);
									m_bIsShoot = false;
								}
							}
							if (m_fAttackAccTime >= needToWaitTime)
							{
								m_bIsAttack = false;
							}
						}
						else // 전 프레임에 위에서 쐈던 것만 고려
						{
							if (m_vecPrevLookDir.y > 0)
							{
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1");

								WaitedTime = needToWaitTime;
								// TODO : 앉아서 Heavy Gun 오른쪽으로 쏘는 애니메이션
								/*
								needToWaitTime += 0.02 * 2;
								if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
								*/
							}
						}
					}
				}

				if (m_curWeapon == PlayerWeapon::Bomb)
				{
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAttackAccTime > 0.08f * 6)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int randnum = rand() % 2;
					if (randnum == 0)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1");

						if (m_fAttackAccTime > 0.08f * 6)
						{
							m_bIsAttack = false;
						}
					}
					else
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1");

						if (m_fAttackAccTime > 0.08f * 9)
						{
							m_bIsAttack = false;
						}
					}
				}

				// 하체 정의
				if (m_bIsJump)
				{
					// TODO : gravity 구현 후 점프 다리 애니메이션 정의
					m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
				}
				else
				{
					m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
				}
			}
		}
#pragma endregion
#pragma region 왼쪽으로 걷고 있을 경우 공격
		else
		{
			// 상체 정의
			if (m_vecLookDir.y > 0) // 위쪽을 바라보며 공격
			{
				m_vecPos.x -= m_fSpeed * DT;

				// 하체 정의
				if (m_bIsJump)
				{
					// TODO : gravity 구현 후 점프 다리 애니메이션 정의
					m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
				}
				else
				{
					if (m_fAcctime >= (0.08 * 4))
						m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAttackAccTime > 0.08f * 10)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun) // Heavy Machine Gun은 탄을 흩뿌림
				{
					float WaitedTime = 0.f;
					float needToWaitTime = 0.f;

					if (m_vecPrevLookDir.y < 0)
					{
						if (m_bIsJump)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.02 * 2;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpL_1");
					}
					else if (m_vecPrevLookDir.y == 0)
					{
						needToWaitTime += 0.02 * 2;
						if (m_fAttackAccTime < needToWaitTime)
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpL_1");
					}

					WaitedTime = needToWaitTime;
					needToWaitTime += 0.08 * 4;
					if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpL_1", true);
							m_bIsShoot = false;
						}
					}
					if (m_fAttackAccTime >= needToWaitTime)
					{
						m_bIsAttack = false;
					}
				}
			}
			else if (m_vecLookDir.y == 0)
			{
				m_vecPos.x -= m_fSpeed * DT;

				// 하체 정의
				if (m_bIsJump)
				{
					// TODO : gravity 구현 후 점프 다리 애니메이션 정의
					m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
				}
				else
				{
					if (m_fAcctime >= (0.08 * 4))
						m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAttackAccTime > 0.08f * 10)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					float WaitedTime = 0.f;
					float needToWaitTime = 0.f;

					if (m_vecPrevLookDir.y < 0)
					{
						if (m_bIsJump)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1");
						}
					}
					else if (m_vecPrevLookDir.y > 0)
					{
						needToWaitTime += 0.02 * 2;
						if (m_fAttackAccTime < needToWaitTime)
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1");
					}

					WaitedTime = needToWaitTime;
					needToWaitTime += 0.08 * 7;
					if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
					{
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackL_1", true);
							m_bIsShoot = false;
						}
					}
					if (m_fAttackAccTime >= needToWaitTime)
					{
						m_bIsAttack = false;
					}
				}
			}
			else // 아래쪽 (m_vecLookDir.y < 0)
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					if (m_bIsJump)
					{
						m_vecPos.x -= m_fSpeed * DT;

						// 하체 정의
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");

						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAttackAccTime > 0.08f * 6)
						{
							m_bIsAttack = false;
						}
					}
					else
					{
						if (m_bIsShoot)
						{
							m_pAnimator2->Stop();
							m_pAnimator1->Play(L"Player\\Attack\\EriSitAttackL", true);
							m_bIsShoot = false;
						}
						if (m_fAttackAccTime > 0.08f * 9)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					float WaitedTime = 0.f;
					float needToWaitTime = 0.f;
					if (m_bIsJump)
					{
						m_vecPos.x += m_fSpeed * DT;

						// 하체 정의
						if (m_bIsJump)
						{
							// TODO : gravity 구현 후 점프 다리 애니메이션 정의
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
						}
						else
						{
							if (m_fAcctime >= (0.08 * 4))
								m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
							else
								m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");
						}

						if (m_vecPrevLookDir.y > 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1");

							WaitedTime = needToWaitTime;
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
						}
						else if (m_vecPrevLookDir.y == 0)
						{
							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownL_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 4;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownL_1", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime >= needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
					else // 전 프레임에 위에서 쐈던 것만 고려
					{
						if (m_vecPrevLookDir.y > 0)
						{
							m_vecPos.x -= m_fSpeed * DT;

							// 하체 정의
							if (m_bIsJump)
							{
								// TODO : gravity 구현 후 점프 다리 애니메이션 정의
								m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
							}
							else
							{
								if (m_fAcctime >= (0.08 * 4))
									m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
								else
									m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");
							}

							needToWaitTime += 0.02 * 2;
							if (m_fAttackAccTime < needToWaitTime)
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1");
						}

						WaitedTime = needToWaitTime;
						needToWaitTime += 0.08 * 7;
						if (m_fAttackAccTime >= WaitedTime && m_fAttackAccTime < needToWaitTime)
						{
							if (m_bIsShoot)
							{
								m_pAnimator2->Stop();
								m_pAnimator1->Play(L"Player\\Attack\\EriSitHeavyGunL", true);
								m_bIsShoot = false;
							}
						}
						if (m_fAttackAccTime > needToWaitTime)
						{
							m_bIsAttack = false;
						}
					}
				}
			}

			if (m_curWeapon == PlayerWeapon::Bomb)
			{
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
					m_bIsShoot = false;
				}
				if (m_fAttackAccTime > 0.08f * 6)
				{
					m_bIsAttack = false;
				}
			}
			else if (m_curWeapon == PlayerWeapon::Knife)
			{
				srand((unsigned)time(NULL));
				int randnum = rand() % 2;
				if (randnum == 0)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1");

					if (m_fAttackAccTime > 0.08f * 6)
					{
						m_bIsAttack = false;
					}
				}
				else
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1");

					if (m_fAttackAccTime > 0.08f * 9)
					{
						m_bIsAttack = false;
					}
				}
			}
		}
#pragma endregion

		break;
	case PlayerStatus::Dead:
		break;
	default:
		m_status = PlayerStatus::Idle;
		m_fAcctime = 0;
		break;
	}
}

void CPlayer::BehaviorUpdate()
{
	switch (m_status)
	{
	case PlayerStatus::Idle:
		if (m_bIsDead)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_bIsJump)
		{
			m_status = PlayerStatus::Jump;
			m_fAcctime = 0;
		}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (m_bIsMove)
		{
			m_status = PlayerStatus::Move;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Move:

		if(m_vecMoveDir.x == 0)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}

		if (m_bIsDead)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (m_bIsJump)
		{
			m_status = PlayerStatus::Jump;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Jump:
		if (m_bIsDead)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Attack:

		if (!m_bIsAttack)
		{
			m_pAnimator1->Stop();
			m_pAnimator2->Stop();
			m_status = PlayerStatus::Idle;
		}

		break;
	case PlayerStatus::Dead:
		break;
	default:
		break;
	}
}

void CPlayer::CreateMissile()
{
	Logger::Debug(L"미사일 생성");

	CMissile* pMissile = new CMissile();
	pMissile->SetPos(m_vecPos);
	pMissile->SetDir(m_vecLookDir);
	pMissile->SetOwner(this);
	pMissile->SetExtension(m_fExtension);
	ADDOBJECT(pMissile);
	/*
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
	*/
}

void CPlayer::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"frontOcean")
	{
		m_bIsJump = false;
	}
	else if (pOtherCollider->GetObjName() == L"obstacle")
	{
		m_fSpeed = 0;
		m_vecPos.x += 5;
	}
}

void CPlayer::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CPlayer::OnCollisionExit(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"obstacle")
	{
		m_fSpeed = 300;
	}
}
