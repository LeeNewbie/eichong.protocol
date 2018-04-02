
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


extern  bool BoxFlag;


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
}

void CEpGateMaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_CcomBoBox2);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	DDX_Control(pDX, IDC_COMBO3, m_epCodeComboBox);
}

BEGIN_MESSAGE_MAP(CEpGateMaintDlg, CDialog)
	
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON5, &CEpGateMaintDlg::OnBnClickedButton_getBom)
	ON_BN_CLICKED(IDC_BUTTON6, &CEpGateMaintDlg::OnBnClickedButton_startCharge)
	ON_BN_CLICKED(IDC_BUTTON8, &CEpGateMaintDlg::OnBnClickedButton_startBesp)
	ON_BN_CLICKED(IDC_BUTTON14, &CEpGateMaintDlg::OnBnClickedButton_cleanGun)
	ON_BN_CLICKED(IDC_BUTTON15, &CEpGateMaintDlg::OnBnClickedButton_connetMonitor)
	ON_BN_CLICKED(IDC_BUTTON16, &CEpGateMaintDlg::OnBnClickedButton_cleanUser)
	ON_BN_CLICKED(IDC_BUTTON7, &CEpGateMaintDlg::OnBnClickedButton_endCharge)
	ON_BN_CLICKED(IDC_BUTTON9, &CEpGateMaintDlg::OnBnClickedButton_endBesp)
	ON_BN_CLICKED(IDC_BUTTON10, &CEpGateMaintDlg::OnBnClickedButton_callEp)
	ON_BN_CLICKED(IDC_BUTTON13, &CEpGateMaintDlg::OnBnClickedButton_sendUpdate)
	ON_BN_CLICKED(IDC_BUTTON3, &CEpGateMaintDlg::OnBnClickedButton_stat)
	ON_BN_CLICKED(IDC_BUTTON4, &CEpGateMaintDlg::OnBnClickedButton_getVer)
	ON_BN_CLICKED(IDC_BUTTON1, &CEpGateMaintDlg::OnBnClickedButton_getGun)
	ON_BN_CLICKED(IDC_BUTTON2, &CEpGateMaintDlg::OnBnClickedButton_getUser)
	ON_BN_CLICKED(IDC_BUTTON11, &CEpGateMaintDlg::OnBnClickedButton_dropClock)
	ON_BN_CLICKED(IDC_BUTTON12, &CEpGateMaintDlg::OnBnClickedButton_sendRate)
	ON_BN_CLICKED(IDC_BUTTON3, &CEpGateMaintDlg::OnBnClickedButton_stat)
	ON_BN_CLICKED(IDC_BUTTON17, &CEpGateMaintDlg::OnBnClickedButton_cleanBesp)
	ON_BN_CLICKED(IDC_BUTTON18, &CEpGateMaintDlg::OnBnClickedButton_queryVer)
	ON_BN_CLICKED(IDC_BUTTON19, &CEpGateMaintDlg::OnBnClickedButton_restoreGun)
	ON_BN_CLICKED(IDC_BUTTON20, &CEpGateMaintDlg::OnBnClickedButton_startCharge_ep)
	ON_BN_CLICKED(IDC_BUTTON21, &CEpGateMaintDlg::OnBnClickedButton_endCharge_ep)
	ON_BN_CLICKED(IDC_BUTTON22, &CEpGateMaintDlg::OnBnClickedButton_besp_ep)
	ON_BN_CLICKED(IDC_BUTTON23, &CEpGateMaintDlg::OnBnClickedButton_endBesp_ep)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CEpGateMaintDlg::OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT5, &CEpGateMaintDlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON24, &CEpGateMaintDlg::OnBnClickedButton_monitorStat)
	ON_BN_CLICKED(IDC_BUTTON25, &CEpGateMaintDlg::OnBnClickedButton_getEp)
	ON_BN_CLICKED(IDC_BUTTON27, &CEpGateMaintDlg::OnBnClickedButton_queryCommSignal)
	ON_BN_CLICKED(IDC_BUTTON26, &CEpGateMaintDlg::OnBnClickedButton_queryConsumeRecord)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CEpGateMaintDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON28, &CEpGateMaintDlg::OnBnClickedButton_getStation)

	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_BUTTON_EP, &CEpGateMaintDlg::OnBnClickedButtonEp)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CEpGateMaintDlg::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()





