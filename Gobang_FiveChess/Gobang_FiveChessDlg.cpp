
// Gobang_FiveChessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Gobang_FiveChess.h"
#include "Gobang_FiveChessDlg.h"
#include "afxdialogex.h"

#include "DialogMore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGobang_FiveChessDlg �Ի���
BEGIN_EASYSIZE_MAP(CGobang_FiveChessDlg)
EASYSIZE(IDC_BUTTON_GAME_START,	ES_KEEPSIZE,	ES_BORDER,
        ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_BUTTON_REGRET,	ES_KEEPSIZE,	ES_BORDER,
        ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_BUTTON_MORE,	ES_KEEPSIZE,	ES_BORDER,
        ES_BORDER,ES_KEEPSIZE,0)

// ����Ч��
//EASYSIZE(IDC_BUTTON_GAME_START,	ES_BORDER,	ES_BORDER,
//       ES_BORDER,ES_BORDER,0)
//EASYSIZE(IDC_BUTTON_REGRET,	ES_BORDER,	ES_KEEPSIZE,
//       ES_BORDER,ES_BORDER,0)

END_EASYSIZE_MAP

CGobang_FiveChessDlg::CGobang_FiveChessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGobang_FiveChessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
   
CGobang_FiveChessDlg::~CGobang_FiveChessDlg()
{   
}

void CGobang_FiveChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGobang_FiveChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_GAME_START, &CGobang_FiveChessDlg::OnBnClickedButtonGameStart)
	ON_BN_CLICKED(IDC_BUTTON_REGRET, &CGobang_FiveChessDlg::OnBnClickedButtonRegret)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CGobang_FiveChessDlg::OnBnClickedButtonMore)
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
	
END_MESSAGE_MAP()


// CGobang_FiveChessDlg ��Ϣ�������

BOOL CGobang_FiveChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect   rcClient;

	GetClientRect(&rcClient);
	m_chess.Init(rcClient);

    INIT_EASYSIZE;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGobang_FiveChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGobang_FiveChessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

        CRect   rcBtnStart, rcBtnRegret, rcBtnMore;
        CRgn    rgnChildWnd,    rgnBtnStart,    rgnBtnRegret,    rgnBtnMore;			
		CDC*	pDC	= GetDC();	

        // ��ȡ��ť�ڸ����ڵ�λ��
        GetDlgItem(IDC_BUTTON_GAME_START)->GetWindowRect(rcBtnStart);
        GetDlgItem(IDC_BUTTON_REGRET)->GetWindowRect(rcBtnRegret);
        GetDlgItem(IDC_BUTTON_MORE)->GetWindowRect(rcBtnMore);
        ScreenToClient(rcBtnStart);
        ScreenToClient(rcBtnRegret);
        ScreenToClient(rcBtnMore);

        // ���а�ť��λ��
        rgnBtnStart.CreateRectRgnIndirect(rcBtnStart);
        rgnBtnRegret.CreateRectRgnIndirect(rcBtnRegret);   
        rgnBtnMore.CreateRectRgnIndirect(rcBtnMore);   
        rgnChildWnd.CreateRectRgn(0, 0, 0, 0);
        rgnChildWnd.CombineRgn(&rgnBtnStart, &rgnBtnRegret, RGN_OR);
        rgnChildWnd.CombineRgn(&rgnChildWnd, &rgnBtnMore, RGN_OR);
        pDC->SelectClipRgn(&rgnChildWnd, RGN_DIFF);

        // �����
		m_chess.Draw(pDC);

        // �ƺ�
        pDC->SelectClipRgn(NULL);
        rgnBtnStart.DeleteObject();
        rgnBtnRegret.DeleteObject();
        rgnBtnMore.DeleteObject();
        rgnChildWnd.DeleteObject();
		ReleaseDC(pDC);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGobang_FiveChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGobang_FiveChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_chess.SetPiecePos(point);
	Invalidate();

    switch(m_chess.GetWinFlag())
    {
    case    WHITE_WIN:
        AfxMessageBox(_T("����Ӯ��"));
        break;
    case    BLACK_WIN:
        AfxMessageBox(_T("����Ӯ��"));
        break;
    case    PEACE:
        AfxMessageBox(_T("ƽ�֣�"));
        break;
    default:
        break;
    }

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CGobang_FiveChessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

//	return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}


void CGobang_FiveChessDlg::OnBnClickedButtonGameStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_chess.NewGame();
    Invalidate();
}


void CGobang_FiveChessDlg::OnBnClickedButtonRegret()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    if(! m_chess.Regret())
    {
        AfxMessageBox(_T("�ٻ���Ͳ�������~"));
    }
    Invalidate();
}


void CGobang_FiveChessDlg::OnBnClickedButtonMore()
{
    CDialogMore* dlgMore  = new  CDialogMore;

	dlgMore->Create(IDD_DIALOG_MORE);   	
    dlgMore->ShowWindow(SW_SHOW);       // ��ģ̬
//  dlgMore->DoModal();

    dlgMore->SetChess(&m_chess);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CGobang_FiveChessDlg::OnSize(UINT nType, int cx, int cy)
{       
    CDialogEx::OnSize(nType, cx, cy);   

    CRect	rcClient;
	GetClientRect(&rcClient);

	m_chess.Init(rcClient);

    UPDATE_EASYSIZE;
    Invalidate();
    // TODO: �ڴ˴������Ϣ����������
}


void CGobang_FiveChessDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CRect   rcBoard = m_chess.GetRectBoard();
    if(rcBoard.PtInRect(point))
    {
        HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS); 
        SetCursor(hCursor);	
    }

    CDialogEx::OnMouseMove(nFlags, point);
}
