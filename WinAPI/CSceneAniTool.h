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
	AniFrame* aniArr1;
	AniFrame* aniArr2;
	AniFrame m_curAniFrame1;
	AniFrame m_curAniFrame2;

	Vector m_vecFrameSize;

	UINT m_curIndex1;
	UINT m_curIndex2;
	UINT m_iCurFrame1;
	UINT m_iCurFrame2;
	bool m_bSelectImg1;
	bool m_bSelectImg2;
	int m_iCount1;
	int m_iCount2;

	float m_duration1;
	float m_duration2;
	float m_fAccTime1;
	float m_fAccTime2;

	bool m_bPlay;

	void CreateFrame();

public:
	void SetAniFrame(AniFrame& frame, Vector lt, Vector slice, float duration = 1.f);
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
	void SetCount1(int count) { m_iCount1 = count; }
	void SetCount2(int count) { m_iCount2 = count; }
	void SetCurIndex1(UINT index1) { m_curIndex1 = index1; }
	void SetCurIndex2(UINT index2) { m_curIndex2 = index2; }
	void SetDuration1(float dur1) { m_duration1 = dur1; }
	void SetDuration2(float dur2) { m_duration2 = dur2; }

private:
	void Init();
	void Enter();
	void Update();
	void Render();
	void Exit();
	void Release();

public:
	CImage* ToolLoadImage();
	void ToolSaveAniData();
	void GoToPrevAni();
	void GoToNextAni();
	void PlayAni();
	void RenderAni();
};

