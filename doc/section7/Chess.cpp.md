### Chess.cpp

```cpp
#include "StdAfx.h"
#include "Chess.h"
#include "MyMemDC.h"

CChess::CChess(void) {
    m_bTurnBlack        = TRUE;
    m_emWin             = FIGHTING;
    m_emVSMode          = PERSON_VS_MACHINE;
    m_emAIDepth         = AI_MIDDLE;
    m_ptCurrent         = CPoint(-1,-1);    // 只要不在棋盘内的点都可以
    memset(m_iPositionPiece, 0,	sizeof(m_iPositionPiece));

    m_stcLastPos.iPieceNum          = 0;
    m_stcLastPos.ptBlack            = CPoint(0,0);
    m_stcLastPos.ptWhite            = CPoint(0,0);
    m_stcLastPos.ptLastCurPoint     = CPoint(-1,-1);
}

CChess::~CChess(void) {
    NewGame();
}

void CChess::NewGame() {
    m_bTurnBlack    = TRUE;
    m_emWin         = FIGHTING;
    m_ptCurrent     = CPoint(-1,-1);
    memset(m_iPositionPiece, 0,	sizeof(m_iPositionPiece));

    m_stcLastPos.iPieceNum          = 0;
    m_stcLastPos.ptBlack            = CPoint(0,0);
    m_stcLastPos.ptWhite            = CPoint(0,0);
    m_stcLastPos.ptLastCurPoint     = CPoint(-1,-1);

    if(MACHINE_VS_MACHINE == m_emVSMode) {
        CPoint  ptFirst;

        srand(time(NULL));
        ptFirst.x = rand()%COLUMNS;
        ptFirst.y = rand()%ROWS;

        m_chessdraw.GetPointWithCoordinate(ptFirst, ptFirst.x,   ptFirst.y);
        SetPiecePos(ptFirst);
    }
}
```
<div style="page-break-after: always;"></div>
```cpp
void CChess::Init(CRect rect) {
    m_chessdraw.InitBoard(rect,
                    COLUMNS,
                    ROWS,
                    rect.Height() / ROWS,
                    rect.Height() / ROWS / 2 - 2);
    m_rcBoard = m_chessdraw.GetRectBoard();
}

CRect CChess::GetRectBoard() {
    return m_rcBoard;
}

void CChess::Draw(CDC* pDC) {
    CMyMemDC memDC(pDC);

    m_chessdraw.SetDC(memDC);
    m_chessdraw.DrawBackground();
    m_chessdraw.DrawBoard();

// ----------------------- 重画整个棋局 -----------------------
    if(FIGHTING	== m_emWin) {
        // 没分胜负时才允许改变m_emWin的状态
        m_emWin	= PEACE;
    }

    for(int i = 0; i < COLUMNS; i++) {
        for(int j = 0; j < ROWS; j++) {
            if(NONE != m_iPositionPiece[i][j]) {
                m_chessdraw.DrawPiece(i, j,BLACK == m_iPositionPiece[i][j]);
            }
            else if(PEACE == m_emWin) {
                m_emWin	= FIGHTING;
                // 若有空位,则还可以继续,否则平局,即 m_emWin = PEACE;
            }
        }
    }
    m_chessdraw.DrawPieceCur(m_ptCurrent.x, m_ptCurrent.y); // 标记当前棋子
}
```
<div style="page-break-after: always;"></div>
```cpp
void CChess::SetPiecePos(CPoint	ptCurrent) {
    if(FIGHTING != m_emWin) {
        return; // 没分胜负时才允许改变m_ptCurrent的状态
    }
    UINT uiPosX, uiPosY;
    if(! m_chessdraw.GetCoordinateWithPoint(ptCurrent,	&uiPosX, &uiPosY)) {
        return; // 不在棋盘内的位置不做处理
    }
    if(NONE	!= m_iPositionPiece[uiPosX][uiPosY]) {
        return; // 已有棋子的位置不做处理
    }
    for(int i = 0; i < 2 ;  i++){
        m_iPositionPiece[uiPosX][uiPosY] = m_bTurnBlack ? BLACK : WHITE;
        // --------------------------  判断输赢  ---------------------------
        if(IsWin(uiPosX, uiPosY, m_iPositionPiece[uiPosX][uiPosY])) {
            m_ptCurrent = CPoint(uiPosX, uiPosY);
            return; // 已分出胜负
        }
        // --------------------------  记录上一次的棋子位置  -----------------
        if((PERSON_VS_PERSON  == m_emVSMode)
                || (m_bTurnBlack && (PERSON_VS_MACHINE == m_emVSMode))) {
            m_stcLastPos.ptLastCurPoint = m_ptCurrent;           
        }
        if(m_bTurnBlack) {
            m_stcLastPos.ptBlack = CPoint(uiPosX, uiPosY);
        }
        else {
            m_stcLastPos.ptWhite = CPoint(uiPosX, uiPosY);
        }
        m_stcLastPos.iPieceNum++;
        m_ptCurrent  = CPoint(uiPosX, uiPosY);
        // --------------------------  轮到白方下棋  ------------------------   
        m_bTurnBlack = ! m_bTurnBlack;   
        if((!m_bTurnBlack && (PERSON_VS_MACHINE == m_emVSMode))) {  
            if(!GetBestPosByAI(uiPosX,
                        uiPosY,
                        (!m_bTurnBlack) ? BLACK : WHITE)) {
                return; // 已分出胜负
            }
        }  
        if(PERSON_VS_PERSON == m_emVSMode) {
            break;    // 如果是人人对战,则退出循环
        }
    }
}

enumWinFlag CChess::GetWinFlag() {
    return m_emWin;
}
```

