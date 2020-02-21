
// CMD_dazai.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCMD_dazaiApp:
// See CMD_dazai.cpp for the implementation of this class
//

class CCMD_dazaiApp : public CWinApp
{
public:
	CCMD_dazaiApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCMD_dazaiApp theApp;