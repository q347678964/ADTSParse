
// ADTSParseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ADTSParse.h"
#include "ADTSParseDlg.h"
#include "afxdialogex.h"
#include "ADTSParseHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ADTSHandler g_AdtsHandler;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CADTSParseDlg 对话框




CADTSParseDlg::CADTSParseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CADTSParseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADTSParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CADTSParseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(OPEN, &CADTSParseDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// CADTSParseDlg 消息处理程序

BOOL CADTSParseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);//使对话框为顶层窗口并且大小不可改变

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CADTSParseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CADTSParseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CADTSParseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CADTSParseDlg::DebugInfo(CString String)
{
	SetDlgItemText(IDC_EDIT2, String); 

	CEdit* pedit = (CEdit*)GetDlgItem(IDC_EDIT2);		//For auto turn to down .
	pedit->LineScroll(pedit->GetLineCount());

	UpdateWindow();
}

void CADTSParseDlg::OnBnClickedOpen()
{
	//   TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");  
	//	注意;代表两个过滤器并行,结尾是双||
	//  注释1|过滤器1.2;过滤器1.2|注释2|过滤器2||
	TCHAR szFilter[] = _T("ADTS(*.aac;*.adts)|*.aac;*.adts||");
    // 构造打开文件对话框   
    CFileDialog fileDlg(TRUE, _T("xls"), NULL, 0, szFilter, this);   

    // 显示打开文件对话框   
    if (IDOK == fileDlg.DoModal())   
    {   
        // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
        strFilePath_adts = fileDlg.GetPathName();   
        SetDlgItemText(IDC_EDIT1, strFilePath_adts);  
		/*Get size*/
		CFile file_excel(strFilePath_adts,CFile::modeRead);
		UpdateWindow();
		file_excel.Close();

		g_AdtsHandler.ADTSParselHandler(strFilePath_adts);
		AfxMessageBox(_T("解析成功！"));
    }		
}
