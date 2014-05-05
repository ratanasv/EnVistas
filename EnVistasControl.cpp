// EnVistasControl.cpp : implementation file
//

#include "stdafx.h"
#include "EnVistas.h"
#include "EnVistasControl.h"
#include "afxdialogex.h"
#include "EnvContextObservable.h"

using namespace std;
// EnVistasControl dialog

IMPLEMENT_DYNAMIC(EnVistasControl, CDialogEx)

EnVistasControl::EnVistasControl(CWnd* pParent, 
	shared_ptr<EnvContextObservable>& observable)
	: CDialogEx(EnVistasControl::IDD, pParent), _observable(observable)
{
	_observable->AddObserver(this);
}

EnVistasControl::~EnVistasControl()
{
	_observable->DeleteObserver(this);
}

void EnVistasControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TIMELINE_SLIDER, _timelineSlider);
}


BEGIN_MESSAGE_MAP(EnVistasControl, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// EnVistasControl message handlers


//void EnVistasControl::OnReleasedcaptureTimelineSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


void EnVistasControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	int what = _timelineSlider.GetPos();
	_observable->SetCurrentYear(what);
	_observable->NotifyObservers();
}


int EnVistasControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	//_timelineSlider.SetRange(0, _observable->GetRunTimeRange(), 1);
}

void EnVistasControl::Update(const VI_Observable* const observable) {
	auto context = dynamic_cast<const EnvContextObservable*>(observable);
	if (!context) {
		return;
	}

	_timelineSlider.SetRange(0, context->GetRunTimeRange(), 1);
	_timelineSlider.SetPos(context->GetCurrentYear());
}
