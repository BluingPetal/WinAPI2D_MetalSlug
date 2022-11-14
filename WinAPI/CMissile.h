#pragma once
#include "CGameObject.h"

class CAnimator;

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
	float m_fAccTime;
	bool m_bIsEntered;

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
};