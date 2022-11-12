#include "framework.h"
#include "CSceneTitle.h"

#include "CImage.h"
#include "CImageObject.h"
#include "CCameraController.h"
#include "CFontImageObj.h"

CSceneTitle::CSceneTitle()
{
	m_fAccTime = 0;
	m_credit = 1;
	fontImgStartObj = nullptr;
	fontImgCredit = nullptr;
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
	pTitleObj->SetLayer(Layer::BackGround);
	AddGameObject(pTitleObj);

	fontImgStartObj = new CFontImageObj;
	fontImgStartObj->SetExtension(extension+1);
	AddGameObject(fontImgStartObj);

	fontImgStartObj->SetInterval(1.f); // 글자 간격 -> create하기 전에 변화해주기
	fontImgStartObj->CreateImg(L"press 1p to start", Vector(WINSIZEX * 0.3, WINSIZEY * 0.85), 17, FontType::Default);

	CFontImageObj* fontImgCreditObj = new CFontImageObj;
	fontImgCreditObj->SetExtension(extension + 1);
	AddGameObject(fontImgCreditObj);

	fontImgCreditObj->SetInterval(1.f); // 글자 간격 -> create하기 전에 변화해주기
	fontImgCreditObj->CreateImg(L"credit ", Vector(WINSIZEX * 0.75, WINSIZEY * 0.95), 7, FontType::Default);

	fontImgCredit = new CFontImageObj;
	fontImgCredit->SetExtension(extension + 1);
	AddGameObject(fontImgCredit);

	wstring creditStr = to_wstring(m_credit);
	fontImgCredit->SetInterval(1.f);
	fontImgCredit->CreateImg(creditStr, Vector(WINSIZEX * 0.92, WINSIZEY * 0.95), 2, FontType::Default);
	//Logger::Debug(to_wstring(m_credit));
}

void CSceneTitle::Enter()
{
	CAMERA->FadeIn(0.25f);
}

void CSceneTitle::Update()
{
	m_fAccTime += DT;

	if (m_fAccTime >= 1.0f)
	{
		for (int i = 0; i < fontImgStartObj->GetImageObj().size(); i++)
		{
			CImageObject* imgObj = fontImgStartObj->GetImageObj()[i];
			imgObj->SetAlpha(!(imgObj->GetAlpha()));
		}
		m_fAccTime = 0;
	}

	if (BUTTONDOWN(VK_F1))
	{
		CHANGESCENE(GroupScene::TileTool);
	}
	if (BUTTONDOWN(VK_F2))
	{
		CHANGESCENE(GroupScene::AniTool);
	}
	if (BUTTONDOWN(VK_F3))
	{
		m_credit++;
		fontImgCredit->DeleteObj();
		wstring creditStr = to_wstring(m_credit);
		fontImgCredit->CreateImg(creditStr, Vector(WINSIZEX * 0.92, WINSIZEY * 0.95), 2, FontType::Default);
	}
	if (m_credit > 0)
	{
		if (BUTTONDOWN(VK_SPACE))
		{
			CAMERA->FadeOut(0.25f);
			DELAYCHANGESCENE(GroupScene::Stage01, 0.25f);
		}
	}

}

void CSceneTitle::Render()
{
}

void CSceneTitle::Exit()
{
}

void CSceneTitle::Release()
{
}
