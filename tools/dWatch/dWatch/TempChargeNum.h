#pragma once


// CTempChargeNum �Ի���

class CTempChargeNum : public CDialog
{
	DECLARE_DYNAMIC(CTempChargeNum)

public:
	CTempChargeNum(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTempChargeNum();

// �Ի�������
	enum { IDD = IDD_DIALOG17 };


	CString num;
	CString time;
	CString flag;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
