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
	map<wstring, CAnimation*>	m_mapAni;	// �ִϸ��̼ǵ��� ��� ���� �ڷᱸ��
	CAnimation* m_pCurAni;					// ���� �ִϸ��̼�

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	void CreateAnimation(const wstring& aniName, CImage* pImg,	// �ִϸ��̼� ����
		 float duration, bool repeat = true);
	void RemoveAnimation(const wstring& aniName);				// �ִϸ��̼� ����
	CAnimation* FindAnimation(const wstring& aniName);			// �ִϸ��̼� Ž��
	CAnimation* GetCurAni();			
	AniFrame GetFirstAniFrame();
	wstring GetCurAniName();

	void Play(const wstring& aniName, bool trigger = false, bool play = true);	// �ִϸ��̼� ���
	void ReversePlay(const wstring& aniName, bool trigger = false, bool reversePlay = true);
	void Stop();												// �ִϸ��̼� ����
};

