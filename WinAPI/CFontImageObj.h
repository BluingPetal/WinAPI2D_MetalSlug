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
	// Left top과 Size를 담을 벡터
	//map<const wstring&, vector<Vector>>m_mapFont;
	CImage* m_pImage;
	float m_fInterval;

	bool m_isFixed;

	vector<Vector> m_vecDefault;
	vector<Vector> m_vecMission;
	vector<Vector> m_vecCoin;
	vector<Vector> m_vecScore;
	vector<Vector> m_vecUi;
	vector<Vector> m_vecTime;

	vector<CImageObject*> m_vecImgObj;

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
	void CreateImg(const wstring& content, Vector startPos, UINT count, FontType font);
	void SetInterval(float interval) { m_fInterval = interval; }
	vector<CImageObject*> GetImageObj() { return m_vecImgObj; }
	void DeleteObj();
};

