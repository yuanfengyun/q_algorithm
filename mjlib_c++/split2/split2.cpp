#include "split2.h"
#include "string.h"

bool split2::get_hu_info(char* hand_cards, char cur_card, char gui_index)
{
	char cards[34];
	memcpy(cards, hand_cards, 34);
	char gui_num = 0;
	if (gui_index != 34) {
		gui_num = cards[gui_index];
		cards[gui_index] = 0;
	}
	int used_gui = 0;
	int cache[] = { 0,0,0,0 };
	int counter[] = { 0,0,0,0 };
	cache[0] = check_normal(cards, 0, gui_num, used_gui);
	used_gui = cache[0];
	if(used_gui > 1 + gui_num) return false;

	cache[1] = check_normal(cards, 9, gui_num, used_gui);
	used_gui = cache[0] + cache[1];
	if (used_gui > 1 + gui_num) return false;

	cache[2] = check_normal(cards, 18, gui_num, used_gui);
	used_gui = cache[0] + cache[1] + cache[2];
	if (used_gui > 1 + gui_num) return false;

	cache[3] = check_zi(cards);
	int need_sum = cache[0] + cache[1] + cache[2] + cache[3];
	if (need_sum > 1 + gui_num) return false;

	if (need_sum + 2 <= gui_num){
		return true;
	}

	int eye_color = -1;

	// 对赖子需求超过赖子数量的，只能做将
	for (int i = 0; i < 4;i++)
	{
		int n = cache[i];
		if (n > gui_num){
			// 另一花色也不满足
			if (eye_color >= 0) return false;
			// 其它花色对赖子需求大于赖子数量
			if (need_sum - n > gui_num) return false;
			// 扣除将以后赖子还不够的
			if (need_sum - 1 > gui_num) return false;
			eye_color = i;
			break;
		}
	}

	if (eye_color > 0){
		if (eye_color == 3){
			return true;
		}
		else{
			return check_color(cards, eye_color * 9, gui_num-(need_sum - cache[eye_color]));
		}
	}

	bool hu = false;
	for (int i = 0; i < 4; ++i)
	{
		if (cache[i] == 0) continue;
		if (i == 3){
			return true;
		}
		else{
			hu = check_color(cards, i * 9, gui_num-(need_sum - cache[i]));
		}
		if(hu) return true;
	}

	return hu;
}

bool split2::check_color(char* cards, char from, char gui_num)
{
	char eye_tbl[9];
	char eye_num = 0;
	for (int i = from; i < from+9; i++) {
		if (cards[i] > 0 && cards[i] + gui_num >= 2) {
			eye_tbl[eye_num++] = i;
		}
	}

	if (eye_num == 0) return false;

	for (int i = 0; i < eye_num; i++) {
		int eye = eye_tbl[i];
		char n = cards[eye];
		if (n == 1) {
			cards[eye] = 0;
			int need_gui = check_normal(cards, from, gui_num - 1, 0);
			if (need_gui < gui_num) return true;
		}
		else {
			cards[eye] -= 2;
			int need_gui = check_normal(cards, from, gui_num, 0);
			if (need_gui <= gui_num) return true;
		}
		
		cards[eye] = n;
	}

	return false;
}

int split2::check_normal(char* cards, int from, int max_gui, int used_gui)
{
	char index = 0;
	char cards_tmp[9];
	int n = 0;
	for (int i = from; i <= from + 8; i++) {

		n = n * 10 + cards[i];
		cards_tmp[index++] = cards[i];
	}
	
	if (n == 0) return 0;

	return next_split(cards_tmp, 0, max_gui, used_gui);
}

int split2::next_split(char* cards, int need_gui, int max_gui, int used_gui)
{
	int index = 0;
	while(index < 9){
		int c = cards[index++];
		if (c == 0) continue;
				
		if (c == 1 || c == 4) {
			one(cards, index, need_gui, max_gui, used_gui);
		}
		else if (c == 2) {
			two(cards, index, need_gui, max_gui, used_gui);
		}
	}
	return need_gui;
}

void split2::one(char* cards, int& index, int& need_gui, int max_gui, int used_gui)
{
	int c1 = cards[index];
	int c2 = cards[index+1];

	if (c1 == 0) ++need_gui;
	else cards[index]--;

	if (c2 == 0) ++need_gui;
	else cards[index+1]--;

	if (need_gui + used_gui > max_gui + 1) {
		index = 9;
		need_gui = 1000;
	}
}

void split2::two(char* cards, int& index, int& need_gui, int max_gui, int used_gui)
{
	int c1 = cards[index];;
	int c2 = cards[index+1];
	int c3 = cards[index+2];
	int c4 = cards[index+3];

	bool choose_ke = true;
	if (c1 == 0) {
	}
	else if (c1 == 1) {
		if (c2 == 0 || c2 == 1)
		{
		}
		else if (c2 == 2) {
			if (c3 == 2) {
				if (c4 == 2) choose_ke = false;
			}
			else if (c3 == 3) {
				if (c4 != 2) choose_ke = false;
			}
			else
				choose_ke = false;
		}
		else if (c2 == 3) {
			if (c3 == 0 || c3 == 2 || c3 == 1 || c3 == 4)
				choose_ke = false;
		}
		else if (c2 == 4) {
			if (c3 == 2) {
				if (c4 == 2 || c4 == 3 || c4 == 4)
					choose_ke = false;
			}
			else if (c3 == 3)
			{
				choose_ke = false;
			}
		}
	}
	else if (c1 == 2) {
		choose_ke = false;
	}
	else if (c1 == 3) {
		if (c2 == 2) {
			if (c3 == 1 || c3 == 4)
				choose_ke = false;
			else if (c3 == 2) {
				if (c4 != 2)
					choose_ke = false;
			}
		}
		else if (c2 == 3)
			choose_ke = false;
		else if (c2 == 4)
		{
			if (c3 == 2) choose_ke = false;
		}
	}
	else if (c1 == 4) {
		if (c2 == 2 && c3 != 2)
			choose_ke = false;
		else if (c2 == 3) {
			if (c3 == 0 || c3 == 1 || c3 == 2)
				choose_ke = false;
		}
		else if (c2 == 4) {
			if (c3 == 2) choose_ke = false;
		}
	}

	if (choose_ke) {
		need_gui += 1;
	}
	else
	{
		if (c1 < 2) {
			need_gui += (2 - c1);
			cards[index] -= c1;
		}
		else {
			cards[index] -= 2;
		}

		if (c2 < 2) {
			need_gui += (2 - c2);
			cards[index+1] -= c2;
		}
		else {
			cards[index+1] -= 2;
		}
	}

	if (need_gui + used_gui > max_gui + 1) {
		index = 9;
	}
}

int split2::check_zi(char* cards)
{
	int need_gui = 0;
	for (int i = 27; i < 34;i++) {
		if (cards[i] == 0) continue;
		if (cards[i] == 1 || cards[i] == 4) {
			need_gui += 2;
		}
		else if (cards[i] == 2) {
			++need_gui;
		}
	}

	return need_gui;
}
