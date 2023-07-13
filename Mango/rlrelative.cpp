#include "pch.h"
#include "rlrelative.h"
#include "XCDDll.h"
#include "MangoDlg.h"

#pragma comment(lib, "XCDDll.lib")
TCHAR szServiceName[64] = _T("mp8185s");
char* serviceName = "mp8185s";
LPSTR lpsServiceName = serviceName;
wchar_t wszServiceName[64];

bool	GLByProDrv = false;

CONTROL_INFO	g_CtrlInfo;

void AdapterRefresh()
{
	int i;

	DLL_findRealtekWlanNics(&g_CtrlInfo.AdapterList, GLByProDrv);

	LOGD(_T("\n"));
	g_CtrlInfo.bindIndex = 0;
    LOGD(_T("%d Adapters:\n\r"), g_CtrlInfo.AdapterList.Cnt);
	for (i = 0; i < g_CtrlInfo.AdapterList.Cnt; i++)
	{
		if (i == g_CtrlInfo.bindIndex)
		{
            LOGD(_T("(*)%d. %s \n"), i + 1, g_CtrlInfo.AdapterList.adapter[i].Description);
            CString str = g_CtrlInfo.AdapterList.adapter[i].Description;
            PrintStatus(str);
			//			_tprintf(_T(" (*)%d. %s \n"), i+1, (char *)g_CtrlInfo.AdapterList.adapter[i].Description);
		}
		else
            LOGD(_T(" %d. %s \n"), i + 1, g_CtrlInfo.AdapterList.adapter[i].Description);

		//_tprintf(_T("	%s\n"), g_CtrlInfo.AdapterList.adapter[i].SymbolicLink);
	}

    LOGD(_T("\n"));
}

BOOL
StartPacketDriver(
    LPTSTR     ServiceName,
    BOOL       bReloadService
    )
{
    BOOL bStatus=FALSE;
    SC_HANDLE SCManagerHandle;
    SC_HANDLE SCServiceHandle;
    SERVICE_STATUS ServiceStatus;
    DWORD dwErr;

    bReloadService = FALSE; //fix bug about win2k start/stop mp8185s service, 2005.11.10, ch-hsu
    // Open a handle to the SC Manager database.
    SCManagerHandle = OpenSCManager(
                        NULL,                   /* local machine           */
                        NULL,                   /* ServicesActive database */
                        SC_MANAGER_ALL_ACCESS); /* full access rights      */

    if(SCManagerHandle == NULL)
    {
        return FALSE;
    }
    else
    {
        // Open a handle to the service.
        SCServiceHandle = OpenService(
                            SCManagerHandle,
                            ServiceName,
                            SERVICE_ALL_ACCESS);

        if(SCServiceHandle == NULL)
        {
      	   	DWORD dwErr = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL);

			_tprintf(_T(" Could not open the service : %s\n\r"), ServiceName);
            bStatus = FALSE;
        }
        else
        {
            // Start the service.
            bStatus = StartService(
                        SCServiceHandle,
                        0,
                        NULL);

            if(!bStatus)
            {
                dwErr = GetLastError();

                if(dwErr == ERROR_SERVICE_ALREADY_RUNNING && bReloadService==TRUE)
                {
                    // Restart the service of the protocol driver.
                    bStatus = ControlService(
                                SCServiceHandle,
                                SERVICE_CONTROL_STOP,
                                &ServiceStatus);
                    if(bStatus)
                    {
                        bStatus = StartService(
                                    SCServiceHandle,
                                    0,
                                    NULL);
                        if(!bStatus)
                        {
                            dwErr = GetLastError();
							_tprintf(_T(" Failed to start the service for reload\n\r"));
                        }
                    }
                    else
                    {
                        dwErr = GetLastError();
						_tprintf(_T(" Failed to stop the service for reload\n\r"));
                    }
                }
                else if(dwErr==ERROR_SERVICE_ALREADY_RUNNING && bReloadService==FALSE)
                {
                    // Open a service already running is OK.
                    bStatus = TRUE;
                }
                else
                {
                    dwErr = GetLastError();
					_tprintf(_T(" Failed to start the service\n\r"));
                }
            }

            CloseServiceHandle(SCServiceHandle);
        }

        CloseServiceHandle(SCManagerHandle);

        return bStatus;
    }
}

