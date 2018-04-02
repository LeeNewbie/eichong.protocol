// startCharge.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "startCharge.h"


// CstartCharge 对话框

IMPLEMENT_DYNAMIC(CstartCharge, CDialog)

CstartCharge::CstartCharge(CWnd* pParent /*=NULL*/)
	: CDialog(CstartCharge::IDD, pParent)
{

}

CstartCharge::~CstartCharge()
{
}

void CstartCharge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox);
	DDX_Control(pDX, IDC_COMBO1, m_payMode);
}


BOOL CstartCharge::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboBox.AddString("二维码充电");
    m_comboBox.AddString("账号充电");
	m_comboBox.AddString("卡充电");
	m_comboBox.SetCurSel(0);

	
	m_payMode.AddString("先付费");
    m_payMode.AddString("后付费");

	m_payMode.SetCurSel(0);

	  ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("200");

	UpdateData(FALSE);
	return true;
}


BEGIN_MESSAGE_MAP(CstartCharge, CDialog)
	ON_BN_CLICKED(IDOK, &CstartCharge::OnBnClickedOk)

END_MESSAGE_MAP()


// CstartCharge 消息处理程序

void CstartCharge::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	int i= m_comboBox.GetCurSel();
	chargeType.Format("%d",i+1);
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(froneAmt);//

	i= m_payMode.GetCurSel();
	payType.Format("%d",i+1);
}

