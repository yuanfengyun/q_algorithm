// mj_paileizuhe.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include<time.h>
#include "pailiezuhe.h"

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
	int n = 1000 * 10000;
	int begin = time(NULL);
	std::cout << "测试九莲宝灯" << n / 10000 * 9 << "万次" << std::endl;
	for (int j = 1; j < n; ++j) {
		for (int i = 1; i <= 9; ++i) {
			cards[i]++;
			bool hu = can_hu_huisu(cards);
			cards[i]--;
		}
	}
	std::cout << "耗时:" << time(NULL) - begin << "秒" << std::endl;
}

int main()
{
	std::cout << "获取牌的字符串" << std::endl;
	std::cout << get_card_str(0x11) << std::endl;
	std::cout << get_card_str(0x28) << std::endl;
	std::cout << get_card_str(0x39) << std::endl;
	std::cout << get_card_str(0x45) << std::endl;
	std::cout << get_card_str(0x57) << std::endl;

	std::cout << "获取牌的颜色" << std::endl;
	std::cout << get_color_str(0x11) << std::endl;
	std::cout << get_color_str(0x53) << std::endl;

	std::cout << "获取牌的值" << std::endl;
	std::cout << get_card_value(0x25) << std::endl;

	check_hu();

	getchar();
	return 0;
}


