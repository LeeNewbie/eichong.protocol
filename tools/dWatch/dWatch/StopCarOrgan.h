#pragma once
#include "afxwin.h"


// CStopCarOrgan �Ի���

class CStopCarOrgan : public CDialog
{
	DECLARE_DYNAMIC(CStopCarOrgan)

public:
	CStopCarOrgan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStopCarOrgan();

// �Ի�������
	enum { IDD = IDD_STOPCARORGAN };

	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString use;
	CComboBox m_ComboBoxUse;
	CString time;
};
