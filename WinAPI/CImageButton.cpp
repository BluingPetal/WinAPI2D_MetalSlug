#include "framework.h"
#include "CImageButton.h"

CImageButton::CImageButton()
{
	m_pImage = nullptr;
}

CImageButton::~CImageButton()
{
}

void CImageButton::Init()
{
}

void CImageButton::Render()
{
	if (m_pImage != nullptr)
	{
		RENDER->Image(
			m_pImage,
			m_vecRenderPos.x,
			m_vecRenderPos.y,
			m_vecRenderPos.x + m_vecScale.x,
			m_vecRenderPos.y + m_vecScale.y
		);
	}
}

void CImageButton::Update()
{
}

void CImageButton::Release()
{
}
