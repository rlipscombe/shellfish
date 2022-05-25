// shellfishDlg.h : header file
//

#if !defined(AFX_SHELLFISHDLG_H__68BA840A_A3DA_4A77_A5FE_CDAC90628B07__INCLUDED_)
#define AFX_SHELLFISHDLG_H__68BA840A_A3DA_4A77_A5FE_CDAC90628B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShellfishDlg dialog

class CShellfishDlg : public CDialog
{
    CStatusBar m_status_bar; ///< Only used for its size grip

public:
    CShellfishDlg(CWnd* pParent = NULL);	// standard constructor
    
    //{{AFX_DATA(CShellfishDlg)
    enum { IDD = IDD_SHELLFISH_DIALOG };
    CListCtrl	m_listCtrl;
    //}}AFX_DATA
    
    //{{AFX_VIRTUAL(CShellfishDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    HICON m_hIcon;
    
    // Generated message map functions
    //{{AFX_MSG(CShellfishDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDblclkListctrl(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListctrl(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGo();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
	
private:
    void AddAbout();
    void SetDialogIcon();

    void PopulateList();
    void AddListItem(const TCHAR *what, const TCHAR *where);

    void EnableButtons();
    void RecalcLayout();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLFISHDLG_H__68BA840A_A3DA_4A77_A5FE_CDAC90628B07__INCLUDED_)
