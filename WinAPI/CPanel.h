#pragma once
#include "CUI.h"
class CPanel : public CUI
{
public:
	CPanel();
	virtual ~CPanel();

private:
	Vector	m_vecDragStartPos;
	bool	m_bIsDragging;
	bool	m_bDraggable;

	wstring			m_strText;
	float			m_iSizeText;
	Color			m_colorText;

public:
	bool GetDraggable();
	void SetDraggable(bool draggable);

	void SetText(const wstring& text, float fontSize = 12, Color color = Color(0, 0, 0, 1));

private:
	void Init() override;
	void Render() override;
	void Update() override;
	void Release() override;

	void OnMouseEnter() final;
	void OnMouseOver() final;
	void OnMouseExit() final;
	void OnMouseUp() final;
	void OnMouseDown() final;
	void OnMouseClicked() final;
};

