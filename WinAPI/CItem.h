#pragma once
#include "CGameObject.h"

class CAnimator;

class CItem : public CGameObject
{
public:
	CItem();
	virtual ~CItem();

private:
	CAnimator* m_pItemAni;
	CGravity* m_pGravity;

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};