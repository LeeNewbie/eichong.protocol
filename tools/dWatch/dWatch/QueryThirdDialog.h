#pragma once


// CQueryThirdDialog �Ի���

class CQueryThirdDialog : public CDialog
{
	DECLARE_DYNAMIC(CQueryThirdDialog)

public:
	CQueryThirdDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryThirdDialog();

// �Ի�������
	enum { IDD = IDD_QUERTHIRD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
