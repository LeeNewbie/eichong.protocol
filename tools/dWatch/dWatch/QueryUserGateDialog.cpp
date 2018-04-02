// QueryUserGateDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "QueryUserGateDialog.h"


// CQueryUserGateDialog 对话框

IMPLEMENT_DYNAMIC(CQueryUserGateDialog, CDialog)

CQueryUserGateDialog::CQueryUserGateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryUserGateDialog::IDD, pParent)
{

}

CQueryUserGateDialog::~CQueryUserGateDialog()
{
}

void CQueryUserGateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryUserGateDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CQueryUserGateDialog::OnBnClickedButton3)
END_MESSAGE_MAP()


// CQueryUserGateDialog 消息处理程序

void CQueryUserGateDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
