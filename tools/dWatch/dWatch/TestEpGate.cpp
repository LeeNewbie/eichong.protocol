// TestEpGate.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "TestEpGate.h"
#include "Config.h"
#include "httpClient.h"
#include "Md5.h"
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
#include "VinCode.h"
#include "testCharge2.h"
#include "PrintMsg.h"
#include "TempChargeNum.h"

extern  EPGATE * m_epGateServer;
extern USERGATE * m_userGateServer;
extern REGEDITGATE * m_regeditGateServer;
extern THIRDGATE * m_thirdGateServer;

// CTestEpGate 对话框

IMPLEMENT_DYNAMIC(CTestEpGate, CDialog)

CTestEpGate::CTestEpGate(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEpGate::IDD, pParent)
{
//    m_dlg = (CEpGateMaintDlg *)pParent;
}

CTestEpGate::~CTestEpGate()
{
}

void CTestEpGate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestEpGate, CDialog)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestEpGate::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTestEpGate::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestEpGate::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestEpGate::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CTestEpGate::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CTestEpGate::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CTestEpGate::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestEpGate::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestEpGate::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CTestEpGate::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTestEpGate::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON20, &CTestEpGate::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CTestEpGate::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CTestEpGate::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &CTestEpGate::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON25, &CTestEpGate::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON28, &CTestEpGate::OnBnClickedButton28)
	ON_BN_CLICKED(IDC_BUTTON15, &CTestEpGate::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON33, &CTestEpGate::OnBnClickedButton33)
	ON_BN_CLICKED(IDC_BUTTON36, &CTestEpGate::OnBnClickedButton36)
	ON_BN_CLICKED(IDC_BUTTON34, &CTestEpGate::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestEpGate::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON35, &CTestEpGate::OnBnClickedButton35)
	ON_BN_CLICKED(IDC_BUTTON_EP, &CTestEpGate::OnBnClickedButtonEp)
	ON_BN_CLICKED(IDC_BUTTON_EP2, &CTestEpGate::OnBnClickedButtonEp2)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_BUTTON16, &CTestEpGate::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON37, &CTestEpGate::OnBnClickedButton37)
	ON_BN_CLICKED(IDC_BUTTON38, &CTestEpGate::OnBnClickedButton38)
	ON_BN_CLICKED(IDC_BUTTON19, &CTestEpGate::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON31, &CTestEpGate::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestEpGate::OnBnClickedButton4)
