#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>

#include "Define.h"
#include "DefineHuTip.h"
#include "PlayerHuTips.h"

/*
void print_cards(char* cards)
{
	for (int i = 0; i<9; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\t");

	for (int i = 9; i<18; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\t");
	for (int i = 18; i<27; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\t");
	for (int i = 27; i<34; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\n");
}

void Print_cards(BYTE* a)
{
	char cards[34] = {0};
	for (int i = 0; i < 14; i++){
		cards[getIndexByVal(a[i])]++;
	}
	print_cards(cards);
}

using namespace std;

static BYTE s_HuCardAll[136];
void main2()
{
	CHuTipsMJ		m_cAlgorithm;
	CPlayerHuTips	cHuTips(&m_cAlgorithm);

	for (int i = 0; i < 34; i++)
	{
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4+1] = i;
		s_HuCardAll[i * 4+2] = i;
		s_HuCardAll[i * 4+3] = i;
	}			
	
	for (int i=0; i<136; ++i)
	{
		s_HuCardAll[i] = getValByIndex(s_HuCardAll[i]);
	}
	BYTE gui_card = getValByIndex(33);
	BYTE gui_array[4];
	gui_array[0] = gui_card;
	gui_array[1] = gui_card;
	gui_array[2] = gui_card;
	gui_array[3] = gui_card;
	
#define  MAX_COUNT 100000

	DWORD dwTimeBegin = GetTickCount();

	int hu = 0;
	// 纯随机跑10000*9次，每次将第一个牌值设为赖子
	srand(1);
	for (int n = 0; n<MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll+132);
		for (int i=0; i<9; ++i)	// 136/14 -> 9
		{	
			stCardData stData(s_HuCardAll+i*14+1, 10);	// 不要把第一个牌值设进去
			stData.addCard(gui_array, 4);
			hu +=  m_cAlgorithm.CheckWin_Dian(stData, gui_card);			
		}
	}
	cout << "总数:" << MAX_COUNT << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;
	cin >> hu;
}

//*/