#pragma once


// CconsumeRecord �Ի���

class CconsumeRecord : public CDialog
{
	DECLARE_DYNAMIC(CconsumeRecord)

public:
	CconsumeRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CconsumeRecord();

		// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString startNum;
	CString entryNum;
};