//int CEpGateMaintDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)  
//{  
//     if (CDialog::OnCreate(lpCreateStruct) == -1)  
//            return -1;  
//      
//    CString strMyClass = AfxRegisterWndClass(CS_VREDRAW |CS_HREDRAW,  
//            ::LoadCursor(NULL, IDC_ARROW),  
//            (HBRUSH) ::GetStockObject(WHITE_BRUSH),  
//            ::LoadIcon(NULL, IDI_APPLICATION));  
//      
//    // Create the frame window with "this" as the parent  
//    m_pMyWnd = new CFrameWnd;  
//    m_pMyWnd->Create(strMyClass,_T(""), WS_CHILD,CRect(0,0,300,300), this);  
//    m_pMyWnd->ShowWindow(SW_SHOW);     
//      
//      
//    if (m_SplitterWnd.CreateStatic(m_pMyWnd,1, 2) == NULL) //1行2列  
//    {  
//         return -1;  
//     }     
//     
//    m_SplitterWnd.CreateView(0,0, RUNTIME_CLASS(CFromServerList), CSize(100,100), NULL);
//	 m_SplitterWnd.CreateView(0,1, RUNTIME_CLASS(CFromOper),CSize(200,200), NULL); 
//      
// 
//     return 0;
//｝  


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

/*
	int m_IsInitialed = false; 
	 CRect m_ClientRect;   
	 this->GetClientRect(&m_ClientRect);  
	 CSize m_Forsize;   
	 m_Forsize = GetTotalSize();
	 double m_x = (double)m_ClientRect.Width() / m_Forsize.cx;
	 double m_y = (double)m_ClientRect.Height() / m_Forsize.cy;
	 CWnd *pWnd = NULL;
	  pWnd = GetWindow(GW_CHILD); 
	   while(pWnd)
	   {
		   CRect rect;
		    pWnd->GetWindowRect(&rect); 
		 ScreenToClient(&rect);
		// m_ClientRect.insert(pair<int, CRect>(pWnd->GetDlgCtrlID(), rect));
		  int width = rect.Width();
		  int height = rect.Height();  
		  char szBuf[256];  
		  GetClassName(pWnd->m_hWnd,szBuf,256);           
		  if( _tcsicmp(szBuf,_T("Edit")) == 0)    
		  {      //Edit只是位置变化，大小没有变    
			  rect.top = m_y * rect.top;   
			  rect.left = m_x * rect.left;    
			  rect.bottom = rect.top + height;   
			  rect.right = rect.left + width;   
		  }  
		  else   
		  {     //其它控件位置和大小均变化    
			  rect.top = m_y * rect.top;    
			  rect.left = m_x * rect.left;    
			  rect.bottom = m_y * rect.bottom;   
			  rect.right = m_x * rect.right;   
		  }    pWnd->MoveWindow(&rect);//设置控件大小   
		  pWnd = pWnd->GetWindow(GW_HWNDNEXT);  
	  
	  }     //控件初始化结束 


 m_IsInitialed = true; 
*/

	//ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	/*CRect rect;  
	GetWindowRect(&rect);  
ScreenToClient(&rect);  
  
