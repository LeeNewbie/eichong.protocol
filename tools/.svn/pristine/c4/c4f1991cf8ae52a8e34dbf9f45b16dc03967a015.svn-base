#pragma once

#include "Config.h"



// CTestEpGate 对话框

class CTestEpGate : public CDialog
{
	DECLARE_EASYSIZE
	DECLARE_DYNAMIC(CTestEpGate)

public:
	CTestEpGate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestEpGate();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	
// 对话框数据
	enum { IDD = IDD_TESTEPGATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton33();
	afx_msg void OnBnClickedButton36();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton35();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedButton19();

	afx_msg void OnBnClickedButtonEp();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButtonEp2();


	CString m_epCodes;
    CString gunNo;
	CString m_accounts;
	CString curIp;
	CString curPort;

	CString   urlValue;
	CString   user;
	CString    key;

	int m_index;
	int m_type;

    CComboBox *m_epCodeComboBox;
	CComboBox *m_userComboBox;

	CString epCode;
	CString account;
	CString psword;
	CString pswordMd5;
	CString cmdstr;
    
	CString m_configName;

	void setParam(CString ip,CString port,CString accounts,CString epCodes,CString sgunNo,CString suser,CString skey,CString configName,int index,int type);
	void initUserComBox();
	void initEpCodeComBox();
	CString Sign(STRLIST paramList);
	bool comp(const CString str1, const CString str2);
	void setUrl(STRLIST paramList,CString cmd);
	void getEpParam();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton4();
	CString calcMd5(CString src);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton39();
};
