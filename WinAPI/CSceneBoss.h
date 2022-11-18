#pragma once
#include "CScene.h"

class CSceneBoss : public CScene
{
public:
	CSceneBoss();
	virtual ~CSceneBoss();






private:
	void Init()		override;
	void Enter()	override;
	void Update()	override;
	void Render()	override;
	void Exit()		override;
	void Release()	override;
};