#pragma once
#include "CScene.h"

class CSceneChooseChar : public CScene
{
public:
	CSceneChooseChar();
	virtual ~CSceneChooseChar();

private:
	//CFontImageObj* fontImgStartObj;
	//CFontImageObj* fontImgCredit;
	//CFontImageObj* fontImgTime;
	float m_fAccTime;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};

