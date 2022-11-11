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
	m_fSpeed = 0;
	m_bRenderAsFrame = false;
	m_fAlpha = 1;
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

void CImageObject::SetSourceInfo(float startX, float startY, float width, float height)
{
	m_fRenderSourceInfo[0] = startX;
	m_fRenderSourceInfo[1] = startY;
	m_fRenderSourceInfo[2] = width;
	m_fRenderSourceInfo[3] = height;
}

void CImageObject::Init()
{
}

void CImageObject::Update()
{
	m_vecPos.x += m_fSpeed * DT;
}

void CImageObject::Render()
{
	if (nullptr != m_pImage)
	{
		if (m_bRenderAsFrame)
		{
			RENDER->FrameImage(
				m_pImage,
				m_vecPos.x + m_vecOffset.x * m_fExtension,
				m_vecPos.y + m_vecOffset.y * m_fExtension,
				m_vecPos.x + ((float)m_fRenderSourceInfo[2] + m_vecOffset.x) * m_fExtension,
				m_vecPos.y + ((float)m_fRenderSourceInfo[3] + m_vecOffset.y) * m_fExtension,
				m_fRenderSourceInfo[0],
				m_fRenderSourceInfo[1],
				m_fRenderSourceInfo[0] + m_fRenderSourceInfo[2],
				m_fRenderSourceInfo[1] + m_fRenderSourceInfo[3], m_fAlpha);
		}
		else
		{
			RENDER->Image(
				m_pImage,
				m_vecPos.x + m_vecOffset.x * m_fExtension,
				m_vecPos.y + m_vecOffset.y * m_fExtension,
				m_vecPos.x + ((float)m_pImage->GetWidth() + m_vecOffset.x) * m_fExtension,
				m_vecPos.y + ((float)m_pImage->GetHeight() + m_vecOffset.y) * m_fExtension);
		}
	}
}

void CImageObject::Release()
{
}
