// shellfish.h : main header file for the SHELLFISH application
//

#if !defined(AFX_SHELLFISH_H__6FD3CCFB_2A50_4C27_8130_006001308977__INCLUDED_)
#define AFX_SHELLFISH_H__6FD3CCFB_2A50_4C27_8130_006001308977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CShellfishApp:
// See shellfish.cpp for the implementation of this class
//

class CShellfishApp : public CWinApp
{
public:
	CShellfishApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellfishApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CShellfishApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLFISH_H__6FD3CCFB_2A50_4C27_8130_006001308977__INCLUDED_)
