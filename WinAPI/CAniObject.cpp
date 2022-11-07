#include "framework.h"
#include "CAniObject.h"

CAniObject::CAniObject()
{
	m_pImg = nullptr;
	m_vecPos = Vector(0, 0);
	m_layer = Layer::ForeGround;
	m_vecOffset = Vector(0, 0);
	m_pAnimator = nullptr;
}

CAniObject::~CAniObject()
{
}

CAnimator* CAniObject::GetAnimator()
{
    return m_pAnimator;
}

void CAniObject::SetImage(CImage* pImage)
{
	m_pImg = pImage;
}

void CAniObject::SetPosWithFirstLt()
{
	AniFrame firstAniFrame = m_pAnimator->GetFirstAniFrame();
	m_vecPos = firstAniFrame.lt + firstAniFrame.slice * 0.5f + firstAniFrame.offset;
}

void CAniObject::Init()
{
	m_pAnimator = new CAnimator;
	AddComponent(m_pAnimator);


}

void CAniObject::Update()
{
	SetPosWithFirstLt();
}

void CAniObject::Render()
{
}

void CAniObject::Release()
{
}
