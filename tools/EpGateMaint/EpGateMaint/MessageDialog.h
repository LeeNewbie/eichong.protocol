#pragma once


// MessageDialog �Ի���

class MessageDialog : public CDialog
{
	DECLARE_DYNAMIC(MessageDialog)

public:
	MessageDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MessageDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	
	DECLARE_MESSAGE_MAP()

public:
	CString msg;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
