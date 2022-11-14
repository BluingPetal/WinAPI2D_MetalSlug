#include "framework.h"
#include "CFontImageObj.h"

#include "CImage.h"
#include "CImageObject.h"

CFontImageObj::CFontImageObj()
{
	m_pImage = RESOURCE->LoadImg(L"Font", L"Image\\Font\\Font.png");
	m_curFont = nullptr;
	m_fInterval = 0.7f;
	m_bIsFixed = false;
}

CFontImageObj::~CFontImageObj()
{
}

// 외부에서 쓸 함수, create img object -> 해당 name이 존재할 경우 무시, 없을 경우에만 만들어서 map에 넣음
void CFontImageObj::CreateImgObj(const wstring& content, const wstring& name,Vector startPos, int count, FontType font)
{
	// 자료구조에 해당 이름의 queue가 있었을 경우
	if (FindImgObjQueue(name) != queue<CImageObject*>()) // -> 이때 덮어쓰기 가능하도록 설정
	{
		CImageObject* firstImgObj = m_mapFont[name].front();
		Vector offset = startPos - firstImgObj->GetPos();

		for (int i = 0; i < count; i++)
		{
			CImageObject* imgObj = m_mapFont[name].front();
			imgObj->SetPos(imgObj->GetPos() + offset);
		}
		return;
	}
	//if (m_queueImgObj.size() != 0)
	//	DeleteObj();

	for (int i = 0; i < count; i++)
	{
		CImageObject* imgObj = new CImageObject;
		imgObj->SetImage(m_pImage);

		int index = -1;
		switch (font)
		{
		case FontType::Default:
			m_curFont = &m_vecDefault;
			index = FindImgInDefault(content[i]);			
			break;
		case FontType::Mission:
			m_curFont = &m_vecMission;
			index = FindImgInMission(content[i]);
			break;
		case FontType::Coin:
			m_curFont = &m_vecCoin;
			index = FindImgInCoin(content[i]);
			break;
		case FontType::Score:
			index = FindImgInScore(content[i]);
			break;
		case FontType::Ui:
			index = FindImgInUi(content[i]);
			break;
		case FontType::Time:
			m_curFont = &m_vecTime;
			index = FindImgInTime(content[i]);
			break;
		}

		if (index >= 0)
		{
			imgObj->SetPos(startPos.x, startPos.y);
			startPos.x += ((*m_curFont)[index + 1].x * m_fExtension * m_fInterval);
			imgObj->SetFixed(m_bIsFixed);
			imgObj->SetExtension(m_fExtension);
			imgObj->SetRenderAsFrame(true);
			imgObj->SetLayer(Layer::ForeGround);
			imgObj->SetSourceInfo((*m_curFont)[index].x, (*m_curFont)[index].y, (*m_curFont)[index + 1].x, (*m_curFont)[index + 1].y);
			m_queueImgObj.push(imgObj);
			//imgObj->SetAlpha(0);
			//ADDOBJECT(imgObj);
		}
		else // 띄어쓰기
		{
			startPos.x += ((*m_curFont)[index + 2].x * m_fExtension * m_fInterval);
		}
	}
	m_mapFont.insert(make_pair(name, m_queueImgObj));
}

void CFontImageObj::DeleteObj()
{
	while (!m_queueImgObj.empty())
	{
		if (!(m_queueImgObj.front()->GetReserveDelete()))
			//m_queueImgObj.front()->SetReserveDelete();
			DELETEOBJECT(m_queueImgObj.front());
		m_queueImgObj.pop();
	}
	/*
	for (int i = 0; i < m_vecImgObj.size(); i++)
	{
		m_vecImgObj[i]->SetReserveDelete();
	}
	m_vecImgObj.clear();
	*/
}

