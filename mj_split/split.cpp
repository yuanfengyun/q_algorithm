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
		if (empty == -1 && cards[i] == 0) empty = i;
		if (cards[i] > 0 && cards[i] + gui_num>=2) {
			eye_tbl[eye_num++] = i;
		}
	}
	if (gui_num > 0) {
		eye_tbl[eye_num++] = empty;
	}

	bool hu = false;
	for (int i = 0; i < eye_num; i++) {
		int eye = eye_tbl[i];
		if (eye == empty) {
			hu = foreach_eye(cards, gui_num - 2);
		}
		else{
			char n = cards[eye];
			if (n == 1) {
				cards[eye] = 0;
				hu = foreach_eye(cards, gui_num - 1);
			}
			else if (n == 0) {
				hu = foreach_eye(cards, gui_num - 2);
			}
			else {
				cards[eye] -= 2;
				hu = foreach_eye(cards, gui_num);
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

bool split::foreach_eye(char* cards, char gui_num)
{
	int left_gui = gui_num;
	left_gui = check_normal(cards, 0, 8, left_gui);
	if (left_gui < 0) {
		return false;
	}
	left_gui = check_normal(cards, 9, 17, left_gui);
	if (left_gui < 0) {
		return false;
	}
	left_gui = check_normal(cards, 18, 26, left_gui);
	if (left_gui < 0) {
		return false;
	}

	left_gui = check_zi(cards, left_gui);
	if (left_gui < 0) {
	//	return false;
	}

	return true;
}

int split::check_normal(char* cards, int from, int to, int left_gui)
{
	int n = 0;
	for (int i = from; i <= to; i++) {
		n = n * 10 + cards[i];
	}
	
	if (n == 0) return left_gui;

	return next_split(n, left_gui);
}

int split::next_split(int n, int gui_num)
{
	int c=0;
	while (n > 0) {
		c = n % 10;
		n = n / 10;
		if (c != 0) break;
	}
	if (c == 1) return one(n, gui_num);
	else if(c == 2) return two(n, gui_num);
	else if (c == 3) {
		if (n == 0) return gui_num;
		return next_split(n, gui_num);
	}
	else if(c==4){
		return next_split(n*10+1, gui_num);
	}

	return -1;
}

int split::one(int n, int gui_num)
{
	int need_gui = 0;
	int c1 = n % 10;
	int c2 = (n % 100) / 10;

	if (c1 == 0) {
		need_gui = need_gui + 1;
	}
	else n = n - 1;

	if (c2 == 0) need_gui = need_gui + 1;
	else n = n - 10;
	if (need_gui > gui_num) return -1;
	gui_num -= need_gui;

	if (n == 0) return gui_num;

	return next_split(n, gui_num);
}

int split::two(int n, int gui_num){
	int need_gui = 0;
	int c1 = n % 10;
	int c2 = (n % 100) / 10;

	// 满足222
	if (c1 >= 2 && c2 >= 2) {
		n = n - 2 - 20;
		if (n == 0) return gui_num;
		return next_split(n, gui_num);
	}

	// 满足加一个鬼变成222
	if (c1 == 1 && c2 >= 2) {
		if (gui_num > 0) {
			n = n - 1 - 20;
			if (n == 0) return gui_num - 1;
			return next_split(n, gui_num - 1);
		}
		else 
			return -1;
	}

	// 满足加一个鬼变成222
	if (c1 >= 2 && c2 == 1) {
		if (gui_num > 0) {
			n = n - 2 - 10;
			if (n == 0) return gui_num - 1;
			return next_split(n, gui_num - 1);
		}
		else
			return -1;
	}

	if (gui_num == 0) return -1;
	if (n == 0) return gui_num - 1;
	return next_split(n, gui_num - 1);
}

int split::check_zi(char* cards,  int gui_num)
{
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
