#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <time.h>

#include "Define.h"
#include "DefineHuTip.h"
#include "PlayerHuTips.h"

using namespace std;

void print_cards(char* cards)
{
	for (int i = 0; i<9; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 9; i<18; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");
	for (int i = 18; i<27; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");
	for (int i = 27; i<34; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\n");
}

#define GUI_NUM 4
#define MAX_COUNT (80000 * 10000)
static BYTE g_HuCardAll[136];
static BYTE s_HuCardAll[136];

void main()
{
	CHuTipsMJ		m_cAlgorithm;
	
	for (int i = 0; i < 34; i++)
	{
		g_HuCardAll[i * 4] = i;
		g_HuCardAll[i * 4+1] = i;
		g_HuCardAll[i * 4+2] = i;
		g_HuCardAll[i * 4+3] = i;
	}
	
	BYTE gui_card = getValByIndex(33);
	int gui_index = 33;
	int hu = 0;
	char cards[34] = { 0 };
	DWORD dwTimeBegin = GetTickCount();
	
	srand(1);
	stCardData stData;
	hu = 0;
	memcpy(s_HuCardAll, g_HuCardAll, sizeof(s_HuCardAll));
	dwTimeBegin = clock();
	for (int n = 0; n<MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll + 126);		// 这个函数对计算有影响
		for (int i = 0; i<9; ++i)	// 136/14 -> 9
		{
			memset(cards, 0, sizeof(cards));
			for (int j = i * 14; j < i * 14 + 14 - GUI_NUM; j++)
				++cards[s_HuCardAll[j]];

			cards[33] = GUI_NUM;
			stData.byNum = 14;
			memcpy(stData.byCardCount, cards, sizeof(cards));
			hu += m_cAlgorithm.CanWin_Do_Nai(stData, gui_index);
		}
	}
	cout << "rjc递归方法, 总数:" << 9 * MAX_COUNT / 10000 << "万次, time:" << clock() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;
	cin >> hu;
}