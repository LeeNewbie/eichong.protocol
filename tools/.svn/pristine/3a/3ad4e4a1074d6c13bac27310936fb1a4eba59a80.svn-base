
// EpGateMaintDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "EpGateMaintDlg.h"

#include "Config.h"


#include "httpClient.h"
#include "Md5.h"
#include "Bespno.h"
#include "CallEp.h"
#include "getVersion.h"
#include "rate.h"
#include "updateEp.h"
#include "stopBespno.h"
#include "startCharge.h"
#include "consumeRecord.h"
#include "getStation.h"
#include "open_white.h"
#include "managerEpcode.h"
#include "StopCarOrgan.h"
#include "flashram.h"
#include "FromServerList.h"
#include "FromOper.h"
#include "repeatSend.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;



EPGATE * m_epGateServer;
USERGATE * m_userGateServer;
REGEDITGATE * m_regeditGateServer;
THIRDGATE * m_thirdGateServer;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CEpGateMaintDlg 对话框




CEpGateMaintDlg::CEpGateMaintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEpGateMaintDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_queryEpGateDialog=NULL;
	m_queryUserGateDialog=NULL;
	m_ctrlUserGateDialog=NULL;
	m_pSpace=NULL;
}

void CEpGateMaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CEpGateMaintDlg, CDialog)
	
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	//}}AFX_MSG_MAP

	
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CEpGateMaintDlg::OnTvnSelchangedTree1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CEpGateMaintDlg::OnTcnSelchangeTab1)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

//BEGIN_EASYSIZE_MAP(CEpGateMaintDlg)
//
//	EASYSIZE(IDC_TREE1,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_BORDER,0)
//    EASYSIZE(IDC_TAB1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
//	
//
//
//END_EASYSIZE_MAP




// CEpGateMaintDlg 消息处理程序

BOOL CEpGateMaintDlg::OnInitDialog()
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


    m_IsInitialed = true; 



	// TODO: 在此添加额外的初始化代码
	/*CRect rect;  
	GetWindowRect(&rect);  
    ScreenToClient(&rect);  
  
    m_pMyFrame->MoveWindow(&rect);  
    m_pMyFrame->ShowWindow(SW_SHOW);*/


    m_tab.InsertItem(0,"");



	m_queryEpGateDialog = new CQueryDialog(this);
	m_ctrlEpGateDialog = new CTestEpGate(this);
	
	m_queryUserGateDialog= new CQueryUserGateDialog(this);
	m_ctrlUserGateDialog= new CCtrlUserGateDialog(this);

	m_pSpace = new m_space(this);

    m_queryEpGateDialog->Create(IDD_VIEW,GetDlgItem(IDC_TAB1));
	m_ctrlEpGateDialog->Create(IDD_TESTEPGATE,GetDlgItem(IDC_TAB1));

	m_queryUserGateDialog->Create(IDD_QERYUSERGATE,GetDlgItem(IDC_TAB1));
	m_ctrlUserGateDialog->Create(IDD_CTRLUSERGATE,GetDlgItem(IDC_TAB1));

	m_pSpace->Create(IDD_DIALOG16,GetDlgItem(IDC_TAB1));


    CRect rs;

     m_tab.GetClientRect(&rs);
    //设置子对话框尺寸并移动到指定位置
    m_queryEpGateDialog->MoveWindow(&rs);
	m_ctrlEpGateDialog->MoveWindow(&rs);

	m_queryUserGateDialog->MoveWindow(&rs);
	m_ctrlUserGateDialog->MoveWindow(&rs);
	m_pSpace->MoveWindow(&rs);


    //分别设置隐藏和显示
     m_queryEpGateDialog->ShowWindow(0);
	m_ctrlEpGateDialog->ShowWindow(0);

	  m_queryUserGateDialog->ShowWindow(0);
	 m_ctrlUserGateDialog->ShowWindow(0);
	 m_pSpace->ShowWindow(1);
  
    //设置默认的选项卡
    m_tab.SetCurSel(0);

	 m_epGateServerNum=0;
	 m_userGateServerNum=0;
	 m_regeditGateServerNum=0;
	 m_thirdGateServerNum=0;
	 ReadEpGateIni();
	 ReadUserGateIni();
	 ReadRegeditGateIni();
	 ReadThirdGateIni();

     initTree();

	UpdateData(true); 
    //	INIT_EASYSIZE;
	
	ShowWindow(SW_MAXIMIZE);

	CRect rect;  
	GetWindowRect(&rect);  
    ScreenToClient(&rect); 
	if(m_pSpace!=NULL)
	{
		CRect rs;
		m_tree.GetClientRect(&rs);
		rs.bottom = rect.bottom-10;
        m_tree.MoveWindow(&rs,true);

		rs.left +=rs.Width();
		rs.right = rect.right-20;
       // 
		m_tab.MoveWindow(&rs,true);
	
		m_tab.GetClientRect(&rect);

        //设置子对话框尺寸并移动到指定位置
		m_pSpace->MoveWindow(&rect,true);
        m_queryEpGateDialog->MoveWindow(&rect,true);
		m_ctrlEpGateDialog->MoveWindow(&rect,true);
        m_queryUserGateDialog->MoveWindow(&rect,true);
	    m_ctrlUserGateDialog->MoveWindow(&rect,true);
		
	
	}
	

