#pragma once

#include <EnvExtension.h>
#include <PtrArray.h>
#include <memory>
#include "envision_vistas_window.h"
#include <FDataObj.h>

using std::shared_ptr;

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
   map<HWND, EnVistasWnd*> parentToEnVistasWindow;

   // other data
   int  m_nextID;
   //---------------------------------------------------------------------                    INPUT        RUNTIME      POSTRUN
  
   // EnvVisualizer overrides
   virtual BOOL Init   ( EnvContext *pContext, LPCTSTR initStr ); 
   virtual BOOL InitRun( EnvContext*, bool ); 
   virtual BOOL Run    ( EnvContext* );
   virtual BOOL InitWindow( EnvContext*, HWND );
   virtual BOOL UpdateWindow( EnvContext*, HWND );
   
private:
   // other methods
   EnVistasWnd* AddWindow(EnvContext* context, CWnd* pParent);

   // persistent data storage (one data object per run)
   //PtrArray< RUNDATA > m_runDataArray;
   //RUNDATA *m_pCurrentRunData;

   int m_currentRun;
   int m_currentYear;    // zero-based!!! not

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