#include "StdAfx.h"
#include "ChessAI.h"

// SCORE_BLACK[i][j] 
// ��ʾ����j+1������(�����߹�i����λ)������
// i = 0��ʾ���߶�����ס�� i = 1��ʾ��һ��û����ס�� i = 2 ��ʾ���߶��п�λ
//const double SCORE_WHITE[][5]  = {{0,  0, 0,	0,	1e10}, {1e0,  1e2,	1e4,  1e6,  1e10}, {1e1,  1e3,	1e5	, 1e7,  1e10} };
//const double SCORE_BLACK[][5]  = {{0,  0, 0,	0,	-1e9}, {-1e1, -1e3,	-1e5, -1e7,	-1e9}, {-1e2, -1e4,	-1e6, -1e8,	-1e9} };
const double SCORE_WHITE[][5] = {{0, 0, 0, 0,  10000000}, { 1,  25,   500,  2500,   10000000}, { 5,   100,  2500,   50000,   10000000}};
const double SCORE_BLACK[][5] = {{0, 0, 0, 0, -1000000 }, {-5, -100, -500, -10000, -1000000 }, {-25, -500, -10000, -250000, -1000000} };

// �������i=0�ķ�����ͣ�i>0ʱ����Խ��Խ�ã�
// �磺[1][3]>[2][2]  ������>��������Ϊ����һ�壬���ļ�������
// ����>�������Ϊ��������һ�壬����ʹ����������,���߻���ɻ������ͱ��������>���� ���з�����һ�壬����ʹ���ı������(���߶�����)�����߻����������
// Ϊ�˲�ʹ�����ص���[i+1][j] >= 4*[i][j] +1��[i][j+1] >= 4*[i+1][j] +1   �����ȡ�����λȡ������9ȡ��Ϊ10,     81ȡ��Ϊ100
// �ڷ��ĸ���Ҫ����������һ���ڷ����������ܷ֣���ʹ����ȥ�º��� �������ĸ���Ϊ�����10��, ��Ȼ����������ʱ�׷�����Ҫ�󣬲�Ȼ����ȥ�ºڷ����Լ���Ӯ

/*
�׷����� > �ڷ�����
���Ե��׷��л�����ڷ�������ʱ��Ӧ�������λ�ã�
������ʹ�ڷ���������ģ���ôҲֻ��Ҫһ��������ס
*/

void GetMaxValuePos(POINT &pt, const int iPieceScore[][ROWS])
{     
    pt.x  = 0;
    pt.y  = 0;
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(iPieceScore[pt.x][pt.y]<iPieceScore[i][j]) 
            {
                pt.x=i;
                pt.y=j;
            }
        }
    }
}

void GetMaxValuePosEx(POINT &pt, const int iPieceScore[][COLUMNS][ROWS])
{  
    POINT ptBlack;

    GetMaxValuePos(ptBlack, iPieceScore[0]);     // ��ȡ�ڷ���õ�λ��
    GetMaxValuePos(pt,      iPieceScore[1]);     // ��ȡ�׷���õ�λ��

    if(iPieceScore[0][ptBlack.x][ptBlack.y] >= iPieceScore[1][pt.x][pt.y]) 
    {
        pt = ptBlack;
    }
}

// ��̬��������
long double GetScore(UINT uiCol, UINT uiRow, const enumChessColor emChess[][ROWS],BOOL bIgnoreBlank = FALSE)   
{    
    int iSameColor[MAXCREASE];
    int iBlankFlag[MAXCREASE] = {0};                     

    GetSameColor(uiCol, uiRow, emChess[uiCol][uiRow], iSameColor, emChess, NULL, bIgnoreBlank, iBlankFlag);
     
    long double dbScore = 0;
    for(int m = 0; m < MAXCREASE; m++)
    {
        int i = iBlankFlag[m];
        int j = iSameColor[m] - 1;
        
        if(WHITE  == emChess[uiCol][uiRow])
            dbScore += SCORE_WHITE[i][j];
        else
            dbScore += SCORE_BLACK[i][j];
    }
    return  dbScore;
}

