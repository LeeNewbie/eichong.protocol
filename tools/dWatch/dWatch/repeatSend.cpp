// repeatSend.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "repeatSend.h"


// CrepeatSend 对话框

IMPLEMENT_DYNAMIC(CrepeatSend, CDialog)

CrepeatSend::CrepeatSend(CWnd* pParent /*=NULL*/)
	: CDialog(CrepeatSend::IDD, pParent)
{

}

CrepeatSend::~CrepeatSend()
{
}

void CrepeatSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CrepeatSend, CDialog)
	ON_BN_CLICKED(IDOK, &CrepeatSend::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CrepeatSend::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CrepeatSend 消息处理程序

void CrepeatSend::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(time);//
	if(time =="")
		time="0";
	int i= m_comboBox.GetCurSel();
	flag.Format("%d",i+1);
}

void CrepeatSend::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}
