#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>

#include "DefineHuTip.h"
#include "HuPaiMJ_32.h"

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


#define MAX_COUNT (100 * 10000)
static BYTE g_HuCardAll[136];

CHuPaiMJ stTssss;

void test_repeat()
{
	char cards[] = {
		2, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 1, 0, 1, 3, 2, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 4
	};
	int hu = 0;
	stCardData stData2;
	DWORD dwTimeBegin = GetTickCount();
	for (int i = 0; i < MAX_COUNT; i++){
		stData2.byNum = 14;
		memcpy(stData2.byCardNum, cards, sizeof(cards));
		hu += stTssss.CheckCanHu(stData2, 33);
	}

	cout << "rjc查表法，相同牌型，总次数: " << MAX_COUNT / 10000 << "万次" << endl;
	cout << "time: " << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "hu:" << hu << endl;
}
char source[MAX_COUNT * 9 * 34];
#define GUI_NUM 2
void main()
{
	stTssss.TrainAll();

	//test_repeat();
	
	for (int i = 0; i < 34; i++)
	{
		g_HuCardAll[i * 4] = i;
		g_HuCardAll[i * 4+1] = i;
		g_HuCardAll[i * 4+2] = i;
		g_HuCardAll[i * 4+3] = i;
	}
	
	int gui_index = 33;
	int total = 0;
	srand(1);
	for (int n = 0; n < MAX_COUNT; ++n)
	{
		random_shuffle(g_HuCardAll, g_HuCardAll + 130);		// 这个函数对计算有影响
		for (int i = 0; i < 9; ++i)	// 136/14 -> 9
		{
			char* cards = &source[total++ * 34];
			memset(cards, 0, 34);
			for (int j = i * 14; j < i * 14 + 14 - GUI_NUM; j++)
				++cards[g_HuCardAll[j]];
			cards[gui_index] = GUI_NUM;
		}
	}

	// rjc查表法
	int hu = 0;
	stCardData stData2;
	DWORD dwTimeBegin = GetTickCount();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		stData2.byNum = 14;
		memcpy(stData2.byCardNum, cards, 34);
		hu += stTssss.CheckCanHu(stData2, gui_index);
	}

	cout << "rjc查表法总数:" << 9 * MAX_COUNT/10000 << "万次, time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;
	cin >> hu;
}