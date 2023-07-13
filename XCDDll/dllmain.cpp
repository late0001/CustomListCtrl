// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "XCD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//                     )
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}



//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CwlanCliDLLApp

BEGIN_MESSAGE_MAP(CXCDDLLApp, CWinApp)
END_MESSAGE_MAP()


// CwlanCliDLLApp construction

CXCDDLLApp::CXCDDLLApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CwlanCliDLLApp object

CXCDDLLApp theApp;


// CwlanCliDLLApp initialization

BOOL CXCDDLLApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}
