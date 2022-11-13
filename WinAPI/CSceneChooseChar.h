#pragma once
#include "CScene.h"

class CImage;
class CImageObject;
class CAniObject;

class CSceneChooseChar : public CScene
{
public:
	CSceneChooseChar();
	virtual ~CSceneChooseChar();

private:
	CImageObject* m_pEriDoor;
	CImageObject* m_pEriSelect;
	CImageObject* m_pEriBox;

	CAniObject* m_pSelectSoldierAniObj;
	CAniObject* m_pEri1;
	CAniObject* m_pEri2;

	float m_fAccTime;
	float m_fExtension;

	bool m_bSelectedChar;

private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};

