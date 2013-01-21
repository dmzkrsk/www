// linkex.h : main header file for the LINKEX DLL
//

#if !defined(AFX_LINKEX_H__CB5A82A3_F0B2_4103_8E96_7B14769B4DD4__INCLUDED_)
#define AFX_LINKEX_H__CB5A82A3_F0B2_4103_8E96_7B14769B4DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLinkexApp
// See linkex.cpp for the implementation of this class
//

class CLinkexApp : public CWinApp
{
public:
	CLinkexApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkexApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLinkexApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKEX_H__CB5A82A3_F0B2_4103_8E96_7B14769B4DD4__INCLUDED_)

