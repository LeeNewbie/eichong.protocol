#pragma once
class CEpGateMaintDlg;

// CQueryDialog �Ի���

class CQueryDialog : public CDialog
{
	DECLARE_DYNAMIC(CQueryDialog)

public:
	CQueryDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryDialog();


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_VIEW };

	CEpGateMaintDlg * m_dlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton6();
	int state;
    void SetCheckFalse();
	CString sec;
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton29();
};
