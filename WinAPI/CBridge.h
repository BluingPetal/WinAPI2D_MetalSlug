#pragma once
#include "CGameObject.h"

class CColliderObject;
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

	vector<ImgFrame> m_vecBridgeImg;
	vector<ImgFrame> m_vecBrokenBridgeImg;
	vector<CColliderObject*> m_vecColliderObj;
	vector<ImgFrame> m_vecCurRenderImg;
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

