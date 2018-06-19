#include "stdafx.h"
#include <map>
#include "hulib.h"

std::map<int,int> table;

int get_key(char* cards)
{
	int key = 0;
	for (int i = 0; i < 9; ++i)
	{
		key = key * 10 + cards[i];
	}
	return key;
}

void add_to_table(char* cards)
{
	table[get_key(cards)] = true;
}

void gen_table_sub(char* cards, int level)
{
	for (int i = 0; i < 16; ++i) {
		if (i < 9) {
			if (cards[i]>1) continue;

			cards[i] += 3;
		}
		else {
			if (cards[i - 9] > 3 || cards[i - 9 + 1] > 3 || cards[i - 9 + 2] > 3) continue;

			cards[i - 9] += 1;
			cards[i - 9 + 1] += 1;
			cards[i - 9 + 2] += 1;
		}

		add_to_table(cards);
		if (level < 4) gen_table_sub(cards,level+1);

		if (i < 9) {
			cards[i] -= 3;
		}
		else {
			cards[i - 9] -= 1;
			cards[i - 9 + 1] -= 1;
			cards[i - 9 + 2] -= 1;
		}
	}
}

void init_table()
{
	char cards[] = { 0,0,0,0,0,0,0,0,0 };
	gen_table_sub(cards,1);

	for (int i = 0; i < 9; ++i) {
		cards[i] += 2;
		gen_table_sub(cards, 1);
		cards[i] -= 2;
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

	if (table.find(get_key(cards)) == table.end()) return false;

	if (yu == 2) eye = true;
	return true;
}

bool can_hu(char* cards)
{
	// 有花牌不能胡
	for (int i = 35; i <= 42; ++i) {
		if (cards[i] != 0) return false;
	}

	bool eye = false;

	if (!check_zi(cards, eye)) {
		return false;
	}

	// 如果已经有将，返回三种花色的拆分结果
	//检查万
	if (!check_color(&cards[1], eye)) return false;

	//检查条
	if (!check_color(&cards[10], eye)) return false;

	//检查筒
	if (!check_color(&cards[19], eye)) return false;

	return true;
}


