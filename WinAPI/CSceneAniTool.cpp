#include "framework.h"
#include "CSceneAniTool.h"

#include "CPanel.h"
#include "CImage.h"

LRESULT CALLBACK    WinAniToolProc(HWND, UINT, WPARAM, LPARAM);

CSceneAniTool::CSceneAniTool()
{
	m_hWndAniTool = 0;
	m_pImage1 = nullptr;
	m_pImage2 = nullptr;
	m_bSelectImg1 = false;
	m_bSelectImg2 = false;

	m_vecFrameSize = Vector(100, 100);
}

CSceneAniTool::~CSceneAniTool()
{
}

void CSceneAniTool::CreateFrame()
{
	RENDER->FrameRect(WINSIZEX * 0.5f - m_vecFrameSize.x *0.5f, WINSIZEY * 0.5f - m_vecFrameSize.y * 0.5f, WINSIZEX * 0.5f, WINSIZEY * 0.5f);
	RENDER->FrameRect(WINSIZEX * 0.5f, WINSIZEY * 0.5f - m_vecFrameSize.y * 0.5f, WINSIZEX * 0.5f + m_vecFrameSize.x * 0.5f, WINSIZEY * 0.5f);
	RENDER->FrameRect(WINSIZEX * 0.5f - m_vecFrameSize.x * 0.5f, WINSIZEY * 0.5f, WINSIZEX * 0.5f, WINSIZEY * 0.5f + m_vecFrameSize.y * 0.5f);
	RENDER->FrameRect(WINSIZEX * 0.5f, WINSIZEY * 0.5f, WINSIZEX * 0.5f + m_vecFrameSize.x * 0.5f, WINSIZEY * 0.5f + m_vecFrameSize.y * 0.5f);
}

CImage* CSceneAniTool::GetImage1()
{
	return m_pImage1;
}

CImage* CSceneAniTool::GetImage2()
{
	return m_pImage2;
}

CImage* CSceneAniTool::ToolLoadImage()
{
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);  // ����ü ������.
	ofn.hwndOwner = hWnd; // �θ� ������ ����.
	wchar_t szName[256] = {};
	ofn.lpstrFile = szName; // ���߿� �ϼ��� ��ΰ� ä���� ���� ����.
	ofn.nMaxFile = sizeof(szName); // lpstrFile�� ������ ������ ���� ��.
	ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png"; // ���� ����
	ofn.nFilterIndex = 0; // �⺻ ���� ����. 0�� all�� �ʱ� ���õ�. ó����.
	ofn.lpstrFileTitle = nullptr; // Ÿ��Ʋ ��
	ofn.nMaxFileTitle = 0; // Ÿ��Ʋ �� ���ڿ� ũ��. nullptr�̸� 0.
	wstring strAniFolder = GETPATH;
	strAniFolder += L"Image";
	ofn.lpstrInitialDir = strAniFolder.c_str(); // �ʱ���. �츮�� Ÿ�� �����Ұű� ������, content->tile ��η� �ص���.
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // ��Ÿ��

	if (GetOpenFileName(&ofn))
	{
		return RESOURCE->LoadImgWithPath(szName, szName);
	}
	return nullptr;
}

void CSceneAniTool::Init()
{
	m_bSelectImg1 = true;
}

void CSceneAniTool::Enter()
{
	// DialogBox : ��޹�� (â�� ��Ŀ���� ���� �ʵ��� ������)
	// CreateDialog : ��޸��� ��� (â�� ��Ŀ�� ������ ���� ����)
	m_hWndAniTool = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ANITOOLBOX), hWnd, WinAniToolProc);
	ShowWindow(m_hWndAniTool, SW_SHOW);

	RECT rect;
	GetWindowRect(m_hWndAniTool, &rect);
	MoveWindow(m_hWndAniTool, WINSTARTX + WINSIZEX, WINSTARTY,
		rect.right - rect.left, rect.bottom - rect.top, true);

	CreateFrame();

	pPanel = new CPanel;
	pPanel->SetScale(Vector(400.f, 600.f));
	pPanel->SetPos(Vector(WINSIZEX - 450.f, 50.f));
	AddGameObject(pPanel);
}

void CSceneAniTool::Update()
{
	if (BUTTONDOWN(VK_ESCAPE))
	{
		CHANGESCENE(GroupScene::Title);
	}

	if (BUTTONDOWN(VK_MBUTTON))
	{
		// �� ����
	}

	if (BUTTONDOWN('Q'))
	{
		m_bSelectImg1 = true;
		m_bSelectImg2 = false;
	}
	if (BUTTONDOWN('W'))
	{
		m_bSelectImg1 = false;
		m_bSelectImg2 = true;
	}
	// AniFrame ����
	if (BUTTONSTAY(VK_UP))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y > 0)
				m_curAniFrame1.lt += Vector(0, -5) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y > 0)
				m_curAniFrame2.lt += Vector(0, -5) * DT;
		}
	}
	if (BUTTONSTAY(VK_DOWN))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y < m_pImage1->GetHeight())
				m_curAniFrame1.lt += Vector(0, 5) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y < m_pImage2->GetHeight())
				m_curAniFrame2.lt += Vector(0, 5) * DT;
		}
	}
	if (BUTTONSTAY(VK_LEFT))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x > 0)
				m_curAniFrame1.lt += Vector(-5, 0)*DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x > 0)
				m_curAniFrame2.lt += Vector(-5, 0)*DT;
		}
	}
	if (BUTTONSTAY(VK_RIGHT))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x < m_pImage1->GetWidth())
				m_curAniFrame1.lt += Vector(5, 0) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x < m_pImage2->GetWidth())
				m_curAniFrame2.lt += Vector(5, 0) * DT;
		}
	}
	// Frame ����
	if (BUTTONSTAY('W'))
	{
		m_vecFrameSize.y += 5.f * DT;
	}
	if (BUTTONSTAY('S'))
	{
		m_vecFrameSize.y -= 5.f * DT;
	}
	if (BUTTONSTAY('A'))
	{
		m_vecFrameSize.x -= 5.f * DT;
	}
	if (BUTTONSTAY('D'))
	{
		m_vecFrameSize.x += 5.f * DT;
	}
}

