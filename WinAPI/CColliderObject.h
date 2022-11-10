#pragma once
#include "CGameObject.h"

class CCollider;

class CColliderObject : public CGameObject
{
public:
	CColliderObject();
	virtual ~CColliderObject(); 

private:
	CCollider* m_pCollider;
	Vector m_vecOffset;

public:
	CCollider* GetCollider();

	void SetOffset(Vector offset) { m_vecOffset = offset; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

