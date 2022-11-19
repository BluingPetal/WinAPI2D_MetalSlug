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
	CAnimator*			m_pAnimator;	// �ִϸ�����

	wstring				m_strName;		// �ִϸ��̼� �̸�
	CImage*				m_pImage;		// �ִϸ��̼� �̹���
	vector<AniFrame>	m_vecFrame;		// �ִϸ��̼��� ������ ������
	int					m_iCurFrame;	// ���� �÷������� �������� �ε���
	float				m_fAccTime;		// ���� �÷������� �������� �����ð�
	bool				m_bRepeat;		// �ִϸ��̼��� �ݺ� ����
	UINT				m_iCount;
	float				m_fExtension;
	AniFrame			m_FirstAniFrame;
	bool				m_bReversePlay;
	float				m_fAlpha;

	Vector*				m_vecReadAni;					// �о�� �ִϸ��̼�
public:
	const wstring& GetName();
	void SetAlpha(float alpha) { m_fAlpha = alpha; }

private:
	void SetName(const wstring& name);
	void SetReversePlay(bool reversePlay) { m_bReversePlay = reversePlay; }
	void SetExtension(float extension) { m_fExtension = extension; }
	void LoadAni(const wstring& aniName);

	void Create(CImage* pImg, float duration, bool repeat);	// �ִϸ��̼� ����
	void Replay();	// �ִϸ��̼� ó������ �ٽ� ����

private:
	void Init();
	void Update();
	void Render();
	void Release();
};
