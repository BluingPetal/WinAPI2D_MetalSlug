#pragma once
#include "CGameObject.h"

class CImage;
class CAnimator;

class CMonster : public CGameObject
{
public:
	CMonster();
	virtual ~CMonster();

protected:
	Vector m_vecMoveDir;
	Vector m_vecLookDir;
	float m_fSpeed;
	int m_hp;

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

