#include "framework.h"
#include "CSceneAniTool.h"

#include "CPanel.h"
#include "CImage.h"
#include "CPlayer.h"

LRESULT CALLBACK    WinAniToolProc(HWND, UINT, WPARAM, LPARAM);

CSceneAniTool::CSceneAniTool()
{
	m_hWndAniTool = 0;
	m_pImage1 = nullptr;
	m_pImage2 = nullptr;
	m_bSelectImg1 = false;
	m_bSelectImg2 = false;
	pPanel = nullptr;
	aniArr1 = nullptr;
	aniArr2 = nullptr;
	m_curIndex1 = 0;
	m_curIndex2 = 0;
	m_iCount1 = 0;
	m_iCount2 = 0;
	m_duration1 = 1;
	m_duration2 = 1;

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

	ofn.lStructSize = sizeof(OPENFILENAME);  // 구조체 사이즈.
	ofn.hwndOwner = hWnd; // 부모 윈도우 지정.
	wchar_t szName[256] = {};
	ofn.lpstrFile = szName; // 나중에 완성된 경로가 채워질 버퍼 지정.
	ofn.nMaxFile = sizeof(szName); // lpstrFile에 지정된 버퍼의 문자 수.
	ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png"; // 필터 설정
	ofn.nFilterIndex = 0; // 기본 필터 세팅. 0는 all로 초기 세팅됨. 처음꺼.
	ofn.lpstrFileTitle = nullptr; // 타이틀 바
	ofn.nMaxFileTitle = 0; // 타이틀 바 문자열 크기. nullptr이면 0.
	wstring strAniFolder = GETPATH;
	strAniFolder += L"Image";
	ofn.lpstrInitialDir = strAniFolder.c_str(); // 초기경로. 우리는 타일 저장할거기 때문에, content->tile 경로로 해두자.
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 스타일

	if (GetOpenFileName(&ofn))
	{
		return RESOURCE->LoadImgWithPath(szName, szName);
	}
	return nullptr;
}

void CSceneAniTool::AddAni(AniFrame aniFr, int index)
{
	//aniArr[index] = aniFr;
	/*
	if (m_bSelectImg1 && m_pImage1 != nullptr)
	{
		if (m_curIndex1 == m_iCount/2)
			return;
		aniArr[m_curIndex1++] = aniFr;
	}
	if (m_bSelectImg2 && m_pImage2 != nullptr)
	{
		if (m_curIndex2 == m_iCount)
			return;
		aniArr[m_curIndex2++] = aniFr;
	}
	*/
}

void CSceneAniTool::GoToPrevAni()
{
	if (m_bSelectImg1 && m_pImage1 != nullptr)
	{
		aniArr1[m_curIndex1] = m_curAniFrame1;
		if (m_curIndex1 == 0)
			m_curIndex1 = m_iCount1 - 1;
		else
			m_curIndex1--;
		if (aniArr1[m_curIndex1].lt.x != NULL)
			m_curAniFrame1 = aniArr1[m_curIndex1];
	}
	else if (m_bSelectImg2 && m_pImage2 != nullptr)
	{
		aniArr2[m_curIndex2] = m_curAniFrame2;
		if (m_curIndex2 == 0)
			m_curIndex2 = m_iCount2 - 1;
		else
			m_curIndex2--;
		if (aniArr2[m_curIndex2].lt.x != NULL)
			m_curAniFrame2 = aniArr2[m_curIndex2];
	}
}

void CSceneAniTool::GoToNextAni()
{
	if (m_bSelectImg1 && m_pImage1 != nullptr)
	{
		aniArr1[m_curIndex1] = m_curAniFrame1;
		if (m_curIndex1 == m_iCount1 - 1)
			m_curIndex1 = 0;
		else
			m_curIndex1++;
		if (aniArr1[m_curIndex1].lt.x != NULL)
			m_curAniFrame1 = aniArr1[m_curIndex1];
	}
	else if (m_bSelectImg2 && m_pImage2 != nullptr)
	{
		aniArr2[m_curIndex2] = m_curAniFrame2;
		if (m_curIndex2 == m_iCount2 - 1)
			m_curIndex2 = 0;
		else
			m_curIndex2++;
		if (aniArr2[m_curIndex2].lt.x != NULL)
			m_curAniFrame2 = aniArr2[m_curIndex2];
	}
}

