#pragma once
#include "CGameObject.h"

class CCollider;
class CGameObject;

class CColliderObject : public CGameObject
{
public:
	CColliderObject();
	virtual ~CColliderObject(); 

private:
	CCollider* m_pCollider;
	Vector m_vecOffset;
	CGameObject* m_pOwner;

public:
	CCollider* GetCollider();

	void SetOffset(Vector offset) { m_vecOffset = offset; }
	CGameObject* GetColliderObjOwner();
	void SetOwner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider);
	void OnCollisionStay(CCollider* pOtherCollider);
	void OnCollisionExit(CCollider* pOtherCollider);
};

