#pragma once
#include "CGameObject.h"

class CImage;

class CImageObject : public CGameObject
{
public:
	CImageObject();
	virtual ~CImageObject();

protected:
	CImage* m_pImage;
	Vector m_vecOffset;
	float m_fSpeed;
	bool m_bIsFixed;
	int m_index;

	bool m_bRenderAsFrame;
	float m_fRenderSourceInfo[4];
	float m_fAlpha;

public:
	CImage* GetImage();
	void SetImage(CImage* pImage);
	void SetSpeed(float speed) { m_fSpeed = speed; }
	float GetSpeed() { return m_fSpeed; }
	void SetRenderAsFrame(bool isFrame) { m_bRenderAsFrame = isFrame; }
	void SetOffset(Vector vecOffset) { m_vecOffset = vecOffset; }
	void SetFixed(bool fixed) { m_bIsFixed = fixed; }
	void SetAlpha(float alpha) { m_fAlpha = alpha; }
	float GetAlpha() { return m_fAlpha; }
	Vector GetOffset() { return m_vecOffset; }
	void SetIndex(int index) { m_index = index; }
	int GetIndex() { return m_index; }
	void SetSourceInfo(float startX, float startY, float width, float height);

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

