#pragma once

#include "XCDDll.h"

extern bool	GLByProDrv;	// by IOCTL mp8185s

extern LPSTR lpsServiceName;
extern TCHAR szServiceName[];
extern wchar_t wszServiceName[];
extern CONTROL_INFO		g_CtrlInfo;
void AdapterRefresh();
BOOL
StartPacketDriver(
    LPTSTR     ServiceName,
    BOOL       bReloadService
    );
