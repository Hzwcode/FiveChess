#include "StdAfx.h"
#include "ChessDraw.h"
#include "FaceFunc.h"

CChessDraw::CChessDraw()
{
	m_pDC = NULL;
	m_rcBK.SetRectEmpty();
	m_rcBoard.SetRectEmpty();
	
	//m_crBKBegin	= RGB(255,232,166);
	//m_crBKEnd		= RGB(255,220,200);
	m_crBKBegin	= RGB(36, 178, 255);	//����ɫ������ɫ��ʼ��
	m_crBKEnd	= RGB(97, 255, 163);	//ǳ��ɫ������ɫ��β��
	//m_crBKBegin	= RGB(177, 96, 53);
	//m_crBKEnd		= RGB(188, 103, 56);
	m_crBoard		= RGB(0,0,0);			//��ɫ��������������ɫ

	m_uiPieceRadius	= 0;		//���Ӱ뾶
	m_uiBoardRows	= 0;		//��������
	m_uiBoardCols	= 0;		//��������
	m_uiBoardWidth	= 0;		//ÿһ�����̵ı߳�(�����θ���)
}

CChessDraw::~CChessDraw()
{
	// ������ô������Ϊָ������ⲿָ�룬��Ҫ���ⲿָ�����κθ���
	// m_pDC->DeleteDC();
	// m_pDC = NULL;	
}

void CChessDraw::InitBoard(CRect rect, UINT	uiRows, UINT uiCols, UINT uiBoardWidth, UINT uiPieceRadius)
{
	m_rcBK = rect;
	m_uiBoardRows	= uiRows;
	m_uiBoardCols	= uiCols;
	m_uiBoardWidth	= uiBoardWidth;
	m_uiPieceRadius	= uiPieceRadius;

	// ������������
	m_rcBoard = m_rcBK;
	m_rcBoard.DeflateRect(CSize(20,20));	// ������������ϵ�ƫ��(20,20)�ٻ�����
	m_rcBoard.right	 = m_rcBoard.left + m_uiBoardWidth * (m_uiBoardCols - 1);
	m_rcBoard.bottom = m_rcBoard.top  + m_uiBoardWidth * (m_uiBoardRows - 1);
}

CRect CChessDraw::GetRectBoard()
{
	return m_rcBoard;
}
	
void CChessDraw::SetDC(CDC*	pDC)
{
    m_pDC = pDC;
}

void CChessDraw::DrawBackground()
{
    if(!m_pDC)
        return;
    GradientFillRect(m_pDC,	m_rcBK,	m_crBKBegin, m_crBKEnd,	TRUE);
}

void CChessDraw::DrawBoard()
{
	if(!m_pDC)
		return;
	// ������
	UINT ptTop = m_rcBoard.top;
	for(UINT i = 0;	i < m_uiBoardRows; i++)
	{
		m_pDC->MoveTo(m_rcBoard.left,  ptTop);
		m_pDC->LineTo(m_rcBoard.right, ptTop);
		ptTop += m_uiBoardWidth;
	}

	// ������
	UINT ptLeft	= m_rcBoard.left;
	for(UINT i = 0;	i < m_uiBoardCols; i++)
	{
		m_pDC->MoveTo(ptLeft, m_rcBoard.top);
		m_pDC->LineTo(ptLeft, m_rcBoard.bottom);
		ptLeft += m_uiBoardWidth;
	}

	// ����Ԫ�ȵ�
	DrawSpecialPoints();
}

BOOL CChessDraw::DrawPiece(CPoint pt, BOOL bBlack)
{
	if(!m_pDC || !GetCoordinateWithPoint(pt))
		return	FALSE;

	CRect rc(pt - CSize(m_uiPieceRadius, m_uiPieceRadius), pt + CSize(m_uiPieceRadius, m_uiPieceRadius));
	CRgn  rgn;

	rgn.CreateEllipticRgnIndirect(rc);
	m_pDC->SelectClipRgn(&rgn, RGN_AND);

	if(bBlack)
		FillGradientEx(m_pDC, rc, RGB(140,130,120),	RGB(0,0,0),	FALSE);             // ������
		// GradientFillRect(m_pDC, rc, RGB(0,0,255), RGB(180,180,180), TRUE);		// �˺����е�����(�����д������к󣬿��Կ�����)
	else
		FillGradientEx(m_pDC, rc, RGB(255,255,255),	RGB(180,200,180), FALSE);		// ������

	m_pDC->SelectClipRgn(NULL);
	return	TRUE;
}
	
BOOL CChessDraw::DrawPiece(UINT	uiX, UINT uiY, BOOL	bBlack)
{
	CPoint	pt;
	if(!m_pDC || !GetPointWithCoordinate(pt, uiX, uiY))
		return	FALSE;

	CRect rc(pt - CSize(m_uiPieceRadius, m_uiPieceRadius),	pt + CSize(m_uiPieceRadius,	m_uiPieceRadius));
	CRgn  rgn;

	rgn.CreateEllipticRgnIndirect(rc);
	m_pDC->SelectClipRgn(&rgn, RGN_AND);

	if(bBlack)
		FillGradientEx(m_pDC, rc, RGB(140,130,120),	RGB(0,0,0),	FALSE);
	else
		FillGradientEx(m_pDC, rc, RGB(255,255,255),	RGB(180,200,180), FALSE);

	m_pDC->SelectClipRgn(NULL);
	return	TRUE;
}