void CSceneAniTool::Render()
{
	CreateFrame(); 
	if (m_pImage1 != nullptr)
	{
		RENDER->FrameRect(
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f
		);
		RENDER->FrameImage(
			m_pImage1,
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f,
			m_curAniFrame1.lt.x,
			m_curAniFrame1.lt.y,
			m_curAniFrame1.lt.x + m_curAniFrame1.slice.x,
			m_curAniFrame1.lt.y + m_curAniFrame1.slice.y
		);
	}
	if (m_pImage2 != nullptr)
	{
		RENDER->FrameRect(
			WINSIZEX * 0.5f - m_curAniFrame2.slice.x * 0.5f,
			WINSIZEY * 0.5f - m_curAniFrame2.slice.y * 0.5f,
			WINSIZEX * 0.5f + m_curAniFrame2.slice.x * 0.5f,
			WINSIZEY * 0.5f + m_curAniFrame2.slice.y * 0.5f
		);
		RENDER->FrameImage(
			m_pImage2,
			WINSIZEX * 0.5f - m_curAniFrame2.slice.x * 0.5f,
			WINSIZEY * 0.5f - m_curAniFrame2.slice.y * 0.5f,
			WINSIZEX * 0.5f + m_curAniFrame2.slice.x * 0.5f,
			WINSIZEY * 0.5f + m_curAniFrame2.slice.y * 0.5f,
			m_curAniFrame2.lt.x,
			m_curAniFrame2.lt.y,
			m_curAniFrame2.lt.x + m_curAniFrame2.slice.x,
			m_curAniFrame2.lt.y + m_curAniFrame2.slice.y
		);
	}
	if (m_pImage1 != nullptr && m_bSelectImg1) // img1�� select�Ǿ��� �� ������ ������
	{
		RENDER->FrameRect(
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f
		);
		RENDER->FrameImage(
			m_pImage1,
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f,
			m_curAniFrame1.lt.x,
			m_curAniFrame1.lt.y,
			m_curAniFrame1.lt.x + m_curAniFrame1.slice.x,
			m_curAniFrame1.lt.y + m_curAniFrame1.slice.y
		);
	}

	wstring description = L"X Position : \n DFSF";
	pPanel->SetText(description, 15);
}

void CSceneAniTool::Exit()
{
}

void CSceneAniTool::Release()
{
}

void CSceneAniTool::Create(CImage* pImg, Vector lt, Vector slice, Vector step, float duration, UINT count, bool repeat)
{

}

LRESULT CALLBACK WinAniToolProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTONOK)
		{
			int iLtX = GetDlgItemInt(hDlg, IDC_EDIT_LTX, nullptr, false);
			int iLtY = GetDlgItemInt(hDlg, IDC_EDIT_LTY, nullptr, false);

			int iSliceX = GetDlgItemInt(hDlg, IDC_EDIT_SX, nullptr, false);
			int iSliceY = GetDlgItemInt(hDlg, IDC_EDIT_SY, nullptr, false);

			int iCount = GetDlgItemInt(hDlg, IDC_EDIT_COUNT, nullptr, false);

			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			if (pAniToolScene->GetSelectImg1())
			{
				pAniToolScene->SetAniFrame(pAniToolScene->GetCurFrame1(), Vector(iLtX, iLtY), Vector(iSliceX, iSliceY));
			}
			else if (pAniToolScene->GetSelectImg2())
			{
				pAniToolScene->SetAniFrame(pAniToolScene->GetCurFrame2(), Vector(iLtX, iLtY), Vector(iSliceX, iSliceY));
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTONLOAD)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			assert(nullptr != pAniToolScene && L"AniTool Scene cast Failed");

			if (pAniToolScene->GetSelectImg1())
			{
				pAniToolScene->SetImage1(pAniToolScene->ToolLoadImage());
			}
			else if (pAniToolScene->GetSelectImg2())
			{
				pAniToolScene->SetImage2(pAniToolScene->ToolLoadImage());
			}
		}
		/*
		else if (LOWORD(wParam) == IDC_BUTTONLOAD)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneTileTool* pTileToolScene = dynamic_cast<CSceneTileTool*>(pCurScene);
			assert(nullptr != pTileToolScene && L"TileTool Scene cast Failed");

			pTileToolScene->LoadTileData();
		}
		else if (LOWORD(wParam) == IDC_BUTTONMAPSAVE)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneTileTool* pTileToolScene = dynamic_cast<CSceneTileTool*>(pCurScene);
			assert(nullptr != pTileToolScene && L"TileTool Scene cast Failed");

			pTileToolScene->SaveMapData();
		}
		else if (LOWORD(wParam) == IDC_BUTTONMAPLOAD)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneTileTool* pTileToolScene = dynamic_cast<CSceneTileTool*>(pCurScene);
			assert(nullptr != pTileToolScene && L"TileTool Scene cast Failed");

			pTileToolScene->LoadMapData();
		}
		*/
		return (INT_PTR)TRUE;
		break;
	}
	return (INT_PTR)FALSE;
}