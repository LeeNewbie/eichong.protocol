// TestEpDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "TestEpDialog.h"
#include "EpGateMaintDlg.h"




// CTestEpDialog 对话框

IMPLEMENT_DYNAMIC(CTestEpDialog, CDialog)

CTestEpDialog::CTestEpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEpDialog::IDD, pParent)
{
     m_dlg = (CEpGateMaintDlg *)pParent;
}

CTestEpDialog::~CTestEpDialog()
{
}

void CTestEpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestEpDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON20, &CTestEpDialog::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CTestEpDialog::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CTestEpDialog::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &CTestEpDialog::OnBnClickedButton23)
END_MESSAGE_MAP()


// CTestEpDialog 消息处理程序

void CTestEpDialog::OnBnClickedButton20()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_startCharge_ep();
}

void CTestEpDialog::OnBnClickedButton21()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_endCharge_ep();
}

void CTestEpDialog::OnBnClickedButton22()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_besp_ep();
}

void CTestEpDialog::OnBnClickedButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_endBesp_ep();
}
