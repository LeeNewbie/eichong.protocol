// TempChargeNum.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "TempChargeNum.h"


// CTempChargeNum �Ի���

IMPLEMENT_DYNAMIC(CTempChargeNum, CDialog)

CTempChargeNum::CTempChargeNum(CWnd* pParent /*=NULL*/)
	: CDialog(CTempChargeNum::IDD, pParent)
{
    num="";
	time="";
    flag="";

}

CTempChargeNum::~CTempChargeNum()
{
}

void CTempChargeNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempChargeNum, CDialog)
	ON_BN_CLICKED(IDOK, &CTempChargeNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CTempChargeNum ��Ϣ�������

void CTempChargeNum::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(num);//
	((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(time);//
	((CEdit *)GetDlgItem(IDC_EDIT3))->GetWindowText(flag);//
}
