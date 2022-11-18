#include "framework.h"
#include "CBridge.h"

#include "CColliderObject.h"
#include "CImageObject.h"
#include "CCollider.h"

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
	vector<ImgFrame> m_vecBridgeImg;
	m_vecBridgeImg = ReadAniFile(L"Bridge");
	float startPosX = m_vecPos.x;
	float startPosY = m_vecPos.y;
	for (int i = 0; i < m_vecBridgeImg.size(); i++)
	{
		if (!(i == 0))
			startPosX += m_vecBridgeImg[i - 1].slice.x * m_fExtension;
		CImageObject* pImgObj = new CImageObject;
		pImgObj->SetExtension(m_fExtension);
		pImgObj->SetImage(m_pBridgeImg);
		pImgObj->SetLayer(m_layer);
		pImgObj->SetPos(startPosX, startPosY);
		pImgObj->SetRenderAsFrame(true);
		pImgObj->SetIndex(i);
		pImgObj->SetScale(m_vecBridgeImg[i].slice * m_fExtension);
		pImgObj->SetSourceInfo(m_vecBridgeImg[i].lt.x, m_vecBridgeImg[i].lt.y, m_vecBridgeImg[i].slice.x, m_vecBridgeImg[i].slice.y);
		ADDOBJECT(pImgObj); // curimgobj
		CImageObject* copyObj = new CImageObject(*pImgObj);
		m_vecBridgeObj.push_back(copyObj);
		m_deqCurRenderObj.push_back(pImgObj);

		//ADDOBJECT(ColObj);
		//CColliderObject* copyObj = new CColliderObject(*ColObj);
		//m_vecColliderObj.push_back(copyObj);
		//m_deqCurColliderObj.push_back(ColObj);
	}
}

void CBridge::InitColliderObj()
{
	for (int i = 0; i < m_vecBridgeObj.size(); i++)
	{
		CColliderObject* ColObj = new CColliderObject;
		ColObj->SetExtension(m_fExtension);
		ColObj->SetPos(m_vecBridgeObj[i]->GetPos());
		switch (i)
		{
		case 0:
		case 1:
			ColObj->SetName(L"ground");
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 50));
			ColObj->SetScale(33, 10);
			//ColObj->SetType()
			//ColObj->SetOwner();
			break;
		case 2:
		case 6:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(-12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 47));
			ColObj->SetScale(35, 10);
			break;
		case 3:
		case 7:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(-12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 40));
			ColObj->SetScale(35, 10);
			break;
		case 8:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(-12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 33));
			ColObj->SetScale(35, 10);
			break;
		case 9:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(-12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 26));
			ColObj->SetScale(35, 10);
			break;
			//ColObj->SetType()
			//ColObj->SetOwner();
		case 4:
		case 12:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 40));
			ColObj->SetScale(35, 10);
			break;
		case 5:
		case 13:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 47));
			ColObj->SetScale(35, 10);
			break;
		case 10:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 26));
			ColObj->SetScale(35, 10);
			break;
		case 11:
			ColObj->SetName(L"slopeGround");
			ColObj->SetRot(12);
			ColObj->SetType(ColliderType::Obb);
			ColObj->SetOffset(Vector(m_vecBridgeObj[i]->GetScale().x / m_fExtension * 0.5f, 33));
			ColObj->SetScale(35, 10);
			break;
		default:
			break;
		}
		CColliderObject* copyObj = new CColliderObject(*ColObj); 
		//CCollider* pColl = new CCollider(*copyObj->GetCollider());
		//ColObj->GetCollider() = nullptr;
		//pColl->SetOwner(ColObj);
		//ColObj->GetCollider() = pColl;
		m_vecColliderObj.push_back(copyObj);
		m_deqCurColliderObj.push_back(ColObj);
		ADDOBJECT(ColObj);
	}
}

