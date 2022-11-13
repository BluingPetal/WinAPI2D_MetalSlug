#pragma once
#include "CMonster.h"

class CImage;
class CAnimator;
class CGravity;
class CColliderObject;
class CGameObject;
enum class CongaStatus { Idle, Walk, NearAttack, FarAttack, Death };

class CConga : public CMonster
{
public:
	CConga();
	virtual ~CConga();

private:
	CAnimator* m_pAnimator;
	CGameObject* m_pTargetObj;
	CColliderObject* m_pFarColliderObj;
	CColliderObject* m_pNearColliderObj;

	CongaStatus m_congaState;
	Vector m_vecMoveDir;
	Vector m_vecLookDir;
	float m_fSpeed;

	float m_fAccTime;

public:
	void SetTarget(CGameObject* target) { m_pTargetObj = target; }
	void SetCongaState(CongaStatus status) { m_congaState = status; }
	void SetAccTime(float time) { m_fAccTime = time;}

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};