// consumeRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "consumeRecord.h"


// CconsumeRecord 对话框

IMPLEMENT_DYNAMIC(CconsumeRecord, CDialog)

CconsumeRecord::CconsumeRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CconsumeRecord::IDD, pParent)
{

}

CconsumeRecord::~CconsumeRecord()
{
}

void CconsumeRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CconsumeRecord, CDialog)
	ON_BN_CLICKED(IDOK, &CconsumeRecord::OnBnClickedOk)
END_MESSAGE_MAP()



BOOL CconsumeRecord::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	  ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("0");
	  ((CEdit *)GetDlgItem(IDC_EDIT9))->SetWindowText("0");

	UpdateData(FALSE);
	return true;
}

// CconsumeRecord 消息处理程序

void CconsumeRecord::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();

	 ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(startNum);//
	if(startNum =="")
		startNum="0";

	 ((CEdit *)GetDlgItem(IDC_EDIT9))->GetWindowText(startNum);//
	if(entryNum =="")
		entryNum="0";

	
}
