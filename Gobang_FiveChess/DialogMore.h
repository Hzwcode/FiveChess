#pragma once
#include "afxwin.h"
#include "Chess.h"


// CDialogMore �Ի���

class CDialogMore : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMore)

public:
	CDialogMore(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogMore();

// �Ի�������
	enum { IDD = IDD_DIALOG_MORE };

private:
    CChess*  m_pChess;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CComboBox m_comboxAI;
    CComboBox m_comboxVSMode;
    virtual BOOL OnInitDialog();
    void    SetChess(CChess  *pChess);  // ��������ָ��
};