long double ScoreEvaluate(const enumChessColor emChess[][ROWS], BOOL bIgnoreBlank = FALSE)
{
    // Ŀǰ��ֵ����֣��׷�Ϊ���������԰׷�����Ϊ�����ڷ�Ϊ��
    // ���������Ӽ������ķ�
    long double dbScoreSum = 0;
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(NONE != emChess[i][j])   
            {                   
                dbScoreSum += GetScore(i, j, emChess, bIgnoreBlank);
            }
        }
    }

    return  dbScoreSum;
}

// Ϊ����ٶȣ���¼ÿһ�������Ĺ�ֵ�����������Ӽ�¼�����������ޣ�����ӵ���¼��  // ���û���
// AlphaBeta(3, -INFINITE, INFINITE, pt)  ��pt������3��

// ����Ƿ�Ϊ��������Ϊ��
// �����ֵ�����ʱ�����ܷ����ĵ�
// �ֵ�����ʱ�����ܷ���С�ĵ�

// alphaBetaMax�������������ģ������ʺϰ׷�  ����5������ʱ������ɫӮ����Ϊ10000������ɫӮ��-10000
// ��ôalphaBetaMax(-10000, 10000, 3); ���صı���10000�� ���������������򷵻ؾ����ܴ��ֵ
// alphaBetaMin�򷵻ؾ�����С��ֵ

// Alpha-beta efficiency depends on move ordering. In the worst case ordering, it's exactly the same as regular minimax.
// It's not better than Beta or better than Alpha
// С����С�Ļ��ߴ�������
// Move ordering, move ordering, move ordering!  Start taking advantage of your iterative deepening.
// Usually this is implemented with a transposition table, but you can do it without one as well. 

long double AlphaBetaMax(long double dbAlphaMin, long double dbBetaMax, int iDepthleft, enumChessColor emChess[][ROWS], POINT& ptBest);
long double AlphaBetaMin(long double dbAlphaMin, long double dbBetaMax, int iDepthleft, enumChessColor emChess[][ROWS], POINT& ptBest);

long double AlphaBetaMax(long double dbAlphaMin, long double dbBetaMax, int iDepthleft, enumChessColor emChess[][ROWS], POINT& ptBest) 
{
    ptBest = CPoint(-1, -1);       // ��������������ptBest����CPoint(-1, -1)��˵������������
    if(iDepthleft == 0) 
    {
        return ScoreEvaluate(emChess); 
    }
    CPoint  ptTmp;      // ��Ϊһ���������ݣ���Ҫ����

    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(NONE == emChess[i][j])
            {
                if(ptBest.x == -1)   
                {
                    ptBest = CPoint(i, j); // ����ǵ�һ�ν���
                }

                emChess[i][j] = WHITE ;     // ��һ����

                long double dbScore = AlphaBetaMin(dbAlphaMin, dbBetaMax, iDepthleft - 1, emChess, ptTmp);  // �ҳ�Min��������ҵ���������
                             
                emChess[i][j] = NONE;       // ���������ߵ���һ����

                if( dbScore >= dbBetaMax )      
                {
                    ptBest = CPoint(i, j);
                    return dbBetaMax;       // dbBetaMax-cutoff
//                  return dbScore;  
                }
                if( dbScore > dbAlphaMin )
                {
                    ptBest = CPoint(i, j);
                    dbAlphaMin = dbScore;   // dbAlphaMin acts like max in MiniMax
                }
            }
        }
    }
    return dbAlphaMin;    //Max
}

long double AlphaBetaMin(long double dbAlphaMin, long double dbBetaMax, int iDepthleft, enumChessColor emChess[][ROWS], POINT& ptBest) 
{
    ptBest = CPoint(-1, -1);
    if(iDepthleft == 0) 
    {
        //  ��AlphaBetaMax()��iDepthleftΪ2ʱ���Ӳ��Ӹ��Ŷ����
        // ��iDepthleftΪ3ʱ���Ӹ����ܺܿ��֦�ó�����������������ȷ���������ﲻ�Ӹ���
//      return -ScoreEvaluate(emChess);       
        return ScoreEvaluate(emChess);      
    }

    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(NONE == emChess[i][j])
            {
                emChess[i][j] = BLACK ;     // ��һ����

                long double dbScore = AlphaBetaMax(dbAlphaMin, dbBetaMax, iDepthleft - 1, emChess, ptBest); // �ҳ�Max��������ҵ�����С����

                emChess[i][j] = NONE;       // ���������ߵ���һ����
               
                if(dbScore <= dbAlphaMin)  
                {   
                    ptBest = CPoint(i, j);
                    return dbAlphaMin;      // dbAlphaMin-cutoff
   //               return dbScore;          
                }
                if(dbScore < dbBetaMax)
                {
                    ptBest = CPoint(i, j);
                    dbBetaMax = dbScore;    // dbBetaMax acts like min in MiniMax
                }
            }
        }
    }
    return dbBetaMax; // Min
}

