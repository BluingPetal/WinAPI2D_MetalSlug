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

	m_iBomb = 10;
	m_iBullet = 0;
	m_bBombDiff = false;
	m_bBulletDiff = false;

	m_hp = 3;
	m_fSpeed = 300.0f;
	m_fAcctime = 0;

	m_curWeapon = PlayerWeapon::HeavyMachineGun;
	m_status = PlayerStatus::Idle;
	m_prevStatus = PlayerStatus::Idle;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
#pragma region Player Image

	m_pIdleImage = RESOURCE->LoadImg(L"PlayerIdle", L"Image\\Player\\EriIdle.png");
	m_pMoveImage = RESOURCE->LoadImg(L"PlayerMove", L"Image\\Player\\EriRun.png");
	m_pJumpImage = RESOURCE->LoadImg(L"PlayerJump", L"Image\\Player\\EriJump.png");
	m_pAttackRImage = RESOURCE->LoadImg(L"PlayerAttackR", L"Image\\Player\\EriAttackR.png");
	m_pAttackLImage = RESOURCE->LoadImg(L"PlayerAttackL", L"Image\\Player\\EriAttackL.png");
	m_pDeathImage = RESOURCE->LoadImg(L"PlayerDeath", L"Image\\Player\\EriDeath.png");

#pragma endregion

	m_pAnimator1 = new CAnimator;
	m_pAnimator2 = new CAnimator;

#pragma region Player Animation

	// Idle Animation
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleR_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumR_1", m_pIdleImage, 0.1f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunR_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunColdR", m_pIdleImage, 0.1f, false);
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleL_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleGumL_1", m_pIdleImage, 0.1f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunL_1", m_pIdleImage, 0.1f);
	m_pAnimator1->CreateAnimation(L"Player\\Idle\\EriIdleHeavyGunColdL", m_pIdleImage, 0.1f, false);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleR_2", m_pIdleImage, 0.1f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Idle\\EriIdleL_2", m_pIdleImage, 0.1f);

	// Move Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunL_1", m_pMoveImage, 0.05f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunL_1", m_pMoveImage, 0.05f);
		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriRunR_1", m_pMoveImage, 0.05f);
	m_pAnimator1->CreateAnimation(L"Player\\Move\\EriHeavyGunRunR_1", m_pMoveImage, 0.05f);
		// BOTTOM - L
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunL_2", m_pMoveImage, 0.05f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunL_2", m_pMoveImage, 0.05f);
		// BOTTOM - R
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriStartToRunR_2", m_pMoveImage, 0.05f, false);
	m_pAnimator2->CreateAnimation(L"Player\\Move\\EriRunR_2", m_pMoveImage, 0.05f);

	// Attack Animation
		// TOP - L
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL1_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionL2_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackL_1", m_pAttackLImage, 0.05f, false);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL1_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionL2_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownL_1", m_pAttackLImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownL_1", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceL_1", m_pAttackLImage, 0.03f, false);

		// TOP - R
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriAttackR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR1_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriKnifeMotionR2_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriBombR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriUpAttackR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriDownAttackR_1", m_pAttackRImage, 0.05f, false);

	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunAttackR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR1_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunKnifeMotionR2_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunUpR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingUpPlaceR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownR_1", m_pAttackRImage, 0.03f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunDownR_1", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriHeavyGunGoingDownPlaceR_1", m_pAttackRImage, 0.03f, false);

		// SitAttack
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitAttackL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitHeavyGunL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitBombL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitKnifeL", m_pAttackLImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitAttackR", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitHeavyGunR", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitBombR", m_pAttackRImage, 0.05f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Attack\\EriSitKnifeR", m_pAttackRImage, 0.05f, false);

	// Death Animation
		// L
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpL", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathL", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathL", m_pDeathImage, 0.08f, false);
		// R
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriDeathJumpR", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriNearDeathR", m_pDeathImage, 0.08f, false);
	m_pAnimator1->CreateAnimation(L"Player\\Death\\EriFarDeathR", m_pDeathImage, 0.08f, false);

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
	AddCollider(ColliderType::Rect, Vector(50, 80), Vector(0, 0));
}

