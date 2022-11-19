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
	//CCollider* m_pCollider;
	Vector m_vecOffset;
	CGameObject* m_pOwner;
	ColliderType m_colliderType;
	float m_fRot;
	int m_index;

public:
	//CCollider* GetCollider();

	void SetOffset(Vector offset) { m_vecOffset = offset; }
	Vector GetOffset() { return m_vecOffset; }
	ColliderType GetColliderType() { return m_colliderType; }
	CGameObject* GetColliderObjOwner();
	void SetOwner(CGameObject* pOwner) { m_pOwner = pOwner; }
	void SetType(ColliderType type) { m_colliderType = type; }
	void SetRot(float rotation) { m_fRot = rotation; }
	void SetIndex(int index) { m_index = index; }
	int GetIndex() { return m_index; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider);
	void OnCollisionStay(CCollider* pOtherCollider);
	void OnCollisionExit(CCollider* pOtherCollider);
};

