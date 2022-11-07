#include "framework.h"
#include "CSceneAniTool.h"

#include "CPanel.h"
#include "CImage.h"
#include "CPlayer.h"

LRESULT CALLBACK    WinAniToolProc(HWND, UINT, WPARAM, LPARAM);

CSceneAniTool::CSceneAniTool()
{
	m_hWndAniTool = 0;

	m_pImage1 = m_pImage2 = nullptr;
	pPanel = pDescptPanel = nullptr;
	aniArr1 = aniArr2 = nullptr;

	m_curIndex1 = m_curIndex2 = 0;
	m_iCount1 = m_iCount2 = 0;
	m_duration1 = m_duration2 = 1;
	m_fAccTime1 = m_fAccTime2 = 0;
	m_iCurFrame1 = m_iCurFrame2 = 0;

	m_bSelectImg1 = m_bSelectImg2 = false;
	m_bPlay = false;

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

void CSceneAniTool::SetAniFrame(AniFrame& frame, Vector lt, Vector slice, float duration)
{
	frame.lt = lt; frame.slice = slice; frame.duration = duration;
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

void CSceneAniTool::ToolSaveAniData()
{
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);  // 구조체 사이즈.
	ofn.hwndOwner = hWnd;					// 부모 윈도우 지정.
	wchar_t szName[256] = {};
	ofn.lpstrFile = szName; // 나중에 완성된 경로가 채워질 버퍼 지정.
	ofn.nMaxFile = sizeof(szName); // lpstrFile에 지정된 버퍼의 문자 수.
	ofn.lpstrFilter = L"ALL\0*.*\0ani\0*.ani"; // 필터 설정
	ofn.nFilterIndex = 0; // 기본 필터 세팅. 0는 all로 초기 세팅됨. 처음꺼.
	ofn.lpstrFileTitle = nullptr; // 타이틀 바
	ofn.nMaxFileTitle = 0; // 타이틀 바 문자열 크기. nullptr이면 0.
	wstring strTileFolder = GETPATH;
	strTileFolder += L"AniData";
	ofn.lpstrInitialDir = strTileFolder.c_str(); // 초기경로. 우리는 타일 저장할거기 때문에, content->tile 경로로 해두자.
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 스타일

	if (GetSaveFileName(&ofn))
	{
		FILE* pFile = nullptr;

		_wfopen_s(&pFile, wstring(szName).c_str(), L"wb");		// w : write, b : binary
		assert(pFile);

		if (m_bSelectImg1 && aniArr1 != nullptr)
		{
			fwrite(&(m_iCount1), sizeof(UINT), 1, pFile);
			for (int i = 0; i < m_iCount1; i++)
			{
				fwrite(&(aniArr1[i].lt), sizeof(Vector), 1, pFile);
				fwrite(&(aniArr1[i].slice), sizeof(Vector), 1, pFile);
				fwrite(&(aniArr1[i].offset), sizeof(Vector), 1, pFile);
			}
			delete[] aniArr1;
			aniArr1 = nullptr;
		}

		if (m_bSelectImg2 && aniArr2 != nullptr)
		{
			fwrite(&(m_iCount2), sizeof(UINT), 1, pFile);
			for (int i = 0; i < m_iCount2; i++)
			{
				fwrite(&(aniArr2[i].lt), sizeof(Vector), 1, pFile);
				fwrite(&(aniArr2[i].slice), sizeof(Vector), 1, pFile);
				fwrite(&(aniArr2[i].offset), sizeof(Vector), 1, pFile);
			}
			delete[] aniArr2;
			aniArr2 = nullptr;
		}

		fclose(pFile);
	}
}

void CSceneAniTool::PlayAni()
{
	m_fAccTime1 += DT;
	m_fAccTime2 += DT;
	if (aniArr1 != nullptr)
	{
		if (m_duration1 < m_fAccTime1)
		{
			if (m_iCurFrame1 == m_iCount1-1)
			{
				m_iCurFrame1 = 0;
			}
			m_iCurFrame1++;	
			m_fAccTime1 = 0;	
		}
	}
	if (aniArr2 != nullptr)
	{
		if (m_duration2 < m_fAccTime2)
		{
			if (m_iCurFrame2 == m_iCount2 - 1)
			{
				m_iCurFrame2 = 0;
			}
			m_iCurFrame2++;
			m_fAccTime2 = 0;

		}
	}
}

void CSceneAniTool::RenderAni()
{
	if (aniArr1 != nullptr)
	{
		RENDER->FrameImage(
			m_pImage1,
			100 - aniArr1[m_iCurFrame1].slice.x * 0.5f + aniArr1[m_iCurFrame1].offset.x,
			100 - aniArr1[m_iCurFrame1].slice.y * 0.5f + aniArr1[m_iCurFrame1].offset.y,
			100 + aniArr1[m_iCurFrame1].slice.x * 0.5f + aniArr1[m_iCurFrame1].offset.x,
			100 + aniArr1[m_iCurFrame1].slice.y * 0.5f + aniArr1[m_iCurFrame1].offset.y,
			aniArr1[m_iCurFrame1].lt.x,
			aniArr1[m_iCurFrame1].lt.y,
			aniArr1[m_iCurFrame1].lt.x + aniArr1[m_iCurFrame1].slice.x,
			aniArr1[m_iCurFrame1].lt.y + aniArr1[m_iCurFrame1].slice.y
		);
	}
	if (aniArr2 != nullptr)
	{
		RENDER->FrameImage(
			m_pImage2,
			100 - aniArr2[m_iCurFrame1].slice.x * 0.5f + aniArr2[m_iCurFrame2].offset.x,
			100 - aniArr2[m_iCurFrame1].slice.y * 0.5f + aniArr2[m_iCurFrame2].offset.y,
			100 + aniArr2[m_iCurFrame1].slice.x * 0.5f + aniArr2[m_iCurFrame2].offset.x,
			100 + aniArr2[m_iCurFrame1].slice.y * 0.5f + aniArr2[m_iCurFrame2].offset.y,
			aniArr2[m_iCurFrame1].lt.x,
			aniArr2[m_iCurFrame1].lt.y,
			aniArr2[m_iCurFrame1].lt.x + aniArr2[m_iCurFrame1].slice.x,
			aniArr2[m_iCurFrame1].lt.y + aniArr2[m_iCurFrame1].slice.y
		);
	}
}

void CSceneAniTool::MakeFlipAni(Vector ltFliped)
{
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);  // 구조체 사이즈.
	ofn.hwndOwner = hWnd;					// 부모 윈도우 지정.
	wchar_t szName[256] = {};
	ofn.lpstrFile = szName; // 나중에 완성된 경로가 채워질 버퍼 지정.
	ofn.nMaxFile = sizeof(szName); // lpstrFile에 지정된 버퍼의 문자 수.
	ofn.lpstrFilter = L"ALL\0*.*\0ani\0*.ani"; // 필터 설정
	ofn.nFilterIndex = 0; // 기본 필터 세팅. 0는 all로 초기 세팅됨. 처음꺼.
	ofn.lpstrFileTitle = nullptr; // 타이틀 바
	ofn.nMaxFileTitle = 0; // 타이틀 바 문자열 크기. nullptr이면 0.
	wstring strTileFolder = GETPATH;
	strTileFolder += L"AniData";
	ofn.lpstrInitialDir = strTileFolder.c_str(); // 초기경로. 우리는 타일 저장할거기 때문에, content->tile 경로로 해두자.
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 스타일

	if (GetOpenFileName(&ofn))
	{
		FILE* pFile = nullptr;
		FILE* pFileWrite = nullptr;

		_wfopen_s(&pFile, szName, L"rb");      // w : write, b : binary
		wstring writeFileName = wstring(szName) + L"_Reversed";
		_wfopen_s(&pFileWrite, writeFileName.c_str(), L"wb");      // w : write, b : binary
		assert(pFile);
		assert(pFileWrite);

		int count;
		Vector lt = Vector(0,0);
		Vector prevLt;
		Vector slice = Vector(0, 0);
		Vector prevSlice;
		Vector offset;
		Vector leftTopFliped = ltFliped;

		fread(&count, sizeof(UINT), 1, pFile);
		Vector* vecReadAni = new Vector[count];
		fwrite(&count, sizeof(UINT), 1, pFileWrite);
		Vector vecDiff = Vector(0, 0);

		for (int i = 0; i < count; i++)
		{
			prevLt = lt;
			prevSlice = slice;

			fread(&lt, sizeof(Vector), 1, pFile);
			fread(&slice, sizeof(Vector), 1, pFile);
			fread(&offset, sizeof(Vector), 1, pFile);
			Logger::Debug(L"R(" + to_wstring(lt.x) + L", " + to_wstring(lt.y) + L")");
			Logger::Debug(L"R(" + to_wstring(slice.x) + L", " + to_wstring(slice.y) + L")");
			Logger::Debug(L"R(" + to_wstring(offset.x) + L", " + to_wstring(offset.y) + L")");

			if (i != 0)
			{
				vecDiff.x = prevLt.x - lt.x - (slice.x - prevSlice.x);
				vecDiff.y = lt.y - prevLt.y;
			}

			if (i == 0)
				fwrite(&leftTopFliped, sizeof(Vector), 1, pFileWrite);
			else
			{
				leftTopFliped += vecDiff;
				fwrite(&leftTopFliped, sizeof(Vector), 1, pFileWrite);
			}

				fwrite(&slice, sizeof(Vector), 1, pFileWrite);
				offset.x *= -1;
				fwrite(&offset, sizeof(Vector), 1, pFileWrite);

				Logger::Debug(L"W(" + to_wstring(leftTopFliped.x) + L", " + to_wstring(leftTopFliped.y) + L")");
				Logger::Debug(L"W(" + to_wstring(slice.x) + L", " + to_wstring(slice.y) + L")");
				Logger::Debug(L"W(" + to_wstring(offset.x) + L", " + to_wstring(offset.y) + L")");
		}

		delete[] vecReadAni;
		vecReadAni = nullptr;

		fclose(pFileWrite);
		fclose(pFile);
	}
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
	pPanel->SetScale(Vector(200.f, 200.f));
	pPanel->SetPos(Vector(WINSIZEX - 230.f, 40.f));
	AddGameObject(pPanel);
	/*
	pDescptPanel = new CPanel;
	pDescptPanel->SetScale(Vector(500.f, 250.f));
	pDescptPanel->SetPos(Vector(WINSIZEX - 510.f, WINSIZEY - 260.f));
	AddGameObject(pDescptPanel);
	*/
}

