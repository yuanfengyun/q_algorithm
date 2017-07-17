#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "table_mgr.h"
#include "hulib.h"
using namespace std;

#define MAX_COUNT 1000000

static BYTE s_HuCardAll[136];

void test_one()
{
	char cards[34] = {
		0,0,0,0,0,0,0,0,3,
		0,0,0,0,1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,2,0,4,0,4
	};

	HuLib::get_hu_info(cards, NULL, 34, 34, 33);
}


void main()
{
	TableMgr::get_instance()->load();
	test_one();
	srand(1);

	for (int i = 0; i < 34; i++)
	{
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4+1] = i;
		s_HuCardAll[i * 4+2] = i;
		s_HuCardAll[i * 4+3] = i;
	}
	DWORD dwTimeBegin = GetTickCount();
	BYTE byNaiZi = s_HuCardAll[135];
	BYTE byCards[14] = {};
	int  nAll = 0;
	BYTE			 byNaiZiNote[1000] = {};
	// 纯随机跑10000*9次，每次将第一个牌值设为赖子
	int hu = 0;
	for (int n = 0; n<MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll + 136);
		for (int i = 0; i<9; ++i)	// 136/14 -> 9
		{
			char cards[34] = { 0 };
			for (int j = i * 14; j < (i + 1) * 14; j++)
			{
				++cards[s_HuCardAll[j]];
			}
			int gui_index = s_HuCardAll[(i + 1) * 14 - 1];

			hu += HuLib::get_hu_info(
				cards,
				NULL,
				34,
				34,
				gui_index
				);
		}
	}
	cout << "nAll:" << MAX_COUNT << "  time:" << GetTickCount() - dwTimeBegin << "ms" << endl;
	cout << "hu: " << hu << endl;;

	cin >> nAll;
}