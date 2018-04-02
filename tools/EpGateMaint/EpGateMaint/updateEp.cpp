// updateEp.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "updateEp.h"


// CupdateEp 对话框

IMPLEMENT_DYNAMIC(CupdateEp, CDialog)

CupdateEp::CupdateEp(CWnd* pParent /*=NULL*/)
	: CDialog(CupdateEp::IDD, pParent)
{
    flag=1;
	CString hardnumber="ICHMC-101";
	CString hardversion="1.6";
	CString softnumber="MC11BA";
	CString softversion="2.0.13";
	CString md5="5afbdda2edf728dda49c44dd8a42be89";
	station="0";
}

CupdateEp::~CupdateEp()
{
}

void CupdateEp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CupdateEp::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	 ((CEdit *)GetDlgItem(IDC_EDIT8))->SetReadOnly(TRUE);//
	
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("ICHMC-101");//
	 ((CEdit *)GetDlgItem(IDC_EDIT2))->SetWindowText("1.6");//
	
	 ((CEdit *)GetDlgItem(IDC_EDIT3))->SetWindowText("MC11BA");//
	 ((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("2.0.13");//
//	 update(TRUE);
	return true;
}


BEGIN_MESSAGE_MAP(CupdateEp, CDialog)
	ON_BN_CLICKED(IDOK, &CupdateEp::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CupdateEp::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CupdateEp::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CupdateEp 消息处理程序

void CupdateEp::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(hardnumber);//
	if(hardnumber =="")
		hardnumber="0";
	((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(hardversion);//
	if(hardversion =="")
		hardversion="0";
	((CEdit *)GetDlgItem(IDC_EDIT3))->GetWindowText(softnumber);//
	if(softnumber =="")
		softnumber="0";
	((CEdit *)GetDlgItem(IDC_EDIT4))->GetWindowText(softversion);//
	if(softversion =="")
		softversion="0";
	((CEdit *)GetDlgItem(IDC_EDIT5))->GetWindowText(md5);//
	if(md5 =="")
		md5="0";
    ((CEdit *)GetDlgItem(IDC_EDIT8))->GetWindowText(station);//
	if(station =="")
		station="0";
}

void CupdateEp::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = 1;
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetReadOnly(TRUE);//

}

void CupdateEp::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	flag =2;
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetReadOnly(FALSE);//
}
