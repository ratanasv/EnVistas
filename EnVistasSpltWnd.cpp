#include "stdafx.h"
#include "EnVistasSpltWnd.h"
#include "EnVistasControl.h"
#include "envision_vistas_window.h"

IMPLEMENT_DYNAMIC(EnVistasSpltWnd, StaticSplitterWnd)
EnVistasSpltWnd::EnVistasSpltWnd()
:StaticSplitterWnd()
{
}


EnVistasSpltWnd::~EnVistasSpltWnd()
{
}

int EnVistasSpltWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	((EnVistasControl*)m_pLeft)->Create(EnVistasControl::IDD, this);
	((EnVistasWnd*)m_pRight)->Create(NULL, "VISTASBackendForEnvision", WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0,0,0,0), this, 2002);

	return 0;
}

void EnVistasSpltWnd::MoveBar(int newLocation)
{
	StaticSplitterWnd::MoveBar(newLocation);
}

void EnVistasSpltWnd::ControlAndDisp(CWnd *pCtrl, CWnd *pDisp)
{
	StaticSplitterWnd::LeftRight(pCtrl, pDisp);
}

BEGIN_MESSAGE_MAP(EnVistasSpltWnd, StaticSplitterWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//  message handlers

void EnVistasSpltWnd::OnSize(UINT nType, int cx, int cy)
{
	StaticSplitterWnd::OnSize(nType, cx, cy);
}

void EnVistasSpltWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	StaticSplitterWnd::OnMouseMove(nFlags, point);
}

void EnVistasSpltWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	StaticSplitterWnd::OnLButtonDown(nFlags, point);
}

void EnVistasSpltWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	StaticSplitterWnd::OnLButtonUp(nFlags, point);
}

void EnVistasSpltWnd::Draw()
{
	StaticSplitterWnd::Draw();
}
