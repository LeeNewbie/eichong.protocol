// testCharge2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "testCharge2.h"


// CtestCharge2 �Ի���

IMPLEMENT_DYNAMIC(CtestCharge2, CDialog)

CtestCharge2::CtestCharge2(CWnd* pParent /*=NULL*/)
	: CDialog(CtestCharge2::IDD, pParent)
{

}

CtestCharge2::~CtestCharge2()
{
}

void CtestCharge2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CtestCharge2, CDialog)
	ON_BN_CLICKED(IDOK, &CtestCharge2::OnBnClickedOk)
END_MESSAGE_MAP()


// CtestCharge2 ��Ϣ�������

void CtestCharge2::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
    ((CEdit *)GetDlgItem(IDC_EDIT9))->GetWindowText(pw);//
}
