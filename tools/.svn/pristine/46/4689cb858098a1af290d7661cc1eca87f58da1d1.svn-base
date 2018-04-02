// StopCarOrgan.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "StopCarOrgan.h"


// CStopCarOrgan 对话框

IMPLEMENT_DYNAMIC(CStopCarOrgan, CDialog)

CStopCarOrgan::CStopCarOrgan(CWnd* pParent /*=NULL*/)
	: CDialog(CStopCarOrgan::IDD, pParent)
{

}

CStopCarOrgan::~CStopCarOrgan()
{
}

void CStopCarOrgan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxUse);
}


BEGIN_MESSAGE_MAP(CStopCarOrgan, CDialog)
	ON_BN_CLICKED(IDOK, &CStopCarOrgan::OnBnClickedOk)
END_MESSAGE_MAP()


// CStopCarOrgan 消息处理程序



BOOL CStopCarOrgan::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComboBoxUse.AddString("送");
    m_ComboBoxUse.AddString("不送");
	m_ComboBoxUse.SetCurSel(0);

	 ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("60");
	
	return true;
}



void CStopCarOrgan::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();

	int i=m_ComboBoxUse.GetCurSel();
	use.Format("%d",i);
	
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(time);
	if(time=="")
		time="0";
}
