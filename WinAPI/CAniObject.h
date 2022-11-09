#pragma once
#include "CGameObject.h"

class CAnimator;
class CImage;

class CAniObject : public CGameObject
{
public:
	CAniObject();
	virtual ~CAniObject();

private:
	CImage* m_pImg;
	CAnimator* m_pAnimator;
	Vector m_vecOffset;

public:
	CAnimator* GetAnimator();
	void SetImage(CImage* pImage);
	void SetOffset(Vector vecOffset) { m_vecOffset = vecOffset; }
	void SetPosWithFirstLt();
	void AniObjAddCollider(ColliderType type, Vector scale, Vector offsetPos);

private:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