void CSceneAniTool::Update()
{
	// 배열이 지정되지 않았을 경우 리턴
	if (aniArr1 == nullptr && aniArr2 == nullptr) return;

#pragma region  SceneAniTool Key Setting
	// Title 씬으로 돌아가기
	if (BUTTONDOWN(VK_ESCAPE))
	{
		CHANGESCENE(GroupScene::Title);
	}

	// 선택할 이미지 변경
	if (BUTTONDOWN(VK_TAB))
	{
		m_bSelectImg1 = !m_bSelectImg1;
		m_bSelectImg2 = !m_bSelectImg2;
	}

	// Left Top 조절
	if (BUTTONDOWN(VK_DOWN))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y > 0)
				m_curAniFrame1.lt += Vector(0, -1);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y > 0)
				m_curAniFrame2.lt += Vector(0, -1);
		}
	}
	if (BUTTONDOWN(VK_UP))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y + m_curAniFrame1.slice.y < m_pImage1->GetHeight())
				m_curAniFrame1.lt += Vector(0, 1);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y + m_curAniFrame2.slice.y < m_pImage2->GetHeight())
				m_curAniFrame2.lt += Vector(0, 1);
		}
	}
	if (BUTTONDOWN(VK_RIGHT))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x > 0)
				m_curAniFrame1.lt += Vector(-1, 0);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x > 0)
				m_curAniFrame2.lt += Vector(-1, 0);
		}
	}
	if (BUTTONDOWN(VK_LEFT))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x + m_curAniFrame1.slice.x < m_pImage1->GetWidth())
				m_curAniFrame1.lt += Vector(1, 0);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x + m_curAniFrame2.slice.x < m_pImage2->GetWidth())
				m_curAniFrame2.lt += Vector(1, 0);
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
	if (BUTTONDOWN(VK_NUMPAD8))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(0, -1);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(0, -1);
		}
	}
	if (BUTTONDOWN(VK_NUMPAD5))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(0, 1);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(0, 1);
		}
	}
	if (BUTTONDOWN(VK_NUMPAD4))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(-1, 0);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(-1, 0);
		}
	}
	if (BUTTONDOWN(VK_NUMPAD6))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			m_curAniFrame1.offset += Vector(1, 0);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			m_curAniFrame2.offset += Vector(1, 0);
		}
	}

	// Slice 조절
	if (BUTTONDOWN('W'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.y + m_curAniFrame1.slice.y < m_pImage1->GetHeight())
				m_curAniFrame1.slice += Vector(0, 2);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.y + m_curAniFrame2.slice.y < m_pImage2->GetHeight())
				m_curAniFrame2.slice += Vector(0, 2);
		}
	}
	if (BUTTONDOWN('S'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.slice.y > 0)
				m_curAniFrame1.slice -= Vector(0, 2);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.slice.y > 0)
				m_curAniFrame2.slice -= Vector(0, 2);
		}
	}
	if (BUTTONDOWN('D'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.lt.x + m_curAniFrame1.slice.x < m_pImage1->GetWidth())
				m_curAniFrame1.slice += Vector(2, 0);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.lt.x + m_curAniFrame2.slice.x < m_pImage2->GetWidth())
				m_curAniFrame2.slice += Vector(2, 0);
		}
	}
	if (BUTTONDOWN('A'))
	{
		if (m_bSelectImg1 && m_pImage1 != nullptr)
		{
			if (m_curAniFrame1.slice.x > 0)
				m_curAniFrame1.slice -= Vector(2, 0);
		}
		else if (m_bSelectImg2 && m_pImage2 != nullptr)
		{
			if (m_curAniFrame2.slice.x > 0)
				m_curAniFrame2.slice -= Vector(2, 0);
		}
	}

	// Zoom In/Out
	if (BUTTONSTAY(VK_F1))
	{
		float zoom = CAMERA->GetZoom();
		float setZoom = zoom - 0.005;
		if (setZoom > 0)
			CAMERA->SetZoom(setZoom);
		else
			CAMERA->SetZoom(0);
	}
	if (BUTTONSTAY(VK_F2))
	{
		float zoom = CAMERA->GetZoom();
		float setZoom = zoom + 0.005;
		CAMERA->SetZoom(setZoom);
	}

	// 애니메이션 재생
	if (BUTTONDOWN(VK_SPACE))
	{
		m_bPlay = !m_bPlay;
	}

	// duration 조절
	if (BUTTONSTAY(VK_F3))
	{
		m_duration1 -= 5 * DT;
		m_duration2 -= 5 * DT;
		if (m_duration1 < 0.1)
			m_duration1 = 0.1f;
		if (m_duration2 < 0.1)
			m_duration2 = 0.1f;
	}
	if (BUTTONSTAY(VK_F4))
	{
		m_duration1 += 5 * DT;
		m_duration2 += 5 * DT;
		if (m_duration1 > 3 )
			m_duration1 = 3.f;
		if (m_duration2 > 3)
			m_duration2 = 3.f;
	}
