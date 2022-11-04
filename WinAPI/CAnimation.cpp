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
	m_pImage = pImg;	// ������ �̹����� ���ִ� �̹��� ����
	m_bRepeat = repeat;	// �ݺ�����

	// �ٵ��ǽ� ������ �̹����� ����ų ��ġ�� ����
	// lt		: ������ �̹����� ������� ��ǥ
	// step		: ������ �̹����� ����
	// slice	: ������ �̹����� ũ��
	// duration : ������ �̹����� ���ӽð�
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
	// �ִϸ��̼� ����� : ���� �����Ӱ� �����ð��� �ʱ�ȭ
	m_iCurFrame = 0;
	m_fAccTime = 0;
}

void CAnimation::Init()
{
}

void CAnimation::Update()
{
	// ���� �÷������� �������� �����ð�
	m_fAccTime += DT;

	// �����ð��� ���� �÷������� �������� ���ӽð����� Ŀ���� ���
	// -> ���� �������� ������� �ϴ� ���
	if (m_vecFrame[m_iCurFrame].duration < m_fAccTime)
	{
		m_iCurFrame++;	// ���� �÷������� �������� �ε����� �ϳ� ����
		m_fAccTime = 0;	// ���� �÷������� �������� �����ð� �ʱ�ȭ

		// ���� �÷������� �������� ������ �������̾��� ���
		if (m_iCurFrame == m_vecFrame.size())
		{
			// �ݺ� �ִϸ��̼��̶�� ó������, �ƴ϶�� �������� �ٽ� ���
			if (m_bRepeat)	m_iCurFrame = 0;
			else			m_iCurFrame--;
		}
	}
}

void CAnimation::Render()
{
	Vector pos = m_pAnimator->GetOwner()->GetPos();	// �ִϸ��̼��� �׷��� ��ġ Ȯ��
	AniFrame frame = m_vecFrame[m_iCurFrame];		// �ִϸ��̼��� �׷��� ������ Ȯ��

	// ������ �̹��� �׸���
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
