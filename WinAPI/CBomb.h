#pragma once
#include "CGameObject.h"

class CBomb : public CGameObject
{
public:
	CBomb();
	virtual ~CBomb();

private:
	Vector m_vecDir;
	float m_fVelocity;

	CAnimator* m_pAnimator;
	CGameObject* m_pOwner;

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};