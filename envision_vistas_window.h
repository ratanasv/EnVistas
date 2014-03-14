#pragma once

#include <EnvExtension.h>
#include <map>
#include <memory>
#include <vistas/vistas.h>

using std::map;
using std::shared_ptr;
class SHP3DProcessor;
class VI_Camera;
class SHP3D;

// Basic idea - Create a window that 

class EnVistasWnd : public CWnd {
	DECLARE_DYNCREATE( EnVistasWnd )
	// Construction
public:
	EnVistasWnd(EnvContext* context = NULL, const int width = 100, const int height = 100);
	virtual ~EnVistasWnd() {}

	int m_currentRun;
	int m_currentYear;
	bool m_useCurrent;
protected:
	bool   m_activated;
	

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
	void Paint(int width, int height);
	void SetWindowSize(int width, int height);
	void AttachVisualization(const std::shared_ptr<SHP3D>& viz);
private:
	HGLRC _glContext;
	int _windowWidth;
	int _windowHeight;
	const shared_ptr<VI_Camera> _camera;
	const shared_ptr<VI_CameraInteractor> _cameraInteractor;
	float _lastX;
	float _lastY;
};
