#pragma once
#include "afxwin.h"


// Copen_white �Ի���

class Copen_white : public CDialog
{
	DECLARE_DYNAMIC(Copen_white)

public:
	Copen_white(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Copen_white();

// �Ի�������
	enum { IDD = IDD_OPEN_WHITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComBoxOpen;
	afx_msg void OnBnClickedOk();

	CString open;
};