m_pMyWnd->MoveWindow(&rect);  
m_pMyWnd->ShowWindow(SW_SHOW);
*/

	epCode="";
    gunNo="0";
	 key="";
	account="";

	serverNum=0;


   char skey[50];
   char ssuer[50];
   if(GetKey(skey,ssuer)<0)
   {
	   AfxMessageBox(LPCTSTR("epgatemaint.ini不存在"));
	  return FALSE;
   }
   key=skey;
   user=ssuer;
    
   char scode[1024];
   if(GetEpCode(scode)<0)
   {
	   AfxMessageBox(LPCTSTR("epgatemaint.ini不存在或电桩编号配置错误"));
	   return TRUE;
   }

    epCode = scode;
	int pos1=0;
	while(true)
	{
		pos1  = epCode.Find(","); 
		if(pos1 <= 0)
		{
           if(epCode != "")
		   {
			   m_epCodeComboBox.AddString(epCode);
		   }
			break;
		}

		m_epCodeComboBox.AddString(epCode.Left(pos1));

        epCode.TrimLeft(epCode.Left(pos1));
		epCode.TrimLeft(",");

	}
	m_epCodeComboBox.SetCurSel(0);
   char sgunNo[2];
   if(GetEpGun(sgunNo)<0)
   {
	   AfxMessageBox(LPCTSTR("epgatemaint.ini不存在或枪口编号配置错误"));
	   return TRUE;
   }
   gunNo=sgunNo;
   char saccount[20];
	if(GetAccount(saccount)<0)
   {
	   AfxMessageBox(LPCTSTR("epgatemaint.ini不存在或枪口编号配置错误"));
	   return TRUE;
   }
	account = saccount;

	
	GetServerNumPara(serverNum);

	if(serverNum<=0)
	{
		 AfxMessageBox(LPCTSTR("epgatemaint.ini配置服务器错误"));
	   return false;
	}
	m_server=new SERVER[serverNum];

	for(int i=1;i<=serverNum;i++)
	{
       CString title;
	   title.Format("SERVER%d",i);
	   char ip[16],port[10],name[100];
      
	    GetServerPara(ip,port,name,title);
		m_server[i-1].name=name;
		m_server[i-1].ip=ip;
		m_server[i-1].port=port;


		CString str = m_server[i-1].name+"("+m_server[i-1].ip+":"+m_server[i-1].port+")";

		m_CcomBoBox2.AddString(str);
	}


	m_CcomBoBox2.SetCurSel(0);

	//((CEdit *)GetDlgItem(IDC_EDIT3))->SetWindowText(epCode);//
	((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText(gunNo);//
	((CEdit *)GetDlgItem(IDC_EDIT5))->SetWindowText(account);//


	if(user=="test")
	{
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
	}

	m_tabCtrl.InsertItem(0,"  查看  ");

	m_tabCtrl.InsertItem(1,"  清除  ");

	m_tabCtrl.InsertItem(2,"  流程测试  ");
	m_tabCtrl.InsertItem(3,"  对电桩单元测试  ");
	m_tabCtrl.InsertItem(4,"  远程升级  ");
	m_tabCtrl.InsertItem(5,"  其它  ");

	
	m_queryDialog = new CQueryDialog(this);
	m_clearDialog = new CClear(this);
	m_testEpGateDialog = new CTestEpGate(this);
	m_testEpDialog = new CTestEpDialog(this);
	m_updateDialog = new CUpdateHexDialog(this);
	m_otherDialog = new COther(this);

	m_queryDialog->Create(IDD_VIEW,GetDlgItem(IDC_TAB1));
	m_clearDialog->Create(IDD_CLEAR,GetDlgItem(IDC_TAB1));
	m_testEpGateDialog->Create(IDD_TESTEPGATE,GetDlgItem(IDC_TAB1));
	m_testEpDialog->Create(IDD_TESTEP,GetDlgItem(IDC_TAB1));
	m_updateDialog->Create(IDD_UPDATEHEX,GetDlgItem(IDC_TAB1));
	m_otherDialog->Create(IDD_OTHER,GetDlgItem(IDC_TAB1));
	

	 CRect rs;
    m_tabCtrl.GetClientRect(&rs);
    //调整子对话框在父窗口中的位置
    rs.top += 20;
    rs.bottom -= 20;
   rs.left += 1;
    rs.right -= 2;
    //设置子对话框尺寸并移动到指定位置
    m_queryDialog->MoveWindow(&rs);
	m_clearDialog->MoveWindow(&rs);
    m_testEpGateDialog->MoveWindow(&rs);
	m_testEpDialog->MoveWindow(&rs);
	m_updateDialog->MoveWindow(&rs);
	m_otherDialog->MoveWindow(&rs);
    //分别设置隐藏和显示
    m_queryDialog->ShowWindow(1);
	m_clearDialog->ShowWindow(0);
	m_testEpGateDialog->ShowWindow(0);
	m_testEpDialog->ShowWindow(0);
	m_updateDialog->ShowWindow(0);
	m_otherDialog->ShowWindow(0);
  
    //设置默认的选项卡
    m_tabCtrl.SetCurSel(0);

	UpdateData(true); 

//	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

void CEpGateMaintDlg::getEpParam()
{
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("");//

	//get方式    
	((CComboBox *)GetDlgItem(IDC_COMBO3))->GetWindowText(epCode);//
	((CEdit *)GetDlgItem(IDC_EDIT4))->GetWindowText(gunNo);//
	((CEdit *)GetDlgItem(IDC_EDIT5))->GetWindowText(account);//
//	((CEdit *)GetDlgItem(IDC_EDIT6))->GetWindowText(ip);//
//	((CEdit *)GetDlgItem(IDC_EDIT7))->GetWindowText(port);//

	
	SetParaStringA(gunNo,"OTHER","GunNo");
	SetParaStringA(account,"OTHER","Account");
//	SetParaStringA(ip,"SERVER","ip");
//	SetParaStringA(port,"SERVER","port");
	int index=m_CcomBoBox2.GetCurSel();

    if(index<0 &&index>=serverNum)
	{
		curIp="0.0.0.0";
		curPort="0";

	}
	else
	{
        curIp = m_server[index].ip;
		curPort=m_server[index].port;
	}
	OnCbnSelchangeCombo3();

}

void CEpGateMaintDlg::OnBnClickedButton_getBom()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
	KillTimer(1);
    
	getEpParam();
	

	list<CString> paramList;
	CString code("");

    CString station;
	int flag=1;
	CgetVersion dlg;
	if(dlg.DoModal()==IDOK)
	{
		station = dlg.stationAddr;
		 flag=dlg.flag;
	}
	else
	{
		return;
	}

	if(flag==1)
	{
		code="code="+epCode;
	    paramList.push_back(code);
		station="stationaddr=0";
        paramList.push_back(station);
	}
	else
	{
		code="code=0";
	    paramList.push_back(code);
		station="stationaddr="+station;
        paramList.push_back(station);
	}

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	 CString cmd("/getBomList?");
	 cmdstr="/getBomList?";

	setUrl(paramList,cmd);

     CHttpClient   conn;
     ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//
	 urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_startCharge()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

	KillTimer(1);
	getEpParam();

	CString froneAmt;
	CString  chargeType;

	CString payType;

	CstartCharge dlg;
	if(dlg.DoModal()==IDOK)
	{
		froneAmt = dlg.froneAmt;
		 chargeType=dlg.chargeType;
		 payType = dlg.payType;
	}
	else
		return;

 
	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="chargetype="+chargeType;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="froneamt="+froneAmt;
    paramList.push_back(code);

	 code="gunno="+gunNo;
    paramList.push_back(code);

	 code="payMode="+payType;
    paramList.push_back(code);

	


    CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testStartCharge?");

	cmdstr="/testStartCharge?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_startBesp()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();
	

	CString bespno;
	CString time;
	CString redo;
	CBespno dlg;
	CString payType;

	if(dlg.DoModal()==IDOK)
	{
		time = dlg.timet;
		 redo=dlg.redo;
		 bespno=dlg.bespno;
		 payType = dlg.payType;
	}
	else
		return;


	list<CString> paramList;
	CString code("");

	code="bespno="+bespno;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
	
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	 code="payMode="+payType;
    paramList.push_back(code);

	 code="phone="+account;
    paramList.push_back(code);

	code="redo="+redo;
    paramList.push_back(code);

	 code="time="+time;
    paramList.push_back(code);


	
		CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testStartBespoke?");

	cmdstr="/testStartBespoke?";


	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_cleanGun()
{
	if(AfxMessageBox("确定清空充电信息?", MB_OKCANCEL |  MB_ICONQUESTION) == IDCANCEL)
		return;
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	list<CString> paramList;
	CString code("");

	code="code="+epCode;
    paramList.push_back(code);
	
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	
	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/removeCharge?");
	cmdstr="/removeCharge?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_connetMonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
    getEpParam();
	
	list<CString> paramList;

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/connetmonitor?");
	cmdstr="/connetmonitor?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	CString s=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(s);//

}

void CEpGateMaintDlg::OnBnClickedButton_getGun()
{
	// TODO: 在此添加控件通知处理程序代码
	

	
getEpParam();

	list<CString> paramList;
	CString code("");
	code="code="+epCode;
	
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);
    CString cmd("/getgundetail?");

	cmdstr="/getgundetail?";

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	setUrl(paramList,cmd);

     CHttpClient   conn;
	 ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	 CString s=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(s);//
}

void CEpGateMaintDlg::OnBnClickedButton_getUser()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

	getEpParam();


		list<CString> paramList;
	CString ac("account=");
	ac+=account;
	
    paramList.push_back(ac);
   
    
    CString cmd("/user?");

	cmdstr="/user?";

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	setUrl(paramList,cmd);

     CHttpClient   conn;
	 ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	 urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_cleanUser()
{
	if(AfxMessageBox("确定清除用户信息?", MB_OKCANCEL |  MB_ICONQUESTION) == IDCANCEL)
		return;
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
getEpParam();

	list<CString> paramList;
	CString ac("account=");
	ac+=account;
	
    paramList.push_back(ac);
   
    
    

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/cleanuser?");
	cmdstr="/cleanuser?";

	setUrl(paramList,cmd);

     CHttpClient   conn;
	 ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	 urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_endCharge()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	
	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	 

CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);
	CString cmd("/testStopCharge?");
	cmdstr="/testStopCharge?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_endBesp()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	list<CString> paramList;
	CString code("");

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testStopBespoke?");
	cmdstr="/testStopBespoke?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_callEp()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	CString time;
	CString type;
	CCallEp dlg;
	if(dlg.DoModal()==IDOK)
	{
		time = dlg.time;
		 type=dlg.type;
	}
	else
		return;

	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="time="+time;
    paramList.push_back(code);

	code="type="+type;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testCallEp?");

	cmdstr="/testCallEp?";
	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_dropClock()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	list<CString> paramList;
	CString code("");
	 code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
	
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	


	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testDropCarPlace?");

	cmdstr="/testDropCarPlace?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_sendRate()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();
	
	CString rateid;
	
	Crate dlg;
	if(dlg.DoModal()==IDOK)
	{
		rateid = dlg.rateid;
	}
	else
		return;

	
	list<CString> paramList;
	CString code("");


	code="code="+epCode;
    paramList.push_back(code);
   
    code="rateid="+rateid;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testRateCmd?");
	cmdstr="/testRateCmd?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_sendUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();


	CString rateid;
	
	CupdateEp dlg;
	int flag;
	CString hardnumber;
	CString hardversion;
	CString softnumber;
	CString softversion;
	CString md5;
	CString station;
	if(dlg.DoModal()==IDOK)
	{
		hardnumber = dlg.hardnumber;
		hardversion = dlg.hardversion;
		softnumber = dlg.softnumber;
		softversion = dlg.softversion;
		md5 = dlg.md5;
		station = dlg.station;

		flag = dlg.flag;
	}
	else
		return;

	
	list<CString> paramList;
	CString code("");
   
	if(flag==1)
	{
		code="code="+epCode;
	    paramList.push_back(code);
	}
	else
	{
		code="code=0";
	    paramList.push_back(code);
	}


	code="hardcode="+hardnumber;
    paramList.push_back(code);
   
    code="hardver="+hardversion;
    paramList.push_back(code);

	code="md5="+md5;
    paramList.push_back(code);
   
    code="softcode="+softnumber;
    paramList.push_back(code);

    code="softver="+softversion;
    paramList.push_back(code);

	if(flag==1)
	{
		station="stationaddr=0";
        paramList.push_back(station);
	}
	else
	{
		station="stationaddr="+station;
        paramList.push_back(station);
	}

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/force_update_ep_hex?");

	cmdstr="/force_update_ep_hex?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_stat()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

getEpParam();
	
	list<CString> paramList;

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);
	CString cmd("/stat?");
	cmdstr="/stat?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_getVer()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

