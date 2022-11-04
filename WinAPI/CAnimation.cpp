#include "framework.h"
#include "CAnimation.h"

#include "CTimeManager.h"
#include "CRenderManager.h"
#include "CAnimator.h"
#include "CGameObject.h"

CAnimation::CAnimation()
{
	m_pAnimator = nullptr;

	m_strName = L"";
	m_pImage = nullptr;
	m_iCurFrame = 0;
	m_fAccTime = 0;
	m_bRepeat = true;
}

CAnimation::~CAnimation()
{
}

const wstring& CAnimation::GetName()
{
	return m_strName;
}

void CAnimation::SetName(const wstring& name)
{
	m_strName = name;
}

void CAnimation::LoadAni(const wstring& aniName)
{
	FILE* pFile = nullptr;

	wstring path = PATH->GetPath() + L"\\AniData\\" +aniName + L".ani";
	_wfopen_s(&pFile, path.c_str(), L"rb");      // w : write, b : binary
	assert(pFile);

	int count;
	Vector lt;
	Vector slice;
	Vector offset;

	fread(&count, sizeof(UINT), 1, pFile);
	m_vecReadAni = new Vector[count * 3];
	m_count = count;
	for (int i = 0; i < count * 3; i+=3)
	{
		fread(&lt, sizeof(Vector), 1, pFile);
		fread(&slice, sizeof(Vector), 1, pFile);
		fread(&offset, sizeof(Vector), 1, pFile);
		m_vecReadAni[i] = lt;
		m_vecReadAni[i + 1] = slice;
		m_vecReadAni[i + 2] = offset;
	}

	fclose(pFile);
}

void CAnimation::Create(CImage* pImg, float duration, bool repeat)
{
	m_pImage = pImg;	// 프레임 이미지가 모여있는 이미지 파일
	m_bRepeat = repeat;	// 반복여부

	// 바둑판식 프레임 이미지를 가리킬 위치를 저장
	// lt		: 프레임 이미지의 좌측상단 좌표
	// step		: 프레임 이미지의 간격
	// slice	: 프레임 이미지의 크기
	// duration : 프레임 이미지의 지속시간
	AniFrame frame;
	for (UINT i = 0; i < m_count * 3; i+=3)
	{
		frame.lt = m_vecReadAni[i];
		frame.slice = m_vecReadAni[i+1];
		frame.offset = m_vecReadAni[i + 2];
		frame.duration = duration;

		m_vecFrame.push_back(frame);
	}
	if (m_vecReadAni != nullptr)
	{
		delete[] m_vecReadAni;
		m_vecReadAni = nullptr;
	}
}

void CAnimation::Replay()
{
	// 애니메이션 재시작 : 현재 프레임과 누적시간을 초기화
	m_iCurFrame = 0;
	m_fAccTime = 0;
}

void CAnimation::Init()
{
}

void CAnimation::Update()
{
	// 현재 플레이중인 프레임의 누적시간
	m_fAccTime += DT;

	// 누적시간이 현재 플레이중인 프레임의 지속시간보다 커졌을 경우
	// -> 다음 프레임을 보여줘야 하는 경우
	if (m_vecFrame[m_iCurFrame].duration < m_fAccTime)
	{
		m_iCurFrame++;	// 현재 플레이중인 프레임의 인덱스를 하나 증가
		m_fAccTime = 0;	// 현재 플레이중인 프레임의 누적시간 초기화

		// 만약 플레이중인 프레임이 마지막 프레임이었을 경우
		if (m_iCurFrame == m_vecFrame.size())
		{
			// 반복 애니메이션이라면 처음부터, 아니라면 마지막을 다시 재생
			if (m_bRepeat)	m_iCurFrame = 0;
			else			m_iCurFrame--;
		}
	}
}

void CAnimation::Render()
{
	Vector pos = m_pAnimator->GetOwner()->GetPos();	// 애니메이션이 그려질 위치 확인
	AniFrame frame = m_vecFrame[m_iCurFrame];		// 애니메이션이 그려질 프레임 확인

	// 프레임 이미지 그리기
	RENDER->FrameImage(
		m_pImage,
		pos.x - frame.slice.x * 0.5f,
		pos.y - frame.slice.y * 0.5f,
		pos.x + frame.slice.x * 0.5f,
		pos.y + frame.slice.y * 0.5f,
		frame.lt.x,
		frame.lt.y,
		frame.lt.x + frame.slice.x,
		frame.lt.y + frame.slice.y
	);
}

void CAnimation::Release()
{
}