void CSceneAniTool::Init()
{
	m_bSelectImg1 = true;
	m_bSelectImg2 = false;
}

void CSceneAniTool::Enter()
{
	// DialogBox : 모달방식 (창이 포커싱이 잃지 않도록 제한함)
	// CreateDialog : 모달리스 방식 (창이 포커싱 제한을 하지 않음)
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
	Logger::Debug(to_wstring(m_curIndex1));
	// 배열이 지정되지 않았을 경우 리턴
	if (aniArr1 == nullptr && aniArr2 == nullptr) return;

#pragma region  SceneAniTool Key Setting
	// Title 씬으로 돌아가기
	if (BUTTONDOWN(VK_ESCAPE))
	{
		CHANGESCENE(GroupScene::Title);
	}
	// Zoom
	if (BUTTONDOWN(VK_MBUTTON))
	{
		// 줌 구현
	}

	// Image 선택
	if (BUTTONDOWN(VK_NUMPAD1))
	{
		m_bSelectImg1 = true;
		m_bSelectImg2 = false;
	}
	if (BUTTONDOWN(VK_NUMPAD2))
	{
		m_bSelectImg1 = false;
		m_bSelectImg2 = true;
	}
	// Left Top 조절
	if (BUTTONSTAY(VK_UP))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y > 0)
				m_curAniFrame1.lt += Vector(0, -10) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y > 0)
				m_curAniFrame2.lt += Vector(0, -10) * DT;
		}
	}
	if (BUTTONSTAY(VK_DOWN))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y + m_curAniFrame1.offset.y + m_curAniFrame1.slice.y < m_pImage1->GetHeight())
				m_curAniFrame1.lt += Vector(0, 10) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y + m_curAniFrame2.offset.y + m_curAniFrame2.slice.y < m_pImage2->GetHeight())
				m_curAniFrame2.lt += Vector(0, 10) * DT;
		}
	}
	if (BUTTONSTAY(VK_LEFT))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x > 0)
				m_curAniFrame1.lt += Vector(-10, 0)*DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x > 0)
				m_curAniFrame2.lt += Vector(-10, 0)*DT;
		}
	}
	if (BUTTONSTAY(VK_RIGHT))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x + m_curAniFrame1.offset.x + m_curAniFrame1.slice.x < m_pImage1->GetWidth())
				m_curAniFrame1.lt += Vector(10, 0) * DT;
		}
		else if (m_bSelectImg2 + m_curAniFrame2.offset.x + +m_curAniFrame2.slice.x && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x < m_pImage2->GetWidth())
				m_curAniFrame2.lt += Vector(10, 0) * DT;
		}
	}
	// Frame 조절
	if (BUTTONSTAY('N'))
	{
		m_vecFrameSize.x -= 50.f * DT;
		m_vecFrameSize.y -= 50.f * DT;
	}
	if (BUTTONSTAY('M'))
	{
		m_vecFrameSize.x += 50.f * DT;
		m_vecFrameSize.y += 50.f * DT;
	}
	// Offset 조절
	if (BUTTONSTAY(VK_NUMPAD8))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(0, -10) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(0, -10) * DT;
		}
	}
	if (BUTTONSTAY(VK_NUMPAD5))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(0, 10) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(0, 10) * DT;
		}
	}
	if (BUTTONSTAY(VK_NUMPAD4))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(-10, 0) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(-10, 0) * DT;
		}
	}
	if (BUTTONSTAY(VK_NUMPAD6))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(10, 0) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(10, 0) * DT;
		}
	}
	// Slice 조절
	if (BUTTONSTAY('W'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y + m_curAniFrame1.offset.y + m_curAniFrame1.slice.y < m_pImage1->GetHeight())
				m_curAniFrame1.slice += Vector(0, 10) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y + m_curAniFrame2.offset.y + m_curAniFrame2.slice.y < m_pImage2->GetHeight())
				m_curAniFrame2.slice += Vector(0, 10) * DT;
		}
	}
	if (BUTTONSTAY('S'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.slice.y > 0)
				m_curAniFrame1.slice -= Vector(0, 10) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.slice.y > 0)
				m_curAniFrame2.slice -= Vector(0, 10) * DT;
		}
	}
	if (BUTTONSTAY('D'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x + m_curAniFrame1.offset.x + m_curAniFrame1.slice.x < m_pImage1->GetWidth())
				m_curAniFrame1.slice += Vector(10, 0) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x + m_curAniFrame2.offset.x + m_curAniFrame2.slice.x < m_pImage2->GetWidth())
				m_curAniFrame2.slice += Vector(10, 0) * DT;
		}
	}
	if (BUTTONSTAY('A'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.slice.x > 0)
				m_curAniFrame1.slice -= Vector(10, 0) * DT;
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.slice.x > 0)
				m_curAniFrame2.slice -= Vector(10, 0) * DT;
		}
	}
