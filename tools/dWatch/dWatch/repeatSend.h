#pragma once
#include "afxwin.h"


// CrepeatSend �Ի���

class CrepeatSend : public CDialog
{
	DECLARE_DYNAMIC(CrepeatSend)

public:
	CrepeatSend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CrepeatSend();

// �Ի�������
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_comboBox;
	CString time;
	CString flag;
	afx_msg void OnCbnSelchangeCombo1();
};
