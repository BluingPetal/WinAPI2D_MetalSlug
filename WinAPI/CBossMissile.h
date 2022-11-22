#pragma once
#include "CMissile.h"
class CBossMissile : public CMissile
{
public:
	CBossMissile();
	virtual ~CBossMissile();

private:
	//Vector m_vecDir;
	//float m_fVelocity;
	//CGameObject* m_pOwner;
	//
	//CAnimator* m_pAnimator;
	//float m_fDisappearAccTime;
	//float m_fAttackAccTime;
	//bool m_bIsEntered;
	//CGravity* m_pGravity;
	CSound* m_pExplode;

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};

