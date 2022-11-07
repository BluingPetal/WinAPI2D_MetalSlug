#pragma once
#include "CGameObject.h"

class CImage;
class CAnimator;

class CMonster : public CGameObject
{
public:
	CMonster();
	virtual ~CMonster();

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

