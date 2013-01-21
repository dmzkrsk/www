// url.h : main header file for the URL DLL
//

#if !defined(AFX_URL_H__F1205A26_AD04_4345_ACBC_2006804CFA63__INCLUDED_)
#define AFX_URL_H__F1205A26_AD04_4345_ACBC_2006804CFA63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUrlApp
// See url.cpp for the implementation of this class
//

class CUrlApp : public CWinApp
{
public:
	CUrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URL_H__F1205A26_AD04_4345_ACBC_2006804CFA63__INCLUDED_)
