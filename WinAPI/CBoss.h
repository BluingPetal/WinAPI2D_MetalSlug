#pragma once
#include "CMonster.h"

enum class BossStatus { Idle, Walk, NearAttack, Death };

class CBoss : public CMonster
{
public:
	CBoss();
	virtual ~CBoss();

private:
	CAnimator* m_pAnimator;

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};

