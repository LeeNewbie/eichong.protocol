// MessageDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "MessageDialog.h"


// MessageDialog �Ի���

IMPLEMENT_DYNAMIC(MessageDialog, CDialog)

MessageDialog::MessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MessageDialog::IDD, pParent)
{

}

MessageDialog::~MessageDialog()
{
}

void MessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MessageDialog, CDialog)
	ON_BN_CLICKED(IDOK, &MessageDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MessageDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// MessageDialog ��Ϣ�������

void MessageDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void MessageDialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
