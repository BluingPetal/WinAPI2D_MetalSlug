#pragma once
#include "CGameObject.h"

class CImage;
class CAnimator;
class CGravity;

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

private:
	CImage* m_pIdleImage;
	CImage* m_pMoveImage;
	CImage* m_pJumpImage;
	CImage* m_pAttackRImage;
	CImage* m_pAttackLImage;
	CImage* m_pDeathImage;

	CAnimator* m_pAnimator1;
	CAnimator* m_pAnimator2;

	CGravity* m_gravity;

	PlayerStatus m_prevStatus;
	PlayerStatus m_status;

	Vector m_vecMoveDir;
	Vector m_vecLookDir, m_vecPrevLookDir, tempLookDir;

	bool m_bIsMove;
	bool m_bIsJump;
	bool m_bIsDead;
	bool m_bIsAttack;
	bool m_bIsShoot;

	bool m_bIsPrevCurLookSame;

	int m_hp;
	float m_fSpeed;
	float m_fAcctime;

	PlayerWeapon m_curWeapon;

public:
	bool GetIsJump() { return m_bIsJump; }
	float GetSpeed() { return m_fSpeed; }
	Vector GetMoveDir() { return m_vecMoveDir; }
	void SetHp(int hp) { m_hp = hp; }
	int GetHp() { return m_hp; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void KeyUpdate();
	void BehaviorUpdate();
	void AnimatorUpdate();
	void StatusUpdate();
	void CreateMissile();

	void HeavyGunGoingDown(wstring RorL);
	void HeavyGunGoingDownPlace(wstring RorL);
	void HeavyGunGoingUp(wstring RorL);
	void HeavyGunGoingUpPlace(wstring RorL);

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};