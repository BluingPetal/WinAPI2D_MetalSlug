#pragma once
#include "CButton.h"

class CImage;

class CImageButton : public CButton
{
public:
	CImageButton();
	virtual ~CImageButton();

private:
	CImage* m_pImage;

public:
	void SetImage(CImage* img) { m_pImage = img; }

private:
	void Init() override;
	void Render() override;
	void Update() override;
	void Release() override;
};

