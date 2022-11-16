#include "framework.h"
#include "CAniObject.h"

#include "CAnimator.h"
#include "CAnimation.h"

CAniObject::CAniObject()
{
	m_strName = L"AniObj";
	m_layer = Layer::BackGround;
	m_pImg = nullptr;
	m_vecOffset = Vector(0, 0);
	m_pAnimator = new CAnimator;
	m_fAlpha = 1;
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
	Logger::Debug(to_wstring(m_vecScale.x) + L", " + to_wstring(m_vecScale.y));
}

void CAniObject::Update()
{
	if (m_pAnimator->GetCurAni() != nullptr)
	{
		m_pAnimator->GetCurAni()->SetAlpha(m_fAlpha);
	}
}

void CAniObject::Render()
{
	AniFrame frame = m_pAnimator->GetFirstAniFrame();
	RENDER->FrameRect(
		m_vecPos.x + (-1 * frame.slice.x * 0.5f + frame.offset.x) * m_fExtension,
		m_vecPos.y + (-1 * frame.slice.y * 0.5f + frame.offset.y) * m_fExtension,
		m_vecPos.x + (frame.slice.x * 0.5f + frame.offset.x) * m_fExtension,
		m_vecPos.y + (frame.slice.y * 0.5f + frame.offset.y) * m_fExtension);
}

void CAniObject::Release()
{
}

void CAniObject::SetPosWithFirstLt()
{
	AniFrame firstAniFrame = m_pAnimator->GetFirstAniFrame();
	// 좌표를 왼쪽 위 좌표로 설정할 수 있도록 변환
	m_vecPos += (firstAniFrame.slice * 0.5f - firstAniFrame.offset+ m_vecOffset)* m_fExtension;
	m_vecScale = firstAniFrame.slice * 0.5f * m_fExtension;
	// Logger::Debug(to_wstring(firstAniFrame.lt.x) + L", " + to_wstring(firstAniFrame.lt.y));
	// Logger::Debug(to_wstring(firstAniFrame.slice.x) + L", " + to_wstring(firstAniFrame.slice.y));
	// Logger::Debug(to_wstring(firstAniFrame.offset.x) + L", " + to_wstring(firstAniFrame.offset.y));
	// Logger::Debug(to_wstring(m_vecPos.x) + L", " + to_wstring(m_vecPos.y));
}