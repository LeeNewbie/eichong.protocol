// stopBespno.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "stopBespno.h"


// CstopBespno �Ի���

IMPLEMENT_DYNAMIC(CstopBespno, CDialog)

CstopBespno::CstopBespno(CWnd* pParent /*=NULL*/)
	: CDialog(CstopBespno::IDD, pParent)
{

}

CstopBespno::~CstopBespno()
{
}

void CstopBespno::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CstopBespno, CDialog)
	ON_BN_CLICKED(IDOK, &CstopBespno::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CstopBespno::OnBnClickedButton1)
END_MESSAGE_MAP()


// CstopBespno ��Ϣ�������

void CstopBespno::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(bespno);//

}

void CstopBespno::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	   time_t t;
	   t=time(NULL);
	   srand((unsigned) t);
	   int k = rand() % (99 - 10 + 1) +10;

	   CString bespno,sk;
	   bespno.Format("%d",t);
	   sk.Format("%d",k);

       ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(bespno+sk);//

}
