// startCharge.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "startCharge.h"


// CstartCharge �Ի���

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

	m_comboBox.AddString("��ά����");
    m_comboBox.AddString("�˺ų��");
	m_comboBox.AddString("�����");
	m_comboBox.SetCurSel(0);

	
	m_payMode.AddString("�ȸ���");
    m_payMode.AddString("�󸶷�");

	m_payMode.SetCurSel(0);

	  ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("200");

	UpdateData(FALSE);
	return true;
}


BEGIN_MESSAGE_MAP(CstartCharge, CDialog)
	ON_BN_CLICKED(IDOK, &CstartCharge::OnBnClickedOk)

END_MESSAGE_MAP()


// CstartCharge ��Ϣ�������

void CstartCharge::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	int i= m_comboBox.GetCurSel();
	chargeType.Format("%d",i+1);
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(froneAmt);//

	i= m_payMode.GetCurSel();
	payType.Format("%d",i+1);
}

