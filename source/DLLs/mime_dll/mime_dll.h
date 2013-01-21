// mime_dll.h : main header file for the MIME_DLL DLL
//

#if !defined(AFX_MIME_DLL_H__0BE3201B_30FA_42E0_AE93_B2495B21BA37__INCLUDED_)
#define AFX_MIME_DLL_H__0BE3201B_30FA_42E0_AE93_B2495B21BA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMime_dllApp
// See mime_dll.cpp for the implementation of this class
//

class CMime_dllApp : public CWinApp
{
public:
	CMime_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMime_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMime_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIME_DLL_H__0BE3201B_30FA_42E0_AE93_B2495B21BA37__INCLUDED_)
