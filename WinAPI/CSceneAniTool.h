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

	queue<AniFrame> m_queueAni;
	AniFrame m_curAniFrame1;
	AniFrame m_curAniFrame2;

	bool m_bSelectImg1;
	bool m_bSelectImg2;

	Vector m_vecFrameSize;

	void CreateFrame();

public:
	void SetAniFrame(AniFrame& frame, Vector lt, Vector slice, float duration = 1.f)
	{
		frame.lt = lt; frame.slice = slice; frame.duration = duration;
	}
	CImage* GetImage1();
	CImage* GetImage2();
	CImage* ToolLoadImage();
	void SetImage1(CImage* img1) { m_pImage1 = img1; }
	void SetImage2(CImage* img2) { m_pImage2 = img2; }
	void AddAni(AniFrame aniFr) { m_queueAni.push(aniFr); }
	bool GetSelectImg1() { return m_bSelectImg1; }
	bool GetSelectImg2() { return m_bSelectImg2; }
	AniFrame& GetCurFrame1() { return m_curAniFrame1; }
	AniFrame& GetCurFrame2() { return m_curAniFrame2; }

private:
	void Init();
	void Enter();
	void Update();
	void Render();
	void Exit();
	void Release();

public:
	void Create(CImage* pImg, Vector lt, Vector slice, Vector step, float duration, UINT count, bool repeat);
	void SaveTile(const wstring& strPath);
	void SaveTileData();
	void Load_img(const wstring& strPath);
	void LoadTile(const wstring& strPath);
	void LoadTileData();
};

