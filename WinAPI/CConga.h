#pragma once
#include "CMonster.h"

class CImage;
class CAnimator;
class CGravity;
class CColliderObject;
class CGameObject;
enum class CongaStatus { Idle, Walk, NearAttack, Death };

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
	int m_hp;
	bool m_isAttack;

public:
	void SetTarget(CGameObject* target) { m_pTargetObj = target; }
	void SetCongaState(CongaStatus status) { m_congaState = status; }
	CongaStatus GetCongaState() { return m_congaState; }
	float GetAccTime() { return m_fAccTime; }
	void SetAccTime(float time) { m_fAccTime = time;}
	void SetHp(int hp) { m_hp = hp; }
	int GetHp() { return m_hp; }

	void CongaAddObject();

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;

	void StateUpdate();
	void AnimationUpdate();
};