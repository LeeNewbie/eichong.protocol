// CallEp.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "CallEp.h"


// CCallEp 对话框

IMPLEMENT_DYNAMIC(CCallEp, CDialog)

CCallEp::CCallEp(CWnd* pParent /*=NULL*/)
	: CDialog(CCallEp::IDD, pParent)
{

}

CCallEp::~CCallEp()
{
}

void CCallEp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
}

BOOL CCallEp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboBox.AddString("LED灯");
    m_comboBox.AddString("声音");
	m_comboBox.AddString("LED和声音");
	m_comboBox.SetCurSel(0);

	UpdateData(FALSE);
	return true;
}


BEGIN_MESSAGE_MAP(CCallEp, CDialog)
	ON_BN_CLICKED(IDOK, &CCallEp::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCallEp::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CCallEp 消息处理程序

void CCallEp::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	int i= m_comboBox.GetCurSel();
	type.Format("%d",i+1);
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(time);//
	if(time =="")
		time="0";
}

void CCallEp::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}