<div style="page-break-after: always;"></div>
```cpp
BOOL CChess::IsWin(UINT uiCol, UINT uiRow, enumChessColor emChessColor) {
    int iSameColor[MAXCREASE];

    GetSameColor(uiCol, uiRow,  emChessColor, iSameColor, m_iPositionPiece);
    for(int m = 0; m < MAXCREASE; m++)
        if(iSameColor[m] > MAXCREASE) {
            m_emWin = (BLACK == emChessColor) ? BLACK_WIN : WHITE_WIN;
            return TRUE;
        }

    return FALSE;
}

BOOL CChess::Regret() {
    if((m_stcLastPos.iPieceNum > 0) && (FIGHTING == m_emWin)) {
        // 如已分出胜负,则不再允许悔棋
        m_stcLastPos.iPieceNum  = 0;   //只允许悔一步
        m_ptCurrent = m_stcLastPos.ptLastCurPoint;

        if(PERSON_VS_MACHINE == m_emVSMode) {
            m_iPositionPiece[m_stcLastPos.ptBlack.x][m_stcLastPos.ptBlack.y] = NONE;
            m_iPositionPiece[m_stcLastPos.ptWhite.x][m_stcLastPos.ptWhite.y] = NONE;
        }

        if(PERSON_VS_PERSON == m_emVSMode) {
            if(!m_bTurnBlack) {
                m_iPositionPiece[m_stcLastPos.ptBlack.x][m_stcLastPos.ptBlack.y] = NONE;
            }
            else {
                m_iPositionPiece[m_stcLastPos.ptWhite.x][m_stcLastPos.ptWhite.y] = NONE;
            }                   
            m_bTurnBlack = ! m_bTurnBlack;   
        }
        return  TRUE;
    }

    return  FALSE;
}

void CChess::SetVSMode(enumVSMode emVSMode)
{
    m_emVSMode  = emVSMode;
}
```

<div style="page-break-after: always;"></div>
```cpp
void CChess::SetAIDepth(int emAIDepth)
{
    m_emAIDepth = emAIDepth;
}

BOOL CChess::GetBestPosByAI(UINT& uiCol,
                    UINT& uiRow,
                    enumChessColor emEnemyChessColor) {
    POINT ptPosWhite, ptPosBlack = CPoint(uiCol, uiRow);  
    BOOL  bContinue = TRUE;

    switch(m_emAIDepth) {
    case AI_FOOLISH:
        bContinue = ::AIFoolish(ptPosWhite,
                        ptPosBlack,
                        emEnemyChessColor,
                        m_iPositionPiece);
        break;
    case AI_PRIMARY:
        bContinue = ::AIPrimary(ptPosWhite, m_iPositionPiece);
        break;

    case AI_MIDDLE:
        bContinue = ::AIMiddle(ptPosWhite, m_iPositionPiece);
        break;
    case AI_HIGH:
        bContinue = ::AIHigh(ptPosWhite, m_iPositionPiece);
        break;
    default:
        break;
    }

    uiCol = ptPosWhite.x;
    uiRow = ptPosWhite.y;

    if(!bContinue) {
        m_emWin	= PEACE;
        return  FALSE;
    }  
    return  TRUE;
}
```
