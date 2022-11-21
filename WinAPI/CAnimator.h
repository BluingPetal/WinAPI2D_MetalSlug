#pragma once
#include "CComponent.h"

class CAnimation;
class CImage;
class CAniObject;
class CAnimation;

class CAnimator : public CComponent
{
	friend CAniObject;
public:
	CAnimator();
	virtual ~CAnimator();

private:
	map<wstring, CAnimation*>	m_mapAni;	// 애니메이션들을 담기 위한 자료구조
	CAnimation* m_pCurAni;					// 현재 애니메이션

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	void CreateAnimation(const wstring& aniName, CImage* pImg,	// 애니메이션 생성
		 float duration, bool repeat = true);
	void RemoveAnimation(const wstring& aniName);				// 애니메이션 삭제
	CAnimation* FindAnimation(const wstring& aniName);			// 애니메이션 탐색
	CAnimation* GetCurAni();			
	AniFrame GetFirstAniFrame();
	wstring GetCurAniName();

	void Play(const wstring& aniName, bool trigger = false, bool play = true);	// 애니메이션 재생
	void ReversePlay(const wstring& aniName, bool trigger = false, bool reversePlay = true);
	void Stop();												// 애니메이션 종료
};

