#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "tbl\table_mgr.h"
#include "tbl\hulib.h"
#include "split\split.h"
#include "split\split2.h"

#include "Define.h"
#include "DefineHuTip.h"
#include "PlayerHuTips.h"
#include "PlayerHuTips2.h"

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
#define MAX_COUNT (100 * 10000)
static BYTE s_HuCardAll[136];
char source[MAX_COUNT * 9*34];

void main()
{
	CPlayerHuTips2 stTssss;
	stTssss.TrainAll();

	CHuTipsMJ		m_cAlgorithm;
	TableMgr::get_instance()->load();

	memset(source, 0, sizeof(source));
	
	for (int i = 0; i < 34; i++)
	{
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4 + 1] = i;
		s_HuCardAll[i * 4 + 2] = i;
		s_HuCardAll[i * 4 + 3] = i;
	}

	int gui_index = 33;
	int total = 0;
	srand(10);
	for (int n = 0; n < MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll + 130);		// 这个函数对计算有影响
		for (int i = 0; i < 9; ++i)	// 136/14 -> 9
		{
			char* cards = &source[total++ * 34];
			memset(cards, 0, 34);
			for (int j = i * 14; j < i * 14 + 13; j++)
				++cards[s_HuCardAll[j]];
			cards[gui_index] = 1;
		}
	}
	
	// 查表法
	int hu = 0;
	DWORD dwTimeBegin = GetTickCount();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		hu += HuLib::get_hu_info(cards, NULL, 34, 34, gui_index);
	}
	cout << "查表法总数:" << total << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;

	// 选将，拆分法
	hu = 0;
	dwTimeBegin = GetTickCount();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		hu += split::get_hu_info(cards, 34, gui_index);
	}
	cout << "选将拆分法总数:" << total << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;

	// 选将，拆分法优化版
	hu = 0;
	dwTimeBegin = GetTickCount();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		hu += split2::get_hu_info(cards, 34, gui_index);
	}
	cout << "优化版选将拆分法总数:" << total << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;

	// rjc查表法
	hu = 0;
	stCardData2 stData2;
	dwTimeBegin = GetTickCount();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		stData2.byNum = 14;
		memcpy(stData2.byCardNum, cards, 34);
		hu += stTssss.CheckCanHu(stData2, gui_index);
	}
	cout << "rjc查表法总数:" << total << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;

	hu = 0;
	stCardData stData;
	dwTimeBegin = GetTickCount();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		stData.byNum = 14;
		memcpy(stData.byCardCount, cards, 34);
		hu += m_cAlgorithm.CanWin_Do_Nai(stData, gui_index);
	}
	cout << "rjc递归方法, 总数:" << total << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;
	cin >> hu;
}