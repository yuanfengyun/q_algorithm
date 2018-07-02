#include "stdafx.h"
#include "huisu.h"
#include <string.h>

struct HUISU_ITEM
{
	char a;
	char b;
	char c;
	bool eye;
};

HUISU_ITEM huisu_items1[] = {
	{1,1,1,false}
};

HUISU_ITEM huisu_items2[] = {
	{ 2,0,0,true},
	{ 2,2,2,false}
};

HUISU_ITEM huisu_items3[] = {
	{ 3,0,0,false},
	{ 3,1,1,true}
};

HUISU_ITEM huisu_items4[] = {
	{ 4,1,1,false},
	{ 4,2,2,true}
};

HUISU_ITEM* get_huisu_ops(char cur_num, int* op_num)
{
	*op_num = 0;
	if (cur_num == 1) {
		*op_num = 1;
		return huisu_items1;
	}
	if (cur_num == 2) {
		*op_num = 2;
		return huisu_items2;
	}
	if (cur_num == 3) {
		*op_num = 2;
		return huisu_items3;
	}
	if (cur_num == 4) {
		*op_num = 2;
		return huisu_items4;
	}
}

// 字牌是否满足胡牌条件
bool check_zi(char* cards, bool& eye)
{
	for (int i = 28; i <= 34; ++i) {
		if (cards[i] == 0) continue;

		if (cards[i] == 1 || cards[i] == 4) return false;

		if (eye && cards[i] == 2) return false;

		if (cards[i] == 2) eye = true;
	}

	return true;
}

bool cai_cur(int cur, char* cards, bool& eye)
{
	if (cur > 8) return true;

	int n = cards[cur];
	if (n == 0) return cai_cur(cur + 1, cards, eye);
	// 获取所有可拆解情况
	int op_num = 0;
	HUISU_ITEM* p = NULL;
	p = get_huisu_ops(n, &op_num);
	for (int i = 0; i < op_num; ++i)
	{
		HUISU_ITEM& pi = p[i];
		if (eye && p[i].eye) continue;
		if (cur + 1 >= 9 && pi.b>0) continue;
		if (cur + 2 >= 9 && pi.c>0) continue;
		if (pi.b>0 && pi.b > cards[cur + 1] || pi.c>0 && cards[cur + 2] < pi.c) continue;

		cards[cur] = 0;
		cards[cur + 1] -= pi.b;
		cards[cur + 2] -= pi.c;
		bool e = eye || pi.eye;
		bool result = cai_cur(cur + 1, cards, e);

		cards[cur] = p[i].a;
		cards[cur + 1] += pi.b;
		cards[cur + 2] += pi.c;
		if (result) {
			eye = e;
			return true;
		}
	}
	return false;
}

//这里的cards是含有九个元素的数组
bool check_color(char* cards, bool& eye)
{
	int sum = 0;
	for (int i = 0; i < 9; i++) {
		sum += cards[i];
	}
	if (sum == 0) return true;
	int yu = sum % 3;
	if (yu == 1) return false;
	if (yu == 2 && eye) return false;

	return cai_cur(0, cards, eye);
}

bool can_hu_huisu(char* cards) {
	// 有花牌不能胡
	for (int i = 35; i <= 42; ++i) {
		if (cards[i] != 0) return false;
	}
	bool eye = false;

	// 检查东西南北中发白
	if (!check_zi(cards, eye)) return false;

	//检查万
	if (!check_color(&cards[1], eye)) return false;

	//检查条
	if (!check_color(&cards[10], eye)) return false;

	//检查筒
	if (!check_color(&cards[19], eye)) return false;

	return true;
}





