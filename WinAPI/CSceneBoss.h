#pragma once
#include "CScene.h"

class CImageObject;
class CPlayer;
class CAniObject;

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
	CAniObject* m_pWarpAni;

	float m_fAccTime;
	float m_fExtension;

	bool m_bStart;

	queue<CImageObject*> m_queueBackGround;


private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};