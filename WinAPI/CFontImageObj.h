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

	bool m_bIsFixed;

	vector<Vector>* m_curFont;

	vector<Vector> m_vecDefault;
	vector<Vector> m_vecMission;
	vector<Vector> m_vecCoin;
	vector<Vector> m_vecScore;
	vector<Vector> m_vecUi;
	vector<Vector> m_vecTime;

	queue<CImageObject*> m_queueImgObj;
	map<const wstring&, queue<CImageObject*>> m_mapFont;
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
	void CreateImgObj(const wstring& content, const wstring& name, Vector startPos, int count, FontType font);
	void SetInterval(float interval) { m_fInterval = interval; }
	void SetFixed(bool fixed) { m_bIsFixed = fixed; }
	queue<CImageObject*> FindImgObjQueue(const wstring& name);
	//queue<CImageObject*> GetImageObj(const wstring& name);// { return m_queueImgObj; }
	void Show(const wstring& name);
	void DeleteObj();
};

