#pragma once

#include <EnvExtension.h>
#include <map>

using std::map;


// Basic idea - Create a window that 

class EnVistasWnd : public CWnd {
	DECLARE_DYNCREATE( EnVistasWnd )
	// Construction
public:
	EnVistasWnd(const int width = 100, const int height = 100);
	virtual ~EnVistasWnd() {}

	int m_currentRun;
	int m_currentYear;
	bool m_useCurrent;
protected:
	bool   m_activated;
	HGLRC glContext;

	// Generated message map functions
protected:
	//{{AFX_MSG(Map)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void CreateWinGLContext();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Paint(int width, int height, EnvContext* envContext);
	void SetWindowSize(int width, int height);
	void SetEnvContext(EnvContext* context);
private:
	int windowWidth;
	int windowHeight;
	EnvContext* recentEnvContext;
};
