#pragma once
#include "afxcmn.h"
#include "Resource.h"

// EnVistasControl dialog

class EnVistasControl : public CDialogEx
{
	DECLARE_DYNAMIC(EnVistasControl)

public:
	EnVistasControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~EnVistasControl();

// Dialog Data
	enum { IDD = ENVISTAS_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl _timelineSlider;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
