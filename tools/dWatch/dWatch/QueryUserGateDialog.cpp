// QueryUserGateDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "QueryUserGateDialog.h"


// CQueryUserGateDialog �Ի���

IMPLEMENT_DYNAMIC(CQueryUserGateDialog, CDialog)

CQueryUserGateDialog::CQueryUserGateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryUserGateDialog::IDD, pParent)
{

}

CQueryUserGateDialog::~CQueryUserGateDialog()
{
}

void CQueryUserGateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryUserGateDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CQueryUserGateDialog::OnBnClickedButton3)
END_MESSAGE_MAP()


// CQueryUserGateDialog ��Ϣ�������

void CQueryUserGateDialog::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
