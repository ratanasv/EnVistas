#include <stdafx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "envision_vistas_window.h"
#include "EnVistasGeometryPlugin.h"

IMPLEMENT_DYNCREATE( EnVistasWnd, CWnd )

	const float margin = 0.1f;

/////////////////////////////////////////////////////////////////////////////
// EnVistasWnd

EnVistasWnd::EnVistasWnd(const int width, const int height): m_currentYear( -1 ), 
	m_currentRun( -1 ), m_useCurrent( true ), m_activated(false), recentEnvContext(NULL), 
	windowWidth(width), windowHeight(height) {}; 

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
	Paint(windowWidth, windowHeight, recentEnvContext);
}


int EnVistasWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)  {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateWinGLContext();

	// Window is created, set it up for VISTAS
	// TODO: 
	bool isOk = VI_Init3D();
	assert(isOk);

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

	CWnd::OnMouseMove(nFlags, point);
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

	glContext = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, glContext);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnPaint();
}

void EnVistasWnd::Paint(int width, int height, EnvContext* envContext) {
	auto oldDevContext = wglGetCurrentDC();
	auto oldGLContext = wglGetCurrentContext();

	auto currentDeviceContext = GetDC();

	wglMakeCurrent(*currentDeviceContext, glContext);

	// 	VI_Camera camera;
	// 	camera.GetScene().AddObject(VI_MeshRenderable::Cube());
	// 	camera.Render(width, height);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-1.,1.,-1.,1.);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.,0.,0.,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(-0.8,-0.8);
	glColor3f(0.0,1.0,0.0);
	glVertex2f( 0.8,-0.8);
	glColor3f(0.0,0.0,1.0);
	glVertex2f( 0.8, 0.8);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(-0.8, 0.8);
	glEnd();



	bool isOk = SwapBuffers(*currentDeviceContext);
	if (!isOk) {
		fprintf(stderr, "%u\n", GetLastError());
	}

	ReleaseDC(currentDeviceContext);
	wglMakeCurrent(oldDevContext, oldGLContext);
}

void EnVistasWnd::SetWindowSize(int width, int height) {
	windowHeight = width;
	windowHeight = height;
}

void EnVistasWnd::SetEnvContext(EnvContext* context) {
	recentEnvContext = context;
}
