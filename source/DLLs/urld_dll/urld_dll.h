// urld_dll.h : main header file for the URLD_DLL DLL
//

#if !defined(AFX_URLD_DLL_H__2C2A5B94_EE08_4E24_B0F4_60F74B2CFFA9__INCLUDED_)
#define AFX_URLD_DLL_H__2C2A5B94_EE08_4E24_B0F4_60F74B2CFFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUrld_dllApp
// See urld_dll.cpp for the implementation of this class
//

class CUrld_dllApp : public CWinApp
{
public:
	CUrld_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrld_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUrld_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLD_DLL_H__2C2A5B94_EE08_4E24_B0F4_60F74B2CFFA9__INCLUDED_)