//	ON_BN_CLICKED(IDC_BUTTON5, &CTestEpGate::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON5, &CTestEpGate::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON13, &CTestEpGate::OnBnClickedButton13)
ON_BN_CLICKED(IDC_BUTTON32, &CTestEpGate::OnBnClickedButton32)
ON_BN_CLICKED(IDC_BUTTON18, &CTestEpGate::OnBnClickedButton18)
ON_BN_CLICKED(IDC_BUTTON39, &CTestEpGate::OnBnClickedButton39)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CTestEpGate)

	 EASYSIZE(IDC_STATIC2,ES_BORDER,ES_BORDER,IDC_STATIC1,ES_BORDER,0)
	 EASYSIZE(IDC_STATIC1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	 EASYSIZE(IDC_EDIT8,ES_BORDER,ES_BORDER,ES_BORDER,IDC_EDIT1,0)
	 EASYSIZE(IDC_EDIT10,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)

END_EASYSIZE_MAP


BOOL CTestEpGate::OnInitDialog()
{
  
	 m_index = 0;
	 m_type = 0;
	m_userComboBox = ((CComboBox *)GetDlgItem(IDC_COMBO2));
	m_epCodeComboBox = ((CComboBox *)GetDlgItem(IDC_COMBO3));
	INIT_EASYSIZE;
	return true;
}

// CTestEpGate 消息处理程序

void CTestEpGate::OnBnClickedButton6()
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
		 psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
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

	code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_endCharge();
	getEpParam();

	CtestCharge2 dlg;
	if(dlg.DoModal()==IDOK)
	{
		 psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
	}
	else
		return;

	
	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	 code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton8()
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
		 psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
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

	code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_dlg->OnBnClickedButton_endBesp();
	getEpParam();

	CtestCharge2 dlg;
	if(dlg.DoModal()==IDOK)
	{
		 psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
	}
	else
		return;

	list<CString> paramList;
	CString code("");

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton10()
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

void CTestEpGate::OnBnClickedButton11()
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

void CTestEpGate::OnBnClickedButton12()
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

void CTestEpGate::OnBnClickedButton1()
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

void CTestEpGate::OnBnClickedButton2()
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

void CTestEpGate::OnCbnSelchangeCombo3()
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

void CTestEpGate::OnCbnSelchangeCombo2()
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


void CTestEpGate::OnBnClickedButton20()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

	
	CtestCharge2 dlg;
	if(dlg.DoModal()==IDOK)
	{
		 psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
	}
	else
		return;

	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton21()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

	CtestCharge2 dlg;
	if(dlg.DoModal()==IDOK)
	{
		 psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
	}
	else
		return;


	list<CString> paramList;
	CString code("");

	code="account="+account;
    paramList.push_back(code);

	code="code="+epCode;
    paramList.push_back(code);
   
    code="gunno="+gunNo;
    paramList.push_back(code);

	code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton22()
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
		  psword = dlg.pw;
		 pswordMd5 = calcMd5(psword);
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

	code="psWord="+pswordMd5;
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

void CTestEpGate::OnBnClickedButton23()
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

void CTestEpGate::OnBnClickedButton25()
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

void CTestEpGate::OnBnClickedButton28()
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

void CTestEpGate::OnBnClickedButton15()
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

void CTestEpGate::OnBnClickedButton33()
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

void CTestEpGate::OnBnClickedButton36()
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

void CTestEpGate::OnBnClickedButton34()
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

void CTestEpGate::OnBnClickedButton3()
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

void CTestEpGate::OnBnClickedButton35()
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

void CTestEpGate::OnBnClickedButtonEp()
{
	// TODO: 在此添加控件通知处理程序代码
	
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

void CTestEpGate::OnBnClickedButtonEp2()
{

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

void CTestEpGate::setParam(CString ip,CString port,CString accounts,CString epCodes,CString sgunNo,CString suser,CString skey,CString configName,int index,int type)
{
	((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText("");//

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
	 
//	 ((CEdit *)GetDlgItem(IDC_EDIT3))->SetWindowText(str);//
	 ((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText(gunNo);//
	
	  m_index=index;
	 m_type=type;
	

     initUserComBox();
	 initEpCodeComBox();

	 if(ip=="119.254.100.83")
	 {
          GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);//
		  GetDlgItem(IDC_BUTTON21)->EnableWindow(FALSE);//
		  GetDlgItem(IDC_BUTTON22)->EnableWindow(FALSE);//
		  GetDlgItem(IDC_BUTTON23)->EnableWindow(FALSE);//
	 }
	 else
	 {
		 GetDlgItem(IDC_BUTTON20)->EnableWindow(TRUE);//
		  GetDlgItem(IDC_BUTTON21)->EnableWindow(TRUE);//
		  GetDlgItem(IDC_BUTTON22)->EnableWindow(TRUE);//
		  GetDlgItem(IDC_BUTTON23)->EnableWindow(TRUE);//
	 }

	

}


bool CTestEpGate::comp(const CString str1, const CString str2)
{
     if(str1.Compare(str2)<=0)
		 return false;
     return true;
}

CString CTestEpGate::calcMd5(CString src)
{
	 CString calcMd5;

     MD5 iMD5;
    iMD5.GenerateMD5((unsigned char *)src.GetBuffer(), src.GetLength() );
    calcMd5 =iMD5.ToString().c_str(); 
	return calcMd5;
}

CString CTestEpGate::Sign(STRLIST paramList)
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

void  CTestEpGate::setUrl(STRLIST paramList,CString cmd)
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


void CTestEpGate::getEpParam()
{
	((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText("");//

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


void CTestEpGate::initEpCodeComBox()
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

void CTestEpGate::initUserComBox()
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
			m_userComboBox->SetCurSel(0);
}

void CTestEpGate::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	UPDATE_EASYSIZE;
}

void CTestEpGate::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	EASYSIZE_MINSIZE(280,250,fwSide,pRect);
}

void CTestEpGate::OnBnClickedButton16()
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

void CTestEpGate::OnBnClickedButton37()
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
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}

void CTestEpGate::OnBnClickedButton38()
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

void CTestEpGate::OnBnClickedButton19()
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

	
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}

void CTestEpGate::OnBnClickedButton31()
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

void CTestEpGate::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	getEpParam();

	CVinCode dlg;
	
	CString vinCode;
	if(dlg.DoModal()==IDOK)
	{
		vinCode = dlg.vinCode;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
    code="carVinCode="+vinCode;
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

	CString cmd("/setVinCode?");
	cmdstr="/setVinCode?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  

	
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}
//
//void CTestEpGate::OnBnClickedButton5()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	// TODO: 在此添加控件通知处理程序代码
//	getEpParam();
//
//	CSms dlg;
//	
//	CString SMS;
//	if(dlg.DoModal()==IDOK)
//	{
//		SMS = dlg.SMS;
//	}
//	else
//		return;
//
//	list<CString> paramList;
//	CString code("");
//	
//    code="SMS="+SMS;
//    paramList.push_back(code);
//
//	CString calcSign=Sign(paramList);
//
//	CString temp = "user=" + user;
//	paramList.push_back(temp);
//
//	temp= "sign=" +calcSign ;
//	paramList.push_back(temp);
//
//	CString cmd("/setSMS?");
//	cmdstr="/setSMS?";
//
//	setUrl(paramList,cmd);
//
//    CHttpClient   conn;
//    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//
//
//	urlValue=conn.doGet(urlValue);  
//
//	
//    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
//}

void CTestEpGate::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	
		getEpParam();

	CPrintMsg dlg;
	
	CString isPrint;
	if(dlg.DoModal()==IDOK)
	{
		isPrint = dlg.isPrint;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
    code="printEpMsg="+isPrint;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/setPrintEpMsg?");
	cmdstr="/setPrintEpMsg?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  

	
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}

void CTestEpGate::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
		getEpParam();

	CPrintMsg dlg;
	
	CString isPrint;
	if(dlg.DoModal()==IDOK)
	{
		isPrint = dlg.isPrint;
	}
	else
		return;

	list<CString> paramList;
	CString code("");
	
    code="printPhoneMsg="+isPrint;
    paramList.push_back(code);

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/setPrintPhoneMsg?");
	cmdstr="/setPrintPhoneMsg?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  

	
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}

void CTestEpGate::OnBnClickedButton32()
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

	CString cmd("/removeUpdate?");
	cmdstr="/removeUpdate?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}

void CTestEpGate::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	
	getEpParam();


	CString num="";
	CString time="";
	CString flag="";
	CTempChargeNum dlg;
	if(dlg.DoModal()==IDOK)
	{
		num=dlg.num;
		time=dlg.time;
		flag=dlg.flag;
	}
	else
		return;

	CString code("");
	list<CString> paramList;

	
	code="code="+epCode;
    paramList.push_back(code);
	code="flag="+flag;
    paramList.push_back(code);
	code="num="+num;
    paramList.push_back(code);
	code="time="+time;
    paramList.push_back(code);

	
	
	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/setEPWorkArg?");
	cmdstr="/setEPWorkArg?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}

void CTestEpGate::OnBnClickedButton39()
{
	// TODO: 在此添加控件通知处理程序代码
	
	getEpParam();

	CString code("");
	list<CString> paramList;

	code="code="+epCode;
	paramList.push_back(code);
		

	CString calcSign=Sign(paramList);

	CString temp = "user=" + user;
	paramList.push_back(temp);

	temp= "sign=" +calcSign ;
	paramList.push_back(temp);

	CString cmd("/getMaxTempChargeNum?");
	cmdstr="/getMaxTempChargeNum?";

	setUrl(paramList,cmd);

    CHttpClient   conn;
    ((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText(urlValue);//

	urlValue=conn.doGet(urlValue);  
    ((CEdit *)GetDlgItem(IDC_EDIT10))->SetWindowText(urlValue);//
}
