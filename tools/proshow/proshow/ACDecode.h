#pragma once


// CACDecode �Ի���

class CACDecode : public CDialog
{
	DECLARE_DYNAMIC(CACDecode)

public:
	CACDecode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CACDecode();

// �Ի�������
	enum { IDD = IDD_DIALOG_AC_DECODE };

	 afx_msg void OnBnClickedcleanmsg(); 
	afx_msg void OnBnClickedcleanpro();
	afx_msg void OnBnClickedparse();


public:
	void ProDecodeShow(CString str);
	void getMsg();
	int  getProVersion();
		


	 unsigned char m_dataMsg[4000];
	 int m_leng;
	 afx_msg void OnBnClickedRadio1();
	 
	 CComboBox m_com;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	
};
