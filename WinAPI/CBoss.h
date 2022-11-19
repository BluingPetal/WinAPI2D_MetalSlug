#pragma once
#include "CMonster.h"

class CMissile;
class CAniObject;
enum class BossStatus { Idle, Walk, FastWalk, Fire, DeployCannon, CannonWalk, CannonFastWalk, CannonFire, Destroyed };

class CBoss : public CMonster
{
public:
	CBoss();
	virtual ~CBoss();

private:
	CAnimator* m_pAnimator;
	BossStatus m_bossStatus;
	float m_fSpeed;
	Vector m_vecMoveDir;
	float m_fAccTime;
	float m_fAttackAccTime;
	float m_fFireDisappearAccTime;
	float m_fMissileDisappearAccTime;
	bool m_bIsAttack;

	CMissile* pFireMissile;
	CMissile* pMissile;
	CAniObject* m_pFireMissileAniObj;
	CAniObject* m_pMissileAniObj;
	CImage* m_pEffectImg;

public:
	BossStatus GetStatus() { return m_bossStatus; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void BehaviorUpdate();
	void StateUpdate();
	void AniUpdate();

	void CreateFireMissile();
	void CreateMissile();

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};

