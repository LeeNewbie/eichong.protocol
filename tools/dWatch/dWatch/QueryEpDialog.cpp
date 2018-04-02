// QueryEpDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "QueryEpDialog.h"


// CQueryEpDialog 对话框

IMPLEMENT_DYNAMIC(CQueryEpDialog, CDialog)

CQueryEpDialog::CQueryEpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryEpDialog::IDD, pParent)
{

}

CQueryEpDialog::~CQueryEpDialog()
{
}

void CQueryEpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryEpDialog, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CQueryEpDialog::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CQueryEpDialog 消息处理程序

void CQueryEpDialog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}