void CBridge::InitBrokenBridge()
{
	vector<ImgFrame> m_vecBrokenBridgeImg;
	m_vecBrokenBridgeImg = ReadAniFile(L"BrokenBridge");
	float startPosX = m_vecPos.x;
	float startPosY = m_vecPos.y;
	for (int i = 0; i < m_vecBrokenBridgeImg.size(); i++)
	{
		if (!(i == 0))
			startPosX += m_vecBrokenBridgeImg[i - 1].slice.x * m_fExtension;
		CImageObject* pImgObj = new CImageObject;
		pImgObj->SetExtension(m_fExtension);
		pImgObj->SetImage(m_pBridgeImg);
		pImgObj->SetLayer(m_layer);
		pImgObj->SetPos(startPosX, startPosY);
		pImgObj->SetRenderAsFrame(true);
		pImgObj->SetIndex(i);
		pImgObj->SetScale(m_vecBrokenBridgeImg[i].slice * m_fExtension);
		pImgObj->SetSourceInfo(m_vecBrokenBridgeImg[i].lt.x, m_vecBrokenBridgeImg[i].lt.y, m_vecBrokenBridgeImg[i].slice.x, m_vecBrokenBridgeImg[i].slice.y);
		m_vecBridgeObj.push_back(pImgObj);
	}
	m_vecBrokenBridgeImg = ReadAniFile(L"BrokenBridge");
}

void CBridge::Init()
{
	m_fExtension = 4.f;
	m_pBridgeImg = RESOURCE->LoadImg(L"Bridge", L"Image\\BackGround\\BossBridge.png");
	m_vecPos = Vector(WINSIZEX * 0.1f, WINSIZEY - 79 * m_fExtension);
	InitBridge();
	InitColliderObj();
	InitBrokenBridge();
	considerIndex = m_deqCurRenderObj.size()-2;
	//m_vecCurRenderImg = m_vecBridgeImg;
	//m_vecCurRenderImg[4] = m_vecBrokenBridgeImg[m_vecCurRenderImg.size() + 4];
}

void CBridge::Update()
{
	// 보스와 충돌하면 다리 없어지기 (alpha == 0)
	if (m_deqCurRenderObj[considerIndex]->GetPos().x < CAMERA->GetLookAt().x + WINSIZEX * 0.5f)
	{
		(considerIndex == m_deqCurRenderObj.size() - 1) ? considerIndex = 0 : considerIndex++;
		for (int i = 0; i < m_deqCurRenderObj.size(); i++)
		{
			if (m_deqCurRenderObj[i]->GetAlpha() == 0)
			{
				int index = m_deqCurRenderObj[0]->GetIndex();
				//CImageObject* frontObj = new CImageObject(*m_deqCurRenderObj[0]);
				DELETEOBJECT(m_deqCurRenderObj[0]);
				m_deqCurRenderObj.pop_front();
				CImageObject* frontImgObj = new CImageObject(*m_vecBridgeObj[index]);
				CImageObject lastImgObj = *m_deqCurRenderObj[m_deqCurRenderObj.size() - 1];
				frontImgObj->SetPos(Vector(lastImgObj.GetPos().x + lastImgObj.GetScale().x, lastImgObj.GetPos().y));
				m_deqCurRenderObj.push_back(frontImgObj);
				ADDOBJECT(frontImgObj);

				m_deqCurColliderObj[0]->SetPos(Vector(lastImgObj.GetPos().x + lastImgObj.GetScale().x, lastImgObj.GetPos().y));
				m_deqCurColliderObj.pop_front();
				m_deqCurColliderObj.push_back(m_deqCurColliderObj[0]);
			}
			else break;
		}
	}

	if (BUTTONDOWN('R'))
	{
		m_deqCurRenderObj[0]->SetAlpha(0);
	}
}

void CBridge::Render()
{
	//float startPosX = m_vecPos.x;
	//float startPosY = m_vecPos.y;
	//for (int i = 0; i < m_vecCurRenderImg.size(); i++)
	//{
	//	if(!(i==0))
	//		startPosX += m_vecCurRenderImg[i-1].slice.x * m_fExtension;
	//	RENDER->FrameImage(m_pBridgeImg, startPosX, startPosY, startPosX + m_vecCurRenderImg[i].slice.x * m_fExtension, startPosY + m_vecCurRenderImg[i].slice.y * m_fExtension,
	//		m_vecCurRenderImg[i].lt.x, m_vecCurRenderImg[i].lt.y, m_vecCurRenderImg[i].lt.x + m_vecCurRenderImg[i].slice.x, m_vecCurRenderImg[i].lt.y + m_vecCurRenderImg[i].slice.y);
	//}
}

void CBridge::Release()
{
	for (auto obj : m_vecBridgeObj)
	{
		delete obj;
	}
	m_vecBridgeObj.clear();

	for (auto obj : m_vecColliderObj)
	{
		delete obj;
	}
	m_vecColliderObj.clear();
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
