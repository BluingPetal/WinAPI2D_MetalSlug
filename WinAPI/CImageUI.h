#pragma once
#include "CUI.h"
class CImageUI : public CUI
{
public: 
	CImageUI();
	virtual ~CImageUI();

private:



private:
	void Init() override;
	void Render() override;
	void Update() override;
	void Release() override;

private:
	void OnMouseEnter() override;
	void OnMouseOver()	override;
	void OnMouseExit()	override;
	void OnMouseUp()	override;
	void OnMouseDown()	override;
	void OnMouseClicked()	override;
};

