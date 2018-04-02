// MessageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "MessageDialog.h"


// MessageDialog 对话框

IMPLEMENT_DYNAMIC(MessageDialog, CDialog)

MessageDialog::MessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MessageDialog::IDD, pParent)
{

}

MessageDialog::~MessageDialog()
{
}

void MessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MessageDialog, CDialog)
	ON_BN_CLICKED(IDOK, &MessageDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MessageDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// MessageDialog 消息处理程序

void MessageDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void MessageDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
