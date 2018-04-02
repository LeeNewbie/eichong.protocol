// AddEpCode.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "AddEpCode.h"


// CAddEpCode 对话框

IMPLEMENT_DYNAMIC(CAddEpCode, CDialog)

CAddEpCode::CAddEpCode(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEpCode::IDD, pParent)
{

}

CAddEpCode::~CAddEpCode()
{
}

void CAddEpCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddEpCode, CDialog)
	ON_BN_CLICKED(IDOK, &CAddEpCode::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddEpCode 消息处理程序

void CAddEpCode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(epcode);
}
