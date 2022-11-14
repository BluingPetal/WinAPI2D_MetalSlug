#pragma once
#include "CScene.h"

class CPlayer;
class CImageObject;
class CColliderObject;
class CFontImageObj;
class CConga;

class CSceneStage01 : public CScene
{
public:
	CSceneStage01();
	virtual ~CSceneStage01();

private:
	CPlayer* m_pPlayer;
	float m_fPlayerMaxPosX;
	float m_fAccTime;
	float m_fMissionAccTime;
	bool m_bIsStarted;

	CFontImageObj* m_pInsertCoinImgObj;

	CFontImageObj* m_pMissionImgObj1;
	CFontImageObj* m_pMissionImgObj2;
	Vector m_vecStartPos1;
	Vector m_vecStartPos2;

	CImageObject* m_pBackGround;
	CColliderObject* m_pObstacle;
	CConga* m_pConga;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};
