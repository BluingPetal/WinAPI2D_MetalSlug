#pragma once
#include "CGameObject.h"

class CImage;
class CAnimator;

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

private:
	CImage* m_pIdleImage;
	CImage* m_pMoveImage;
	CImage* m_pJumpImage;
	CImage* m_pAttackRImage;
	CImage* m_pAttackLImage;
	CImage* m_pDeathImage;

	CAnimator* m_pAnimator1;
	CAnimator* m_pAnimator2;

	Vector m_vecMoveDir;
	Vector m_vecLookDir;

	bool m_bIsMove;

	float m_fSpeed = 200.0f;
	float test = 0;

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void AnimatorUpdate();
	void CreateMissile();

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};