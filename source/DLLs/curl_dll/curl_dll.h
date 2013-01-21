// curl_dll.h : main header file for the CURL_DLL DLL
//

#if !defined(AFX_CURL_DLL_H__A5B1C3FB_B51B_46A0_88F5_B5C67558BC7D__INCLUDED_)
#define AFX_CURL_DLL_H__A5B1C3FB_B51B_46A0_88F5_B5C67558BC7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCurl_dllApp
// See curl_dll.cpp for the implementation of this class
//

class CCurl_dllApp : public CWinApp
{
public:
	CCurl_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurl_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCurl_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURL_DLL_H__A5B1C3FB_B51B_46A0_88F5_B5C67558BC7D__INCLUDED_)
