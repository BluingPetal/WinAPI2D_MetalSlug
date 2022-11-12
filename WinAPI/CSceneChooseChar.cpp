#include "framework.h"
#include "CSceneChooseChar.h"

#include "CImage.h"
#include "CImageObject.h"
#include "CFontImageObj.h"

#include "CCameraController.h"

CSceneChooseChar::CSceneChooseChar()
{
	m_fAccTime = 0;
}

CSceneChooseChar::~CSceneChooseChar()
{
}

void CSceneChooseChar::Init()
{
	CImage* pBlackImg = RESOURCE->LoadImg(L"pBlackImg", L"Image\\Start\\BlackImg.png");
	CImageObject* pBlackObj = new CImageObject;
	pBlackObj->SetImage(pBlackImg);
	AddGameObject(pBlackObj);

	CImage* pSelectCharImg = RESOURCE->LoadImg(L"ChooseCharBackImg", L"Image\\Start\\SelectChar.png");
	CImageObject* pBackObj = new CImageObject;
	pBackObj->SetImage(pSelectCharImg);
	pBackObj->SetRenderAsFrame(true);
	float sourceBackInfo[4] = { 3, 116, 304, 216 };
	pBackObj->SetSourceInfo(sourceBackInfo[0], sourceBackInfo[1], sourceBackInfo[2], sourceBackInfo[3]);
	float extension = WINSIZEY / sourceBackInfo[3];
	pBackObj->SetPos((WINSIZEX - sourceBackInfo[2] * extension) * 0.5, 0);
	pBackObj->SetExtension(extension);
	AddGameObject(pBackObj);

	CImageObject* pDoor1 = new CImageObject;
	pDoor1->SetImage(pSelectCharImg);
	pDoor1->SetRenderAsFrame(true);
	float sourceDoor1Info[4] = { 70, 335, 64, 134 };
	pDoor1->SetSourceInfo(sourceDoor1Info[0], sourceDoor1Info[1], sourceDoor1Info[2], sourceDoor1Info[3]);
	Vector Door1Offset = Vector(19, 73);
	pDoor1->SetPos(pBackObj->GetPos());
	pDoor1->SetOffset(Door1Offset);
	//pDoor1->SetPos(pBackObj->GetPos() + Door1Offset * extension);
	pDoor1->SetExtension(extension);
	AddGameObject(pDoor1);
	
	CCameraController* pCamController = new CCameraController;
	AddGameObject(pCamController);
}

void CSceneChooseChar::Enter()
{
	CAMERA->FadeIn(0.25f);
}

void CSceneChooseChar::Update()
{
	if (BUTTONDOWN(VK_SPACE))
	{
		CAMERA->FadeOut(0.25f);
		DELAYCHANGESCENE(GroupScene::Stage01, 0.25f);
	}
}

void CSceneChooseChar::Render()
{
}

void CSceneChooseChar::Exit()
{
}

void CSceneChooseChar::Release()
{
}