KillTimer(1);
getEpParam();

	 CString station;
	int flag=1;
	CgetVersion dlg;
	if(dlg.DoModal()==IDOK)
	{
		station = dlg.stationAddr;
		 flag=dlg.flag;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	if(flag==1)
	{
		code="code="+epCode;
	    paramList.push_back(code);
		station="stationaddr=0";
        paramList.push_back(station);
	}
	else
	{
		code="code=0";
	    paramList.push_back(code);
		station="stationaddr="+station;
        paramList.push_back(station);
	}
	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getversion?");
	cmdstr="/getversion?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}


bool CEpGateMaintDlg::comp(const CString str1, const CString str2)
{
     if(str1.Compare(str2)<=0)
		 return false;
     return true;
}

CString CEpGateMaintDlg::Sign(STRLIST paramList)
{
	//paramList.sort(comp);

	CString src("");
	STRLIST::iterator i;
	i= paramList.begin();

  while(i != paramList.end())
  {
	  src += *i;
		src.Append("&");
		i++;
  }
	src += "user=" + user;
	src +=key;

    CString calcSign;

     MD5 iMD5;
    iMD5.GenerateMD5((unsigned char *)src.GetBuffer(), src.GetLength() );
    calcSign =iMD5.ToString().c_str(); 
	
	
	   
	return calcSign;
}

void  CEpGateMaintDlg::setUrl(STRLIST paramList,CString cmd)
{

	CString src;

	urlValue="http://"+curIp+":"+curPort+cmd;


	STRLIST::iterator i;
	i= paramList.begin();

    while(i != paramList.end())
    {
	    src = *i;
		src.Append("&");
		urlValue+=src;
		i++;
    }
	urlValue.TrimRight("&");

}
void CEpGateMaintDlg::OnBnClickedButton_cleanBesp()
{
	if(AfxMessageBox("确定清空预约信息?", MB_OKCANCEL |  MB_ICONQUESTION) == IDCANCEL)
		return;

	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	list<CString> paramList;
	CString code("");

	code="code="+epCode;
    paramList.push_back(code);
	
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/removebesp?");
	cmdstr="/removebesp?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  

	
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_queryVer()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();


	 CString station;
	int flag=1;
	CgetVersion dlg;
	if(dlg.DoModal()==IDOK)
	{
		station = dlg.stationAddr;
		 flag=dlg.flag;
	}
	else
		return;

	CString code("");
	list<CString> paramList;

	if(flag==1)
	{
		code="code="+epCode;
	    paramList.push_back(code);
		station="stationaddr=0";
        paramList.push_back(station);
	}
	else
	{
		code="code=0";
	    paramList.push_back(code);
		station="stationaddr="+station;
        paramList.push_back(station);
	}

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/queryversion?");

	cmdstr="/queryversion?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_restoreGun()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();


	list<CString> paramList;
	CString code("");

	code="code="+epCode;
    paramList.push_back(code);
	
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/gun_restore?");
	cmdstr="/gun_restore?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  

	
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_startCharge_ep()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	


    CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testStartCharge2?");

	cmdstr="/testStartCharge2?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//

}

void CEpGateMaintDlg::OnBnClickedButton_endCharge_ep()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	
	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	 

CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);
	CString cmd("/testStopCharge2?");
	
	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_besp_ep()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();

	CString bespno;
	CString time;
	CString redo;
	CBespno dlg;
	CString payType;

	if(dlg.DoModal()==IDOK)
	{
		time = dlg.timet;
		 redo=dlg.redo;
		 bespno=dlg.bespno;
		 payType = dlg.payType;
	}
	else
		return;


	list<CString> paramList;
	CString code("");

	code="bespno="+bespno;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
	
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	 code="payMode="+payType;
    paramList.push_back(code);

	 code="phone="+account;
    paramList.push_back(code);

	code="redo="+redo;
    paramList.push_back(code);

	 code="time="+time;
    paramList.push_back(code);

	
	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testStartBespoke2?");

	cmdstr="/testStartBespoke2?";
	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_endBesp_ep()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//