#pragma endregion
}

void CSceneAniTool::Render()
{
	CreateFrame(); 
#pragma region Image Rendering
	if (m_pImage1 != nullptr)
	{
		RENDER->FrameRect(
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y
		);
		RENDER->FrameImage(
			m_pImage1,
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y,
			m_curAniFrame1.lt.x,
			m_curAniFrame1.lt.y,
			m_curAniFrame1.lt.x + m_curAniFrame1.slice.x,
			m_curAniFrame1.lt.y + m_curAniFrame1.slice.y
		);
	}
	if (m_pImage2 != nullptr)
	{
		RENDER->FrameRect(
			WINSIZEX * 0.5f - m_curAniFrame2.slice.x * 0.5f + m_curAniFrame2.offset.x,
			WINSIZEY * 0.5f - m_curAniFrame2.slice.y * 0.5f + m_curAniFrame2.offset.y,
			WINSIZEX * 0.5f + m_curAniFrame2.slice.x * 0.5f + m_curAniFrame2.offset.x,
			WINSIZEY * 0.5f + m_curAniFrame2.slice.y * 0.5f + m_curAniFrame2.offset.y
		);
		RENDER->FrameImage(
			m_pImage2,
			WINSIZEX * 0.5f - m_curAniFrame2.slice.x * 0.5f + m_curAniFrame2.offset.x,
			WINSIZEY * 0.5f - m_curAniFrame2.slice.y * 0.5f + m_curAniFrame2.offset.y,
			WINSIZEX * 0.5f + m_curAniFrame2.slice.x * 0.5f + m_curAniFrame2.offset.x,
			WINSIZEY * 0.5f + m_curAniFrame2.slice.y * 0.5f + m_curAniFrame2.offset.y,
			m_curAniFrame2.lt.x,
			m_curAniFrame2.lt.y,
			m_curAniFrame2.lt.x + m_curAniFrame2.slice.x,
			m_curAniFrame2.lt.y + m_curAniFrame2.slice.y
		);
	}
	if (m_pImage1 != nullptr && m_bSelectImg1) // img1이 select되었을 때 앞으로 오도록
	{
		RENDER->FrameRect(
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y
		);
		RENDER->FrameImage(
			m_pImage1,
			WINSIZEX * 0.5f - m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f - m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y,
			WINSIZEX * 0.5f + m_curAniFrame1.slice.x * 0.5f + m_curAniFrame1.offset.x,
			WINSIZEY * 0.5f + m_curAniFrame1.slice.y * 0.5f + m_curAniFrame1.offset.y,
			m_curAniFrame1.lt.x,
			m_curAniFrame1.lt.y,
			m_curAniFrame1.lt.x + m_curAniFrame1.slice.x,
			m_curAniFrame1.lt.y + m_curAniFrame1.slice.y
		);
	}
#pragma endregion

	wstring description =	L"Frame1 Left Top : (" + to_wstring((int)m_curAniFrame1.lt.x) + L", " + to_wstring((int)m_curAniFrame1.lt.y) + L")\n" +
							L"Frame1 Slice    : (" + to_wstring((int)m_curAniFrame1.slice.x) + L", " + to_wstring((int)m_curAniFrame1.slice.y) + L")\n" +
							L"Frame1 Offset   : (" + to_wstring((int)m_curAniFrame1.offset.x) + L", " + to_wstring((int)m_curAniFrame1.offset.y) + L")\n" +
							L"Frame1 Index    : (" + to_wstring((int)m_curIndex1+1) + L" / " + to_wstring((int)m_iCount1) + L")\n\n" +

							L"Frame2 Left Top : (" + to_wstring((int)m_curAniFrame2.lt.x) + L", " + to_wstring((int)m_curAniFrame2.lt.y) + L")\n" +
							L"Frame2 Slice    : (" + to_wstring((int)m_curAniFrame2.slice.x) + L", " + to_wstring((int)m_curAniFrame2.slice.y) + L")\n"+
							L"Frame2 Offset   : (" + to_wstring((int)m_curAniFrame2.offset.x) + L", " + to_wstring((int)m_curAniFrame2.offset.y) + L")\n"+
							L"Frame2 Index    : (" + to_wstring((int)m_curIndex2+1) + L" / " + to_wstring((int)m_iCount2) + L")\n\n";

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
			int iDur = GetDlgItemInt(hDlg, IDC_EDIT_DURATION, nullptr, false);

			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);

			if (pAniToolScene->GetSelectImg1())
			{
				AniFrame* newAniFrame1 = new AniFrame[iCount];
				pAniToolScene->SetAniFrame(pAniToolScene->GetCurFrame1(), Vector(iLtX, iLtY), Vector(iSliceX, iSliceY));
				AniFrame* aniArr = pAniToolScene->GetAniArr1();
				if (aniArr == nullptr)
				{
					// 생성된 애니메이션 배열이 없었을 경우 새로 생성된 aniframe 넣어주기
					pAniToolScene->GetAniArr1() = newAniFrame1;
					pAniToolScene->SetCount1(iCount);	// 크기 지정
					pAniToolScene->SetCurIndex1(0);		// 인덱스 지정
					pAniToolScene->SetDuration1(iDur);
				}
				else
				{
					// 아닐경우 생성했던 aniframe 지워주기
					delete[] newAniFrame1;
				}
			}
			else if (pAniToolScene->GetSelectImg2())
			{
				AniFrame* newAniFrame2 = new AniFrame[iCount];
				pAniToolScene->SetAniFrame(pAniToolScene->GetCurFrame2(), Vector(iLtX, iLtY), Vector(iSliceX, iSliceY));
				AniFrame* aniArr = pAniToolScene->GetAniArr2();
				if (aniArr == nullptr)
				{
					// 생성된 애니메이션 배열이 없었을 경우 새로 생성된 aniframe 넣어주기
					pAniToolScene->GetAniArr2() = newAniFrame2;
					pAniToolScene->SetCount2(iCount);
					pAniToolScene->SetCurIndex2(0);
					pAniToolScene->SetDuration2(iDur);
				}
				else
				{
					// 아닐경우 생성했던 aniframe 지워주기
					delete[] newAniFrame2;
				}
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
		else if (LOWORD(wParam) == IDC_BUTTONPREV)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			assert(nullptr != pAniToolScene && L"AniTool Scene cast Failed");

			if (pAniToolScene->GetSelectImg1())
			{
				pAniToolScene->GoToPrevAni();
			}
			else if (pAniToolScene->GetSelectImg2())
			{
				pAniToolScene->GoToPrevAni();
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTONNEXT)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			assert(nullptr != pAniToolScene && L"AniTool Scene cast Failed");

			pAniToolScene->GoToNextAni();
		}
		return (INT_PTR)TRUE;
		break;
	}
	return (INT_PTR)FALSE;
}