#pragma once
//  ChessCommon.h
//  Build   on  2015.11.15
//  Copyright(C) by Hong Zhiwei, Liu Weishun, Sun Zhiwei
//  Email: 1359434736@qq.com
//
//  License
//  This article, along with any associated source code and files, is licensed under The Code Project Open License (CPOL)
//  ����Ŀ����Դ�������CPOL��ԴЭ�飬��ϸ��������Code Project ����


//  ����ͷ�ļ�   Ŀǰֻ��ChessAI.h �� Chess.h ���ã�
//	����ĿĬ��Ϊ15*15��������,����Ӧ�õ�������������࣬���������޸ģ�
//
//  ע��: �˻���սʱ��Ĭ����Ϊ�ڷ�(����)������Ϊ�׷������£���

const int	MAXCREASE	= 4;    // ����ĿΪ�����壬���������ĿΪ5-1
const UINT	ROWS		= 15;   // ���̵�����
const UINT	COLUMNS	    = 15;   // ���̵�����
enum  enumChessColor {NONE, WHITE, BLACK};		// NONE = 0 : �����ӣ�WHITE =1 : �׷����ӣ�BLACK =2 : �ڷ�����

// ��ͬ��ɫ�����ļӷ���
void SameColorAdder(UINT uiCol,	UINT uiRow,	enumChessColor emChessColor, BOOL bResetCount, int& iColorCount, BOOL& bOver,   
                    const enumChessColor iChess[][ROWS], int* piBlankCount = NULL );
  
// ��ĳ�㴦��������ͬ��ɫ��������
void GetSameColor(UINT uiCol, UINT uiRow, enumChessColor emChessColor, int* iSameColor,   
                  const enumChessColor iChess[][ROWS], int* piCreaseDirect = NULL, BOOL bIgnoreBlank = FALSE, int* piBlankFlag = NULL);

// ĳ���Ƿ���������
BOOL PtInBroad(UINT	uiCol, UINT uiRow);