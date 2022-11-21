#pragma once
#include "CMissile.h"

class CImage;
class CAniObject;

class CPlayerMissile : public CMissile
{
public:
	CPlayerMissile();
	virtual ~CPlayerMissile();

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
	CImage* m_pEffectImage;
	CAniObject* m_pMissileAniObj;
	float m_fDisappearAccTime;
	bool m_reserveDelete;
	bool m_bCreatedAni;

public:
	bool GetMissileDelete() { return m_reserveDelete; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};
