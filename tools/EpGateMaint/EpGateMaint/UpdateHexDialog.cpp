// UpdateHexDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "UpdateHexDialog.h"

#include "EpGateMaintDlg.h"

// CUpdateHexDialog �Ի���

IMPLEMENT_DYNAMIC(CUpdateHexDialog, CDialog)

CUpdateHexDialog::CUpdateHexDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateHexDialog::IDD, pParent)
{
       m_dlg=(CEpGateMaintDlg*)pParent;
}

CUpdateHexDialog::~CUpdateHexDialog()
{

}

void CUpdateHexDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateHexDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON13, &CUpdateHexDialog::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON4, &CUpdateHexDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CUpdateHexDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON18, &CUpdateHexDialog::OnBnClickedButton18)
END_MESSAGE_MAP()


// CUpdateHexDialog ��Ϣ�������

//void CUpdateHexDialog::getParrentDialog(CEpGateMaintDlg * dlg)
//{
//    m_dlg = dlg;
//} 
void CUpdateHexDialog::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlg->OnBnClickedButton_sendUpdate();
}

void CUpdateHexDialog::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlg->OnBnClickedButton_getVer();
}

void CUpdateHexDialog::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlg->OnBnClickedButton_getBom();
}

void CUpdateHexDialog::OnBnClickedButton18()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlg->OnBnClickedButton_queryVer();
}
