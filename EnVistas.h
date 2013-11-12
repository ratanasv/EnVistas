#pragma once

#include <EnvExtension.h>
#include <PtrArray.h>
#include "EnVistasGeometryPlugin.h"
//#include <FDataObj.h>




// Basic idea - Create a window that 

class EnVistasWnd : public CWnd {
	DECLARE_DYNCREATE( EnVistasWnd )
	// Construction
public:
	EnVistasWnd();
	~EnVistasWnd() { }

	int m_currentRun;
	int m_currentYear;
	bool m_useCurrent;
	CDC* deviceContext;
protected:
	bool   m_activated;

	// Generated message map functions
protected:
	//{{AFX_MSG(Map)
	//afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void CreateWinGLContext();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnSize(UINT nType, int cx, int cy);
};



class EnVistas : public EnvVisualizer
{
public:
   EnVistas( void ) : EnvVisualizer( VT_RUNTIME | VT_POSTRUN_GRAPH )
      , m_currentRun( 0 )
      , m_currentYear( 0 )
      , m_nextID( 86000 )
      { }

   PtrArray< EnVistasWnd > m_wndArray;
   CMap< HWND, HWND, EnVistasWnd*, EnVistasWnd* > m_hwndToEnVistasWndMap;

   // other data
   int  m_nextID;
   //---------------------------------------------------------------------                    INPUT        RUNTIME      POSTRUN
  
   // EnvVisualizer overrides
   virtual BOOL Init   ( EnvContext *pContext, LPCTSTR initStr ); 
   virtual BOOL InitRun( EnvContext*, bool ); 
   virtual BOOL Run    ( EnvContext* );
   virtual BOOL InitWindow( EnvContext*, HWND );
   virtual BOOL UpdateWindow( EnvContext*, HWND );
   
   // other methods
   EnVistasWnd *AddWindow( CWnd *pParent );

   // persistent data storage (one data object per run)
   //PtrArray< RUNDATA > m_runDataArray;
   //RUNDATA *m_pCurrentRunData;

   int m_currentRun;
   int m_currentYear;    // zero-based!!! not

   // VISTAS interface
   //EnVistasGeometryPlugin *m_pGeometryPlugin;
   //EnVistasTablePlugin    *m_pTablePlugin;
   //SHP3D                  *m_pShp3D;

};



/*
////////////////////////////////////
// data provider for Vistas engine
////////////////////////////////////

class EnVistasDataPlugin : public VI_TableDataPlugin
{
public:
   EnVistasDataPlugin( MapLayer *pLayer ): VI_TableDataPlugin(), m_pMapLayer( pLayer ) { }

   //virtual VI_ShapeArrayRef GetShapeArray();
};

*/