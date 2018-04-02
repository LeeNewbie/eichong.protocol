// rate.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "rate.h"


// Crate 对话框

IMPLEMENT_DYNAMIC(Crate, CDialog)

Crate::Crate(CWnd* pParent /*=NULL*/)
	: CDialog(Crate::IDD, pParent)
{
   rateid="0";
}

Crate::~Crate()
{
}

void Crate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Crate, CDialog)
	ON_BN_CLICKED(IDOK, &Crate::OnBnClickedOk)
END_MESSAGE_MAP()


// Crate 消息处理程序

void Crate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(rateid);//
	if(rateid =="")
		rateid="0";
}
