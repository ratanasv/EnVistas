// EnVistas.cpp : Defines the initialization routines for the DLL.
//


#include <stdafx.h>
#pragma hdrstop

#include "EnVistas.h"
#include <cassert>
#include <stdexcept>
#include "EnVistasGeometryPlugin.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern EnVistas *theViz;

//*********************************************************


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

	pWnd->m_useCurrent = true;
	pWnd->m_currentYear = pContext->currentYear;
	pWnd->m_currentRun  = pContext->run;

	return TRUE;
}


BOOL EnVistas::UpdateWindow( EnvContext* pContext, HWND hParent ) {
	EnVistasWnd* pWnd = NULL;

	BOOL found = m_hwndToEnVistasWndMap.Lookup( hParent, pWnd );
	if (!found) {
		return FALSE;
	}
	auto potentialMatch = parentToEnVistasWindow.find(hParent);
	if (potentialMatch == parentToEnVistasWindow.end()) {
		return false;
	} else {
		pWnd = potentialMatch->second;
	}

	// have window, interpret pContext to determine correct
	// drawing.
	pWnd->m_currentRun  = pContext->run;
	pWnd->m_currentYear = pContext->currentYear;
	pWnd->m_useCurrent  = pContext->run < 0  ? true : false;

	CWnd *pParent = pWnd->GetParent();
	RECT rect;

	pParent->GetClientRect( &rect );
	pWnd->MoveWindow( &rect, FALSE );

	pWnd->SetWindowSize(rect.right, rect.bottom);
	pWnd->Paint(rect.right, rect.bottom, pContext);

	return TRUE; 
} 


EnVistasWnd* EnVistas::AddWindow(CWnd* parentWindowObject) {
	RECT rect;
	parentWindowObject->GetClientRect( &rect );

	EnVistasWnd* windowObject = new EnVistasWnd(rect.right, rect.bottom);
	windowObject->Create( NULL, "VISTASBackendForEnvision", WS_CHILD | WS_VISIBLE | WS_BORDER, 
		rect, parentWindowObject, m_nextID++ );

	m_wndArray.Add( windowObject );
	m_hwndToEnVistasWndMap.SetAt(parentWindowObject->GetSafeHwnd(), windowObject);
	parentToEnVistasWindow[parentWindowObject->GetSafeHwnd()] = windowObject;

	return windowObject;
}