//	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CEpGateMaintDlg::setItemData(int serverType,int serverIndex,int ipType,int userType,HTREEITEM item)
{
	
	   TREEITEMTYPE *itemType =new TREEITEMTYPE();
       itemType->serverType=serverType;
	   itemType->serverIndex=serverIndex;
	    itemType->ipType=ipType;
	   itemType->userType=userType;
	   m_tree.SetItemData(item,(DWORD)(itemType));
}

void CEpGateMaintDlg::initTree()
{
	HTREEITEM hItem,hSubItem,hSub_SubItem;
	 CString str;

   for(int i=0;i<m_epGateServerNum;i++)
   {
     
      hItem = m_tree.InsertItem(m_epGateServer[i].name,TVI_ROOT);
	   str ="内网("+m_epGateServer[i].in_ip+":"+m_epGateServer[i].in_port+")";
      hSubItem = m_tree.InsertItem(str,hItem);
	  hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	  setItemData(0,i,0,0,hSub_SubItem);
#ifdef TEST
	  hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	  setItemData(0,i,0,1,hSub_SubItem);
#endif
	  str ="外网("+m_epGateServer[i].out_ip+":"+m_epGateServer[i].out_port+")";
      hSubItem = m_tree.InsertItem(str,hItem,hSubItem);
	  hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	  setItemData(0,i,1,0,hSub_SubItem);
#ifdef TEST
	  hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	  setItemData(0,i,1,1,hSub_SubItem);
#endif   
    }
    for(int i=0;i<m_userGateServerNum;i++)
    {

       hItem = m_tree.InsertItem(m_userGateServer[i].name,TVI_ROOT,hItem);

	    str ="内网("+m_userGateServer[i].in_ip+":"+m_userGateServer[i].in_port+")";
	   hSubItem = m_tree.InsertItem(str,hItem);
	   hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	   setItemData(1,i,0,0,hSub_SubItem);
#ifdef TEST
	   hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
       setItemData(1,i,0,1,hSub_SubItem);
#endif
	   str ="外网("+m_userGateServer[i].out_ip+":"+m_userGateServer[i].out_port+")";
       hSubItem = m_tree.InsertItem(str,hItem,hSubItem);
	   hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
       setItemData(1,i,1,0,hSub_SubItem);
#ifdef TEST
	   hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	   setItemData(1,i,1,1,hSub_SubItem);
#endif
	}
	for(int i=0;i<m_regeditGateServerNum;i++)
    {
       hItem = m_tree.InsertItem(m_regeditGateServer[i].name,TVI_ROOT,hItem);

	    str ="内网("+m_regeditGateServer[i].in_ip+":"+m_regeditGateServer[i].in_port+")";
	   hSubItem = m_tree.InsertItem(str,hItem);
	   hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	   setItemData(2,i,0,0,hSub_SubItem);
#ifdef TEST
	   hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	   setItemData(2,i,0,1,hSub_SubItem);
#endif
	    str ="外网("+m_regeditGateServer[i].out_ip+":"+m_regeditGateServer[i].out_port+")";
       hSubItem = m_tree.InsertItem(str,hItem,hSubItem);
	   hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	   setItemData(2,i,1,0,hSub_SubItem);
#ifdef TEST
	   hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	   setItemData(2,i,1,1,hSub_SubItem);
#endif
	}
	for(int i=0;i<m_thirdGateServerNum;i++)
    {
       hItem = m_tree.InsertItem(m_thirdGateServer[i].name,TVI_ROOT,hItem);

	    str ="内网("+m_thirdGateServer[i].in_ip+":"+m_thirdGateServer[i].in_port+")";
	   hSubItem = m_tree.InsertItem(str,hItem);
	   hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	   setItemData(3,i,0,0,hSub_SubItem);
#ifdef TEST
	   hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	   setItemData(3,i,0,1,hSub_SubItem);
#endif
	    str ="外网("+m_thirdGateServer[i].out_ip+":"+m_thirdGateServer[i].out_port+")";
       hSubItem = m_tree.InsertItem(str,hItem,hSubItem);
	   hSub_SubItem = m_tree.InsertItem("查看",hSubItem);
	   setItemData(3,i,1,0,hSub_SubItem);
#ifdef TEST
	   hSub_SubItem = m_tree.InsertItem("控制",hSubItem,hSub_SubItem);
	   setItemData(3,i,1,1,hSub_SubItem);
#endif
	}
	
}
void CEpGateMaintDlg::ReadEpGateIni()
{

	char str[1024];
    if(GetIniResult(str,"EPGATE","num")<0)
    {
	   AfxMessageBox(LPCTSTR("dwatch.ini不存在"));
	   return ;
    }
	int num = atoi(str);
	if(num>0)
	{
		m_epGateServerNum=num;
        m_epGateServer=new EPGATE[num];
		for(int i=1;i<=num;i++)
		{
		   CString title;
		   title.Format("EPGATE%d",i);
		   if(GetIniResult(str,title,"name")==0)
           {
			   m_epGateServer[i-1].name=str;
           }
		    if(GetIniResult(str,title,"out_ip")==0)
           {
	           m_epGateServer[i-1].out_ip=str;
           }
		   if(GetIniResult(str,title,"out_port")==0)
           {
	          m_epGateServer[i-1].out_port=str;
           }
		   if(GetIniResult(str,title,"in_ip")==0)
           {
	          m_epGateServer[i-1].in_ip=str;
           }
		   if(GetIniResult(str,title,"in_port")==0)
           {
	          m_epGateServer[i-1].in_port=str;
           }
		    if(GetIniResult(str,title,"GunNo")==0)
           {
	          m_epGateServer[i-1].gunNo=str;
           }
		   if(GetIniResult(str,title,"Accounts")==0)
           {
	           m_epGateServer[i-1].accounts=str;
		   }
		   if(GetIniResult(str,title,"EpCodes")==0)
           {
	           m_epGateServer[i-1].epCodes=str;
           }
		   title.Format("EPGATE%d:QUERYUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_epGateServer[i-1].queryUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
			   m_epGateServer[i-1].queryUser.key=str;
           }
		   title.Format("EPGATE%d:CONTRLUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_epGateServer[i-1].ctrlUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
	          m_epGateServer[i-1].ctrlUser.key=str;
           }
		}
	}
	

	
}
void CEpGateMaintDlg::ReadEpGateIni(int i)
{

	if(i>=0&&i<m_epGateServerNum)
	{
		  i=i+1;
		char str[1024];
		   CString title;
		   title.Format("EPGATE%d",i);
		
		    if(GetIniResult(str,title,"GunNo")==0)
           {
	          m_epGateServer[i-1].gunNo=str;
           }
		   if(GetIniResult(str,title,"Accounts")==0)
           {
	           m_epGateServer[i-1].accounts=str;
		   }
		   if(GetIniResult(str,title,"EpCodes")==0)
           {
	           m_epGateServer[i-1].epCodes=str;
           }
		  
	}
	

	
}
void CEpGateMaintDlg::ReadUserGateIni()
{
	

	char str[1024];
    if(GetIniResult(str,"USERGATE","num")<0)
    {
	   AfxMessageBox(LPCTSTR("dwatch.ini不存在"));
	   return ;
    }
	int num = atoi(str);
	if(num>0)
	{
		m_userGateServerNum=num;
        m_userGateServer=new USERGATE[num];
		for(int i=1;i<=num;i++)
		{
		   CString title;
		   title.Format("USERGATE%d",i);
		   if(GetIniResult(str,title,"name")==0)
           {
			   m_userGateServer[i-1].name=str;
           }
		   if(GetIniResult(str,title,"out_ip")==0)
           {
	           m_userGateServer[i-1].out_ip=str;
           }
		   if(GetIniResult(str,title,"out_port")==0)
           {
	          m_userGateServer[i-1].out_port=str;
           }
		   if(GetIniResult(str,title,"in_ip")==0)
           {
	          m_userGateServer[i-1].in_ip=str;
           }
		   if(GetIniResult(str,title,"in_port")==0)
           {
	          m_userGateServer[i-1].in_port=str;
           }
		    if(GetIniResult(str,title,"GunNo")==0)
           {
	          m_userGateServer[i-1].gunNo=str;
           }
		   if(GetIniResult(str,title,"Accounts")==0)
           {
	           m_userGateServer[i-1].accounts=str;
		   }
		   if(GetIniResult(str,title,"EpCodes")==0)
           {
	           m_userGateServer[i-1].epCodes=str;
           }
		   title.Format("USERGATE%d:QUERYUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_userGateServer[i-1].queryUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
			   m_userGateServer[i-1].queryUser.key=str;
           }
		   title.Format("USERGATE%d:CONTRLUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_userGateServer[i-1].ctrlUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
	          m_userGateServer[i-1].ctrlUser.key=str;
           }
		}
	}
	
}

