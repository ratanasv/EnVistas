#include <stdafx.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "envision_vistas_window.h"
#include "envcontext_processor.h"

IMPLEMENT_DYNCREATE( EnVistasWnd, CWnd )

	const float margin = 0.1f;

/////////////////////////////////////////////////////////////////////////////
// EnVistasWnd

EnVistasWnd::EnVistasWnd(EnvContext* context, const int width, const int height) : 
	m_currentYear( -1 ), m_currentRun( -1 ), m_useCurrent( true ), m_activated(false), 
	_recentEnvContext(context), _windowWidth(width), _windowHeight(height)
{
	_camera.reset(new VI_Camera());
	_cameraInteractor.reset(new VI_SphereInteractor(_camera, _camera->GetScene()));
}; 

BEGIN_MESSAGE_MAP(EnVistasWnd, CWnd)
	//{{AFX_MSG_MAP(EnVistasWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Map message handlers

void EnVistasWnd::OnPaint() {
	gEvtMgr->DispatchAll();
	auto oldDevContext = wglGetCurrentDC();
	auto oldGLContext = wglGetCurrentContext();

	auto currentDeviceContext = GetDC();

	wglMakeCurrent(*currentDeviceContext, _glContext);
	
	Paint(_windowWidth, _windowHeight, _recentEnvContext);


	bool isOk = SwapBuffers(*currentDeviceContext);
	if (!isOk) {
		fprintf(stderr, "%u\n", GetLastError());
	}

	ReleaseDC(currentDeviceContext);
	wglMakeCurrent(oldDevContext, oldGLContext);
}


int EnVistasWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)  {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateWinGLContext();

	bool isOk = VI_Init3D();
	assert(isOk);
	if (_recentEnvContext != NULL) {
		//must be called after glewInit since it internally makes gl calls.
		_processor.reset(new SHP3DProcessor(_recentEnvContext));
		_processor->UpdateScene(_camera->GetScene());
		_camera->SetPosition(VI_Vector3(0.0, 100.0, 50.0));
		assert(_camera->GetMatrix()->IsValid());
		_camera->SetPointOfInterest(VI_Vector3(0.0, 0.0, 0.0));
		assert(_camera->GetMatrix()->IsValid());
		_camera->SetUpVector(VI_Vector3(0.0, 0.0, -1.0));
		assert(_camera->GetMatrix()->IsValid());
	}
	return 0;
}


void EnVistasWnd::OnLButtonDblClk(UINT nFlags, CPoint point) {

}




void EnVistasWnd::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);

	//invalidate so paint will be called
	Invalidate();
}


void EnVistasWnd::OnMouseMove( UINT nFlags, CPoint point) {
	bool isShiftDown = nFlags == VK_SHIFT;
	bool isCtrlDown = nFlags == VK_CONTROL;
	bool isAltDown = nFlags == VK_MENU;
	CWnd::OnMouseMove(nFlags, point);
	_cameraInteractor->MouseLeftDown(isShiftDown, isAltDown, isCtrlDown);
	if (point.x > -1 && point.y > -1) {
		_cameraInteractor->MouseMotion(point.x - _lastX, point.y - _lastY, isShiftDown, 
			isAltDown, isCtrlDown);
	}
	_lastX = point.x;
	_lastY = point.y;
}

void EnVistasWnd::CreateWinGLContext() {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE, 0, 0, 0, 0
	};

	auto deviceContext = GetDC()->m_hDC;

	int  letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat(deviceContext, &pfd); 
	SetPixelFormat(deviceContext,letWindowsChooseThisPixelFormat, &pfd);

	_glContext = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, _glContext);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void EnVistasWnd::Paint(int width, int height, EnvContext* envContext) {

	
	if (_processor->DoesNeedUpdate(envContext)) {
		_processor->UpdateScene(_camera->GetScene());
	}
	_camera->Render(width, height);
}

void EnVistasWnd::SetWindowSize(int width, int height) {
	_windowHeight = width;
	_windowHeight = height;
}

void EnVistasWnd::SetEnvContext(EnvContext* context) {
	_recentEnvContext = context;
}
