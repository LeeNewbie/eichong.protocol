// Clear.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "Clear.h"

#include "EpGateMaintDlg.h"


// CClear 对话框

IMPLEMENT_DYNAMIC(CClear, CDialog)

CClear::CClear(CWnd* pParent /*=NULL*/)
	: CDialog(CClear::IDD, pParent)
{
   m_dlg = (CEpGateMaintDlg *)pParent;
}

CClear::~CClear()
{
}

void CClear::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClear, CDialog)
	ON_BN_CLICKED(IDC_BUTTON14, &CClear::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON17, &CClear::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON16, &CClear::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON19, &CClear::OnBnClickedButton19)
END_MESSAGE_MAP()


// CClear 消息处理程序

void CClear::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_cleanGun();
}

void CClear::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_cleanBesp();
}

void CClear::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_cleanUser();
}

void CClear::OnBnClickedButton19()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_restoreGun();
}
