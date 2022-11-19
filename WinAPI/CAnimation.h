#pragma once

class CAnimator;
class CImage;

struct AniFrame
{
	Vector	lt;
	Vector	slice;
	Vector	offset;
	float	duration;
};

class CAnimation
{
	friend CAnimator;
public:
	CAnimation();
	virtual ~CAnimation();

private:
	CAnimator*			m_pAnimator;	// 애니메이터

	wstring				m_strName;		// 애니메이션 이름
	CImage*				m_pImage;		// 애니메이션 이미지
	vector<AniFrame>	m_vecFrame;		// 애니메이션의 프레임 정보들
	int					m_iCurFrame;	// 현재 플레이중인 프레임의 인덱스
	float				m_fAccTime;		// 현재 플레이중인 프레임의 축적시간
	bool				m_bRepeat;		// 애니메이션의 반복 여부
	UINT				m_iCount;
	float				m_fExtension;
	AniFrame			m_FirstAniFrame;
	bool				m_bReversePlay;
	float				m_fAlpha;

	Vector*				m_vecReadAni;					// 읽어온 애니메이션
public:
	const wstring& GetName();
	void SetAlpha(float alpha) { m_fAlpha = alpha; }

private:
	void SetName(const wstring& name);
	void SetReversePlay(bool reversePlay) { m_bReversePlay = reversePlay; }
	void SetExtension(float extension) { m_fExtension = extension; }
	void LoadAni(const wstring& aniName);

	void Create(CImage* pImg, float duration, bool repeat);	// 애니메이션 생성
	void Replay();	// 애니메이션 처음부터 다시 시작

private:
	void Init();
	void Update();
	void Render();
	void Release();
};