void CPlayer::Update()
{
	KeyUpdate();
	BehaviorUpdate();
	StatusUpdate();
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
	tempLookDir.y = m_vecLookDir.y;
	m_prevStatus = m_status;
	m_bBombDiff = false;
	m_bBulletDiff = false;

	if (!(m_status == PlayerStatus::Dead))
	{
		if (BUTTONSTAY(VK_LEFT))
		{
			m_vecLookDir.x = -1;
			m_vecMoveDir.x = -1;
		}
		else if (BUTTONSTAY(VK_RIGHT))
		{
			m_vecLookDir.x = +1;
			m_vecMoveDir.x = +1;
		}
		else
		{
			m_vecMoveDir.x = 0;
		}

		if (BUTTONSTAY(VK_UP))
		{
			m_vecLookDir.y = -1;
		}
		else if (BUTTONSTAY(VK_DOWN))
		{
			m_vecLookDir.y = +1;
		}
		else
		{
			m_vecLookDir.y = 0;
		}

		if (BUTTONDOWN(VK_SPACE))
		{
			if (!m_bIsJump && m_status != PlayerStatus::SitAttack)
			{
				m_bIsJump = true;
				m_vecMoveDir.y = -1;
				m_status = PlayerStatus::Jump;
				m_gravity->SetVelocity(-400);
			}
		}

		if (BUTTONDOWN('A'))
		{
			m_bBulletDiff = true;
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
			m_curWeapon = PlayerWeapon::Pistol;
			m_fAcctime = 0;
			m_bIsAttack = true;
			m_bIsShoot = true;
		}
		if (BUTTONDOWN('F') && m_iBomb > 0)
		{
			m_bBombDiff = true;
			if (m_iBomb > 0)
				m_iBomb--;
			else m_iBomb == 0;
			m_curWeapon = PlayerWeapon::Bomb;
			m_fAcctime = 0;
			m_bIsAttack = true;
			m_bIsShoot = true;
		}
		// PrevLookDir.y 정의
		if (tempLookDir.y != m_vecLookDir.y)
		{
			m_vecPrevLookDir.y = tempLookDir.y;
		}
	}
	if (BUTTONDOWN('Q'))
	{
		m_bIsDead = false;
		m_hp = 3;
		m_bIsJump = true;
		m_fSpeed = 300;
		m_vecPos = CAMERA->ScreenToWorldPoint(Vector(WINSIZEX * 0.2, WINSIZEY * 0.5));
		m_status = PlayerStatus::Idle;
	}
}

