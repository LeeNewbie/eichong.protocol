#pragma once
#include "afxwin.h"



// CmanagerEpcode �Ի���

class CmanagerEpcode : public CDialog
{
	DECLARE_DYNAMIC(CmanagerEpcode)

public:
	CmanagerEpcode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CmanagerEpcode();

// �Ի�������
	enum { IDD = IDD_DIALOG_EP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	CListBox m_epCodeList;

	  CString epCodes;
	  void setParam(CString configName,CString itemName);

	  CString m_configName;
	  CString m_itemName;
};
