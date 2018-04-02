// QueryDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "QueryDialog.h"
#include "EpGateMaintDlg.h"


// CQueryDialog 对话框

IMPLEMENT_DYNAMIC(CQueryDialog, CDialog)

CQueryDialog::CQueryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryDialog::IDD, pParent)
{
       m_dlg = (CEpGateMaintDlg *)pParent;
}

CQueryDialog::~CQueryDialog()
{
}

void CQueryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CQueryDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CQueryDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CQueryDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON25, &CQueryDialog::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON24, &CQueryDialog::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON4, &CQueryDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CQueryDialog::OnBnClickedButton5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CQueryDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON6, &CQueryDialog::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON26, &CQueryDialog::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON29, &CQueryDialog::OnBnClickedButton29)
END_MESSAGE_MAP()


// CQueryDialog 消息处理程序

void CQueryDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_getGun();
}

void CQueryDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_getUser();
}

void CQueryDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_stat();
}

void CQueryDialog::OnBnClickedButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CQueryDialog::OnBnClickedButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_monitorStat();
}

void CQueryDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_getEp();
}

void CQueryDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_getStation();
}




void CQueryDialog::OnTimer(UINT nIDEvent)
{
	
    // TODO: Add your message handler code here and/or call default
    switch(nIDEvent) {
    case 1:
        {   
			CWnd *pWnd= GetFocus();
			if(pWnd == NULL)
				break;
            int focusID=pWnd ->GetDlgCtrlID(); 
            switch(focusID)
			{
			case  IDC_BUTTON1:
				{
					OnBnClickedButton1();
				}
				break;
			default:
                break;
			}
        }
		break;
    default:
        break;
    }
   
    CDialog::OnTimer(nIDEvent);
}

BOOL CQueryDialog::OnInitDialog()
{
  //  SetTimer(1,10000,0);
	((CEdit *)GetDlgItem(IDC_EDIT2))->SetWindowText("1");//
	state=0;
	return true;
}
void CQueryDialog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK1 ) )
    {
		((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(sec);//

		int iSec=atoi(sec);
         // 勾选
		state=1;
		m_dlg->SetTimer(1,iSec*1000,0);
    }
    else
    {
		state=0;
		m_dlg->KillTimer(1);
    }
}

void CQueryDialog::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
    m_dlg->OnBnClickedButton_getReal();
}

void CQueryDialog::SetCheckFalse()
{
   ((CButton*)(GetDlgItem( IDC_CHECK1 )))->SetCheck( 0 ) ;
}
void CQueryDialog::OnBnClickedButton26()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlg->OnBnClickedButton_getLastConsumeRecord();
}

void CQueryDialog::OnBnClickedButton29()
{
	// TODO: 在此添加控件通知处理程序代码
	 m_dlg->OnBnClickedButton_getChNum();
}
