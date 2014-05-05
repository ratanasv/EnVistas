#pragma once
#include "afxcmn.h"
#include "Resource.h"
#include <memory>
#include <vistas/Observer.h>

class EnvContextObservable;

// EnVistasControl dialog
class EnVistasControl : public CDialogEx, public VI_Observer
{
	DECLARE_DYNAMIC(EnVistasControl)

public:
	EnVistasControl(CWnd* pParent, 
		std::shared_ptr<EnvContextObservable>& observable);   // standard constructor
	virtual ~EnVistasControl();

// Dialog Data
	enum { IDD = ENVISTAS_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl _timelineSlider;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void Update(const VI_Observable* const observable);

private:
	std::shared_ptr<EnvContextObservable> _observable;
};
