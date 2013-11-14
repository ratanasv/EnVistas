// EnVistas.cpp : Defines the initialization routines for the DLL.
//


#include <stdafx.h>
#pragma hdrstop

#include "EnVistas.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cassert>


#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern EnVistas *theViz;

//*********************************************************

IMPLEMENT_DYNCREATE( EnVistasWnd, CWnd )

const float margin = 0.1f;

/////////////////////////////////////////////////////////////////////////////
// EnVistasWnd

EnVistasWnd::EnVistasWnd( void ): CWnd(), m_currentYear( -1 ), 
	m_currentRun( -1 ), m_useCurrent( true ) {
	m_activated = false;
}

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
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	auto deviceContext = GetDC();

	int  letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat(*deviceContext, &pfd); 
	SetPixelFormat(*deviceContext,letWindowsChooseThisPixelFormat, &pfd);

	glContext = wglCreateContext(*deviceContext);
	wglMakeCurrent(*deviceContext, glContext);
}

void EnVistasWnd::Paint(int width, int height, EnvContext* envContext) {
	auto oldDevContext = wglGetCurrentDC();
	auto oldGLContext = wglGetCurrentContext();

	auto devContext = GetDC();
	wglMakeCurrent(*devContext, glContext);

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



	bool isOk = SwapBuffers(*devContext);
	if (!isOk) {
		fprintf(stderr, "%u\n", GetLastError());
	}

	ReleaseDC(devContext);
	wglMakeCurrent(oldDevContext, oldGLContext);
}


///////////////////////////////////////////////////////////////
//     V I S U A L I Z E R 
///////////////////////////////////////////////////////////////

BOOL EnVistas::Init( EnvContext *pContext, LPCTSTR initStr ) {
	//
	
	// 1) create a shape provider for Vistas
	m_pGeometryPlugin.reset(new EnVistasGeometryPlugin( pContext->pMapLayer ));
	auto shapeArray = m_pGeometryPlugin->GetShapeArray();
	

	// something similar for data
	return TRUE;
}  


BOOL EnVistas::InitRun( EnvContext *pContext, bool ) {
	m_currentRun  = pContext->run;
	m_currentYear = 0;

	// m_useCurrent = false;
	int rows = pContext->endYear - pContext->startYear + 1;
	return TRUE; 
} 


BOOL EnVistas::Run( EnvContext *pContext ) {
	// called during runtime (during a simulation rather than a playback)

	// Because Envision will also call UpdateWindow() during runtime, we don't need 
	// to do anything here, we'll use UpdateWindow() instead
	m_currentYear = pContext->currentYear;
	return TRUE; 
}


BOOL EnVistas::InitWindow( EnvContext *pContext, HWND hParent ) {
	CWnd *pParent = pContext->pWnd;

	EnVistasWnd *pWnd = AddWindow( pParent );   // adds and creates a window;

	//CClientDC dc( pWnd );

	pWnd->m_useCurrent = true;
	pWnd->m_currentYear = pContext->currentYear;
	pWnd->m_currentRun  = pContext->run;

	return TRUE;
}


BOOL EnVistas::UpdateWindow( EnvContext *pContext, HWND hParent ) {
	EnVistasWnd *pWnd = NULL;

	BOOL found = m_hwndToEnVistasWndMap.Lookup( hParent, pWnd );
	if ( ! found )
		return FALSE;

	// have window, interpret pContext to determine correct
	// drawing.
	pWnd->m_currentRun  = pContext->run;
	pWnd->m_currentYear = pContext->currentYear;
	pWnd->m_useCurrent  = pContext->run < 0  ? true : false;

	CWnd *pParent = pWnd->GetParent();
	RECT rect;

	pParent->GetClientRect( &rect );
	pWnd->MoveWindow( &rect, FALSE );

	pWnd->Paint(rect.bottom, rect.right, pContext);
	//bool isOk = SwapBuffers(*pWnd->deviceContext);
//	assert(isOk);

	// VISTAS STUFF
	/*
	CClientDC dc( pWnd );

	//MemDC mdc( &dc );
	//pWnd->DrawBackground( mdc );
	//pWnd->DrawNetworkCircular( mdc, pWnd->m_useCurrent );   

	// make this window the current GL Context
	glu.....

	/// VISTAS
	pSHP3D->OnOption( VI_OptionEvent( ) );   // updated data passed here?

	*/

	return TRUE; 
} 


EnVistasWnd *EnVistas::AddWindow( CWnd *pParent ) {
	RECT rect;
	pParent->GetClientRect( &rect );

	EnVistasWnd *pWnd = new EnVistasWnd;

	pWnd->Create( NULL, "EnVistasWnd", WS_CHILD | WS_VISIBLE | WS_BORDER, rect, pParent, m_nextID++ );

	m_wndArray.Add( pWnd );
	m_hwndToEnVistasWndMap.SetAt( pParent->GetSafeHwnd(), pWnd );

	return pWnd;
}


