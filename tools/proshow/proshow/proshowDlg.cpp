
// proshowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "proshow.h"
#include "proshowDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CproshowDlg 对话框




CproshowDlg::CproshowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CproshowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CproshowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CproshowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP


	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CproshowDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CproshowDlg 消息处理程序

BOOL CproshowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	 //初始化
//	AfxSocketInit();
	m_tabCtrl.InsertItem(0,"通讯协议解释");

//	m_tabCtrl.InsertItem(1,"模拟电桩测试");

	m_decodeDialog.Create(IDD_DIALOG_AC_DECODE,GetDlgItem(IDC_TAB1));
	//m_eptestDialog.Create(IDD_DIALOG_EPTEST,GetDlgItem(IDC_TAB1));

	 CRect rs;
    m_tabCtrl.GetClientRect(&rs);
    //调整子对话框在父窗口中的位置
    rs.top += 20;
    rs.bottom -= 20;
    rs.left += 1;
    rs.right -= 2;
    //设置子对话框尺寸并移动到指定位置
    m_decodeDialog.MoveWindow(&rs);
	//m_eptestDialog.MoveWindow(&rs);
    
    //分别设置隐藏和显示
    m_decodeDialog.ShowWindow(1);
	//m_eptestDialog.ShowWindow(0);
  
    //设置默认的选项卡
    m_tabCtrl.SetCurSel(0);

   	 //初始化
	AfxSocketInit();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CproshowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CproshowDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CproshowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CproshowDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int CurSel = m_tabCtrl.GetCurSel();
 
    switch(CurSel)
    {
     case 0:
       m_decodeDialog.ShowWindow(true);
     //  m_eptestDialog.ShowWindow(false);
     break;
     case 1:
       m_decodeDialog.ShowWindow(false);
    //   m_eptestDialog.ShowWindow(true);
     break;
     default:
     break;
    }
}
