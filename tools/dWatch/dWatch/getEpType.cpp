// getEpType.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "getEpType.h"


// CgetEpType 对话框

IMPLEMENT_DYNAMIC(CgetEpType, CDialog)

CgetEpType::CgetEpType(CWnd* pParent /*=NULL*/)
	: CDialog(CgetEpType::IDD, pParent)
{

}

CgetEpType::~CgetEpType()
{
}

void CgetEpType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CgetEpType, CDialog)
	ON_BN_CLICKED(IDOK, &CgetEpType::OnBnClickedOk)
END_MESSAGE_MAP()


// CgetEpType 消息处理程序

void CgetEpType::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(type);//
	if(type =="")
		type="0";
}
