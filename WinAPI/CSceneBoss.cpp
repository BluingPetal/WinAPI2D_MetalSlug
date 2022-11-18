#include "framework.h"
#include "CSceneBoss.h"

#include "CBridge.h"

CSceneBoss::CSceneBoss()
{
}

CSceneBoss::~CSceneBoss()
{
}

void CSceneBoss::Init()
{
	CBridge* pBridge = new CBridge;
	AddGameObject(pBridge);
}

void CSceneBoss::Enter()
{
}

void CSceneBoss::Update()
{
	Logger::Debug(to_wstring(MOUSEWORLDPOS.x) + L", " + to_wstring(MOUSEWORLDPOS.y));
}

void CSceneBoss::Render()
{
}

void CSceneBoss::Exit()
{
}

void CSceneBoss::Release()
{
}
