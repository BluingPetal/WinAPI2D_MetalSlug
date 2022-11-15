#pragma once
#include "CComponent.h"
class CGravity : public CComponent
{
public:
	CGravity();
	virtual ~CGravity();

private:
	float m_fVelY;
	float m_fGravity;

public:
	void SetVelocity(float velocity) { m_fVelY = velocity; }
	float GetVelocity() { return m_fVelY; }
	void SetGravity(float gravity) { m_fGravity = gravity; }

private:
	void Init()				override;
	void Update()			override;
	void PhysicsUpdate()	override;
	void Render()			override;
	void Release()			override;
};

