// open_white.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "open_white.h"


// Copen_white 对话框

IMPLEMENT_DYNAMIC(Copen_white, CDialog)

Copen_white::Copen_white(CWnd* pParent /*=NULL*/)
	: CDialog(Copen_white::IDD, pParent)
{

}

Copen_white::~Copen_white()
{
}

void Copen_white::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComBoxOpen);
}


BEGIN_MESSAGE_MAP(Copen_white, CDialog)
	ON_BN_CLICKED(IDOK, &Copen_white::OnBnClickedOk)
END_MESSAGE_MAP()


// Copen_white 消息处理程序


BOOL Copen_white::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComBoxOpen.AddString("使用");
    m_ComBoxOpen.AddString("不使用");
	m_ComBoxOpen.SetCurSel(0);

	
	return true;
}
void Copen_white::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int i=m_ComBoxOpen.GetCurSel();
	if(i==0)
		i=1;
	else
		i=0;

	open.Format("%d",i);
	OnOK();
}
