// repeatSend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "repeatSend.h"


// CrepeatSend �Ի���

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


// CrepeatSend ��Ϣ�������

void CrepeatSend::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(time);//
	if(time =="")
		time="0";
	int i= m_comboBox.GetCurSel();
	flag.Format("%d",i+1);
}

void CrepeatSend::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
