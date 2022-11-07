#pragma once
#include "CGameObject.h"

class CImage;

class CImageObject : public CGameObject
{
public:
	CImageObject();
	virtual ~CImageObject();

private:
	CImage* m_pImage;
	Vector m_vecOffset;

public:
	CImage* GetImage();
	void SetImage(CImage* pImage);
	void SetOffset(Vector vecOffset) { m_vecOffset = vecOffset; }

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

