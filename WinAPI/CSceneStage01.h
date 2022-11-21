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
	int m_fTime;
	int m_iTimeCount;
	bool m_bIsStarted;
	bool m_bCallObj;

	int prevBomb;
	int prevBullet;

	int countBullet;

	CFontImageObj* m_pInsertCoinImgObj;

	CFontImageObj* m_pMissionImgObj1;
	CFontImageObj* m_pMissionImgObj2;
	CFontImageObj* m_pTimeImgObj;
	CFontImageObj* m_pBarFontObj;
	CFontImageObj* m_pBulletObj;
	CFontImageObj* m_pBombObj;
	Vector m_vecStartPos1;
	Vector m_vecStartPos2;

	CImageObject* m_pBackGround;
	CImageObject* pBoat;
	CImageObject* pBoatCastle;
	CAniObject* pWaterAniObj;
	CColliderObject* m_pObstacle;
	CColliderObject* m_pBoatCastleCollider;
	CConga* m_pConga;

public:
	CAniObject* GetWaterAni();

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};
