#pragma once


// Crate �Ի���

class Crate : public CDialog
{
	DECLARE_DYNAMIC(Crate)

public:
	Crate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Crate();

// �Ի�������
	enum { IDD = IDD_RATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	
public:
	afx_msg void OnBnClickedOk();

	CString rateid;
};