// ------------------------------------------------------------ AI  Function ------------------------------------------------------------

// ���ڷ�������������ʱ���谭�����������Լ���
// ptPosBlack	        �ڷ���ǰ���ӵ�λ��
// ptPosWhite           �����Լ�Ӧ���ӵ�λ��
// emEnemyChessColor	�з��ı�־WHITE,  BLACK
BOOL    AIFoolish(POINT& ptPosWhite, POINT ptPosBlack,	enumChessColor emEnemyChessColor, const enumChessColor emChess[][ROWS])
{
    int  piCreaseDirect[MAXCREASE] = {0};
    int	 iSameColor[MAXCREASE];
	UINT uiCol = ptPosBlack.x;
	UINT uiRow = ptPosBlack.y;	

    GetSameColor(uiCol, uiRow, emEnemyChessColor, iSameColor, emChess, piCreaseDirect);

    int iSameMaxValue = iSameColor[0], iSameMaxIndex  = 0;
    for(int m = 1; m < 4; m++)
    {
        if(iSameColor[m] > iSameMaxValue)
        {
            iSameMaxValue = iSameColor[m];
            iSameMaxIndex = m;
        }
    }

    if(iSameColor[iSameMaxIndex] > 0)
    {
        switch(iSameMaxIndex)
        {
        case 0:
            ptPosWhite.y = ptPosBlack.y;
            ptPosWhite.x = ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];	    // ����

            if(! PtInBroad(ptPosWhite.x, ptPosWhite.y) || (NONE != emChess[ptPosWhite.x][ptPosWhite.y]))    // �����������          
            {
                ptPosWhite.x = ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex]	+ 1;							// ���ҷ�                 
            }	
            break;
        case 1:
            ptPosWhite.x = ptPosBlack.x;
            ptPosWhite.y = ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];	    // ���Ϸ�
	
            if(! PtInBroad(ptPosWhite.x, ptPosWhite.y) || (NONE != emChess[ptPosWhite.x][ptPosWhite.y]))                  
            {
                ptPosWhite.y = ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex]	+ 1;						    // ���·�
            }		
            break;
        case 2:
            ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];
            ptPosWhite.y= ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];	    // �����Ϸ�

            if(! PtInBroad(ptPosWhite.x, ptPosWhite.y) || (NONE != emChess[ptPosWhite.x][ptPosWhite.y]))                  
            {
                ptPosWhite.x = ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex]	+ 1;
                ptPosWhite.y = ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex]	+ 1;							// �����·�
            }		
            break;
        case	3:
            ptPosWhite.x = ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];
            ptPosWhite.y = ptPosBlack.y	- piCreaseDirect[iSameMaxIndex] + iSameColor[iSameMaxIndex];		// �����·�

            if(! PtInBroad(ptPosWhite.x, ptPosWhite.y) || (NONE != emChess[ptPosWhite.x][ptPosWhite.y]))                  
            {
                ptPosWhite.x = ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex]	+ 1;
                ptPosWhite.y = ptPosBlack.y	- piCreaseDirect[iSameMaxIndex]	- 1;							// �����Ϸ�
            }		
            break;
        default:
            break;
        }

        // ��Ҫ�ж�����Ƿ�Ϊ�գ������Ϊ�գ�������������
        if(PtInBroad(ptPosWhite.x, ptPosWhite.y) &&	(NONE == emChess[ptPosWhite.x][ptPosWhite.y]))
        {
            return	TRUE;
        }
    }   // if > 0

    // ���û�ҵ�����λ�ã���������һ��
    for(int	m = 0; m < COLUMNS; m++)
    {
        for(int	n = 0; n < ROWS; n++)
		{
            if(NONE == emChess[m][n])
            {
                ptPosWhite.x = m;
                ptPosWhite.y = n;	
                return	TRUE;
            }
		}
    }

	return	FALSE;		// ˵����������
}

