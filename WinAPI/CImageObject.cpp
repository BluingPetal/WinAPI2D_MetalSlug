#include "framework.h"
#include "CImageObject.h"

#include "CRenderManager.h"

#include "CImage.h"

CImageObject::CImageObject()
{
	m_pImage = nullptr;
	m_vecPos = Vector(0, 0);
	m_layer = Layer::BackGround;
	m_vecOffset = Vector(0, 0);
}

CImageObject::~CImageObject()
{
}

CImage* CImageObject::GetImage()
{
    return m_pImage;
}

void CImageObject::SetImage(CImage* pImage)
{
	m_pImage = pImage;
}

void CImageObject::Init()
{
}

void CImageObject::Update()
{
}

void CImageObject::Render()
{
	if (nullptr != m_pImage)
	{
		RENDER->Image(
			m_pImage,
			m_vecPos.x + m_vecOffset.x * m_fExtension,
			m_vecPos.y + m_vecOffset.y * m_fExtension,
			m_vecPos.x +((float)m_pImage->GetWidth() + m_vecOffset.x) * m_fExtension,
			m_vecPos.y + ((float)m_pImage->GetHeight() + m_vecOffset.y)* m_fExtension);
	}
}

void CImageObject::Release()
{
}
