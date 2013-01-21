// fsys_dll.h : main header file for the FSYS_DLL DLL
//

#if !defined(AFX_FSYS_DLL_H__59775557_B0F5_442C_805C_3999D90CBF01__INCLUDED_)
#define AFX_FSYS_DLL_H__59775557_B0F5_442C_805C_3999D90CBF01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFsys_dllApp
// See fsys_dll.cpp for the implementation of this class
//

class CFsys_dllApp : public CWinApp
{
public:
	CFsys_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFsys_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFsys_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSYS_DLL_H__59775557_B0F5_442C_805C_3999D90CBF01__INCLUDED_)
