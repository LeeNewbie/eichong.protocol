#pragma once

#include "Config.h"
#include "afxwin.h"



class CQueryDialog : public CDialog
{
	DECLARE_EASYSIZE
	DECLARE_DYNAMIC(CQueryDialog)

public:
	CQueryDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryDialog();


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();


	CString Sign(STRLIST paramList);
	bool comp(const CString str1, const CString str2);
	void setUrl(STRLIST paramList,CString cmd);
	void getEpParam();

// 对话框数据
	enum { IDD = IDD_VIEW };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedCheck1();



	int state;
    void SetCheckFalse();
    void setParam(CString ip,CString port,CString accounts,CString epCodes,CString sgunNo,CString suser,CString skey,CString configName,int index,int type);
	void initUserComBox();
	void initEpCodeComBox();
    void Pscp(CString pw,CString sp,CString filename);


	CString sec;
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);


	CString m_epCodes;
    CString gunNo;
	CString m_accounts;
	CString curIp;
	CString curPort;

	CString   urlValue;
	CString   user;
	CString    key;

    CComboBox *m_epCodeComboBox;
	CComboBox *m_userComboBox;

	CString epCode;
	CString account;
	
	CString cmdstr;
	
	afx_msg void OnBnClickedButtonEp();
	
	CString m_configName;
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo2();
	
	afx_msg void OnBnClickedButtonEp2();
	

	CTabCtrl *m_tab;
	//CCtrlEpDialog *m_ctrlEpDialog;
	int m_index;
	int m_type;


	void OnBnClickedButton_restoreGun();
	void OnBnClickedButton_clearUsr();
	void OnBnClickedButton_clearBesp();
	void OnBnClickedButton_clearCharge();
	void OnBnClickedButton_removeWhite();
	void OnBnClickedButton_setStop();
	void OnBnClickedButton_setWhite();
	void OnBnClickedButton_config();
	void OnBnClickedButton_addWhite();
	void OnBnClickedButton_reconnect();
	void OnBnClickedButton_sendVersion();
	void OnBnClickedButton_forceUpdate();
	void OnBnClickedButton_epEndBesp();
	void OnBnClickedButton_epStartBesp();
	void OnBnClickedButton_epEndCharge();
	void OnBnClickedButton_epStartCharge();
	void OnBnClickedButton_configCentor();
	void OnBnClickedButton_createIdenty();
	void OnBnClickedButton_sendRate();
	void OnBnClickedButton_drop();
	void OnBnClickedButton_callEp();
	void OnBnClickedButton_endBesp();
	void OnBnClickedButton_startBesp();
	void OnBnClickedButton_endCharge();
	void OnBnClickedButton_startCharge();

	afx_msg void OnBnClickedButton_getGameConfig();
	afx_msg void OnBnClickedButton_sendQueryRate();
	afx_msg void OnBnClickedButton_sendFlash();
	afx_msg void OnBnClickedButton_sendCentorConfig();
	afx_msg void OnBnClickedButton_sendEp();
	afx_msg void OnBnClickedButton_sendComm();
	afx_msg void OnBnClickedButton_getVersion();
	afx_msg void OnBnClickedButton_getBom();
	afx_msg void OnBnClickedButton_getChannel();
	afx_msg void OnBnClickedButton_getLastRecord();
	afx_msg void OnBnClickedButton_getReal();
	afx_msg void OnBnClickedButton_getCentor();
	afx_msg void OnBnClickedButton_getEp();
	afx_msg void OnBnClickedButton_getMonitor();
	afx_msg void OnBnClickedButton_getStat();
	afx_msg void OnBnClickedButton_getUser();
	afx_msg void OnBnClickedButton_getGun();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	

	afx_msg void OnBnClickedButton_getThirdstat();
	afx_msg void OnBnClickedButton_usrGate();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
};