#pragma endregion

	if (m_bPlay)
	{
		PlayAni();
	}
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

	if (m_bPlay)
	{
		RenderAni();
	}

	wstring info =	L"Frame1 Left Top : (" + to_wstring((int)m_curAniFrame1.lt.x) + L", " + to_wstring((int)m_curAniFrame1.lt.y) + L")\n" +
					L"Frame1 Slice    : (" + to_wstring((int)m_curAniFrame1.slice.x) + L", " + to_wstring((int)m_curAniFrame1.slice.y) + L")\n" +
					L"Frame1 Offset   : (" + to_wstring((int)m_curAniFrame1.offset.x) + L", " + to_wstring((int)m_curAniFrame1.offset.y) + L")\n" +
					L"Frame1 Index    : (" + to_wstring((int)m_curIndex1+1) + L" / " + to_wstring((int)m_iCount1) + L")\n\n" +

					L"Frame2 Left Top : (" + to_wstring((int)m_curAniFrame2.lt.x) + L", " + to_wstring((int)m_curAniFrame2.lt.y) + L")\n" +
					L"Frame2 Slice    : (" + to_wstring((int)m_curAniFrame2.slice.x) + L", " + to_wstring((int)m_curAniFrame2.slice.y) + L")\n"+
					L"Frame2 Offset   : (" + to_wstring((int)m_curAniFrame2.offset.x) + L", " + to_wstring((int)m_curAniFrame2.offset.y) + L")\n"+
					L"Frame2 Index    : (" + to_wstring((int)m_curIndex2+1) + L" / " + to_wstring((int)m_iCount2) + L")\n\n";

	pPanel->SetText(info, 15);
	/*
	RENDER->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	wstring descrpt =	L"================ HOW TO USE ================ \n"
						L"ESC\t\t\t: Go To Title\t\t\n"
						L"TAB\t\t\t: Switch Target Image\t\n"
						L"N,M\t\t\t: Change Frame Size\t\n"
						L"ARROW KEYS(↑↓←→)\t : Change Left-Top Position\n"
						L"NUMPAD(8,5,4,6)\t: Change Offset\t\t\n"
						L"W,S,A,D\t\t\t: Change Slice Size\t\n"
						L"F2, F1\t\t\t: Zoom In/Out\t\t\n"
						L"F4, F3\t\t\t: Duration Up/Down\t\n"
						L"SPACE\t\t\t: Play Animation\t\t\n"
						L"============================================ \n";

	pDescptPanel->SetText(descrpt, 15);
	RENDER->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	*/
}

