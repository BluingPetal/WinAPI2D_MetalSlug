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
	// Left top�� Size�� ���� ����
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
	void CreateImg(const wstring& content, Vector startPos, int count, FontType font);
	void SetInterval(float interval) { m_fInterval = interval; }
	void SetFixed(bool fixed) { m_bIsFixed = fixed; }
	const queue<CImageObject*>& GetImageObj() { return m_queueImgObj; }
	void DeleteObj();
};