void CChessDraw::DrawPieceCur(CPoint pt)
{
	if(!m_pDC || !GetCoordinateWithPoint(pt))
		return;

	CPen  penNew(PS_DASH, 1, RGB(255,0,0));
	CPen* penOld = m_pDC->SelectObject(&penNew);

	//int iSize	= 5;
	int	iSize = m_uiPieceRadius/3;
	// ������
	m_pDC->MoveTo(pt.x - iSize, pt.y);
	m_pDC->LineTo(pt.x + iSize, pt.y);
	// ������
	m_pDC->MoveTo(pt.x,	pt.y - iSize);
	m_pDC->LineTo(pt.x,	pt.y + iSize);

	m_pDC->SelectObject(penOld);
}

void CChessDraw::DrawPieceCur(UINT uiX, UINT uiY)
{
	CPoint pt;
	if(!m_pDC || !GetPointWithCoordinate(pt, uiX, uiY))
		return	;

	CPen	penNew(PS_DASH, 1, RGB(255,0,0));
	CPen*	penOld	= m_pDC->SelectObject(&penNew);

	int	iSize	= m_uiPieceRadius/2 - 1;
	// ������
	m_pDC->MoveTo(pt.x - iSize, pt.y);
	m_pDC->LineTo(pt.x + iSize, pt.y);
	// ������
	m_pDC->MoveTo(pt.x,	pt.y - iSize);
	m_pDC->LineTo(pt.x,	pt.y + iSize);

	m_pDC->SelectObject(penOld);
}

void CChessDraw::DrawSpecialPoints()
{
	if(! m_pDC)
		return;

	CRect	 rcTmp;
	CRect	 rc	= m_rcBoard;
	COLORREF rgb = RGB(100,100,100);						//������Ԫ�����ɫΪ��ɫ
	UINT	 uiSpecialRadius = m_uiPieceRadius/2 - 1;		// ��Ԫ�ȵ�İ뾶(Ĭ��Ϊ�����Σ���ֵ��ʾ�߳���һ��)
	CSize	 szRadius(uiSpecialRadius, uiSpecialRadius);
	const int SPACE	= 3 * m_uiBoardWidth;				    // ƫ��3��

	rc.DeflateRect(CSize(SPACE,	SPACE));

	rcTmp = CRect(rc.TopLeft(), rc.TopLeft());				//���Ͻ���Ԫ��
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,	rgb);

	rcTmp = CRect(rc.BottomRight(), rc.BottomRight());		//���½���Ԫ��
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,	rgb);

	rcTmp = CRect(CPoint(rc.right, rc.top),	CPoint(rc.right, rc.top));		//���Ͻ���Ԫ��
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,	rgb);

	rcTmp = CRect(CPoint(rc.left, rc.bottom), CPoint(rc.left, rc.bottom));	//���½���Ԫ��
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,	rgb);

	rcTmp = CRect(rc.CenterPoint(),	rc.CenterPoint());		//������Ԫ��
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,	rgb);
}

BOOL	CChessDraw::GetCoordinateWithPoint(CPoint& pt, UINT* puiX, UINT* puiY)
{
	// ����������ȡ����ʹ��պ����������洦
	CRect rcTmp	= m_rcBoard;

	rcTmp.InflateRect(CSize(m_uiBoardWidth/2, m_uiBoardWidth/2));
	if(!rcTmp.PtInRect(pt))
		return	FALSE;

	//UINT	uiPosX	= (pt.x - m_rcBoard.left + m_uiBoardWidth/2)/m_uiBoardWidth;			// �ڵ�uiPosX�������
	//UINT	uiPosY	= (pt.y - m_rcBoard.top + m_uiBoardWidth/2) /m_uiBoardWidth;
	UINT uiPosX	= (pt.x - rcTmp.left)/m_uiBoardWidth;			// �ڵ�uiPosX�������
	UINT uiPosY	= (pt.y - rcTmp.top) /m_uiBoardWidth;

	pt.x = m_rcBoard.left + m_uiBoardWidth * uiPosX;
	pt.y = m_rcBoard.top  + m_uiBoardWidth * uiPosY;

	if(puiX)
		*puiX = uiPosX;
	if(puiY)
		*puiY = uiPosY;

	return	TRUE;
}
	
BOOL CChessDraw::GetPointWithCoordinate(CPoint&	pt,	UINT uiX, UINT uiY)
{
	if((uiX	 < 0) || (uiX >= m_uiBoardCols)	|| (uiY	 < 0) || (uiY >= m_uiBoardRows))
		return	FALSE;

	pt.x = m_rcBoard.left + m_uiBoardWidth * uiX;
	pt.y = m_rcBoard.top  + m_uiBoardWidth * uiY;

	return	TRUE;
}
