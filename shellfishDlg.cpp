// shellfishDlg.cpp : implementation file
//

#include "stdafx.h"
#include "shellfish.h"
#include "shellfishDlg.h"
#include <malloc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellfishDlg dialog

CShellfishDlg::CShellfishDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShellfishDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShellfishDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShellfishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShellfishDlg)
	DDX_Control(pDX, IDC_LISTCTRL, m_listCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShellfishDlg, CDialog)
	//{{AFX_MSG_MAP(CShellfishDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL, OnDblclkListctrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, OnItemchangedListctrl)
	ON_BN_CLICKED(IDC_GO, OnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellfishDlg message handlers

void CShellfishDlg::AddAbout()
{
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
}

void CShellfishDlg::SetDialogIcon()
{
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
}

BOOL CShellfishDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!m_status_bar.CreateEx(this, SBARS_SIZEGRIP,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE("SB::CreateEx failed (%x)\n", GetLastError());
	}

	UINT32 sep = ID_SEPARATOR;
	m_status_bar.SetIndicators(&sep,1);

	RepositionBars(AFX_IDW_STATUS_BAR, AFX_IDW_STATUS_BAR, 0);

	AddAbout();
	SetDialogIcon();

	m_listCtrl.InsertColumn(0, _T("What"), LVCFMT_LEFT, 100, 0);
	m_listCtrl.InsertColumn(1, _T("Where"), LVCFMT_LEFT, 280, 1);

	SHFILEINFO shfi;
	HIMAGELIST h = (HIMAGELIST)SHGetFileInfo(_T("."), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	m_listCtrl.SetImageList(CImageList::FromHandle(h), LVSIL_SMALL);

	PopulateList();
	EnableButtons();
	RecalcLayout();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#define SHELL_FOLDER_KEY _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders")

void CShellfishDlg::PopulateList()
{
	HKEY hKey;
	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, SHELL_FOLDER_KEY, 0, KEY_READ, &hKey);
	if (result != NO_ERROR)
		return;

	DWORD cValues;
	DWORD cbMaxValueNameLen;
	DWORD cbMaxValueLen;
	result = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL,
		&cValues, &cbMaxValueNameLen, &cbMaxValueLen, NULL, NULL);
	if (result == NO_ERROR)
	{
		TCHAR *value_name = (TCHAR *)_alloca(sizeof(TCHAR) * (cbMaxValueNameLen + 1));
		TCHAR *value = (TCHAR *)_alloca(sizeof(TCHAR) * (cbMaxValueLen + 1));

		for (DWORD index = 0; index < cValues; ++index)
		{
			DWORD cbValueName = cbMaxValueNameLen + 1;
			DWORD cbValue = cbMaxValueLen + 1;
			RegEnumValue(hKey, index, value_name, &cbValueName, NULL, NULL, (BYTE *)value, &cbValue);

			AddListItem(value_name, value);
		}
	}

	RegCloseKey(hKey);
}

void CShellfishDlg::AddListItem(const TCHAR *what, const TCHAR *where)
{
	SHFILEINFO shfi;
	SHGetFileInfo(where, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iItem = m_listCtrl.GetItemCount();
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iSubItem = 0;
	lvi.pszText = const_cast<TCHAR *>(what);
	lvi.iImage = shfi.iIcon;

	m_listCtrl.InsertItem(&lvi);
	lvi.iSubItem = 1;
	lvi.pszText = const_cast<TCHAR *>(where);
	m_listCtrl.SetItem(&lvi);
}

void CShellfishDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CShellfishDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShellfishDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CShellfishDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	RecalcLayout();
}

void CShellfishDlg::RecalcLayout()
{
	CRect dialog_rect;
	GetClientRect(&dialog_rect);

	CWnd *pGo = GetDlgItem(IDC_GO);
	CRect go_rect(0,0,0,0);
	if (pGo)
	{
		pGo->GetWindowRect(&go_rect);
		pGo->SetWindowPos(NULL, dialog_rect.Width() - go_rect.Width() - 7, 7, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	CWnd *pClose = GetDlgItem(IDCANCEL);
	if (pClose)
	{
		CRect close_rect;

		pClose->GetWindowRect(&close_rect);
		pClose->SetWindowPos(NULL, dialog_rect.Width() - close_rect.Width() - 7,
			dialog_rect.Height() - close_rect.Height() - 14,
			0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	CWnd *pList = GetDlgItem(IDC_LISTCTRL);
	if (pList)
	{
		pList->SetWindowPos(NULL, 7, 7, dialog_rect.Width() - 21 - go_rect.Width(), dialog_rect.Height() - 14, SWP_NOZORDER);
	}

	RepositionBars(AFX_IDW_STATUS_BAR, AFX_IDW_STATUS_BAR, 0);
}

void CShellfishDlg::OnDblclkListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	OnGo();
}

void CShellfishDlg::OnItemchangedListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EnableButtons();
	*pResult = 0;
}

void CShellfishDlg::EnableButtons()
{
	// The "OK" button needs a selection in the list control.
	if (m_listCtrl.GetSelectedCount() > 0)
		GetDlgItem(IDC_GO)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_GO)->EnableWindow(FALSE);
}

void CShellfishDlg::OnGo() 
{
	int nItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
		return;

	TCHAR where[MAX_PATH];
	m_listCtrl.GetItemText(nItem, 1, where, MAX_PATH);

	if (!where[0])
		return;

	ShellExecute(NULL, _T("open"), where, NULL, NULL, SW_SHOWNORMAL);
}
