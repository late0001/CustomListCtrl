#pragma once
#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

// CwlanCliDLLApp
// See wlanCliDLL.cpp for the implementation of this class
//

class CXCDDLLApp : public CWinApp
{
public:
	CXCDDLLApp();

	// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};