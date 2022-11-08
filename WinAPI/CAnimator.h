#pragma once
#include "CComponent.h"

class CAnimation;
class CImage;
class CAniObject;

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
	AniFrame GetFirstAniFrame();
	wstring GetCurAniName();

	void Play(const wstring& aniName, bool trigger = false);	// �ִϸ��̼� ���
	void Stop();												// �ִϸ��̼� ����
};