void CEpGateMaintDlg::ReadRegeditGateIni()
{
	

	char str[1024];
    if(GetIniResult(str,"REGEDITGATE","num")<0)
    {
	   AfxMessageBox(LPCTSTR("dwatch.ini不存在"));
	   return ;
    }
	int num = atoi(str);
	if(num>0)
	{
		m_regeditGateServerNum=num;
        m_regeditGateServer=new REGEDITGATE[num];
		for(int i=1;i<=num;i++)
		{
		   CString title;
		   title.Format("REGEDITGATE%d",i);
		   if(GetIniResult(str,title,"name")==0)
           {
			   m_regeditGateServer[i-1].name=str;
           }
		   if(GetIniResult(str,title,"out_ip")==0)
           {
	           m_regeditGateServer[i-1].out_ip=str;
           }
		   if(GetIniResult(str,title,"out_port")==0)
           {
	          m_regeditGateServer[i-1].out_port=str;
           }
		   if(GetIniResult(str,title,"in_ip")==0)
           {
	          m_regeditGateServer[i-1].in_ip=str;
           }
		   if(GetIniResult(str,title,"in_port")==0)
           {
	          m_regeditGateServer[i-1].in_port=str;
           }
		    if(GetIniResult(str,title,"GunNo")==0)
           {
	          m_regeditGateServer[i-1].gunNo=str;
           }
		   if(GetIniResult(str,title,"Accounts")==0)
           {
	           m_regeditGateServer[i-1].accounts=str;
		   }
		   if(GetIniResult(str,title,"EpCodes")==0)
           {
	           m_regeditGateServer[i-1].epCodes=str;
           }
		   title.Format("REGEDITGATE%d:QUERYUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_regeditGateServer[i-1].queryUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
			   m_regeditGateServer[i-1].queryUser.key=str;
           }
		   title.Format("REGEDITGATE%d:CONTRLUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_regeditGateServer[i-1].ctrlUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
	          m_regeditGateServer[i-1].ctrlUser.key=str;
           }
		}
	}
}

