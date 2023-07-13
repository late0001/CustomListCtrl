// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

extern TCHAR   szDbgMsg[256];
extern char   szDbgMsgA[256];
#define LOGD(fmt,...)				\
	do {										\
  		_stprintf(szDbgMsg, _T("[Mango]: ")); OutputDebugString(szDbgMsg);	\
		_stprintf(szDbgMsg, fmt, __VA_ARGS__); OutputDebugString(szDbgMsg);\
  		_stprintf(szDbgMsg, _T("                     [%s, line #(%d)]\n"), TEXT(__FUNCTION__), __LINE__);OutputDebugString(szDbgMsg);\
	} while(0)

#define LOGDA(fmt,...)				\
	do {										\
  		sprintf(szDbgMsgA, "[Mango]: "); OutputDebugStringA(szDbgMsgA);	\
		sprintf(szDbgMsgA, fmt, __VA_ARGS__); OutputDebugStringA(szDbgMsgA);\
  		sprintf(szDbgMsgA, "                     [%s, line #(%d)]\n", __FUNCTION__, __LINE__);OutputDebugStringA(szDbgMsgA);\
	} while(0)
#include "CustomOid.h"	// For Realtek OID.
#include <winsvc.h>
#include <cfgmgr32.h>	// Used for CM_Xxxx functions
#include <setupapi.h>	// Used for SetupDiXxx functions
#include <stdint.h>

#endif //PCH_H
