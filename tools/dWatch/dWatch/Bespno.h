#pragma once
#include "afxwin.h"


// CBespno �Ի���

class CBespno : public CDialog
{
	DECLARE_DYNAMIC(CBespno)

public:
	CBespno(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBespno();

// �Ի�������
	enum { IDD = IDD_BESPNO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	CString redo;
	CString timet;
	afx_msg void OnBnClickedOk();

	int flag;
	CString bespno;
	CString  payType;
	CString pw;

	afx_msg void OnBnClickedButton1();
	CComboBox m_payMode;
	CComboBox m_BespnoTime;
};
