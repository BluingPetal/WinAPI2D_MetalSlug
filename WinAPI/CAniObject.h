#pragma once
#include "CGameObject.h"

class CAnimator;
class CImage;

class CAniObject : public CGameObject
{
public:
	CAniObject();
	virtual ~CAniObject();

private:
	CImage* m_pImg;
	CAnimator* m_pAnimator;
	Vector m_vecOffset;
	float m_fAlpha;

public:
	CAnimator* GetAnimator();
	void SetImage(CImage* pImage);
	void SetOffset(Vector vecOffset) { m_vecOffset = vecOffset; }
	void SetPosWithFirstLt();
	void SetAlpha(float alpha) { m_fAlpha = alpha; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

