#pragma once
#include "CGameObject.h"

class CAnimator;
class CGravity;

class CMissile : public CGameObject
{
public:
	CMissile();
	virtual ~CMissile();

private:
	Vector m_vecDir;
	float m_fVelocity;
	CGameObject* m_pOwner;

	CAnimator* m_pAnimator;
	float m_fDisappearAccTime;
	float m_fAttackAccTime;
	bool m_bIsEntered;
	CGravity* m_pGravity;

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;

public:
	void SetDir(Vector dir);
	void SetVelocity(float velocity);
	void SetOwner(CGameObject* owner) { m_pOwner = owner; }
	CGameObject* GetOwner() { return m_pOwner; }
	CAnimator* GetAnimator();
};