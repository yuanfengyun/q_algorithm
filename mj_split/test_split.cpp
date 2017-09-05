#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "split.h"

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

void test_split()
{
	char cards[] = {
		2,0,0,0,0,0,0,1,0,
		0,1,0,1,3,2,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,4
	};

	split::get_hu_info(cards, 34, 33);
}

#define MAX_COUNT (100 * 10000)
static BYTE s_HuCardAll[136];

void main()
{
	test_split();
	for (int i = 0; i < 34; i++)
	{
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4 + 1] = i;
		s_HuCardAll[i * 4 + 2] = i;
		s_HuCardAll[i * 4 + 3] = i;
	}

	int hu = 0;
	char cards[34] = { 0 };
	// 纯随机跑10000*9次，每次将第一个牌值设为赖子
	srand(1);
	DWORD dwTimeBegin = GetTickCount();
	for (int n = 0; n<MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll + 132);		// 这个函数对计算有影响
		for (int i = 0; i<9; ++i)	// 136/14 -> 9
		{
			memset(cards, 0, sizeof(cards));
			for (int j = i * 14; j < i * 14 + 10; j++)
				++cards[s_HuCardAll[j]];

			cards[33] = 4;
			hu += split::get_hu_info(cards, 34, 33);
		}
	}
	cout << "total: " << MAX_COUNT*9/10000 <<"万次"<< endl;
	cout <<"time: " << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "Hu: " << hu << endl;
	cin >> hu;
}
