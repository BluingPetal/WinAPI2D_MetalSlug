#pragma once
#include "CGameObject.h"
class CGravity;
class CAnimator;
class CCollider;
class CAniObject;
class CImage;

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
	CGravity* m_pGravity;

	CImage* m_pEffectImage;
	CAniObject* m_pBombAniObj;
	float m_fDisappearAccTime;
	bool m_reserveDelete;
	bool m_bCreatedAni;

	CSound* m_pExplode;

	int count;

public:
	void SetDir(Vector dir) { m_vecDir = dir; }
	void SetOwner(CGameObject* owner) { m_pOwner = owner; }
	CGameObject* GetOwner();

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};