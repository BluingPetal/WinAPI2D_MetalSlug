#pragma once
#include "CGameObject.h"

enum class NPCStatus {Trapped, Free, Walk, Give, Run};

class CNPC : public CGameObject
{
public:
	CNPC();
	virtual ~CNPC();

private:
	CAnimator* m_pNPCAni;
	CGravity* m_pGravity;

	NPCStatus m_status;
	float m_fAccTime;
	float m_fSpeed;

	bool m_isFree;
	bool m_isMeet;
	bool m_isGave;

public:
	void SetMeet(bool meet) { m_isMeet = meet; }
	NPCStatus GetStatus() { return m_status; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void BehaviorUpdate();
	void StateUpdate();
	void AniUpdate();

	void GiveItem();

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};

