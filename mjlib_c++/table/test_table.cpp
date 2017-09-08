#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include "table_mgr.h"
#include "hulib.h"

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
static BYTE s_HuCardAll[136];

void test_hu()
{
	char cards[] = {
		1,0,1,0,0,2,0,2,0,
		1,1,1,0,0,3,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,2
	};
	int hu = 0;
	DWORD dwTimeBegin = GetTickCount();
	for (int i = 0; i < MAX_COUNT; i++){
		hu += HuLib::get_hu_info(cards, 34, 33);
	}

	cout << "查表法，相同牌型，总次数: " << MAX_COUNT / 10000 << "万次" << endl;
	cout << "time: " << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "hu:" << hu << endl;
}

int GUI_NUM = 4;
char source[MAX_COUNT * 9 * 34];

void main()
{
	TableMgr::init();
	TableMgr::load();

	//test_hu();
	
	for (int i = 0; i < 34; i++)
	{
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4 + 1] = i;
		s_HuCardAll[i * 4 + 2] = i;
		s_HuCardAll[i * 4 + 3] = i;
	}
	
	int gui_index = 33;
	int total = 0;
	srand(1);
	for (int n = 0; n < MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll + 130);		// 这个函数对计算有影响
		for (int i = 0; i < 9; ++i)	// 136/14 -> 9
		{
			char* cards = &source[total++ * 34];
			memset(cards, 0, 34);
			for (int j = i * 14; j < i * 14 + 14 - GUI_NUM; j++)
				++cards[s_HuCardAll[j]];
			cards[gui_index] = GUI_NUM;
		}
	}

	int hu = 0;
	DWORD dwTimeBegin = clock();
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		hu += HuLib::get_hu_info(cards, 34, gui_index);
	}
	cout << "查表法总数:" << 9 * MAX_COUNT / 10000 << "万次，time:" << clock() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;
	cin >> hu;
}