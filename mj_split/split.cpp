#include "stdafx.h"
#include "split.h"

bool split::get_hu_info(char* cards, char cur_card, char gui_index)
{
	char gui_num = 0;
	if (gui_index != 34) {
		gui_num = cards[gui_index];
		cards[gui_index] = 0;
	}

	char eye_tbl[34];
	char eye_num = 0;
	char empty = -1;
	for (int i = 0; i < 34; i++) {
		// 优化手段，三不靠的牌，必做将
		int min = (i / 9) * 9;
		int max = min + 8;
		if(cards[i] == 1 &&
		   (i-2 < min || cards[i-2] == 0) &&
		   (i-1 < min || cards[i-1] == 0) &&
		   (i+1 > max || cards[i+1] == 0) &&
		   (i+2 > max || cards[i+2] == 0)){
			if (gui_num<0) {
				return false;
			}
		    eye_num = 1;
			eye_tbl[0] = i;
			empty = -1;
			break;
		}
		if (empty == -1 && cards[i] == 0) empty = i;
		if (cards[i] > 0 && cards[i] + gui_num>=2) {
			eye_tbl[eye_num++] = i;
		}
	}
	if (empty > 0) {
		eye_tbl[eye_num++] = empty;
	}

	bool hu = false;
	int cache[] = { 0,0,0,0 };
	for (int i = 0; i < eye_num; i++) {
		int eye = eye_tbl[i];
		if (eye == empty) {
			hu = foreach_eye(cards, gui_num - 2, 1000, cache);
		}
		else{
			char n = cards[eye];
			if (n == 1) {
				cards[eye] = 0;
				hu = foreach_eye(cards, gui_num - 1, eye/9, cache);
			}
			else if (n == 0) {
				hu = foreach_eye(cards, gui_num - 2, eye/9, cache);
			}
			else {
				cards[eye] -= 2;
				hu = foreach_eye(cards, gui_num, eye/9, cache);
			}
			cards[eye] = n;
		}
		if (hu) {
			break;
		}
	}

	if (gui_num > 0) {
		cards[gui_index] = gui_num;
	}
	return hu;
}

bool split::foreach_eye(char* cards, char gui_num, int eye_color, int* cache)
{
	int left_gui = gui_num;
	for (int i = 0; i < 3; i++) {
		int cache_index = -1;
		if (eye_color != i) cache_index = i;
		int old_gui = left_gui;
		left_gui = check_normal(cards, i*9, i*9+8, left_gui, cache_index, cache);
		if (cache_index>0) {
			cache[i] = old_gui - left_gui + 1;
		}
		if (left_gui < 0) {
			return false;
		}
	}

	int cache_index = -1;
	if (eye_color != 3) cache_index = 3;
	int old_gui = left_gui;
	left_gui = check_zi(cards, left_gui, cache_index, cache);
	if (cache_index>0) {
		cache[3] = old_gui - left_gui + 1;
	}
	if (left_gui < 0) {
		return false;
	}

	return true;
}

int split::check_normal(char* cards, int from, int to, int left_gui, int cache_index, int* cache)
{
	if (cache_index >= 0) {
		int need_gui = cache[cache_index];
		if (need_gui > 0) {
			return left_gui - (need_gui - 1);
		}
	}

	int n = 0;
	for (int i = from; i <= to; i++) {
		n = n * 10 + cards[i];
	}
	
	if (n == 0) return left_gui;

	bool n3 = false;
	for (int i = 0; i <= left_gui; i++) {
		if ((n + i) % 3 == 0) {
			n3 = true;
			break;
		}
	}

	if (!n3) {
		return -1;
	}

	return next_split(n, left_gui);
}

int split::next_split(int n, int gui_num)
{
	int c=0;
	while(true){
		if (n == 0) return gui_num;

		while (n > 0) {
			c = n % 10;
			n = n / 10;
			if (c != 0) break;
		}
		if (c == 1 || c == 4) {
			return one(n, gui_num);
		}
		else if (c == 2) {
			return two(n, gui_num);
		}
		else if (c == 3) {
			if (n == 0) return gui_num;
		}
	}
	return -1;
}

int split::one(int n, int gui_num)
{
	int need_gui = 0;
	int c1 = n % 10;
	int c2 = (n % 100) / 10;

	if (c1 == 0) ++need_gui;
	else n -= 1;

	if (c2 == 0) ++need_gui;
	else n -= 10;
	gui_num -= need_gui;

	if (n == 0 || gui_num < 0) return gui_num;

	return next_split(n, gui_num);
}

int split::two(int n, int gui_num)
{
	int c1 = n % 10;
	int c2 = (n % 100) / 10;
	int c3 = (n % 1000) / 100;

	bool choose_ke = true;
	if (c1 == 0) {
		// c1 == 0 全拆刻子
	}
	else if (c1 == 1)
	{
		// 刻子
		if (c2 == 0 || c2 == 1)
		{

		}
		else if (c2 == 2) {
			if (c3 == 0 || c3 == 1 || c3 == 3 || c3 == 4) {
				choose_ke = false;
			}
		}
		else if (c2 == 3) {
			if (c3 == 0 || c3 == 1)
			{
				choose_ke = false;
			}
		}
		else if (c2 == 4) {
			if (c3 == 3) {
				choose_ke = false;
			}
		}
	}
	else if (c1 == 2)
	{
		choose_ke = false;
	}
	else if (c1 == 3) {
		if (c2 == 2) {
			if (c3 == 1) {
				choose_ke = false;
			}
		}
		if (c2 == 3) {
			choose_ke = false;
		}
		else if (c2 == 4) {
			if (c3 == 2) {
				choose_ke = false;
			}
		}
	}
	else if (c1 == 4) {
		if (c2 == 2) {
			choose_ke = false;
		}
		else if (c2 == 3) {
			if (c3 == 2) {
				choose_ke = false;
			}
		}
	}

	int need_gui = 0;
	if (choose_ke) {
		need_gui = 1;
	}
	else
	{
		if (c1 < 2) {
			need_gui += (2 - c1);
			n -= c1;
		}
		else {
			n -= 2;
		}

		if (c2 < 2) {
			need_gui += (2 - c2);
			n -= c2;
		}
		else {
			n -= 20;
		}
	}

	gui_num -= need_gui;
	if (n == 0 || gui_num <0) return gui_num;
	return next_split(n, gui_num);
}

int split::check_zi(char* cards,  int gui_num, int cache_index, int* cache)
{
	if (cache_index >= 0) {
		int need_gui = cache[cache_index];
		if (need_gui > 0) {
			return gui_num - (need_gui - 1);
		}
	}

	int need_gui = 0;
	for (int i = 27; i < 34;i++) {
		int c = cards[i];
		if (c == 0) continue;
		if (c == 1 || c == 4) {
			need_gui = need_gui + 2;
		}
		else if (c == 2) {
			need_gui = need_gui + 1;
		}
	}

	return gui_num - need_gui;
}
