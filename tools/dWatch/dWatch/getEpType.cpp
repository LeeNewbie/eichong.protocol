// getEpType.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "getEpType.h"


// CgetEpType �Ի���

IMPLEMENT_DYNAMIC(CgetEpType, CDialog)

CgetEpType::CgetEpType(CWnd* pParent /*=NULL*/)
	: CDialog(CgetEpType::IDD, pParent)
{

}

CgetEpType::~CgetEpType()
{
}

void CgetEpType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CgetEpType, CDialog)
	ON_BN_CLICKED(IDOK, &CgetEpType::OnBnClickedOk)
END_MESSAGE_MAP()


// CgetEpType ��Ϣ�������

void CgetEpType::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(type);//
	if(type =="")
		type="0";
}
