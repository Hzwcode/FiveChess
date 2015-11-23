#include "StdAfx.h"
#include "ChessCommon.h"

// �ж�ĳ���Ƿ�����������
BOOL    PtInBroad(UINT	uiCol,	UINT	uiRow)
{
    if((uiCol >= 0)	&& (uiCol < COLUMNS) && (uiRow >= 0) &&	(uiRow < ROWS)){
        return	TRUE;
    }
    return	FALSE;
}

// ����ͬ��ɫ�����ļӷ�����
// λ��(uiCol, uiRow)   ��ɫΪemChessColor�ĸ���
// emChessColor     ���ӵ���ɫ
// bResetCount      �Ƿ��������
// iColorCount      ��ͬ��ɫ����Ŀ
// bOver            �Ƿ�ֹͣ����
// piBlankCount     ��λ��״��
// emChess          ������ֵ���Ϣ
void     SameColorAdder(UINT	uiCol,	UINT	uiRow,	enumChessColor   emChessColor,    BOOL    bResetCount ,int &iColorCount,   BOOL&    bOver,   
                        const enumChessColor emChess[][ROWS],  int* piBlankCount )
{
	// ��m_iPositionPieceԽ��ʱ��VS��������������������������Բ�����Խ��Ҳ����ȷִ��
	// ����ȫ���������봦��Խ��
    if(PtInBroad(uiCol, uiRow))       // �߽��ж�
    {
        if(emChessColor	== emChess[uiCol][uiRow]){
            iColorCount++;
        }
        else{
            if(bResetCount){
                iColorCount = 0;
            }
            else{
                bOver = TRUE;
            }
            
            // ��λ����
            if(piBlankCount){
                if(0 == emChess[uiCol][uiRow]){
                    if((0 == (*piBlankCount)) || bOver){
                        (*piBlankCount)++;
                    }
                }
                else if(! bOver){
                    (*piBlankCount) = 0;
                }
            }
        }
    }  
}

// ����ĳ�㴦��������ͬ��ɫ����������
// λ��(uiCol,   uiRow)   ������������ͬ��ɫ������������λ״��
// emChessColor     ���ӵ���ɫ
// iSameColor       ������������ͬ��ɫ��������
// piCreaseDirect   ���������iCrease����
// bIgnoreBlank     �Ƿ���Կո�
// piBlankFlag      ���������Ͽ�λ��״��
// emChess          ������ֵ���Ϣ
void     GetSameColor(UINT	uiCol, UINT	uiRow, enumChessColor emChessColor, int* iSameColor,  
                      const enumChessColor emChess[][ROWS], int* piCreaseDirect, BOOL bIgnoreBlank, int* piBlankFlag)
{    	     
    int	iColTemp, iRowTemp, iCrease;
    BOOL bBreak  =FALSE;

    memset(iSameColor, 0, sizeof(iSameColor) * MAXCREASE);
    if(piCreaseDirect)
    {
        memset(piCreaseDirect,0, sizeof(piCreaseDirect) * MAXCREASE);
    }
    if(piBlankFlag)
    {
        memset(piBlankFlag,	0, sizeof(piBlankFlag) * MAXCREASE);
    }

    for(int iDirect = 0; iDirect < 4; iDirect++, bBreak  =FALSE)
    {          
        int iBlankCount = 0;//  0 ���߶��޿�λ     1ֻ��һ���п�λ    2���߶��п�λ

        for(iCrease = -MAXCREASE; !bBreak && (iCrease <= MAXCREASE); iCrease++)
        {
            if(bIgnoreBlank && !iCrease)    // ��ΪAI��ԭλ��Ϊ�գ����Բ�Ҫ����
            {
                continue;   //  ����  iCrease++;
            }
            switch(iDirect)
			{
            case    0:      // �жϺ���
                iColTemp     = uiCol + iCrease;
                iRowTemp   = uiRow;
                break;
            case    1:		// �ж�����
                iColTemp     = uiCol;
                iRowTemp   = uiRow + iCrease;
                break;
            case    2:		// �жϷ�б��\��
                iColTemp     = uiCol + iCrease;
                iRowTemp   = uiRow + iCrease;
                break;
            case    3:      // �ж�б��/��
                iColTemp     = uiCol + iCrease;
                iRowTemp   = uiRow - iCrease;
                break;
            default:
                break;
            }    
            if(PtInBroad(iColTemp, iRowTemp)) 
            {           
                SameColorAdder(iColTemp, iRowTemp, emChessColor, iCrease <= 0, iSameColor[iDirect], bBreak, emChess, &iBlankCount);     
            }
            else  if(iCrease > 0)
            {
                iCrease+=1;
                break;
            }
        } //for iCrease 
      
        if(piCreaseDirect)
        {
            piCreaseDirect[iDirect] = iCrease - 2;
        }

        if(piBlankFlag)
        {
            piBlankFlag[iDirect] = iBlankCount;
        }
    } //for  iDirect < 4 
}
