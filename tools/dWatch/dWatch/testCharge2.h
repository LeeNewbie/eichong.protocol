#pragma once


// CtestCharge2 �Ի���

class CtestCharge2 : public CDialog
{
	DECLARE_DYNAMIC(CtestCharge2)

public:
	CtestCharge2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CtestCharge2();

// �Ի�������
	enum { IDD = IDD_DIALOG13 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString pw;
};
