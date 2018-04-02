
// EpGateMaintDlg.h : 头文件
//


#pragma once


#include <list>
#include "afxwin.h"



#include "QueryDialog.h"

#include "TestEpGate.h"
#include "CtrlUserGateDialog.h"

#include "QueryUserGateDialog.h"
#include "m_space.h"




#include "afxcmn.h"




// CEpGateMaintDlg 对话框
class CEpGateMaintDlg : public CDialog
{
	//DECLARE_EASYSIZE
// 构造
public:
	CEpGateMaintDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EPGATEMAINT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	  void ReadEpGateIni(int i);
      void ReadEpGateIni();
	  void ReadUserGateIni();
	  void ReadRegeditGateIni();
	  void ReadThirdGateIni();
	  void initTree();
	  void setItemData(int serverType,int serverIndex,int ipType,int userType,HTREEITEM item);
	  void showEpGate(int serverType,int ipType,int userType);
	  void showUserGate(int serverType,int ipType,int userType);
	  void showRegeditGate(int serverType,int ipType,int userType);
	  void showThirdGate(int serverType,int ipType,int userType);

// 实现
protected:
	HICON m_hIcon;
    
	virtual void OnOK();

	BOOL PreTranslateMessage(MSG* pMsg);
  
 
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void ScrollClient(int nBar, int nPos) ;
	int GetScrollPos(int nBar, UINT nSBCode)  ;
	void HVScrool(int nBar, UINT nSBCode) ;

	DECLARE_MESSAGE_MAP()
public:


  
public:
//	CString Sign(STRLIST paramList);
//	bool comp(const CString str1, const CString str2);
//	void setUrl(STRLIST paramList,CString cmd);

//	void getEpParam();

public:
 
	
	/*CFrameWnd *m_pMyFrame;  
    CSplitterWnd  m_cSplitter*/;  
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	bool m_IsInitialed;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CTreeCtrl m_tree;
	CTabCtrl m_tab;

	CQueryDialog *m_queryEpGateDialog;
	CTestEpGate *m_ctrlEpGateDialog;

	CQueryUserGateDialog *m_queryUserGateDialog;
	CCtrlUserGateDialog *m_ctrlUserGateDialog;

	m_space *m_pSpace;


	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);


	

	int m_epGateServerNum;
	int m_userGateServerNum;
	int m_regeditGateServerNum;
	int m_thirdGateServerNum;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


};
