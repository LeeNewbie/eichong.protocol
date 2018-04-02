// QueryThirdDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "QueryThirdDialog.h"


// CQueryThirdDialog 对话框

IMPLEMENT_DYNAMIC(CQueryThirdDialog, CDialog)

CQueryThirdDialog::CQueryThirdDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryThirdDialog::IDD, pParent)
{

}

CQueryThirdDialog::~CQueryThirdDialog()
{
}

void CQueryThirdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryThirdDialog, CDialog)
END_MESSAGE_MAP()


// CQueryThirdDialog 消息处理程序