// ��Ԥ��
// ����ֽ������֣��ҵ���ֵ����λ��    
BOOL AIPrimary(POINT &pt, enumChessColor emChess[][ROWS])
{
    AlphaBetaMax(-1e10, 1e11, 1, emChess, pt);

    if(pt.x == -1)   
    {
        return  FALSE;  // ����������
    }
    return  TRUE;
}

// Ԥ��һ��
// ����ֽ������֣��ҵ���ֵ����λ��,��ֵ���ú�AIPrimary��һ�������ֻ�Ӱ�����������
BOOL AIMiddle(POINT &pt, const enumChessColor emChess[][ROWS])
{   
    BOOL   bIsBoardFull    = TRUE;
    const  int PLAYER_NUM  = 2;
    int iPieceScore[PLAYER_NUM][COLUMNS][ROWS]={0};     // ��¼���ͷ���
    int iArraySameColor[MAXCREASE];
    int iBlankFlag[MAXCREASE];                                   

    // ���ֿ���Ӱ��������    �������ĵ�һ�黻�����һ�飬���������ͺܶ�
    const  int SCORE_BLACK[][4] = {{0, 0,	0,	50000}, {2, 10,  110,  50000}, {21, 81,	2000,  50000} };
    const  int SCORE_WHITE[][4] = {{0, 0,	0,	10000}, {1, 9,	 100,  10000}, {20, 80,	400	,  10000} };

    int i, j, iPlayer, iScore=0;   
    for(iPlayer=0; iPlayer < PLAYER_NUM; iPlayer++)
    {
        for(i=0; i<COLUMNS; i++)
            for(j=0; j<ROWS;  j++)
            { 
                if(emChess[i][j]==0)     // AI��ֻ�����λ���ǿ�λ������Ϊ0
                {
                    GetSameColor(i, j, iPlayer ? BLACK : WHITE, iArraySameColor, emChess, NULL, TRUE, iBlankFlag);

                    for(int iDirect = 0; iDirect < MAXCREASE; iDirect++)
                    {                                            
                        for(int iBlank = 2; iBlank >= 0; iBlank--)
                            if(iBlankFlag[iDirect] == iBlank)                 //  ��iDirect���������ߵĿ�λ��
                            {
                                for(int iSameColor = 4; iSameColor > 0; iSameColor--)
                                    if(iArraySameColor[iDirect] == iSameColor)     //  ��iDirect��������ͬ��ɫ��������           
                                    {
                                        if(iPlayer)    //��¼�������
                                        {
                                            iScore += SCORE_BLACK[iBlank][iSameColor-1];      
                                        }
                                        else           //��¼�������    
                                        {
                                            iScore += SCORE_WHITE[iBlank][iSameColor-1];
                                        }
                                    }
                            }
                    }// for iDirect<4

                    iPieceScore[iPlayer][i][j]  = iScore;           // ��ΪiPosPiece֮�����ã��������ڸ��ڡ��׷���¼���ͷ���
                    iScore=0;                                     
                    bIsBoardFull = FALSE;
                }//  if(emChess[i][j]==0)  
            }// for j<ROWS
    }

    GetMaxValuePosEx(pt, iPieceScore);
    
    if(bIsBoardFull)
    {
        return  FALSE; // ����������
    }
    return  TRUE;
}
    
// Ԥ��2������
BOOL AIHigh(POINT &pt, enumChessColor emChess[][ROWS])
{
//   AlphaBetaMax(-1e9, 1e10, 2, emChess, pt);
//   AlphaBetaMax(-1e11, INFINITE, 2, emChess, pt);
    AlphaBetaMax(-10000000, 100000000, 2, emChess, pt);
    
    if(pt.x == -1)   
    {
        return  FALSE;  // ����������
    }
    return  TRUE;
}
