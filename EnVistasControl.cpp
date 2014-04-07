// EnVistasControl.cpp : implementation file
//

#include "stdafx.h"
#include "EnVistas.h"
#include "EnVistasControl.h"
#include "afxdialogex.h"


// EnVistasControl dialog

IMPLEMENT_DYNAMIC(EnVistasControl, CDialogEx)

EnVistasControl::EnVistasControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(EnVistasControl::IDD, pParent)
{

}

EnVistasControl::~EnVistasControl()
{
}

void EnVistasControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TIMELINE_SLIDER, _timelineSlider);
}


BEGIN_MESSAGE_MAP(EnVistasControl, CDialogEx)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// EnVistasControl message handlers


//void EnVistasControl::OnReleasedcaptureTimelineSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


void EnVistasControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int what = _timelineSlider.GetPos();
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
