#pragma once
#include "CScene.h"

class CImageObject;
class CPlayer;
class CAniObject;
class CColliderObject;
class CBoss;
class CFontImageObj;

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
	CColliderObject* m_pDeathObj;
	CFontImageObj* m_pMissionComplete1;
	CFontImageObj* m_pMissionComplete2;
	Vector m_vecMissionCompleteStartPos1;
	Vector m_vecMissionCompleteStartPos2;

	CFontImageObj* m_pInsertCoinImgObj;
	CFontImageObj* m_pTimeImgObj;
	CFontImageObj* m_pBarFontObj;
	CFontImageObj* m_pBulletObj;
	CFontImageObj* m_pBombObj;

	float m_fStartAccTime;
	float m_fBossAppearAccTime;
	float m_fExtension;
	float m_fVictoryAccTime;
	float m_fAccTime;
	int m_iTimeCount;
	int m_fTime;

	bool m_bStart;
	bool m_bBossAppear;
	bool m_bVictory;
	bool m_bVictorySoundPlay;

	CSound* pBackGroundSound;
	CSound* pMissionCompleteSound;
	CSound* pMissionClearSound;
	CSound* pBossAppearSound;

	queue<CImageObject*> m_queueBackGround;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};