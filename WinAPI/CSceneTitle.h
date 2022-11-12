#pragma once
#include "CScene.h"

class CFontImageObj;

class CSceneTitle : public CScene
{
public:
	CSceneTitle();
	virtual ~CSceneTitle();

private:
	CFontImageObj* fontImgStartObj;
	CFontImageObj* fontImgCredit;
	CFontImageObj* fontImgTime;
	float m_fAccTime;
	int m_credit;
	int m_time;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};
