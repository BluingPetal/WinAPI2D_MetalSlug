#pragma once
#include "CGameObject.h"

enum class FontType { Default, Mission, Coin, Score, Ui, Time };
class CImageObject;

class CFontImageObj : public CGameObject
{
public:
	CFontImageObj();
	virtual ~CFontImageObj();

private:
	CImage* m_pImage; // -> font image

	vector<Vector>* m_curFont;
	// source의 left top, size, offset을 담은 벡터 자료구조
	vector<Vector> m_vecDefault;
	vector<Vector> m_vecMission;
	vector<Vector> m_vecCoin;
	vector<Vector> m_vecScore;
	vector<Vector> m_vecUi;
	vector<Vector> m_vecTime;

	float m_fInterval;	// 자간
	bool m_bIsFixed;	// 화면 고정 여부

	//map<const wstring&, queue<CImageObject*>> m_mapFont;
	vector<CImageObject*> m_vecImgObj;
	Vector m_vecStartPos;

private:
	void InitFont(const wstring& name, FontType font);
	UINT FindImgInDefault(const wchar_t str);
	UINT FindImgInMission(const wchar_t str);
	UINT FindImgInCoin(const wchar_t str);
	UINT FindImgInScore(const wchar_t str);
	UINT FindImgInUi(const wchar_t str);
	UINT FindImgInTime(const wchar_t str);

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	//queue<CImageObject*>* FindImgObjQueue(const wstring& name);
	void SetInterval(float interval) { m_fInterval = interval; }
	void SetFixed(bool fixed) { m_bIsFixed = fixed; }
	//queue<CImageObject*> GetImageObj(const wstring& name);// { return m_queueImgObj; }
	vector<CImageObject*> GetImageObj() { return m_vecImgObj; }

public:
	void CreateImgObj(const wstring& content, Vector startPos, int count, FontType font);
	void Show();
	void DeleteObj();
};

