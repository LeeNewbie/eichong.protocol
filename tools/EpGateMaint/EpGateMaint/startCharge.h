#pragma once
#include "afxwin.h"


// CstartCharge �Ի���

class CstartCharge : public CDialog
{
	DECLARE_DYNAMIC(CstartCharge)

public:
	CstartCharge(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CstartCharge();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

	CString froneAmt;
	CString  chargeType;
	CString  payType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_comboBox;
	CComboBox m_payMode;

	
};
