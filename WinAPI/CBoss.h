#pragma once
#include "CMonster.h"

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

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void BehaviorUpdate();
	void StateUpdate();
	void AniUpdate();

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};