void CEpGateMaintDlg::ReadThirdGateIni()
{
	char str[1024];
    if(GetIniResult(str,"THIRDGATE","num")<0)
    {
	   AfxMessageBox(LPCTSTR("dwatch.ini不存在"));
	   return ;
    }
	int num = atoi(str);
	if(num>0)
	{
		m_thirdGateServerNum=num;
        m_thirdGateServer=new THIRDGATE[num];
		for(int i=1;i<=num;i++)
		{
		   CString title;
		   title.Format("THIRDGATE%d",i);
		   if(GetIniResult(str,title,"name")==0)
           {
			   m_thirdGateServer[i-1].name=str;
           }
		   if(GetIniResult(str,title,"out_ip")==0)
           {
	           m_thirdGateServer[i-1].out_ip=str;
           }
		   if(GetIniResult(str,title,"out_port")==0)
           {
	          m_thirdGateServer[i-1].out_port=str;
           }
		   if(GetIniResult(str,title,"in_ip")==0)
           {
	          m_thirdGateServer[i-1].in_ip=str;
           }
		   if(GetIniResult(str,title,"in_port")==0)
           {
	          m_thirdGateServer[i-1].in_port=str;
           }
		    if(GetIniResult(str,title,"GunNo")==0)
           {
	          m_thirdGateServer[i-1].gunNo=str;
           }
		   if(GetIniResult(str,title,"Accounts")==0)
           {
	           m_thirdGateServer[i-1].accounts=str;
		   }
		   if(GetIniResult(str,title,"EpCodes")==0)
           {
	           m_thirdGateServer[i-1].epCodes=str;
           }
		  title.Format("THIRDGATE%d:QUERYUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_thirdGateServer[i-1].queryUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
			   m_thirdGateServer[i-1].queryUser.key=str;
           }
		   title.Format("THIRDGATE%d:CONTRLUSER",i);
		   if(GetIniResult(str,title,"user")==0)
           {
	           m_thirdGateServer[i-1].ctrlUser.user=str;
           }
		   if(GetIniResult(str,title,"key")==0)
           {
	          m_thirdGateServer[i-1].ctrlUser.key=str;
           }
		}
	}
	
}