KillTimer(1);
	getEpParam();


	CString sbespno;
	CstopBespno dlg;
	if(dlg.DoModal()==IDOK)
	{
		sbespno = dlg.bespno;
	}
	else
		return;



	list<CString> paramList;
	CString code("");

	code="bespno="+sbespno;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/testStopBespoke2?");
	cmdstr="/testStopBespoke2?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CEpGateMaintDlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CEpGateMaintDlg::OnBnClickedButton_monitorStat()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

	getEpParam();

	list<CString> paramList;
	CString code("");

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getmonitorstat?");
	cmdstr="/getmonitorstat?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_getEp()
{
	// TODO: 在此添加控件通知处理程序代码
		
    getEpParam();

	list<CString> paramList;
	CString code("");

	code="code="+epCode;
    paramList.push_back(code);
   
    CString cmd("/getepdetail?");
	cmdstr="/getepdetail?";

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	setUrl(paramList,cmd);

     CHttpClient   conn;
	 ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	 CString s=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(s);//
	
}

void CEpGateMaintDlg::OnBnClickedButton_queryCommSignal()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
    getEpParam();
KillTimer(1);
	 CString station;
	int flag=1;
	CgetVersion dlg;
	if(dlg.DoModal()==IDOK)
	{
		station = dlg.stationAddr;
		 flag=dlg.flag;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	if(flag==1)
	{
		code="code="+epCode;
	    paramList.push_back(code);
		station="stationaddr=0";
        paramList.push_back(station);
	}
	else
	{
		code="code=0";
	    paramList.push_back(code);
		station="stationaddr="+station;
        paramList.push_back(station);
	}
	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/queryCommSignal?");

	cmdstr="/queryCommSignal?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_queryConsumeRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();
KillTimer(1);
	 CString startNum;
	  CString entryNum;
	CconsumeRecord dlg;
	if(dlg.DoModal()==IDOK)
	{
		startNum = dlg.startNum;
		entryNum = dlg.entryNum;
	}
	else
		return;

	list<CString> paramList;
	CString code("");

    code="code="+epCode;
	paramList.push_back(code);
	 
    code="gunno="+gunNo;
    paramList.push_back(code);


	code="startNum="+entryNum;
    paramList.push_back(code);

	code="startPos="+startNum;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/queryConsumeRecord?");

	cmdstr="/queryConsumeRecord?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	switch(m_tabCtrl.GetCurSel())
    {
      case 0:
      {
          m_queryDialog->ShowWindow(1);
	      m_clearDialog->ShowWindow(0);
	      m_testEpGateDialog->ShowWindow(0);
	      m_testEpDialog->ShowWindow(0);
	      m_updateDialog->ShowWindow(0);
	     m_otherDialog->ShowWindow(0);
		  m_tabCtrl.SetCurSel(0);
         break;
      }
      case 1:
	 {
		  m_queryDialog->ShowWindow(0);
	      m_clearDialog->ShowWindow(1);
	      m_testEpGateDialog->ShowWindow(0);
	      m_testEpDialog->ShowWindow(0);
	      m_updateDialog->ShowWindow(0);
	     m_otherDialog->ShowWindow(0);
		 m_tabCtrl.SetCurSel(1);
         break;
      }
	 case 2:
	 {
		  m_queryDialog->ShowWindow(0);
	      m_clearDialog->ShowWindow(0);
	      m_testEpGateDialog->ShowWindow(1);
	      m_testEpDialog->ShowWindow(0);
	      m_updateDialog->ShowWindow(0);
	     m_otherDialog->ShowWindow(0);
		 m_tabCtrl.SetCurSel(2);
         break;
      }
	 case 3:
	 {
		  m_queryDialog->ShowWindow(0);
	      m_clearDialog->ShowWindow(0);
	      m_testEpGateDialog->ShowWindow(0);
	      m_testEpDialog->ShowWindow(1);
	      m_updateDialog->ShowWindow(0);
	     m_otherDialog->ShowWindow(0);
		 m_tabCtrl.SetCurSel(3);
         break;
      }
	 case 4:
	 {
		  m_queryDialog->ShowWindow(0);
	      m_clearDialog->ShowWindow(0);
	      m_testEpGateDialog->ShowWindow(0);
	      m_testEpDialog->ShowWindow(0);
	      m_updateDialog->ShowWindow(1);
	     m_otherDialog->ShowWindow(0);
		 m_tabCtrl.SetCurSel(4);
         break;
      }
	  case 5:
	 {
		  m_queryDialog->ShowWindow(0);
	      m_clearDialog->ShowWindow(0);
	      m_testEpGateDialog->ShowWindow(0);
	      m_testEpDialog->ShowWindow(0);
	      m_updateDialog->ShowWindow(0);
	     m_otherDialog->ShowWindow(1);
		 m_tabCtrl.SetCurSel(5);
         break;
      }
	}
}

