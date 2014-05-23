#pragma once
#include "StaticSplitterWnd.h"

class EnVistasSpltWnd : public StaticSplitterWnd
{
	DECLARE_DYNAMIC(EnVistasSpltWnd)

public:
	EnVistasSpltWnd();
	virtual ~EnVistasSpltWnd();

	void ControlAndDisp(CWnd *pCtrl, CWnd *pDisp);
	void MoveBar(int newLocation);
	void Draw();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

