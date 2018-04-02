// QueryDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "afxwin.h"
#include "EpGateMaint.h"
#include "QueryDialog.h"
#include "managerEpcode.h"

#include "httpClient.h"
#include "Md5.h"
#include "flashram.h"
#include "getVersion.h"
#include "rate.h"
#include "updateEp.h"
#include "consumeRecord.h"
#include "getStation.h"

#include "rate.h"
#include "updateEp.h"
#include "stopBespno.h"
#include "startCharge.h"
#include "consumeRecord.h"
#include "Bespno.h"
#include "CallEp.h"
#include "getStation.h"
#include "getVersion.h"
#include "repeatSend.h"
#include "open_white.h"
#include "managerEpcode.h"
#include "StopCarOrgan.h"

#include "getEpType.h"



extern  bool BoxFlag;

extern  EPGATE * m_epGateServer;
extern USERGATE * m_userGateServer;
extern REGEDITGATE * m_regeditGateServer;
extern THIRDGATE * m_thirdGateServer;

// CQueryDialog 对话框

IMPLEMENT_DYNAMIC(CQueryDialog, CDialog)

CQueryDialog::CQueryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryDialog::IDD, pParent)
{
     //  m_dlg = (CEpGateMaintDlg *)pParent;
}

CQueryDialog::~CQueryDialog()
{
}

void CQueryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CQueryDialog, CDialog)
	
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CQueryDialog::OnBnClickedCheck1)
	
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_BUTTON_EP, &CQueryDialog::OnBnClickedButtonEp)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CQueryDialog::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CQueryDialog::OnCbnSelchangeCombo2)

	ON_BN_CLICKED(IDC_BUTTON_EP2, &CQueryDialog::OnBnClickedButtonEp2)
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CQueryDialog::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CQueryDialog::OnBnClickedButton_getGun)
	ON_BN_CLICKED(IDC_BUTTON2, &CQueryDialog::OnBnClickedButton_getUser)
	ON_BN_CLICKED(IDC_BUTTON3, &CQueryDialog::OnBnClickedButton_getStat)
	ON_BN_CLICKED(IDC_BUTTON8, &CQueryDialog::OnBnClickedButton_getBom)
	ON_BN_CLICKED(IDC_BUTTON6, &CQueryDialog::OnBnClickedButton_getReal)
	ON_BN_CLICKED(IDC_BUTTON4, &CQueryDialog::OnBnClickedButton_getEp)
	ON_BN_CLICKED(IDC_BUTTON5, &CQueryDialog::OnBnClickedButton_getCentor)
	ON_BN_CLICKED(IDC_BUTTON7, &CQueryDialog::OnBnClickedButton_getVersion)
	ON_BN_CLICKED(IDC_BUTTON29, &CQueryDialog::OnBnClickedButton_getChannel)
	ON_BN_CLICKED(IDC_BUTTON24, &CQueryDialog::OnBnClickedButton_getMonitor)
	ON_BN_CLICKED(IDC_BUTTON26, &CQueryDialog::OnBnClickedButton_getLastRecord)
	ON_BN_CLICKED(IDC_BUTTON11, &CQueryDialog::OnBnClickedButton_getGameConfig)
	ON_BN_CLICKED(IDC_BUTTON27, &CQueryDialog::OnBnClickedButton_sendComm)
	ON_BN_CLICKED(IDC_BUTTON30, &CQueryDialog::OnBnClickedButton_sendEp)
	ON_BN_CLICKED(IDC_BUTTON9, &CQueryDialog::OnBnClickedButton_sendCentorConfig)
	ON_BN_CLICKED(IDC_BUTTON13, &CQueryDialog::OnBnClickedButton_sendFlash)
	ON_BN_CLICKED(IDC_BUTTON10, &CQueryDialog::OnBnClickedButton_sendQueryRate)
	ON_BN_CLICKED(IDC_BUTTON12, &CQueryDialog::OnBnClickedButton_getThirdstat)
	ON_BN_CLICKED(IDC_BUTTON14, &CQueryDialog::OnBnClickedButton_usrGate)
	ON_BN_CLICKED(IDC_BUTTON17, &CQueryDialog::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CQueryDialog::OnBnClickedButton18)
END_MESSAGE_MAP()