void CEpGateMaintDlg::OnBnClickedButton_getStation()
{
	
	// TODO: 在此添加控件通知处理程序代码
    getEpParam();

	 CString stationAddr;
	
	CgetStation dlg;
	if(dlg.DoModal()==IDOK)
	{
		stationAddr = dlg.stationAddr;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
	code="stationaddr="+stationAddr;
    paramList.push_back(code);
	
	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getstationdetail?");
	cmdstr="/getstationdetail?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}




void CEpGateMaintDlg::OnTimer(UINT nIDEvent)
{
	if(BoxFlag)
	{
		KillTimer(1);
		m_queryDialog->SetCheckFalse();
		return;
	}

    // TODO: Add your message handler code here and/or call default
    switch(nIDEvent) {
    case 1:
        {   
			if(cmdstr=="/getgundetail?")
			{
				OnBnClickedButton_getGun();
			}
			else if(cmdstr=="/getepdetail?")
			{
				OnBnClickedButton_getEp();
			}
			else if(cmdstr=="/user?")
			{
				OnBnClickedButton_getUser();
			}
			else if(cmdstr=="/getmonitorstat?")
			{
				OnBnClickedButton_monitorStat();
			}
			else if(cmdstr=="/getReal?")
			{
				OnBnClickedButton_getReal();
			}
			else if(cmdstr=="/getstationdetail?")
			{
			   OnBnClickedButton_getStation();
			}
			else if(cmdstr=="/stat?")
			{
			   OnBnClickedButton_stat();
			}
			else if(cmdstr=="/getChNum?")
			{
			   OnBnClickedButton_getChNum();
			}
        }
		break;
    default:
        break;
    }
   
    CDialog::OnTimer(nIDEvent);
}

void CEpGateMaintDlg::OnBnClickedButton_getReal()
{
	// TODO: 在此添加控件通知处理程序代码

    getEpParam();


	list<CString> paramList;
	CString code("");
	
	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getReal?");
	cmdstr="/getReal?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_createIdentyCode()
{
	 getEpParam();


	list<CString> paramList;
	CString code("");
	
	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/createIdentyCode?");
	cmdstr="/createIdentyCode?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_addWhite()
{
	 getEpParam();


	list<CString> paramList;
	CString code("");
	
	code="code="+epCode;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/addwritelist?");
	cmdstr="/addwritelist?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_setWhite()
{
	 getEpParam();


	  CString open;
	
	Copen_white dlg;
	if(dlg.DoModal()==IDOK)
	{
		open = dlg.open;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
	code="open="+open;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/openwritelist?");
	cmdstr="/openwritelist?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}
void CEpGateMaintDlg::OnBnClickedButton_setStopCarOrgan()
{
	getEpParam();


	  CString open;
	    CString time;
	
	CStopCarOrgan dlg;
	if(dlg.DoModal()==IDOK)
	{
		open = dlg.use;
		time=dlg.time;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
	code="flag="+open;
    paramList.push_back(code);

	code="time="+time;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/setStopCarOrganValid?");
	cmdstr="/setStopCarOrganValid?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_removeWhite()
{
	 getEpParam();


	list<CString> paramList;
	CString code("");
	
	code="code="+epCode;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/removewritelist?");
	cmdstr="/removewritelist?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}


void CEpGateMaintDlg::OnBnClickedButton_getLastConsumeRecord()
{
	getEpParam();
	list<CString> paramList;
	CString code("");
	
	code="code="+epCode;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getLastConsumeRecord?");
	cmdstr="/getLastConsumeRecord?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButtonEp()
{
	// TODO: 在此添加控件通知处理程序代码
	CmanagerEpcode dlg;
	if(dlg.DoModal()==IDOK)
	{
	
	 char scode[1024];
   GetEpCode(scode);
 
   m_epCodeComboBox.ResetContent();

    epCode = scode;
	int pos1=0;
	while(true)
	{
		pos1  = epCode.Find(","); 
		if(pos1 <= 0)
		{
           if(epCode != "")
		   {
			   m_epCodeComboBox.AddString(epCode);
		   }
			break;
		}

		m_epCodeComboBox.AddString(epCode.Left(pos1));

        epCode.TrimLeft(epCode.Left(pos1));
		epCode.TrimLeft(",");

	}
	m_epCodeComboBox.SetCurSel(0);
	}
	
}



void CEpGateMaintDlg::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,str1,strlist;
	((CComboBox *)GetDlgItem(IDC_COMBO3))->GetWindowText(str);//
	int num = m_epCodeComboBox.GetCount();
	bool flag = false;
	for(int i=0;i<num;i++)
	{

		m_epCodeComboBox.GetLBText(i,str1) ;
	    if(str1 ==	str)
		{
			flag = true;
		}
		strlist = strlist+str1+",";
	}
	if(!flag)
	{
		m_epCodeComboBox.AddString(str);
		strlist = strlist+str;
		SetParaStringA(strlist,"OTHER","EpCodes");
	}
    
	

}


void CEpGateMaintDlg::OnBnClickedButton_queryStationEp()
{
	getEpParam();


	  CString addr;
	
	CgetStation dlg;
	if(dlg.DoModal()==IDOK)
	{
		addr = dlg.stationAddr;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
	code="stationaddr="+addr;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/queryConcentratorConfig?");
	cmdstr="/queryConcentratorConfig?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CEpGateMaintDlg::OnBnClickedButton_queryRate()
{
	getEpParam();

	list<CString> paramList;
	CString code("");
	
	code="code="+epCode;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/queryRate?");
	cmdstr="/queryRate?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}


void CEpGateMaintDlg::OnBnClickedButton_getChNum()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

    getEpParam();
	
	list<CString> paramList;

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);
	CString cmd("/getChNum?");
	cmdstr="/getChNum?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}


void CEpGateMaintDlg::OnBnClickedButton_stationSetEpCode()
{
	getEpParam();


	  CString addr;
	
	CgetStation dlg;
	if(dlg.DoModal()==IDOK)
	{
		addr = dlg.stationAddr;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
	code="stationaddr="+addr;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/setConcentratorConfig?");
	cmdstr="/setConcentratorConfig?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}



void CEpGateMaintDlg::OnBnClickedButton_queryflashRam()
{
	getEpParam();


	  CString addr;
	  CString type;
	  CString pos;
	  CString len;
	  CString flag;
	
	Cflashram dlg;
	if(dlg.DoModal()==IDOK)
	{
		addr = dlg.stationAddr;
		type = dlg.type;
		pos = dlg.startPos;
		len = dlg.len;
		flag = dlg.flag;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	if(flag=="1")
	{
		code="code=0000000000000000";
	}
	else
	    code="code="+epCode;
    paramList.push_back(code);

	code="leng="+len;
    paramList.push_back(code);

	code="pos="+pos;
    paramList.push_back(code);
	
	code="stationaddr="+addr;
    paramList.push_back(code);

	code="type="+type;
    paramList.push_back(code);
   

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/queryFlashRam?");
	cmdstr="/queryFlashRam?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}


void CEpGateMaintDlg::OnBnClickedButton_repeatSend()
{
	getEpParam();


	  CString time;
	  CString flag;
	
	CrepeatSend dlg;
	if(dlg.DoModal()==IDOK)
	{
		time = dlg.time;
		flag = dlg.flag;
	}
	else
		return;

	list<CString> paramList;
	CString code("");

	code="flag="+flag;
    paramList.push_back(code);

	code="time="+time;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/epReSend?");
	cmdstr="/epReSend?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}
