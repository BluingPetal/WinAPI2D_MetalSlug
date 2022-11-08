#include "framework.h"
#include "CAniObject.h"

#include "CAnimator.h"

CAniObject::CAniObject()
{
	m_pImg = nullptr;
	m_vecPos = Vector(0, 0);
	m_layer = Layer::BackGround;
	m_vecOffset = Vector(0, 0);
	m_pAnimator = new CAnimator;
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

void CAniObject::Init()
{
	AddComponent(m_pAnimator);
}

void CAniObject::Update()
{
}

void CAniObject::Render()
{
}

void CAniObject::Release()
{
}

void CAniObject::SetPosWithFirstLt()
{
	AniFrame firstAniFrame = m_pAnimator->GetFirstAniFrame();
	// ��ǥ�� ���� �� ��ǥ�� ������ �� �ֵ��� ��ȯ
	m_vecPos += (firstAniFrame.slice * 0.5f - firstAniFrame.offset+ m_vecOffset)* m_fExtension;
	Logger::Debug(to_wstring(firstAniFrame.lt.x) + L", " + to_wstring(firstAniFrame.lt.y));
	Logger::Debug(to_wstring(firstAniFrame.slice.x) + L", " + to_wstring(firstAniFrame.slice.y));
	Logger::Debug(to_wstring(firstAniFrame.offset.x) + L", " + to_wstring(firstAniFrame.offset.y));
	Logger::Debug(to_wstring(m_vecPos.x) + L", " + to_wstring(m_vecPos.y));
}
