// getStation.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "getStation.h"


// CgetStation 对话框

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


// CgetStation 消息处理程序

void CgetStation::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(stationAddr);//
	if(stationAddr =="")
		stationAddr="0";
}
