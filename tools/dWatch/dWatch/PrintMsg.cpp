// PrintMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "PrintMsg.h"


// CPrintMsg 对话框

IMPLEMENT_DYNAMIC(CPrintMsg, CDialog)

CPrintMsg::CPrintMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintMsg::IDD, pParent)
{

}

CPrintMsg::~CPrintMsg()
{
}

void CPrintMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_PrintMsg);
}


BEGIN_MESSAGE_MAP(CPrintMsg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPrintMsg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CPrintMsg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CPrintMsg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_PrintMsg.AddString("是");
    m_PrintMsg.AddString("否");
	m_PrintMsg.SetCurSel(0);

	UpdateData(FALSE);
	return true;
}

// CPrintMsg 消息处理程序

void CPrintMsg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CPrintMsg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();

	int i= m_PrintMsg.GetCurSel();
	isPrint.Format("%d",i);
}
