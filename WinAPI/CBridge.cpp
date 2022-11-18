#include "framework.h"
#include "CBridge.h"

#include "CColliderObject.h"

CBridge::CBridge()
{
}

CBridge::~CBridge()
{
}

vector<ImgFrame> CBridge::ReadAniFile(const wstring& str)
{
	vector<ImgFrame> vecFrame;
	ImgFrame imgFrame;
	FILE* pFile = nullptr;

	wstring path = PATH->GetPath() + L"\\AniData\\BackGround\\" + str + L".ani";
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
		imgFrame.index = i;
		imgFrame.lt = lt;
		imgFrame.slice = slice;
		imgFrame.offset = offset;
		vecFrame.push_back(imgFrame);
	}

	fclose(pFile);
	return vecFrame;
}

void CBridge::InitBridge()
{
	m_vecBridgeImg = ReadAniFile(L"Bridge");
}

void CBridge::InitColliderObj()
{
	for (int i = 0; i < m_vecBridgeImg.size(); i++)
	{
		CColliderObject* ColObj = new CColliderObject;
		if (i == 2 || i == 3 || i == 6 || i == 7 || i == 8 || i == 9)
		{
			//ColObj->SetExtension(m_fExtension);
			ColObj->SetName(L"slopeGround");
			//ColObj->SetPos(m_vecBridgeImg[i].)
		}
		else if (i == 4 || i == 5 || i == 10 || i == 11 || i == 12 || i == 13)
		{
			//ColObj->SetExtension(m_fExtension);
			ColObj->SetName(L"slopeGround");
		}
		else
		{
			//ColObj->SetExtension(m_fExtension);
			ColObj->SetName(L"ground");
		}
	}
}

void CBridge::InitBrokenBridge()
{
	m_vecBrokenBridgeImg = ReadAniFile(L"BrokenBridge");
}

void CBridge::Init()
{
	m_fExtension = 4.f;
	m_pBridgeImg = RESOURCE->LoadImg(L"Bridge", L"Image\\BackGround\\BossBridge.png");
	InitBridge();
	InitColliderObj();
	InitBrokenBridge();
	m_vecPos = Vector(WINSIZEX * 0.1f, WINSIZEY - m_vecBridgeImg[0].slice.y * m_fExtension);
	m_vecCurRenderImg = m_vecBridgeImg;
	//m_vecCurRenderImg[4] = m_vecBrokenBridgeImg[m_vecCurRenderImg.size() + 4];
}

void CBridge::Update()
{
}

void CBridge::Render()
{
	float startPosX = m_vecPos.x;
	float startPosY = m_vecPos.y;
	for (int i = 0; i < m_vecCurRenderImg.size(); i++)
	{
		if(!(i==0))
			startPosX += m_vecCurRenderImg[i-1].slice.x * m_fExtension;
		RENDER->FrameImage(m_pBridgeImg, startPosX, startPosY, startPosX + m_vecCurRenderImg[i].slice.x * m_fExtension, startPosY + m_vecCurRenderImg[i].slice.y * m_fExtension,
			m_vecCurRenderImg[i].lt.x, m_vecCurRenderImg[i].lt.y, m_vecCurRenderImg[i].lt.x + m_vecCurRenderImg[i].slice.x, m_vecCurRenderImg[i].lt.y + m_vecCurRenderImg[i].slice.y);
	}
}

void CBridge::Release()
{
}

void CBridge::OnCollisionEnter(CCollider* pOtherCollider)
{
}

void CBridge::OnCollisionStay(CCollider* pOtherCollider)
{
}

void CBridge::OnCollisionExit(CCollider* pOtherCollider)
{
}
