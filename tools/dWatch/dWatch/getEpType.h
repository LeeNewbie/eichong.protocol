#pragma once


// CgetEpType �Ի���

class CgetEpType : public CDialog
{
	DECLARE_DYNAMIC(CgetEpType)

public:
	CgetEpType(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CgetEpType();

// �Ի�������
	enum { IDD = IDD_DIALOG15 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString type;
};
