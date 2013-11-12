// EnVistas.cpp : Defines the initialization routines for the DLL.
//


#include <stdafx.h>
#pragma hdrstop

#include "EnVistas.h"
#include <vistas\vistas.h>


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

//void EnVistasWnd::OnPaint() 
//   {
//   CPaintDC dc(this); // device context for painting
//
//   //MemDC mdc( &dc );
//   //
//   //// draw map
//   //DrawBackground( mdc );
//   //DrawNetworkCircular( mdc, m_useCurrent );
//   }


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

void EnVistasWnd::CreateWinGLContext()
{
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

	CDC* ourWindowHandleToDeviceContext = GetDC();

	int  letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat(*ourWindowHandleToDeviceContext, &pfd); 
	SetPixelFormat(*ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

	HGLRC ourOpenGLRenderingContext = wglCreateContext(*ourWindowHandleToDeviceContext);
	wglMakeCurrent(*ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);
}



///////////////////////////////////////////////////////////////
//     V I S U A L I Z E R 
///////////////////////////////////////////////////////////////

BOOL EnVistas::Init( EnvContext *pContext, LPCTSTR initStr ) {
	//
	/*
	// 1) create a shape provider for Vistas
	m_pGeometryPlugin = new EnVistasGeometryPlugin( pContext->pMapLayer );

	// 2) load VISTAS plugin
	gPluginMgr->LoadPlugin( "Shp3D.DLL" );  // need fully qualified path

	m_pShp3D = gPluginMgr->GetPluginInstance( _T("SHP3D" ) );   // in VISTAS 

	m_pShp3D->SetData( pProv, 0 );   
	*/

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


