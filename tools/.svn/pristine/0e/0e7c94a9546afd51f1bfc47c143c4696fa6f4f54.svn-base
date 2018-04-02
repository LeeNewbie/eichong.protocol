// managerEpcode.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "managerEpcode.h"
#include "Config.h"
#include "AddEpCode.h"

// CmanagerEpcode 对话框

IMPLEMENT_DYNAMIC(CmanagerEpcode, CDialog)

CmanagerEpcode::CmanagerEpcode(CWnd* pParent /*=NULL*/)
	: CDialog(CmanagerEpcode::IDD, pParent)
{
     
}

CmanagerEpcode::~CmanagerEpcode()
{
}

void CmanagerEpcode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_epCodeList);
}

BOOL CmanagerEpcode::OnInitDialog()
{
   CDialog::OnInitDialog();
  // m_configName="";
  // m_itemName="";

   
	char scode[1024];
   if(GetIniResult(scode,m_configName,m_itemName)<0)
   {
	   AfxMessageBox(LPCTSTR("dwatch.ini不存在或电桩编号配置错误"));
	   return true;
   }

    epCodes = scode;
	int pos1=0;
	while(true)
	{
		pos1  = epCodes.Find(","); 
		if(pos1 <= 0)
		{
           if(epCodes != "")
		   {
			   m_epCodeList.AddString(epCodes);
		   }
			break;
		}

		m_epCodeList.AddString(epCodes.Left(pos1));

        epCodes.TrimLeft(epCodes.Left(pos1));
		epCodes.TrimLeft(",");

	}

	
	return true;
}


BEGIN_MESSAGE_MAP(CmanagerEpcode, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CmanagerEpcode::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CmanagerEpcode::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CmanagerEpcode::OnBnClickedOk)
END_MESSAGE_MAP()

void CmanagerEpcode::setParam(CString configName,CString itemName)
{

    m_configName = configName;
	m_itemName = itemName;

}

// CmanagerEpcode 消息处理程序

void CmanagerEpcode::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	

	 CString epstr;
	 CAddEpCode dlg;
	if(dlg.DoModal()==IDOK)
	{
		epstr = dlg.epcode;
	}
	else
	{
		return;
	}
	if(epstr == "")
		return;
	if(m_epCodeList.FindString(-1,epstr)==LB_ERR)
	{
		m_epCodeList.AddString(epstr);
		epCodes=","+epstr;
	}

}

void CmanagerEpcode::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_epCodeList.GetText(m_epCodeList.GetCurSel(),str);

	epCodes.Replace(str,"");
	epCodes.TrimLeft(",");
	epCodes.TrimRight(",");

   m_epCodeList.DeleteString(m_epCodeList.GetCurSel());
  
   
}

void CmanagerEpcode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	CString str,str1;
	
	int num = m_epCodeList.GetCount();
	for(int i=0;i<num;i++)
	{
		m_epCodeList.GetText(i,str1);
		str+=str1+",";

	}
	SetParaStringA(str,m_configName,m_itemName);
}


