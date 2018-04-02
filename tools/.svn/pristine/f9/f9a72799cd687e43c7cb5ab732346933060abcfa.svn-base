// Other.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "Other.h"


#include "EpGateMaintDlg.h"



// COther 对话框

IMPLEMENT_DYNAMIC(COther, CDialog)

COther::COther(CWnd* pParent /*=NULL*/)
	: CDialog(COther::IDD, pParent)
{
    m_dlg = (CEpGateMaintDlg *)pParent;
}

COther::~COther()
{
}

void COther::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COther, CDialog)
	ON_BN_CLICKED(IDC_BUTTON15, &COther::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON27, &COther::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON26, &COther::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON29, &COther::OnBnClickedButton29)
	ON_BN_CLICKED(IDC_BUTTON30, &COther::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON31, &COther::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON1, &COther::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COther::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &COther::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &COther::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON32, &COther::OnBnClickedButton32)
END_MESSAGE_MAP()


// COther 消息处理程序

void COther::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_connetMonitor();
}

void COther::OnBnClickedButton27()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_queryCommSignal();

}

void COther::OnBnClickedButton26()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_queryConsumeRecord();
}

void COther::OnBnClickedButton29()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_addWhite();
}

void COther::OnBnClickedButton30()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_setWhite();
}

void COther::OnBnClickedButton31()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_removeWhite();
}

void COther::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_setStopCarOrgan();
}

void COther::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_queryStationEp();
}

void COther::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_queryRate();
}

void COther::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_queryflashRam();
}

void COther::OnBnClickedButton32()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_repeatSend();
}
