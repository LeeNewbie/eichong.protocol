// AddEpCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "AddEpCode.h"


// CAddEpCode �Ի���

IMPLEMENT_DYNAMIC(CAddEpCode, CDialog)

CAddEpCode::CAddEpCode(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEpCode::IDD, pParent)
{

}

CAddEpCode::~CAddEpCode()
{
}

void CAddEpCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddEpCode, CDialog)
	ON_BN_CLICKED(IDOK, &CAddEpCode::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddEpCode ��Ϣ�������

void CAddEpCode::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	 ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(epcode);
}
