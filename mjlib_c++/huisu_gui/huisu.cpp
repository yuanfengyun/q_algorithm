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
	{1,1,1,false},
	{2,0,0,true}
};

HUISU_ITEM huisu_items2[] = {
	{2,0,0,true},
	{2,2,2,false},
	{3,0,0}
};

HUISU_ITEM huisu_items3[] = {
	{3,0,0,false},
	{3,1,1,true}
};

HUISU_ITEM huisu_items4[] = {
	{4,1,1,false},
	{4,2,2,true},
	{5,0,0,true},
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
int check_zi(char* cards, int gui_num, bool& eye)
{
	int n = 0;
	int eye_num = 0;
	for (int i = 27; i < 34; ++i) {
		if (cards[i] == 0) continue;

		if (cards[i] == 1 || cards[i] == 4) {
			++n;
			++eye_num;
		}
		else if (cards[i] == 2) {
			++eye_num;
		}
	}

	if (eye_num > 1) {
		n = n + eye_num - 1;
		eye_num = 1;
	}

	eye = eye_num;

	return n;
}

int cai_cur(int cur, char* cards, bool& eye, int total_gui)
{
	if (cur > 8) return 0;

	int n = cards[cur];
	if (n == 0) return cai_cur(cur + 1, cards, eye, total_gui);
	// 获取所有可拆解情况
	int op_num = 0;
	HUISU_ITEM* p = NULL;
	p = get_huisu_ops(n, &op_num);
	for (int i = 0; i < op_num; ++i)
	{
		HUISU_ITEM& pi = p[i];
		if (eye && p[i].eye) continue;

		int old_cur = cards[cur];
		int old_cur1 = cards[cur+1];
		int old_cur2 = cards[cur+2];

		cards[cur] -= pi.a;
		cards[cur + 1] -= pi.b;
		cards[cur + 2] -= pi.c;
		int gui = 0;
		if (cards[cur] < 0) {
			gui += -cards[cur];
			cards[cur] = 0;
		}
		if (cards[cur+1] < 0) {
			gui += -cards[cur+1];
			cards[cur+1] = 0;
		}
		if (cards[cur+2] < 0) {
			gui += -cards[cur+2];
			cards[cur+2] = 0;
		}
		bool e = eye || pi.eye;
		int result = 1000;
		
		if (gui <= total_gui)
		{
			result = cai_cur(cur + 1, cards, e, total_gui - gui);
		}

		cards[cur] = old_cur;
		cards[cur + 1] = old_cur1;
		cards[cur + 2] = old_cur2;
		if (result!=1000) {
			eye = e;
			return gui+result;
		}
	}
	return 1000;
}

//这里的cards是含有九个元素的数组
int check_color(char* color_cards, bool& eye,int total_gui)
{
	// 为拆顺子多留两个空间
	char cards[11] = {0};
	memcpy(cards, color_cards, 9);

	int sum = 0;
	for (int i = 0; i < 9; i++) {
		sum += cards[i];
	}

	// 没有可用赖子时
	if (total_gui == 0) {
		int yu = sum % 3;
		if (yu == 1) return 1000;
		if (eye && yu == 2) return 1000;
	}

	for (int i = 0; i <= total_gui;++i)
	{
		int yu = (i + sum) % 3;
		if (yu == 1) continue;
		bool cur_eye = eye;
		if (cai_cur(0, cards, cur_eye,i)!=1000) {
			eye = cur_eye;
			return i;
		}
	}
	return 1000;
}

bool can_hu_huisu(char* hand_cards, int gui_index1, int gui_index2){
	char cards[34];
	memcpy(cards, hand_cards, sizeof(cards));

	// 统计赖子数量
	int gui_num = 0;
	if (gui_index1 != 34) {
		gui_num += cards[gui_index1];
		cards[gui_index1] = 0;
	}
	if (gui_index2 != 34) {
		gui_num += cards[gui_index2];
		cards[gui_index2] = 0;
	}

	bool eye = 0;

	// 检查东西南北中发白
	int need_gui1 = check_zi(cards, gui_num, eye);
	if(need_gui1>gui_num) return false;

	//检查万
	int need_gui2 = check_color(&cards[0], eye, gui_num - need_gui1);
	if (need_gui1+need_gui2>gui_num) return false;

	//检查条
	int need_gui3 = check_color(&cards[9], eye, gui_num - need_gui1 - need_gui2);
	if (need_gui1 + need_gui2 + need_gui3 > gui_num) return false;

	//检查筒
	int need_gui4 = check_color(&cards[18], eye, gui_num - need_gui1 - need_gui2 - need_gui3);
	if (need_gui1 + need_gui2 + need_gui3 + need_gui4 > gui_num) return false;

	return true;
}





