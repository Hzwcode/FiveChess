#pragma once
#include "ChessCommon.h"

//  ChessAI.h
//  Build   on  2015.11.15  
//  Copyright(C) by Hong Zhiwei, Liu Weishun, Sun Zhiwei
//  Email: 1359434736@qq.com
//
//  License
//  This article, along with any associated source code and files, is licensed under The Code Project Open License (CPOL)
//  ����Ŀ����Դ�������CPOL��ԴЭ�飬��ϸ��������Code Project ����


// ����AI�����ٶ��𽥼�����CPUռ�������ߣ�����Ҳ������
// AIFoolish         ��Ȼ������ͣ�����ռ�õ��ڴ��CPU����͵ģ�
// AIPrimary         ռ���ڴ�ͣ�ռ��CPUҲ��
// AIMiddle          �ڴ��CPUռ�þ���
// AIHigh            ռ���ڴ�ͣ�ռ��CPU��

// ����Ĭ�ϻ���Ϊ�׷�

//AIBlock �谭��AI�������谭���ֳ�5�� �����ϵ�      
BOOL AIFoolish(POINT& ptPosWhite, POINT ptPosBlack,	enumChessColor emChessColor, const enumChessColor emChess[][ROWS]);
// ����AI 
BOOL AIPrimary(POINT &pt, enumChessColor emChess[][ROWS]);                     
// �м�AI     
BOOL AIMiddle(POINT &pt, const enumChessColor emChess[][ROWS]);
// �߼�AI   ������,����Сalpha-beta��֦�Ż�
BOOL AIHigh(POINT &pt, enumChessColor emChess[][ROWS]);                
