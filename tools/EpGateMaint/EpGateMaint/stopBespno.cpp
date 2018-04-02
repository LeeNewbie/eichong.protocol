// stopBespno.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "stopBespno.h"


// CstopBespno 对话框

IMPLEMENT_DYNAMIC(CstopBespno, CDialog)

CstopBespno::CstopBespno(CWnd* pParent /*=NULL*/)
	: CDialog(CstopBespno::IDD, pParent)
{

}

CstopBespno::~CstopBespno()
{
}

void CstopBespno::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CstopBespno, CDialog)
	ON_BN_CLICKED(IDOK, &CstopBespno::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CstopBespno::OnBnClickedButton1)
END_MESSAGE_MAP()


// CstopBespno 消息处理程序

void CstopBespno::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(bespno);//

}

void CstopBespno::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	   time_t t;
	   t=time(NULL);
	   srand((unsigned) t);
	   int k = rand() % (99 - 10 + 1) +10;

	   CString bespno,sk;
	   bespno.Format("%d",t);
	   sk.Format("%d",k);

       ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(bespno+sk);//

}