void CSceneAniTool::Exit()
{
}

void CSceneAniTool::Release()
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

				pAniToolScene->GoToPrevAni();
		}
		else if (LOWORD(wParam) == IDC_BUTTONNEXT)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			assert(nullptr != pAniToolScene && L"AniTool Scene cast Failed");

			pAniToolScene->GoToNextAni();
		}
		else if (LOWORD(wParam) == IDC_BUTTONSAVE)
		{
			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			assert(nullptr != pAniToolScene && L"AniTool Scene cast Failed");

			pAniToolScene->ToolSaveAniData();
		}
		else if (LOWORD(wParam) == IDC_BUTTONMAKE)
		{
			int ltX = GetDlgItemInt(hDlg, IDC_EDIT_LEFT_TOP_X, nullptr, false);
			int ltY = GetDlgItemInt(hDlg, IDC_EDIT_LEFT_TOP_Y, nullptr, false);

			CScene* pCurScene = SCENE->GetCurScene();
			CSceneAniTool* pAniToolScene = dynamic_cast<CSceneAniTool*>(pCurScene);
			assert(nullptr != pAniToolScene && L"AniTool Scene cast Failed");

			pAniToolScene->MakeFlipAni(Vector((float)ltX, (float)ltY));
		}
		return (INT_PTR)TRUE;
		break;
	}
	return (INT_PTR)FALSE;
}