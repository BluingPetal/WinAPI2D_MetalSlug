#pragma once
#include "CScene.h"

class CImageObject;
class CPlayer;
class CAniObject;
class CColliderObject;
class CBoss;

class CSceneBoss : public CScene
{
public:
	CSceneBoss();
	virtual ~CSceneBoss();



private:
	CImageObject* pBossBackgroundObj1;
	CImageObject* pBossBackgroundObj2;
	CImageObject* pCurBackgroundObj;

	CPlayer* m_pPlayer;
	CBoss* pBoss;
	CAniObject* m_pWarpAni;
	CAniObject* m_pGoAni;
	CAniObject* m_pWater;

	CColliderObject* m_pObstacle;

	float m_fStartAccTime;
	float m_fBossAppearAccTime;
	float m_fExtension;

	bool m_bStart;
	bool m_bBossAppear;

	queue<CImageObject*> m_queueBackGround;


private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};