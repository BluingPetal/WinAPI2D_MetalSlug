#pragma once
#include "CGameObject.h"

class CColliderObject;
class CImageObject;
class CImage;
struct ImgFrame
{
	int index;
	Vector lt, slice, offset;
};

class CBridge : public CGameObject
{
public:
	CBridge();
	virtual ~CBridge();

private:
	CImage* m_pBridgeImg;

	vector<CImageObject*> m_vecBridgeObj;
	vector<ImgFrame> m_vecBrokenBridgeImg;
	vector<CColliderObject*> m_vecColliderObj;

	vector<CColliderObject*> m_vecCurColliderObj;
	vector<CImageObject*> m_vecCurRenderObj;

	vector<ImgFrame> m_vecNextRenderImg; 

private:
	vector<ImgFrame> ReadAniFile(const wstring& str);
	void InitBridge();
	void InitColliderObj();
	void InitBrokenBridge();




private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void OnCollisionEnter(CCollider* pOtherCollider) override;
	void OnCollisionStay(CCollider* pOtherCollider) override;
	void OnCollisionExit(CCollider* pOtherCollider) override;
};

