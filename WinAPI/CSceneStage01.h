#pragma once
#include "CScene.h"

class CPlayer;
class CImageObject;
class CColliderObject;
class CFontImageObj;

class CSceneStage01 : public CScene
{
public:
	CSceneStage01();
	virtual ~CSceneStage01();

private:
	CPlayer* m_pPlayer;
	float m_fPlayerMaxPosX;
	float m_fAccTime;

	CFontImageObj* m_pFontImgObj;
	CImageObject* m_pBackGround;
	CColliderObject* m_pObstacle;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};
