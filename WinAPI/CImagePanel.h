#pragma once
#include "CPanel.h"
class CImagePanel : public CPanel
{
public:
	CImagePanel();
	virtual ~CImagePanel();

private:
	CImage* m_pImage;

	UINT m_uiImgWidth;
	UINT m_uiImgHeight;

private:
	void Init();
	void Update();
	void Render();
	void Release();
};

