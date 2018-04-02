// getVersion.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "getVersion.h"


// CgetVersion 对话框

IMPLEMENT_DYNAMIC(CgetVersion, CDialog)

CgetVersion::CgetVersion(CWnd* pParent /*=NULL*/)
	: CDialog(CgetVersion::IDD, pParent)
{
   flag =1;
   stationAddr="0";
}

CgetVersion::~CgetVersion()
{
}

void CgetVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CgetVersion::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);//
	return true;
}


BEGIN_MESSAGE_MAP(CgetVersion, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CgetVersion::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CgetVersion::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, &CgetVersion::OnBnClickedOk)
END_MESSAGE_MAP()


// CgetVersion 消息处理程序

void CgetVersion::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	 flag =1;
	 ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	 ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);//
}

void CgetVersion::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	 flag =2;
	  ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	 ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(FALSE);//
}

void CgetVersion::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(stationAddr);//
	if(stationAddr =="")
		stationAddr="0";
}