void CFontImageObj::InitFont(const wstring& name, FontType font)
{
	FILE* pFile = nullptr;

	wstring path = PATH->GetPath() + L"\\AniData\\Font\\" + name + L".ani";
	_wfopen_s(&pFile, path.c_str(), L"rb");      // w : write, b : binary
	assert(pFile);

	int count;
	Vector lt;
	Vector slice;
	Vector offset;

	fread(&count, sizeof(UINT), 1, pFile);

	for (int i = 0; i < count; i++)
	{
		fread(&lt, sizeof(Vector), 1, pFile);
		fread(&slice, sizeof(Vector), 1, pFile);
		fread(&offset, sizeof(Vector), 1, pFile);
		switch (font)
		{
		case FontType::Default:
			m_vecDefault.push_back(lt);
			m_vecDefault.push_back(slice);
			m_vecDefault.push_back(offset);
			break;
		case FontType::Mission:
			m_vecMission.push_back(lt);
			m_vecMission.push_back(slice);
			m_vecMission.push_back(offset);
			break;
		case FontType::Coin:
			m_vecCoin.push_back(lt);
			m_vecCoin.push_back(slice);
			m_vecCoin.push_back(offset);
			break;
		case FontType::Score:
			m_vecScore.push_back(lt);
			m_vecScore.push_back(slice);
			m_vecScore.push_back(offset);
			break;
		case FontType::Ui:
			m_vecUi.push_back(lt);
			m_vecUi.push_back(slice);
			m_vecUi.push_back(offset);
			break;
		case FontType::Time:
			m_vecTime.push_back(lt);
			m_vecTime.push_back(slice);
			m_vecTime.push_back(offset);
			break;
		}
	}
	fclose(pFile);
}

UINT CFontImageObj::FindImgInDefault(const wchar_t str)
{
	switch (str)
	{
	case 'a':
		return 0 * 3;
		break;
	case 'b':
		return 1 * 3;
		break;
	case 'c':
		return 2 * 3;
		break;
	case 'd':
		return 3 * 3;
		break;
	case 'e':
		return 4 * 3;
		break;
	case 'f':
		return 5 * 3;
		break;
	case 'g':
		return 6 * 3;
		break;
	case 'h':
		return 7 * 3;
		break;
	case 'i':
		return 8 * 3;
		break;
	case 'j':
		return 9 * 3;
		break;
	case 'k':
		return 10 * 3;
		break;
	case 'l':
		return 11 * 3;
		break;
	case 'm':
		return 12 * 3;
		break;
	case 'n':
		return 13 * 3;
		break;
	case 'o':
		return 14 * 3;
		break;
	case 'p':
		return 15 * 3;
		break;
	case 'q':
		return 16 * 3;
		break;
	case 'r':
		return 17 * 3;
		break;
	case 's':
		return 18 * 3;
		break;
	case 't':
		return 19 * 3;
		break;
	case 'u':
		return 20 * 3;
		break;
	case 'v':
		return 21 * 3;
		break;
	case 'w':
		return 22 * 3;
		break;
	case 'x':
		return 23 * 3;
		break;
	case 'y':
		return 24 * 3;
		break;
	case 'z':
		return 25 * 3;
		break;
	case '0':
		return 26 * 3;
		break;
	case '1':
		return 27 * 3;
		break;
	case '2':
		return 28 * 3;
		break;
	case '3':
		return 29 * 3;
		break;
	case '4':
		return 30 * 3;
		break;
	case '5':
		return 31 * 3;
		break;
	case '6':
		return 32 * 3;
		break;
	case '7':
		return 33 * 3;
		break;
	case '8':
		return 34 * 3;
		break;
	case '9':
		return 35 * 3;
		break;
	default:
		return -1;
		break;
	}
}

UINT CFontImageObj::FindImgInMission(const wchar_t str)
{
	switch (str)
	{
	case 'a':
		return 0 * 3;
		break;
	case 'b':
		return 1 * 3;
		break;
	case 'c':
		return 2 * 3;
		break;
	case 'd':
		return 3 * 3;
		break;
	case 'e':
		return 4 * 3;
		break;
	case 'f':
		return 5 * 3;
		break;
	case 'g':
		return 6 * 3;
		break;
	case 'h':
		return 7 * 3;
		break;
	case 'i':
		return 8 * 3;
		break;
	case 'j':
		return 9 * 3;
		break;
	case 'k':
		return 10 * 3;
		break;
	case 'l':
		return 11 * 3;
		break;
	case 'm':
		return 12 * 3;
		break;
	case 'n':
		return 13 * 3;
		break;
	case 'o':
		return 14 * 3;
		break;
	case 'p':
		return 15 * 3;
		break;
	case 'q':
		return 16 * 3;
		break;
	case 'r':
		return 17 * 3;
		break;
	case 's':
		return 18 * 3;
		break;
	case 't':
		return 19 * 3;
		break;
	case 'u':
		return 20 * 3;
		break;
	case 'v':
		return 21 * 3;
		break;
	case 'w':
		return 22 * 3;
		break;
	case 'x':
		return 23 * 3;
		break;
	case 'y':
		return 24 * 3;
		break;
	case 'z':
		return 25 * 3;
		break;
	default:
		return -1;
		break;
	}
}

