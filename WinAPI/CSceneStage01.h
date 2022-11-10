#pragma once
#include "CScene.h"

class CPlayer;
class CImageObject;
class CAniObject;

class CSceneStage01 : public CScene
{
public:
	CSceneStage01();
	virtual ~CSceneStage01();

private:
	CPlayer* pPlayer;
	float m_fPlayerMaxPosX;

	CImageObject* pBackGround;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};
