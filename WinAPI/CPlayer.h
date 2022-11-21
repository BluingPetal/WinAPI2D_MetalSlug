#pragma once
#include "CGameObject.h"

class CImage;
class CAnimator;
class CAniObject;
class CGravity;
class CPlayerMissile;

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
	CImage* m_pEffectImage;

	CAnimator* m_pAnimator1;
	CAnimator* m_pAnimator2;
	CAnimator* m_explosionAni;
	//CAniObject* m_pMissileAniObj;
	//CPlayerMissile* m_pMissile;

	list<CPlayerMissile*> m_listMissile;
	list<pair<float*, CAniObject*>> m_listMissileAniObj;

	float m_fMissileDisappearAccTime;

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
	bool m_bIsSit;

	bool m_bIsPrevCurLookSame;

	int m_hp;
	float m_fSpeed;
	float m_fAcctime;
	float m_fShootTime;

	int m_iBomb;
	int m_iBullet;

	bool m_bBombDiff;
	bool m_bBulletDiff;

	PlayerWeapon m_curWeapon;
	PlayerWeapon m_curGun;

public:
	bool GetIsJump() { return m_bIsJump; }
	float GetSpeed() { return m_fSpeed; }
	Vector GetMoveDir() { return m_vecMoveDir; }
	bool GetIsDead() { return m_bIsDead; }
	void SetHp(int hp) { m_hp = hp; }
	int GetHp() { return m_hp; }
	int GetBullet() { return m_iBullet; }
	int GetBomb() { return m_iBomb; }
	bool GetBulletDiff() { return m_bBulletDiff; }
	bool GetBombDiff() { return m_bBombDiff; }
	PlayerWeapon GetCurWeapon() { return m_curWeapon; }
	void SetCurWeapon(PlayerWeapon weapon) { m_curWeapon = weapon; }
	void SetStatus(PlayerStatus status) { m_status = status; }

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