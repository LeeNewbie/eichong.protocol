// flashram.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "flashram.h"


// Cflashram 对话框

IMPLEMENT_DYNAMIC(Cflashram, CDialog)

Cflashram::Cflashram(CWnd* pParent /*=NULL*/)
	: CDialog(Cflashram::IDD, pParent)
{

}

Cflashram::~Cflashram()
{
}

void Cflashram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComBoxType);
}


BEGIN_MESSAGE_MAP(Cflashram, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cflashram::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &Cflashram::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &Cflashram::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Cflashram::OnBnClickedRadio2)
END_MESSAGE_MAP()


BOOL Cflashram::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComBoxType.AddString("外部flash");
    m_ComBoxType.AddString("内部flash");
	m_ComBoxType.AddString("ram");
	m_ComBoxType.SetCurSel(0);

    OnBnClickedRadio1();
	return true;
}

// Cflashram 消息处理程序

void Cflashram::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void Cflashram::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	int i=m_ComBoxType.GetCurSel();
	 type.Format("%d",(i+1));

	 if(flag=="1")
	 {
		 ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(stationAddr);
		 
	 }
	 else
        stationAddr="0";
	  ((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(startPos);
	  ((CEdit *)GetDlgItem(IDC_EDIT3))->GetWindowText(len);
}

void Cflashram::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	 flag="0";
	  ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	  ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	  ((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);
}

void Cflashram::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	 flag="1";
	  ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	  ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	   ((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(FALSE);
}
