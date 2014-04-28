#pragma once

#include <EnvExtension.h>
#include <PtrArray.h>
#include <memory>
#include <vector>
#include <FDataObj.h>
#include <map>
#include <atomic>

class EnVistasWnd;
class SHP3DProcessor;
class EnvContextObservable;

class EnVistas : public EnvVisualizer
{
public:
   EnVistas( void );

   std::vector<CWnd*> _listOfWindows;
   std::map<HWND, EnVistasWnd*> _parentToEnVistasWindow;

   // other data
   std::atomic<int>  m_nextID;
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
   std::shared_ptr<SHP3DProcessor> _processor;
   std::shared_ptr<EnvContextObservable> _observable;
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