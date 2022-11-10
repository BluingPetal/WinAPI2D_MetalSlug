#pragma once
#include "CMonster.h"

class CImage;
class CAnimator;
class CGravity;

class CConga : public CMonster
{
public:
	CConga();
	virtual ~CConga();

private:
	CImage* m_pCongaTotalImgL;
	CImage* m_pCongaTotalImgR;

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