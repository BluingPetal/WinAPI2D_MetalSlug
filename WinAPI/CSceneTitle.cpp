#include "framework.h"
#include "CSceneTitle.h"

#include "CImage.h"
#include "CImageObject.h"
#include "CCameraController.h"

CSceneTitle::CSceneTitle()
{
}

CSceneTitle::~CSceneTitle()
{
}

void CSceneTitle::Init()
{
	CImage* pTitleImg = RESOURCE->LoadImg(L"TitleImg", L"Image\\Start\\Title.jpg");
	CImageObject* pTitleObj = new CImageObject;
	float extension = WINSIZEY / (float)(pTitleImg->GetHeight());

	Vector pTitleObjOffset = Vector(-85, 0);
	pTitleObj->SetImage(pTitleImg);
	pTitleObj->SetExtension(extension);
	pTitleObj->SetOffset(pTitleObjOffset);
	AddGameObject(pTitleObj);

	CCameraController* pCamController = new CCameraController;
	AddGameObject(pCamController);
}

void CSceneTitle::Enter()
{
	CAMERA->FadeIn(0.25f);
}

void CSceneTitle::Update()
{
	if (BUTTONDOWN(VK_F1))
	{
		CHANGESCENE(GroupScene::TileTool);
	}
	if (BUTTONDOWN(VK_F2))
	{
		CHANGESCENE(GroupScene::AniTool);
	}
	if (BUTTONDOWN(VK_SPACE))
	{
		CAMERA->FadeOut(0.25f);
		DELAYCHANGESCENE(GroupScene::Stage01, 0.25f);
	}
}

void CSceneTitle::Render()
{
	/*
	RENDER->Text(L"press space to start",
		WINSIZEX * 0.5f - 100,
		WINSIZEY * 0.5f - 10,
		WINSIZEX * 0.5f + 100,
		WINSIZEY * 0.5f + 10,
		Color(0, 0, 0, 1.f),
		20.f);
	*/
}

void CSceneTitle::Exit()
{
}

void CSceneTitle::Release()
{
}
