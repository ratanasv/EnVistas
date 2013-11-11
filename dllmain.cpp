// dllmain.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>


#include <EnvContext.h>
#include "EnVistas.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


EnVistas *theViz     = NULL;


/**** TODO: - indicate DLL Name (file search and replace)****/
static AFX_EXTENSION_MODULE EnVistasDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
      /**** TODO: update trace string with module name ****/
		TRACE0("EnVistas.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
      /**** TODO: update AfxInitExtensionModule with module name ****/
		if (!AfxInitExtensionModule(EnVistasDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

      /**** TODO: update CDynLinkLibrary constructor with module name ****/
		new CDynLinkLibrary(EnVistasDLL);
     
      ASSERT( theViz == NULL );
      theViz = new EnVistas;
      ASSERT( theViz != NULL );
	}
    
	else if (dwReason == DLL_PROCESS_DETACH)
	{
      /**** TODO: Update module name in trace string ****/
		TRACE0("EnVistas.DLL Terminating!\n");

      if ( theViz != NULL)
         delete theViz;

      // Terminate the library before destructors are called
      /**** TODO: Update module name in AfxTermExtensionModule ****/
		AfxTermExtensionModule(EnVistasDLL);
	}
	return 1;   // ok
}

extern "C" void PASCAL EXPORT GetExtInfo( ENV_EXT_INFO *pInfo );

extern "C" BOOL PASCAL EXPORT VInit        ( EnvContext*, LPCTSTR initInfo ); 
extern "C" BOOL PASCAL EXPORT VInitRun     ( EnvContext*, bool ); 
extern "C" BOOL PASCAL EXPORT VRun         ( EnvContext* );
extern "C" BOOL PASCAL EXPORT VEndRun      ( EnvContext* );
extern "C" BOOL PASCAL EXPORT VInitWindow  ( EnvContext*, HWND );
extern "C" BOOL PASCAL EXPORT VUpdateWindow( EnvContext*, HWND );
extern "C" BOOL PASCAL EXPORT VSetup       ( EnvContext*, HWND );

/////////////////////////////////////////////////////////////////////////////////////
///////////               E X T E N S I O N     I N F O                 /////////////
/////////////////////////////////////////////////////////////////////////////////////

void PASCAL GetExtInfo( ENV_EXT_INFO *pInfo ) 
   { 
   // TODO:  update as needed
   pInfo->types = EET_POSTRUN_VISUALIZER | EET_RT_VISUALIZER;
   pInfo->description = "EnVistas - Landscape Visualizer";
   }



/////////////////////////////////////////////////////////////////////////////////////
///////////                        V I S U A L I Z E R                  /////////////
/////////////////////////////////////////////////////////////////////////////////////

BOOL PASCAL VInit        ( EnvContext *pEnvContext, LPCTSTR initInfo )    { return theViz->Init( pEnvContext, initInfo ); }
BOOL PASCAL VInitRun     ( EnvContext *pEnvContext, bool useInitialSeed ) { return theViz->InitRun( pEnvContext, useInitialSeed ); }
BOOL PASCAL VRun         ( EnvContext *pEnvContext )                      { return theViz->Run( pEnvContext );  }
BOOL PASCAL VEndRun      ( EnvContext *pEnvContext )                      { return theViz->EndRun( pEnvContext );  }
BOOL PASCAL VInitWindow  ( EnvContext *pEnvContext, HWND hWnd )           { return theViz->InitWindow( pEnvContext, hWnd );  }
BOOL PASCAL VUpdateWindow( EnvContext *pEnvContext, HWND hWnd )           { return theViz->UpdateWindow( pEnvContext, hWnd );  }
BOOL PASCAL VSetup       ( EnvContext *pEnvContext, HWND hWnd )           { return theViz->Setup( pEnvContext, hWnd );  }