void CEpGateMaintDlg::OnOK()
{
   
}

BOOL CEpGateMaintDlg::PreTranslateMessage(MSG* pMsg)
{
  if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)  return TRUE;
  if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
  else
    return CDialog::PreTranslateMessage(pMsg);
}


void CEpGateMaintDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEpGateMaintDlg::OnPaint()
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


HCURSOR CEpGateMaintDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CEpGateMaintDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//需要注册窗口类
    //CString strMyClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,  
    //               ::LoadCursor(NULL, IDC_ARROW),    (HBRUSH) ::GetStockObject(WHITE_BRUSH),   
    //              ::LoadIcon(NULL, IDI_APPLICATION));

    //// Create the frame window with "this" as the parent
    //m_pMyFrame = new CFrameWnd;
    //m_pMyFrame->Create(strMyClass,"", WS_CHILD,   CRect(0,0,300,300), this);
    //m_pMyFrame->ShowWindow(SW_SHOW);

    //// and finally, create the splitter with the frame as the parent
    //m_cSplitter.CreateStatic(m_pMyFrame,1, 2); //在Frame里切分视图窗口为1×2，就是一行两列
    //m_cSplitter.CreateView(0,0, RUNTIME_CLASS(CFromServerList),   CSize(100,100), NULL);//第一行一列
    //m_cSplitter.CreateView(0,1, RUNTIME_CLASS(CFromOper), CSize(100,100), NULL);//第一行二列

	return 0;
}

void CEpGateMaintDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);


	// TODO: Add your message handler code here  
 /*   if(nType != SIZE_RESTORED && nType != SIZE_MAXIMIZED)  
        return;  
  
    SCROLLINFO si;  
    si.cbSize = sizeof(SCROLLINFO);  
  
    const int bar[] = { SB_HORZ, SB_VERT };  
    const int page[] = { cx, cy };  
  
    for(size_t i = 0; i < ARRAYSIZE(bar); ++i)  
    {  
        si.fMask = SIF_PAGE;  
        si.nPage = page[i];  
        SetScrollInfo(bar[i], &si, TRUE);  
  
        si.fMask = SIF_RANGE | SIF_POS;  
        GetScrollInfo(bar[i], &si);  
  
        const int maxScrollPos = si.nMax - (page[i] - 1);  
  
        const bool needToScroll =  
            (si.nPos != si.nMin && si.nPos == maxScrollPos) ||  
            (nType == SIZE_MAXIMIZED);  
  
        if(needToScroll)  
        {  
            ScrollClient(bar[i], si.nPos);  
        }  
    } */ 

	 

	// TODO: 在此处添加消息处理程序代码
	//UPDATE_EASYSIZE;

	
}

void CEpGateMaintDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	//EASYSIZE_MINSIZE(280,250,fwSide,pRect);
}


void CEpGateMaintDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString strText; // 树节点的标签文本字符串   
       
    // 获取当前选中节点的句柄   
    HTREEITEM hItem = m_tree.GetSelectedItem(); 

	TREEITEMTYPE *itemType = (TREEITEMTYPE *)(m_tree.GetItemData(hItem));
   
	if(itemType ==0)
	{
		return;
	}
    switch(itemType->serverType)
	{
	case 0:
		{
			showEpGate(itemType->serverIndex,itemType->ipType,itemType->userType);
		}
		break;
	case 1:
		{
			showUserGate(itemType->serverIndex,itemType->ipType,itemType->userType);
		}
		break;
	case 2:
		{
			showRegeditGate(itemType->serverIndex,itemType->ipType,itemType->userType);
		}
		break;
	case 3:
		{
			showThirdGate(itemType->serverIndex,itemType->ipType,itemType->userType);
		}
		break;
	default:
		break;
	}
}


void CEpGateMaintDlg::showEpGate(int serverType,int ipType,int userType)
{
	if(serverType<0||serverType>=m_epGateServerNum)
		return;
	int i=serverType;
	CString configName;
	configName.Format("EPGATE%d",i+1);
	ReadEpGateIni(serverType);

	if(ipType==0) //内网
	{
		if(userType==0) //查看
		{
             
			
			m_queryEpGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
				m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName,i,ipType);

			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(1);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);
		  
		}
		else //控制
		{

			m_ctrlEpGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
				m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName,i,ipType);

			m_queryEpGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(1);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);

		}
	}
	else //外网
	{
		if(userType==0) //查看
		{
			m_queryEpGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
				m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName,i,ipType);

			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(1);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);
		  
		}
		else //控制
		{

			m_ctrlEpGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
				m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName,i,ipType);

			m_ctrlEpGateDialog->ShowWindow(1);
			m_queryEpGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);

		}
	}

	
	//CRect rect;  
	//GetWindowRect(&rect);  
 //   ScreenToClient(&rect); 
	//if(m_queryEpGateDialog!=NULL)
	//{
	//	CRect rs;
	//	m_tree.GetClientRect(&rs);
	//	rs.bottom = rect.bottom;
 //       m_tree.MoveWindow(&rs,true);

	//	rs.left +=rs.Width();
	//	rs.right = rect.right;
 //      // 
	//	m_tab.MoveWindow(&rs,true);
	//
	//	m_tab.GetClientRect(&rect);

 //       //设置子对话框尺寸并移动到指定位置
 //       m_queryEpGateDialog->MoveWindow(&rect,true);
	//	m_ctrlEpGateDialog->MoveWindow(&rect,true);
 //       m_queryUserGateDialog->MoveWindow(&rect,true);
	//    m_ctrlUserGateDialog->MoveWindow(&rs,true);
	//
	//}


}