BEGIN_EASYSIZE_MAP(CQueryDialog)

	
	 EASYSIZE(IDC_STATIC2,ES_BORDER,ES_BORDER,IDC_STATIC1,ES_BORDER,0)
	 EASYSIZE(IDC_STATIC1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	 EASYSIZE(IDC_EDIT8,ES_BORDER,ES_BORDER,ES_BORDER,IDC_EDIT1,0)
	 EASYSIZE(IDC_EDIT1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	

	

END_EASYSIZE_MAP

// CQueryDialog 消息处理程序



bool CQueryDialog::comp(const CString str1, const CString str2)
{
     if(str1.Compare(str2)<=0)
		 return false;
     return true;
}

CString CQueryDialog::Sign(STRLIST paramList)
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

void  CQueryDialog::setUrl(STRLIST paramList,CString cmd)
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

void CQueryDialog::OnBnClickedButton_getGun()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_getGun();

	
	
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

	FILE *file = NULL;
	char filename[256];
    char path[256];
	GetModuleFileName(NULL, path, 256);
	char* p = strrchr(path, '\\');
	if (p)
		*p = 0;
    strcat(path, "\\comm.log");

	sprintf(filename, path);
	file = fopen(filename, "ab");
	if(file !=NULL)
	{
       fwrite(s, 1, s.GetLength(), file);
	   fclose(file);
	   file= NULL;
	}
		

}

void CQueryDialog::OnBnClickedButton_getUser()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_getUser();
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

void CQueryDialog::OnBnClickedButton_getStat()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_stat();
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


void CQueryDialog::OnBnClickedButton_getMonitor()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_monitorStat();
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

void CQueryDialog::OnBnClickedButton_getEp()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_getEp();
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

void CQueryDialog::OnBnClickedButton_getCentor()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_getStation();
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




void CQueryDialog::OnTimer(UINT nIDEvent)
{
	
    if(BoxFlag)
	{
		KillTimer(1);
		SetCheckFalse();
		return;
	}
    // TODO: Add your message handler code here and/or call default
    switch(nIDEvent) {
    case 1:
        {   
			if(cmdstr=="/getgundetail?")
			{
				// TODO: 在此添加控件通知处理程序代码
	 getEpParam();
	 CString station;
	int flag=1;


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
				OnBnClickedButton_getMonitor();
			}
			else if(cmdstr=="/getReal?")
			{
				OnBnClickedButton_getReal();
			}
			else if(cmdstr=="/getstationdetail?")
			{
			   OnBnClickedButton_getCentor();
			}
			else if(cmdstr=="/stat?")
			{
			   OnBnClickedButton_getStat();
			}
			else if(cmdstr=="/getChNum?")
			{
			   OnBnClickedButton_getChannel();
			}
			else if(cmdstr=="/queryCommSignal?")
			{
			   OnBnClickedButton_sendComm();
			}
        }
		break;
    default:
        break;
    }
   
    CDialog::OnTimer(nIDEvent);
}

BOOL CQueryDialog::OnInitDialog()
{
  //  SetTimer(1,10000,0);
	((CEdit *)GetDlgItem(IDC_EDIT2))->SetWindowText("1");//
	state=0;
    m_index=-1;
	 m_type=-1;
	m_userComboBox = ((CComboBox *)GetDlgItem(IDC_COMBO2));
	m_epCodeComboBox = ((CComboBox *)GetDlgItem(IDC_COMBO3));

	//m_tab = ((CTabCtrl *)GetDlgItem(IDC_TAB1));
	
	//m_ctrlEpDialog = new CCtrlEpDialog();


	//m_tab->InsertItem(0,"dddd");
    
	//m_ctrlEpDialog->Create(IDD_CTRLEP,GetDlgItem(IDC_TAB1));
	//


   // CRect rs;

   // m_tab->GetClientRect(&rs);
   //设置子对话框尺寸并移动到指定位置
  //  m_ctrlEpDialog->MoveWindow(&rs);
	

    //分别设置隐藏和显示
  //   m_ctrlEpDialog->ShowWindow(1);
	
  
    //设置默认的选项卡
   //  m_tab->SetCurSel(0);

	INIT_EASYSIZE;
	return true;
}
void CQueryDialog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK1 ) )
    {
		((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(sec);//

		int iSec=atoi(sec);
         // 勾选
		state=1;
        SetTimer(1,iSec*1000,0);
    }
    else
    {
		state=0;
	    KillTimer(1);
    }
}

void CQueryDialog::OnBnClickedButton_getReal()
{
	// TODO: 在此添加控件通知处理程序代码
  //  m_dlg->OnBnClickedButton_getReal();
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


void CQueryDialog::OnBnClickedButton_getLastRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_dlg->OnBnClickedButton_getLastConsumeRecord();
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

void CQueryDialog::OnBnClickedButton_getChannel()
{
	// TODO: 在此添加控件通知处理程序代码
	// m_dlg->OnBnClickedButton_getChNum();
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

void CQueryDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	UPDATE_EASYSIZE;
}

void CQueryDialog::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	EASYSIZE_MINSIZE(280,250,fwSide,pRect);
}

void CQueryDialog::setParam(CString ip,CString port,CString accounts,CString epCodes,CString sgunNo,CString suser,CString skey,CString configName,int index,int type)
{
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("");//

	 m_epCodes=epCodes;
     gunNo =sgunNo;
	 m_accounts =accounts;
	 curIp = ip;
	 curPort = port;

	 user =suser;
	 key=skey;
     m_configName=configName;
	 CString str = curIp+":"+curPort;
	 
	// ((CEdit *)GetDlgItem(IDC_EDIT3))->SetWindowText(str);//
	 ((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText(gunNo);//
	 m_index=index;
	 m_type=type;
	 
     initUserComBox();
	 initEpCodeComBox();

	


}

void CQueryDialog::OnBnClickedButtonEp()
{
	// TODO: 在此添加控件通知处理程序代码
	CmanagerEpcode dlg;
	dlg.setParam(m_configName,"EpCodes");
	if(dlg.DoModal()==IDOK)
	{
        m_epCodeComboBox->ResetContent();
		char scode[1024];
       GetIniResult(scode,m_configName,"EpCodes");
 
       m_userComboBox->ResetContent();

        m_epCodes = scode;
		   
	    initEpCodeComBox();
	}
	
}

void CQueryDialog::SetCheckFalse()
{
   ((CButton*)(GetDlgItem( IDC_CHECK1 )))->SetCheck( 0 ) ;
}

void CQueryDialog::initEpCodeComBox()
{
	if(m_epCodes=="")
		return;

	m_epCodeComboBox->ResetContent();
	 int pos1=0;
	    while(true)
	    {
		  pos1  = m_epCodes.Find(","); 
		  if(pos1 <= 0)
		  {
             if(m_epCodes != "")
		     {
			     m_epCodeComboBox->AddString(m_epCodes);
		     }
			break;
		  }

		  m_epCodeComboBox->AddString(m_epCodes.Left(pos1));

          m_epCodes.TrimLeft(m_epCodes.Left(pos1));
		  m_epCodes.TrimLeft(",");

	    }
		if(m_index>=0)
		{
	   if(m_epGateServer[m_index].curEpCode=="")
	   {
		     m_epCodeComboBox->SetCurSel(0);
	   }
	   else
	   {
		   m_epCodeComboBox->SetWindowText(m_epGateServer[m_index].curEpCode);
	   }
		}
		else
            m_epCodeComboBox->SetWindowText(m_epGateServer[m_index].curEpCode);
}

void CQueryDialog::initUserComBox()
{
	if(m_accounts=="")
		return;

	
	m_userComboBox->ResetContent();

	 int pos1=0;
	    while(true)
	    {
		  pos1  = m_accounts.Find(","); 
		  if(pos1 <= 0)
		  {
             if(m_accounts != "")
		     {
			      m_userComboBox->AddString(m_accounts);
		     }
			break;
		  }

		  m_userComboBox->AddString(m_accounts.Left(pos1));

          m_accounts.TrimLeft(m_accounts.Left(pos1));
		  m_accounts.TrimLeft(",");

	    }
		if(m_index>=0)
		{

	    if(m_epGateServer[m_index].curAccount=="")
	   {
		     m_userComboBox->SetCurSel(0);
	   }
	   else
	   {
		   m_userComboBox->SetWindowText(m_epGateServer[m_index].curAccount);
	   }
		}
		else
            m_userComboBox->SetWindowText(m_epGateServer[m_index].curAccount);
}


void CQueryDialog::getEpParam()
{
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("");//

	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("");//

	//get方式    
	((CComboBox *)GetDlgItem(IDC_COMBO3))->GetWindowText(epCode);//
	((CEdit *)GetDlgItem(IDC_EDIT4))->GetWindowText(gunNo);//
	((CComboBox *)GetDlgItem(IDC_COMBO2))->GetWindowText(account);//
    if(m_index >=0 && m_type >=0)
	{
		m_epGateServer[m_index].curEpCode = epCode;
		m_epGateServer[m_index].curAccount = account;
	}

	SetParaStringA(gunNo,m_configName,"GunNo");

	OnCbnSelchangeCombo3();
	OnCbnSelchangeCombo2();

}
void CQueryDialog::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,str1,strlist;
	((CComboBox *)GetDlgItem(IDC_COMBO3))->GetWindowText(str);//
	int num = m_epCodeComboBox->GetCount();
	bool flag = false;
	for(int i=0;i<num;i++)
	{

		m_epCodeComboBox->GetLBText(i,str1) ;
	    if(str1 ==	str)
		{
			flag = true;
		}
		strlist = strlist+str1+",";
	}
	if(!flag)
	{
		m_epCodeComboBox->AddString(str);
		m_epCodes = strlist+str;
		SetParaStringA(m_epCodes,m_configName,"EpCodes");
	}
}

void CQueryDialog::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,str1,strlist;
	((CComboBox *)GetDlgItem(IDC_COMBO2))->GetWindowText(str);//
	int num = m_userComboBox->GetCount();
	bool flag = false;
	for(int i=0;i<num;i++)
	{

		m_userComboBox->GetLBText(i,str1) ;
	    if(str1 ==	str)
		{
			flag = true;
		}
		strlist = strlist+str1+",";
	}
	if(!flag)
	{
		m_userComboBox->AddString(str);
		m_accounts = strlist+str;
		SetParaStringA(m_accounts,m_configName,"Accounts");
	}
}

void CQueryDialog::OnBnClickedButton_getBom()
{
	// TODO: 在此添加控件通知处理程序代码
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

void CQueryDialog::OnBnClickedButton_getVersion()
{
	// TODO: 在此添加控件通知处理程序代码
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

void CQueryDialog::OnBnClickedButton_sendComm()
{
	// TODO: 在此添加控件通知处理程序代码
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

	CString cmd("/queryCommSignal?");

	cmdstr="/queryCommSignal?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//

	

}

void CQueryDialog::OnBnClickedButton_sendEp()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

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

void CQueryDialog::OnBnClickedButton_sendCentorConfig()
{
	// TODO: 在此添加控件通知处理程序代码
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

void CQueryDialog::OnBnClickedButton_sendFlash()
{
	// TODO: 在此添加控件通知处理程序代码
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

void CQueryDialog::OnBnClickedButton_sendQueryRate()
{
	// TODO: 在此添加控件通知处理程序代码
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

void CQueryDialog::OnBnClickedButtonEp2()
{
	// TODO: 在此添加控件通知处理程序代码
	CmanagerEpcode dlg;
	dlg.setParam(m_configName,"Accounts");
	if(dlg.DoModal()==IDOK)
	{
	
	   char scode[1024];
       GetIniResult(scode,m_configName,"Accounts");
 
       m_userComboBox->ResetContent();

          m_accounts = scode;
		   initUserComBox();
	}

}

void CQueryDialog::OnBnClickedButton_getGameConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

	list<CString> paramList;

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getGameConfig?");
	cmdstr="/getGameConfig?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}



void CQueryDialog::OnBnClickedButton_startCharge()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_startCharge();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_endCharge()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_endCharge();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_startBesp()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_startBesp();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_endBesp()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_endBesp();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_callEp()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_callEp();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}



void CQueryDialog::OnBnClickedButton_drop()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_dropClock();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}



void CQueryDialog::OnBnClickedButton_sendRate()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_sendRate();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_createIdenty()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_createIdentyCode();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_configCentor()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_stationSetEpCode();
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_epStartCharge()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_epEndCharge()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_epStartBesp()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_epEndBesp()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_forceUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_sendVersion()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_reconnect()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(s);//
}


void CQueryDialog::OnBnClickedButton_addWhite()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_config()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_setWhite()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_setStop()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_removeWhite()
{
	// TODO: 在此添加控件通知处理程序代码
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_clearCharge()
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

	CString cmd("/removeCharge?");
	cmdstr="/removeCharge?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_clearBesp()
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

	CString cmd("/removebesp?");
	cmdstr="/removebesp?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  

	
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}


void CQueryDialog::OnBnClickedButton_clearUsr()
{
	// TODO: 在此添加控件通知处理程序代码
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


void CQueryDialog::OnBnClickedButton_restoreGun()
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

	CString cmd("/removeCharge?");
	cmdstr="/removeCharge?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}
void CQueryDialog::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CQueryDialog::OnBnClickedButton_getThirdstat()
{
	// TODO: 在此添加控件通知处理程序代码
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

	CString cmd("/getThirdstat?");
	cmdstr="/getThirdstat?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CQueryDialog::OnBnClickedButton_usrGate()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

	list<CString> paramList;
	CString code("");



	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getUsrGatestat?");
	cmdstr="/getUsrGatestat?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CQueryDialog::OnBnClickedButton17()
{

	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

	CString type;
	
	CgetEpType dlg;
	if(dlg.DoModal()==IDOK)
	{
		type = dlg.type;
	}
	else
		return;

	list<CString> paramList;
	CString code("");

    code="id="+type;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getbomListById?");
	cmdstr="/getbomListById?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(urlValue);//
}

void CQueryDialog::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	char path[256];
	char cmdStr[256];

	char antflag[256];
	GetIniResult(antflag,m_configName,"ant");
	CString ant = antflag;
	if(ant != "0")
	{
		GetModuleFilePath(path);
		sprintf(cmdStr, "%sant.bat "+ant, path);
		system(cmdStr);
	}

	char pword[256];
	GetIniResult(pword,m_configName,"pw");
	CString pw = pword;

	char spath[256];
	GetIniResult(spath,m_configName,"server_path");
	CString sp = spath;

	char filelist[256];
	GetIniResult(filelist,m_configName,"file_list");
	CString flist = filelist;

	int pos1=0;
	while(true)
	{
		pos1  = flist.Find(","); 
		if(pos1 <= 0)
		{
			if(flist != "")
			{
				Pscp(pw,sp,flist);
			}
			break;
		}

		Pscp(pw,sp,flist.Left(pos1));

		flist.TrimLeft(flist.Left(pos1));
		flist.TrimLeft(",");
	}

	CString strCmd = "./restart.sh " + curIp + " " + pw;
	//ShellExecute(NULL, "open", "C:\\cygwin\\bin\\mintty.exe", strCmd, NULL, SW_SHOW);  
}

void CQueryDialog::Pscp(CString pw,CString sp,CString filename)
{
	CString temp = "pscp.exe -l root -pw \"" + pw + "\" -scp ";
	//CString basepath = "D:\\eichong\\protocol\\dEpPlatform\\";
	CString basepath = "D:\\idea\\eichong\\";
	CString localpath = basepath+"ecnLibs3.0\\lib\\";

	if (filename == "dEpGate.jar") {
		char version[256];
		CString fname = basepath+"dEpGate\\release\\dEpGateUpdate.log";
		CString key = "deploy";
		GetVersionString(version,fname.GetBuffer(),key.GetBuffer());
		CString ver = version;
		localpath = basepath+"dEpGate\\release\\";
		filename = "dEpGate." + ver + ".jar";
		sp += "dEpGate/bin";
	} else if (filename == "dPhoneGate.jar") {
		char version[256];
		CString fname = basepath+"dPhoneGate\\release\\dPhoneGateUpdate.log";
		CString key = "deploy";
		GetVersionString(version,fname.GetBuffer(),key.GetBuffer());
		CString ver = version;
		localpath = basepath+"dPhoneGate\\release\\";
		filename = "dPhoneGate." + ver + ".jar";
		sp += "dPhoneGate/bin";
	} else {
		sp += "ecnLibs3.0/lib";
	}
	temp += localpath + filename;
	temp += " root@" + curIp + ":" + sp;
	//temp += " & pause";

	PVOID OldValue;
	BOOL bRet = Wow64DisableWow64FsRedirection (&OldValue);
	if (bRet == TRUE)
	{
		system(temp.GetBuffer());
		Wow64RevertWow64FsRedirection (OldValue);
	}
}
