// QueryEpDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "QueryEpDialog.h"


// CQueryEpDialog �Ի���

IMPLEMENT_DYNAMIC(CQueryEpDialog, CDialog)

CQueryEpDialog::CQueryEpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryEpDialog::IDD, pParent)
{

}

CQueryEpDialog::~CQueryEpDialog()
{
}

void CQueryEpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryEpDialog, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CQueryEpDialog::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CQueryEpDialog ��Ϣ�������

void CQueryEpDialog::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
