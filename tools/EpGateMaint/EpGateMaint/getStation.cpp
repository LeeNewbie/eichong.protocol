// getStation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "getStation.h"


// CgetStation �Ի���

IMPLEMENT_DYNAMIC(CgetStation, CDialog)

CgetStation::CgetStation(CWnd* pParent /*=NULL*/)
	: CDialog(CgetStation::IDD, pParent)
{

}

CgetStation::~CgetStation()
{
}

void CgetStation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CgetStation, CDialog)
	ON_BN_CLICKED(IDOK, &CgetStation::OnBnClickedOk)
END_MESSAGE_MAP()


// CgetStation ��Ϣ�������

void CgetStation::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(stationAddr);//
	if(stationAddr =="")
		stationAddr="0";
}
