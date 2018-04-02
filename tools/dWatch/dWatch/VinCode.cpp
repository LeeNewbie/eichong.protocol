// VinCode.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "VinCode.h"


// CVinCode 对话框

IMPLEMENT_DYNAMIC(CVinCode, CDialog)

CVinCode::CVinCode(CWnd* pParent /*=NULL*/)
	: CDialog(CVinCode::IDD, pParent)
{

}

CVinCode::~CVinCode()
{
}

void CVinCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVinCode, CDialog)
	ON_BN_CLICKED(IDOK, &CVinCode::OnBnClickedOk)
END_MESSAGE_MAP()


// CVinCode 消息处理程序

void CVinCode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(vinCode);//
	if(vinCode =="")
		vinCode="0";
}