UINT CFontImageObj::FindImgInCoin(const wchar_t str)
{
	switch (str)
	{
	case 'a':
		return 0 * 3;
		break;
	case 'c':
		return 1 * 3;
		break;
	case 'd':
		return 2 * 3;
		break;
	case 'e':
		return 3 * 3;
		break;
	case 'i':
		return 4 * 3;
		break;
	case 'l':
		return 5 * 3;
		break;
	case 'n':
		return 6 * 3;
		break;
	case 'o':
		return 7 * 3;
		break;
	case 'p':
		return 8 * 3;
		break;
	case 'r':
		return 9 * 3;
		break;
	case 's':
		return 10 * 3;
		break;
	case 't':
		return 11 * 3;
		break;
	case 'u':
		return 12 * 3;
		break;
	case 'w':
		return 13 * 3;
		break;
	case '0':
		return 14 * 3;
		break;
	case '1':
		return 15 * 3;
		break;
	case '2':
		return 16 * 3;
		break;
	case '3':
		return 17 * 3;
		break;
	case '4':
		return 18 * 3;
		break;
	case '5':
		return 19 * 3;
		break;
	case '6':
		return 20 * 3;
		break;
	case '7':
		return 21 * 3;
		break;
	case '8':
		return 22 * 3;
		break;
	case '9':
		return 23 * 3;
		break;
	default:
		return -1;
		break;
	}
}

UINT CFontImageObj::FindImgInScore(const wchar_t str)
{
	return 0;
}

UINT CFontImageObj::FindImgInUi(const wchar_t str)
{
	return 0;
}

// 해당 이미지의 lt, slice, offset을 담은 처음 index를 반환
UINT CFontImageObj::FindImgInTime(const wchar_t str)
{
	switch (str)
	{
	case '0':
		return 0 * 3;
		break;
	case '1':
		return 1 * 3;
		break;
	case '2':
		return 2 * 3;
		break;
	case '3':
		return 3 * 3;
		break;
	case '4':
		return 4 * 3;
		break;
	case '5':
		return 5 * 3;
		break;
	case '6':
		return 6 * 3;
		break;
	case '7':
		return 7 * 3;
		break;
	case '8':
		return 8 * 3;
		break;
	case '9':
		return 9 * 3;
		break;
	case ':':
		return 10 * 3;
		break;
	case '.':
		return 11 * 3;
		break;
	default:
		return -1;
		break;
	}
}

queue<CImageObject*> CFontImageObj::FindImgObjQueue(const wstring& name)
{
	if (m_mapFont[name].size() != 0)
		return m_mapFont[name];
	else
		return NULL;
}

void CFontImageObj::Init()
{
	InitFont(L"Default", FontType::Default);
	InitFont(L"Mission", FontType::Mission);
	InitFont(L"Coin", FontType::Coin);
	//InitFont(L"Score", FontType::Score);
	//InitFont(L"Ui", FontType::Ui);
	InitFont(L"Time", FontType::Time);
}

void CFontImageObj::Update()
{
}

void CFontImageObj::Render()
{
}

void CFontImageObj::Release()
{
}

void CFontImageObj::Show(const wstring& name)
{
	queue<CImageObject*> imgObjQueue = FindImgObjQueue(name);
	if (imgObjQueue == queue<CImageObject*>())
		assert(0 && L"No font image queue");

	while (!imgObjQueue.empty())
	{
		ADDOBJECT(imgObjQueue.front());
		imgObjQueue.pop();
	}
}