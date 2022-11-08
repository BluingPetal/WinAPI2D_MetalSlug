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

	CAniObject* pFrontOceanObj1;
	CAniObject* pFrontOceanObj2;

	CImageObject* pBackGround;
	CImageObject* pFrontGround;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};