void CPlayer::AnimatorUpdate()
{
	m_fAcctime += DT;

	switch (m_status)
	{
	case PlayerStatus::Idle:
	{
		if (m_vecLookDir.x > 0) // m_vecLookDir.x == 0인 경우는 없음
		{
			m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
			if (m_fAcctime >= 5.f)
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleGumR_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator2->Stop();
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunColdR");
				}
			}
			else
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleR_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunR_1");
				}
			}
		}
		else
		{
			m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
			if (m_fAcctime >= 5.f)
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleGumL_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator2->Stop();
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunColdL");
				}
			}
			else
			{
				if (m_curWeapon == PlayerWeapon::Pistol)
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleL_1");
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					m_pAnimator1->Play(L"Player\\Idle\\EriIdleHeavyGunL_1");
				}
			}
		}
		if (m_fAcctime > 7.5f)
			m_fAcctime = 0;
	}
		break;
	case PlayerStatus::Move:
	{
		if (m_vecMoveDir.x > 0)
		{
			if (m_fAcctime >= 0.05 * 4)
				m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunR_2");

			if (m_curWeapon == PlayerWeapon::Pistol)
				m_pAnimator1->Play(L"Player\\Move\\EriRunR_1");
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				m_pAnimator1->Play(L"Player\\Move\\EriHeavyGunRunR_1");
		}
		else
		{
			if (m_fAcctime >= 0.05 * 4)
				m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
			else
				m_pAnimator2->Play(L"Player\\Move\\EriStartToRunL_2");

			if (m_curWeapon == PlayerWeapon::Pistol)
				m_pAnimator1->Play(L"Player\\Move\\EriRunL_1");
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				m_pAnimator1->Play(L"Player\\Move\\EriHeavyGunRunL_1");
		}
	}
		break;
	case PlayerStatus::Jump:
	{
		if (m_vecLookDir.x > 0)
		{
			if (m_vecMoveDir.x == 0)
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpR_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpMoveR_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
			}
		}
		else
		{
			if (m_vecMoveDir.x == 0)
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpL_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Jump\\EriJumpMoveL_1");
				m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
			}
		}
	}
		break;
	case PlayerStatus::Attack:
	{
		if (m_vecMoveDir.x > 0) // 오른쪽으로 움직이는 경우
		{
			if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if(m_vecPrevLookDir.y == 0)
						HeavyGunGoingUp(L"R");
					else if (m_vecPrevLookDir.y < 0)
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else if (m_vecLookDir.y == 0)
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunR_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime > m_fShootTime)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if (m_vecPrevLookDir.y < 0)
					{
						HeavyGunGoingUpPlace(L"R");
					}
					else if (m_vecPrevLookDir.y > 0)
					{
						HeavyGunGoingDownPlace(L"R");
					}
					else
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else // else if (m_vecLookDir.y > 0)
			{
				if (m_bIsJump)
				{
					if (m_bIsShoot)
					{
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveR_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingDown(L"R");
						else if (m_vecPrevLookDir.y > 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
			}
		}
		else if (m_vecMoveDir.x < 0) // 왼쪽으로 움직이는 경우
		{
			if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if (m_vecPrevLookDir.y == 0)
						HeavyGunGoingUp(L"L");
					else if (m_vecPrevLookDir.y < 0)
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else if (m_vecLookDir.y == 0)
			{
				if (m_bIsShoot)
				{
					if (m_bIsJump)
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
					else
						m_pAnimator2->Play(L"Player\\Move\\EriRunL_2");
				}

				if (m_curWeapon == PlayerWeapon::Pistol)
				{
					m_fShootTime = 0.05f * 10;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime > m_fShootTime)
					{
						m_bIsAttack = false;
					}
				}
				else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
				{
					if (m_vecPrevLookDir.y < 0)
					{
						HeavyGunGoingUpPlace(L"L");
					}
					else if (m_vecPrevLookDir.y > 0)
					{
						HeavyGunGoingDownPlace(L"L");
					}
					else
					{
						m_fShootTime = 0.05f * 4;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
				}
				else if (m_curWeapon == PlayerWeapon::Bomb)
				{
					m_fShootTime = 0.05f * 6;
					if (m_bIsShoot)
					{
						m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
						m_bIsShoot = false;
					}
					if (m_fAcctime >= m_fShootTime)
						m_bIsAttack = false;
				}
				else if (m_curWeapon == PlayerWeapon::Knife)
				{
					srand((unsigned)time(NULL));
					int playAnimatorNum = rand() % 2;
					if (playAnimatorNum == 0)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else
					{
						m_fShootTime = 0.05f * 9;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
				}
			}
			else // else if (m_vecLookDir.y > 0)
			{
				if (m_bIsJump)
				{
					if (m_bIsShoot)
					{
						m_pAnimator2->Play(L"Player\\Jump\\EriJumpMoveL_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingDown(L"L");
						else if (m_vecPrevLookDir.y > 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
			}
		}
		else // 아무 쪽으로도 가지 않고 있을 경우 (m_vecMoveDir.x == 0)
		{
			if (m_vecLookDir.x > 0)
			{
				if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingUp(L"R");
						else if (m_vecPrevLookDir.y < 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y == 0)
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleR_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriAttackR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y < 0)
						{
							HeavyGunGoingUpPlace(L"R");
						}
						else if (m_vecPrevLookDir.y > 0)
						{
							HeavyGunGoingDownPlace(L"R");
						}
						else
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime > m_fShootTime)
							{
								m_bIsAttack = false;
							}
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else // else if (m_vecLookDir.y > 0)
				{
					if (m_bIsJump)
					{
						if (m_bIsShoot)
						{
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpR_2");
						}

						if (m_curWeapon == PlayerWeapon::Pistol)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
						{
							if (m_vecPrevLookDir.y == 0)
								HeavyGunGoingDown(L"R");
							else if (m_vecPrevLookDir.y > 0)
							{
								m_fShootTime = 0.05f * 4;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownR_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
						else if (m_curWeapon == PlayerWeapon::Bomb)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriBombR_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::Knife)
						{
							srand((unsigned)time(NULL));
							int playAnimatorNum = rand() % 2;
							if (playAnimatorNum == 0)
							{
								m_fShootTime = 0.05f * 6;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR1_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
							else
							{
								m_fShootTime = 0.05f * 9;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionR2_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
					}
				}
			}
			else //(m_vecLookDir.x < 0)
			{
				if (m_vecLookDir.y < 0) // 위쪽을 보고 있는 경우
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriUpAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y == 0)
							HeavyGunGoingUp(L"L");
						else if (m_vecPrevLookDir.y < 0)
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUpL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else if (m_vecLookDir.y == 0)
				{
					if (m_bIsShoot)
					{
						if (m_bIsJump)
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
						else
							m_pAnimator2->Play(L"Player\\Idle\\EriIdleL_2");
					}

					if (m_curWeapon == PlayerWeapon::Pistol)
					{
						m_fShootTime = 0.05f * 10;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriAttackL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime > m_fShootTime)
						{
							m_bIsAttack = false;
						}
					}
					else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
					{
						if (m_vecPrevLookDir.y < 0)
						{
							HeavyGunGoingUpPlace(L"L");
						}
						else if (m_vecPrevLookDir.y > 0)
						{
							HeavyGunGoingDownPlace(L"L");
						}
						else
						{
							m_fShootTime = 0.05f * 4;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttackL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime > m_fShootTime)
							{
								m_bIsAttack = false;
							}
						}
					}
					else if (m_curWeapon == PlayerWeapon::Bomb)
					{
						m_fShootTime = 0.05f * 6;
						if (m_bIsShoot)
						{
							m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
							m_bIsShoot = false;
						}
						if (m_fAcctime >= m_fShootTime)
							m_bIsAttack = false;
					}
					else if (m_curWeapon == PlayerWeapon::Knife)
					{
						srand((unsigned)time(NULL));
						int playAnimatorNum = rand() % 2;
						if (playAnimatorNum == 0)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else
						{
							m_fShootTime = 0.05f * 9;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
					}
				}
				else // else if (m_vecLookDir.y > 0)
				{
					if (m_bIsJump)
					{
						if (m_bIsShoot)
						{
							m_pAnimator2->Play(L"Player\\Jump\\EriJumpL_2");
						}

						if (m_curWeapon == PlayerWeapon::Pistol)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriDownAttackL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
						{
							if (m_vecPrevLookDir.y == 0)
								HeavyGunGoingDown(L"L");
							else if (m_vecPrevLookDir.y > 0)
							{
								m_fShootTime = 0.05f * 4;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDownL_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
						else if (m_curWeapon == PlayerWeapon::Bomb)
						{
							m_fShootTime = 0.05f * 6;
							if (m_bIsShoot)
							{
								m_pAnimator1->Play(L"Player\\Attack\\EriBombL_1", true);
								m_bIsShoot = false;
							}
							if (m_fAcctime >= m_fShootTime)
								m_bIsAttack = false;
						}
						else if (m_curWeapon == PlayerWeapon::Knife)
						{
							srand((unsigned)time(NULL));
							int playAnimatorNum = rand() % 2;
							if (playAnimatorNum == 0)
							{
								m_fShootTime = 0.05f * 6;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL1_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
							else
							{
								m_fShootTime = 0.05f * 9;
								if (m_bIsShoot)
								{
									m_pAnimator1->Play(L"Player\\Attack\\EriKnifeMotionL2_1", true);
									m_bIsShoot = false;
								}
								if (m_fAcctime >= m_fShootTime)
									m_bIsAttack = false;
							}
						}
					}
				}
			}
		}
	}
		break;
	case PlayerStatus::SitAttack:
	{
		if (m_vecLookDir.x > 0) // 오른쪽을 보고 있을 경우
		{
			if (m_bIsShoot)
			{
				m_pAnimator2->Stop();
			}
			if (m_curWeapon == PlayerWeapon::Pistol)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitAttackR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
			{
				m_fShootTime = 0.05f * 7;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitHeavyGunR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Bomb)
			{
				m_fShootTime = 0.05f * 8;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitBombR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Knife)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitKnifeR", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
		}
		else if (m_vecLookDir.x < 0) // 왼쪽을 보고 있을 경우
		{
			if (m_bIsShoot)
			{
				m_pAnimator2->Stop();
			}
			if (m_curWeapon == PlayerWeapon::Pistol)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitAttackL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::HeavyMachineGun)
			{
				m_fShootTime = 0.05f * 7;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitHeavyGunL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Bomb)
			{
				m_fShootTime = 0.05f * 8;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitBombL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
			else if (m_curWeapon == PlayerWeapon::Knife)
			{
				m_fShootTime = 0.05f * 9;
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriSitKnifeL", true);
					m_bIsShoot = false;
				}
				if (m_fAcctime >= m_fShootTime)
					m_bIsAttack = false;
			}
		}
	}
		break;
	case PlayerStatus::Dead:
	{
		m_pAnimator2->Stop();

		if (m_vecLookDir.x > 0)
		{
			if (m_bIsJump)
			{
				m_pAnimator1->Play(L"Player\\Death\\EriDeathJumpL");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Death\\EriNearDeathL");
			}
		}
		else
		{
			if (m_bIsJump)
			{
				m_pAnimator1->Play(L"Player\\Death\\EriDeathJumpR");
			}
			else
			{
				m_pAnimator1->Play(L"Player\\Death\\EriNearDeathR");
			}
		}
	}
		break;
	default:
		break;
	}
}

void CPlayer::BehaviorUpdate()
{
	switch (m_status)
	{
	case PlayerStatus::Idle:
		break;
	case PlayerStatus::Move:
		m_vecPos.x += m_vecMoveDir.x * m_fSpeed * DT;
		break;
	case PlayerStatus::Jump:
		m_vecPos.x += m_vecMoveDir.x * m_fSpeed * DT;
		break;
	case PlayerStatus::Attack:
		m_vecPos.x += m_vecMoveDir.x * m_fSpeed * DT;

		break;
	case PlayerStatus::SitAttack:
		m_bIsSit = true;
		break;
	case PlayerStatus::Dead:
		if (!m_bIsDead)
		{
			m_bIsDead = true;
			m_gravity->SetVelocity(-300);
		}
		m_vecPos.x -= m_vecMoveDir.x * m_fSpeed * DT;
		break;
	}

}

void CPlayer::HeavyGunGoingDownPlace(wstring RorL)
{
	float goingDownPlaceTime = 0.03f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_bIsJump)
	{
		if (m_fAcctime < goingDownPlaceTime)
			m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDownPlace" + RorL + L"_1");
		else
		{
			if (m_fAcctime >= goingDownPlaceTime && m_fAcctime < goingDownPlaceTime + m_fShootTime)
			{
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttack" + RorL + L"_1", true);
					m_bIsShoot = false;
				}
			}
			if (m_fAcctime >= goingDownPlaceTime + m_fShootTime)
			{
				m_bIsAttack = false;
			}
		}
	}
}

void CPlayer::HeavyGunGoingDown(wstring RorL)
{
	float goingDownTime = 0.03f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_bIsJump)
	{
		if (m_fAcctime < goingDownTime)
			m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingDown" + RorL + L"_1");
		else
		{
			if (m_fAcctime >= goingDownTime && m_fAcctime < goingDownTime + m_fShootTime)
			{
				if (m_bIsShoot)
				{
					m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunDown" + RorL + L"_1", true);
					m_bIsShoot = false;
				}
			}
			if (m_fAcctime >= goingDownTime + m_fShootTime)
			{
				m_bIsAttack = false;
			}
		}
	}
}

void CPlayer::HeavyGunGoingUpPlace(wstring RorL)
{
	float goingUpPlaceTime = 0.02f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_fAcctime < goingUpPlaceTime)
		m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUpPlace" + RorL + L"_1");
	else
	{
		if (m_fAcctime >= goingUpPlaceTime && m_fAcctime < goingUpPlaceTime + m_fShootTime)
		{
			if (m_bIsShoot)
			{
				m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunAttack" + RorL + L"_1", true);
				m_bIsShoot = false;
			}
		}
		if (m_fAcctime >= goingUpPlaceTime + m_fShootTime)
		{
			m_bIsAttack = false;
		}
	}
}

void CPlayer::HeavyGunGoingUp(wstring RorL)
{
	float goingUpTime = 0.03f * 2;
	m_fShootTime = 0.1f * 4;

	if (m_fAcctime < goingUpTime)
		m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunGoingUp" + RorL + L"_1");
	else
	{
		if (m_fAcctime >= goingUpTime && m_fAcctime < goingUpTime + m_fShootTime)
		{
			if (m_bIsShoot)
			{
				m_pAnimator1->Play(L"Player\\Attack\\EriHeavyGunUp" + RorL + L"_1", true);
				m_bIsShoot = false;
			}
		}
		if (m_fAcctime >= goingUpTime + m_fShootTime)
		{
			m_bIsAttack = false;
		}
	}
}

void CPlayer::StatusUpdate()
{
	switch (m_status)
	{
	case PlayerStatus::Idle:
		if (m_hp == 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_vecMoveDir.y > 0)
		{
			m_status = PlayerStatus::Jump;
			m_fAcctime = 0;
		}
		//else if (m_bIsAttack && !m_bIsJump && m_vecLookDir.y > 0)
		//{
		//	m_status = PlayerStatus::SitAttack;
		//	m_fAcctime = 0;
		//}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (m_vecMoveDir.x != 0)
		{
			m_status = PlayerStatus::Move;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Move:
		if(m_hp == 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if(m_vecMoveDir.x == 0)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		//else if (m_bIsAttack && !m_bIsJump && m_vecLookDir.y > 0)
		//{
		//	m_status = PlayerStatus::SitAttack;
		//	m_fAcctime = 0;
		//}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (m_vecMoveDir.y >0)
		{
			m_status = PlayerStatus::Jump;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Jump:
		if (m_hp == 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
		}
		else if (m_bIsAttack)
		{
			m_status = PlayerStatus::Attack;
			m_fAcctime = 0;
		}
		else if (!m_bIsJump)
		{
			m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::Attack:
		if (m_hp == 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
			break;
		}
		else if (!m_bIsJump && m_vecLookDir.y > 0)
		{
			m_status = PlayerStatus::SitAttack;
			m_fAcctime = 0;
			break;
		}
		if (!m_bIsAttack)
		{
			m_vecPrevLookDir.y = 0;
			if (m_vecMoveDir.y < 0)
				m_status = PlayerStatus::Jump;
			else if (m_vecMoveDir.x != 0)// && !(m_vecLookDir.y > 0) && !(m_vecPrevLookDir.y > 0))
				m_status = PlayerStatus::Move;
			else
				m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
		}
		break;
	case PlayerStatus::SitAttack:
		if (m_hp == 0)
		{
			m_status = PlayerStatus::Dead;
			m_fAcctime = 0;
			m_bIsSit = false;
			break;
		}
		//if (m_vecMoveDir.y < 0)
		//{
		//	m_status = PlayerStatus::Jump;
		//	m_fAcctime = 0;
		//	m_bIsAttack = false;
		//	break;
		//}
		if (!m_bIsAttack)
		{
			m_vecPrevLookDir.y = 0;
			if (m_bIsJump || m_vecLookDir.y <= 0)
				m_status = PlayerStatus::Attack;
			else if (m_vecMoveDir.x != 0)// && !(m_vecLookDir.y > 0) && !(m_vecPrevLookDir.y > 0))
				m_status = PlayerStatus::Move;
			else
				m_status = PlayerStatus::Idle;
			m_fAcctime = 0;
			m_bIsSit = false;
			//else if (m_vecMoveDir.x == 0 && m_vecMoveDir.y <= 0)
			//	m_status = PlayerStatus::Idle;
		}
	case PlayerStatus::Dead:

		// 죽었으면 새로 부활
		break;
	}
}

void CPlayer::CreateMissile()
{
	Logger::Debug(L"미사일 생성");

	CMissile* pMissile = new CMissile();
	pMissile->SetName(L"PlayerMissile");
	pMissile->SetPos(m_vecPos);

	if (m_vecLookDir.y < 0)
	{
		if (m_bIsSit)
			pMissile->SetDir(Vector(m_vecLookDir.x, 0));
		else
			pMissile->SetDir(Vector(0, m_vecLookDir.y));
	}
	else if (m_vecLookDir.y > 0)
	{
		if(m_bIsJump)
			pMissile->SetDir(Vector(0, m_vecLookDir.y));
		else
			pMissile->SetDir(Vector(m_vecLookDir.x, 0));
	}
	else
	{
		pMissile->SetDir(Vector(m_vecLookDir.x, 0));
	}
	/*
	if (m_vecLookDir.y < 0)
		pMissile->SetDir(Vector(0, m_vecLookDir.y));
	else if (m_bIsJump && m_vecLookDir.y > 0)
		pMissile->SetDir(Vector(0, m_vecLookDir.y));
	else if(m_vecLookDir.x > 0)
		pMissile->SetDir(Vector(m_vecLookDir.x, 0));
	else if (m_vecLookDir.x < 0)
		pMissile->SetDir(Vector(m_vecLookDir.x, 0));
		*/
	pMissile->SetOwner(this);
	pMissile->SetExtension(m_fExtension);
	ADDOBJECT(pMissile);
}

void CPlayer::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (pOtherCollider->GetObjName() == L"frontOcean")
	{
		m_bIsJump = false;
		m_vecMoveDir.y = 0;
		m_bIsAttack = false;
		if (m_status == PlayerStatus::Dead)
			m_fSpeed = 0;
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