void CEpGateMaintDlg::showUserGate(int serverType,int ipType,int userType)
{
    if(serverType<0||serverType>=m_epGateServerNum)
		return;
	int i=serverType;
	CString configName;
	configName.Format("USERGATE%d",i+1);
	ReadUserGateIni();

	if(ipType==0) //内网
	{
		if(userType==0) //查看
		{
			//m_queryUserGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
			//	m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(1);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);
		  
		}
		else //控制
		{
		//	m_ctrlUserGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName);

			m_queryUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(1);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);
		}
	}
	else //外网
	{
		if(userType==0) //查看
		{
		//	m_queryUserGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(1);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);
		  
		}
		else //控制
		{
		//	m_ctrlUserGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(1);
			m_queryUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(0);
		}
	}
}

void CEpGateMaintDlg::showRegeditGate(int serverType,int ipType,int userType)
{
    if(serverType<0||serverType>=m_epGateServerNum)
		return;
	int i=serverType;
	CString configName;
	configName.Format("USERGATE%d",i+1);
    ReadRegeditGateIni();


	if(ipType==0) //内网
	{
		if(userType==0) //查看
		{
			//m_queryUserGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
			//	m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_queryUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		  
		}
		else //控制
		{
		//	m_ctrlUserGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName);

			m_queryUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		}
	}
	else //外网
	{
		if(userType==0) //查看
		{
		//	m_queryUserGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		  
		}
		else //控制
		{
		//	m_ctrlUserGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		}
	}
}

void CEpGateMaintDlg::showThirdGate(int serverType,int ipType,int userType)
{
    if(serverType<0||serverType>=m_epGateServerNum)
		return;
	int i=serverType;
	CString configName;
	configName.Format("USERGATE%d",i+1);
	ReadThirdGateIni();

	if(ipType==0) //内网
	{
		if(userType==0) //查看
		{
			//m_queryUserGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
			//	m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		  
		}
		else //控制
		{
		//	m_ctrlUserGateDialog->setParam(m_epGateServer[i].in_ip,m_epGateServer[i].in_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		}
	}
	else //外网
	{
		if(userType==0) //查看
		{
		//	m_queryUserGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].queryUser.user,m_epGateServer[i].queryUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		  
		}
		else //控制
		{
		//	m_ctrlUserGateDialog->setParam(m_epGateServer[i].out_ip,m_epGateServer[i].out_port,m_epGateServer[i].accounts,m_epGateServer[i].epCodes,
		//		m_epGateServer[i].gunNo,m_epGateServer[i].ctrlUser.user,m_epGateServer[i].ctrlUser.key,configName);

			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlUserGateDialog->ShowWindow(0);
			m_ctrlEpGateDialog->ShowWindow(0);
			m_queryEpGateDialog->ShowWindow(0);
			m_pSpace->ShowWindow(1);
		}
	}
}
void CEpGateMaintDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CEpGateMaintDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
	switch (nSBCode)
	{
	case SB_LEFT:
		ScrollWindow((scrollinfo.nPos-scrollinfo.nMin)*10,0);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		break;
	case SB_RIGHT:
		ScrollWindow((scrollinfo.nPos-scrollinfo.nMax)*10,0);
		scrollinfo.nPos = scrollinfo.nMax;
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		break;
	case SB_LINELEFT:
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(10,0);
		break;
	case SB_LINERIGHT:
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(-10,0);
		break;
	case SB_PAGELEFT:
		scrollinfo.nPos -= 5;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(10*5,0);
		break;
	case SB_PAGERIGHT:
		scrollinfo.nPos += 5;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(-10*5,0);
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_THUMBTRACK:
		ScrollWindow((scrollinfo.nPos-nPos)*10,0);
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		break;
	case SB_ENDSCROLL:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEpGateMaintDlg::HVScrool(int nBar, UINT nSBCode)  
{  
    const int scrollPos = GetScrollPos(nBar,nSBCode);  
  
    if(scrollPos == -1)  
        return;  
  
    SetScrollPos(nBar, scrollPos, TRUE);  
    ScrollClient(nBar, scrollPos);  
} 

void CEpGateMaintDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 SCROLLINFO scrollinfo; 
    GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
    switch (nSBCode) 
    { 
    case SB_BOTTOM: 
        ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10); 
        scrollinfo.nPos = scrollinfo.nMax; 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        break; 
    case SB_TOP: 
        ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10); 
        scrollinfo.nPos = scrollinfo.nMin; 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        break; 
    case SB_LINEUP: 
        scrollinfo.nPos -= 1; 
        if (scrollinfo.nPos<scrollinfo.nMin) 
        { 
            scrollinfo.nPos = scrollinfo.nMin; 
            break; 
        } 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        ScrollWindow(0,10); 
        break; 
    case SB_LINEDOWN: 
        scrollinfo.nPos += 1; 
        if (scrollinfo.nPos>scrollinfo.nMax) 
        { 
            scrollinfo.nPos = scrollinfo.nMax; 
            break; 
        } 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        ScrollWindow(0,-10); 
        break; 
    case SB_PAGEUP: 
        scrollinfo.nPos -= 5; 
        if (scrollinfo.nPos<scrollinfo.nMin) 
        { 
            scrollinfo.nPos = scrollinfo.nMin; 
            break; 
        } 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        ScrollWindow(0,10*5); 
        break; 
    case SB_PAGEDOWN: 
        scrollinfo.nPos += 5; 
        if (scrollinfo.nPos>scrollinfo.nMax) 
        { 
            scrollinfo.nPos = scrollinfo.nMax; 
            break; 
        } 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        ScrollWindow(0,-10*5); 
        break; 
    case SB_ENDSCROLL: 
        // MessageBox("SB_ENDSCROLL"); 
        break; 
    case SB_THUMBPOSITION: 
        // ScrollWindow(0,(scrollinfo.nPos-nPos)*10); 
        // scrollinfo.nPos = nPos; 
        // SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        break; 
    case SB_THUMBTRACK: 
        ScrollWindow(0,(scrollinfo.nPos-nPos)*10); 
        scrollinfo.nPos = nPos; 
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
        break; 
    } 
     
	HVScrool(SB_VERT,nSBCode);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CEpGateMaintDlg::ScrollClient(int nBar, int nPos)  
{  
    static int s_prevx = 1;  
    static int s_prevy = 1;  
  
    int cx = 0;  
    int cy = 0;  
  
    int& delta = (nBar == SB_HORZ ? cx : cy);  
    int& prev = (nBar == SB_HORZ ? s_prevx : s_prevy);  
  
    delta = prev - nPos;  
    prev = nPos;  
  
    if(cx || cy)  
    {  
        ScrollWindow(cx, cy, NULL, NULL);  
    }  
}  

int CEpGateMaintDlg::GetScrollPos(int nBar, UINT nSBCode)  
{  
    SCROLLINFO si;  
    si.cbSize = sizeof(SCROLLINFO);  
    si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;  
    GetScrollInfo(nBar, &si);  
  
    const int minPos = si.nMin;  
    const int maxPos = si.nMax - (si.nPage - 1);  
  
    int result = -1;  
  
    switch(nSBCode)  
    {  
    case SB_LINEUP /*SB_LINELEFT*/:  
        result = max(si.nPos - 1, minPos);  
        break;  
    case SB_LINEDOWN /*SB_LINERIGHT*/:  
        result = min(si.nPos + 1, maxPos);  
        break;  
    case SB_PAGEUP /*SB_PAGELEFT*/:  
        result = max(si.nPos - (int)si.nPage, minPos);  
        break;  
    case SB_PAGEDOWN /*SB_PAGERIGHT*/:  
        result = min(si.nPos + (int)si.nPage, maxPos);  
        break;  
    case SB_THUMBPOSITION:  
        // do nothing  
        break;  
    case SB_THUMBTRACK:  
        result = si.nTrackPos;  
        break;  
    case SB_TOP /*SB_LEFT*/:  
        result = minPos;  
        break;  
    case SB_BOTTOM /*SB_RIGHT*/:  
        result = maxPos;  
        break;  
    case SB_ENDSCROLL:  
        // do nothing  
        break;  
    }  
  
    return result;  
}  