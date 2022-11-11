#pragma once
#include "CImageObject.h"

enum class FontType {Mission, Score, Ui, Time };

class CFontImage : public CImageObject
{
public:
	CFontImage();
	virtual ~CFontImage();

private:
	void RenderImg(Vector startPos, UINT count, wstring font);
};

