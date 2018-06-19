// table.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include<time.h>
#include "hulib.h"

void check_hu()
{
	char cards[] = {
		0,
		3,1,1,1,1,1,1,1,3,		// 万
		0,0,0,0,0,0,0,0,0,		// 筒
		0,0,0,0,0,0,0,0,0,		// 条
		0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
	};
	bool hu = false;
	int n = 1000 * 10000;
	int begin = time(NULL);
	std::cout << "查表法测试九莲宝灯" << n / 10000 * 9 << "万次" << std::endl;
	for (int j = 0; j < n; ++j) {
		for (int i = 1; i <= 9; ++i) {
			cards[i]++;
			hu = can_hu(cards);
			cards[i]--;
		}
	}
	std::cout << "耗时:" << time(NULL) - begin << "秒" << std::endl;
	std::cout << hu << std::endl;
}

int main()
{
	init_table();
	check_hu();

	getchar();
	return 0;
}


