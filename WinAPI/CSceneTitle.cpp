#include "framework.h"
#include "CSceneTitle.h"

#include "CImage.h"
#include "CImageObject.h"
#include "CFontImageObj.h"

CSceneTitle::CSceneTitle()
{
	m_fAccTime = 0;
	m_credit = 1;
	m_time = 30;
	fontImgStartObj = nullptr;
	fontImgCredit = nullptr;
	fontImgTime = nullptr;
	m_bStopUpdate = false;
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
	fontImgStartObj->CreateImgObj(L"press 1p to start", Vector(WINSIZEX * -0.1f, WINSIZEY * 0.88), 17, FontType::Default);

	fontImgCreditObj = new CFontImageObj;
	fontImgCreditObj->SetExtension(extension + 1);
	AddGameObject(fontImgCreditObj);
	fontImgCreditObj->SetInterval(1.f); // 글자 간격 -> create하기 전에 변화해주기
	fontImgCreditObj->CreateImgObj( L"credit", Vector(WINSIZEX * 0.6, WINSIZEY * 0.95), 6, FontType::Default);

	fontImgCredit = new CFontImageObj;
	fontImgCredit->SetExtension(extension + 1);
	AddGameObject(fontImgCredit);
	wstring creditStr = to_wstring(m_credit);
	fontImgCredit->SetInterval(1.f);
	fontImgCredit->CreateImgObj(creditStr, Vector(WINSIZEX * 0.9, WINSIZEY * 0.95), 2, FontType::Default);

	fontImgTimeObj = new CFontImageObj;
	fontImgTimeObj->SetExtension(extension + 1);
	AddGameObject(fontImgTimeObj);
	fontImgTimeObj->SetInterval(1.f); // 글자 간격 -> create하기 전에 변화해주기
	fontImgTimeObj->CreateImgObj( L"time", Vector(WINSIZEX * 0.38, WINSIZEY * 0.73), 4, FontType::Default);

	fontImgTime = new CFontImageObj;
	fontImgTime->SetExtension(extension + 1);
	AddGameObject(fontImgTime);
	wstring timeStr = to_wstring(m_time);
	fontImgTime->SetInterval(1.f);
	fontImgTime->CreateImgObj( timeStr, Vector(WINSIZEX * 0.45, WINSIZEY * 0.8), 2, FontType::Default);

	pBackGroundSound = RESOURCE->LoadSound(L"titleSound", L"Sound\\metalSlug3.mp3");
	pCoinSound = RESOURCE->LoadSound(L"CoinSound", L"Sound\\score.mp3");
}

void CSceneTitle::Enter()
{
	SOUND->Play(pBackGroundSound);
	fontImgStartObj->Show();
	fontImgCredit->Show();
	fontImgCreditObj->Show();
	fontImgTime->Show();
	fontImgTimeObj->Show();
}

void CSceneTitle::Update()
{
	m_fAccTime += DT;
	if (!m_bStopUpdate)
	{
		if (m_fAccTime >= 1.0f)
		{
			// time
			m_time--;

			wstring timeStr = to_wstring(m_time);
			fontImgTime->DeleteObj();
			fontImgTime->CreateImgObj(timeStr, Vector(WINSIZEX * 0.45, WINSIZEY * 0.8), 2, FontType::Default);
			fontImgTime->Show();

			// "press 1p to start" blink
			vector<CImageObject*> vecImgObj = fontImgStartObj->GetImageObj();
			for (int i = 0; i < vecImgObj.size(); i++)
			{
				vecImgObj[i]->SetAlpha(!(vecImgObj[i]->GetAlpha()));
			}
			//while (!vecImgObj.empty())
			//{
			//	CImageObject* imgObj = vecImgObj.front();
			//	imgObj->SetAlpha(!(imgObj->GetAlpha()));
			//	vecImgObj.pop();
			//}
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
			SOUND->Play(pCoinSound);
			m_credit++;

			wstring creditStr = to_wstring(m_credit);
			fontImgCredit->DeleteObj();
			fontImgCredit->CreateImgObj(creditStr, Vector(WINSIZEX * 0.9, WINSIZEY * 0.95), 2, FontType::Default);
			fontImgCredit->Show();
		}
	}

	if (m_credit > 0)
	{
		if (BUTTONDOWN(VK_SPACE) || m_time == 0)
		{
			if (!m_bStopUpdate)
				m_fAccTime = 0;

			m_time--;
			m_bStopUpdate = true;

			if(m_fAccTime>=0.1f)
				DeleteAll(); 
			else
			{
				CAMERA->FadeOut(0.25f);
				SCENE->SetCredit(m_credit);
				DELAYCHANGESCENE(GroupScene::SelectChar, 0.25f);
				SOUND->Stop(pBackGroundSound);
				SOUND->Stop(pCoinSound);
			}
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
