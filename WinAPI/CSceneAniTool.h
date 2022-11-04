#pragma once
#include "CScene.h"

class CAnimator;
class CAnimation;
class CImage;
struct AniFrame;
class CPanel;

class CSceneAniTool : public CScene
{
	friend CAnimator;
	friend CAnimation;
public:
	CSceneAniTool();
	virtual ~CSceneAniTool();

private:
	HWND m_hWndAniTool;
	CImage* m_pImage1;
	CImage* m_pImage2;
	CPanel* pPanel;
	CAnimation* m_pAnimation;

	AniFrame* aniArr1;
	AniFrame* aniArr2;
	AniFrame m_curAniFrame1;
	AniFrame m_curAniFrame2;
	UINT m_curIndex1;
	UINT m_curIndex2;

	bool m_bSelectImg1;
	bool m_bSelectImg2;
	bool m_bPlay;

	Vector m_vecFrameSize;

	int m_iCount1;
	int m_iCount2;

	float m_duration1;
	float m_duration2;

	float m_fAccTime1;
	float m_fAccTime2;
	UINT m_iCurFrame1;
	UINT m_iCurFrame2;

	void CreateFrame();

public:
	void SetAniFrame(AniFrame& frame, Vector lt, Vector slice, float duration = 1.f)
	{
		frame.lt = lt; frame.slice = slice; frame.duration = duration;
	}
	CImage* GetImage1();
	CImage* GetImage2();
	AniFrame*& GetAniArr1() { return aniArr1; }
	AniFrame*& GetAniArr2() { return aniArr2; }
	void SetImage1(CImage* img1) { m_pImage1 = img1; }
	void SetImage2(CImage* img2) { m_pImage2 = img2; }
	bool GetSelectImg1() { return m_bSelectImg1; }
	bool GetSelectImg2() { return m_bSelectImg2; }
	AniFrame& GetCurFrame1() { return m_curAniFrame1; }
	AniFrame& GetCurFrame2() { return m_curAniFrame2; }
	void PlayAni();
	void RenderAni();
	void SetCount1(int count) { m_iCount1 = count; }
	void SetCount2(int count) { m_iCount2 = count; }
	void SetCurIndex1(UINT index1) { m_curIndex1 = index1; }
	void SetCurIndex2(UINT index2) { m_curIndex2 = index2; }
	void SetDuration1(float dur1) { m_duration1 = dur1; }
	void SetDuration2(float dur2) { m_duration2 = dur2; }

	void GoToPrevAni();
	void GoToNextAni();

private:
	void Init();
	void Enter();
	void Update();
	void Render();
	void Exit();
	void Release();

public:
	void Create(CImage* pImg, Vector lt, Vector slice, Vector step, float duration, UINT count, bool repeat);
	CImage* ToolLoadImage();
	void ToolSaveAniData();
};

