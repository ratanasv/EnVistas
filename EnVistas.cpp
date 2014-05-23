// EnVistas.cpp : Defines the initialization routines for the DLL.
//


#include <stdafx.h>
#pragma hdrstop

#include "EnVistas.h"
#include <cassert>
#include <stdexcept>
#include "envision_vistas_window.h"
#include "envcontext_processor.h"
#include "EnVistasControl.h"
#include "EnvContextObservable.h"
#include "EnVistasSpltWnd.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern EnVistas *theViz;

using namespace std;


///////////////////////////////////////////////////////////////
//     V I S U A L I Z E R 
///////////////////////////////////////////////////////////////

BOOL EnVistas::Init( EnvContext *pContext, LPCTSTR initStr ) {
	
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

	if (!_observable) {
		_observable.reset(new EnvContextObservable(pContext));
	}
	_observable->SetCurrentYear(pContext->yearOfRun);
	_observable->SetEnvContext(pContext);
	_observable->NotifyObservers();

	return TRUE; 
}


BOOL EnVistas::InitWindow( EnvContext* pContext, HWND hParent ) {
	if (!_observable) {
		_observable.reset(new EnvContextObservable(pContext));
	}

	CWnd* pParent = pContext->pWnd;

	EnVistasWnd* pWnd = AddWindow(pContext, pParent);
	//must be called after glewInit since it internally makes OpenGL calls.
	_processor.reset(new SHP3DProcessor(_observable));
	pWnd->AttachVisualization(_processor->_vizPlugin);

	pWnd->m_useCurrent = true;
	pWnd->m_currentYear = pContext->currentYear;
	pWnd->m_currentRun  = pContext->run;

	_observable->NotifyObservers();
	return TRUE;
}


BOOL EnVistas::UpdateWindow( EnvContext* pContext, HWND hParent ) {
	EnVistasWnd* pWnd = NULL;

	auto potentialMatch = _parentToEnVistasWindow.find(hParent);
	if (potentialMatch == _parentToEnVistasWindow.end()) {
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


	return TRUE; 
} 

EnVistasWnd* EnVistas::AddWindow(EnvContext* context, CWnd* parentWindowObject)
{
	RECT rect;
	parentWindowObject->GetClientRect(&rect);

	EnVistasSpltWnd* spltWnd = new EnVistasSpltWnd();
	EnVistasWnd* glCanvasWnd = new EnVistasWnd(context, rect.right, rect.bottom);

	EnVistasControl* controlWnd = new EnVistasControl(spltWnd, _observable);
	spltWnd->ControlAndDisp(controlWnd, glCanvasWnd);
	spltWnd->Create(NULL, "VISTASSPLIT", WS_CHILD | WS_VISIBLE, rect, parentWindowObject, m_nextID++);
	spltWnd->MoveBar(100);
	_listOfWindows.push_back(spltWnd);
	_listOfWindows.push_back(glCanvasWnd);
	_listOfWindows.push_back(controlWnd);
	_parentToEnVistasWindow[spltWnd->GetSafeHwnd()] = glCanvasWnd;

	return glCanvasWnd;

}

EnVistas::EnVistas(void) : EnvVisualizer( VT_RUNTIME | VT_POSTRUN_GRAPH )
	, m_currentRun( 0 )
	, m_currentYear( 0 )
	, m_nextID( 86000 ) 
{
	
}


