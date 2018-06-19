#include <vector>
#include "paileizuhe.h"


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

//这里的cards是含有九个元素的数组
bool check_color(char* cards, bool& eye)
{
	struct T {
		int t;	//
		int i;	//是哪张牌
		T(int t,int i) {
			this->t = t;
			this->i = i;
		}
	};
	std::vector<T> items;
	//获取所有组合
	for (int i = 0; i < 9; ++i)
	{
		if (i == 0) continue;

		// 1张牌只存在 顺子的可能
		if (i == 1) {
			items.push_back(T(1,i));
		}
		// 2张牌 两个顺子
		else if (i == 2) {
			items.push_back(T(1, i));
			items.push_back(T(1, i));
		}
		// 3张牌 1个顺子和1个刻子
		else if (i == 3) {
			items.push_back(T(2, i));
			items.push_back(T(1, i));
		}
		// 4张牌 2个顺子和1个刻子
		else if (i == 3) {
			items.push_back(T(2, i));
			items.push_back(T(1, i));
			items.push_back(T(1, i));
		}
	}

	// 排列组合，选出3*n + 2里的n组牌，进行组合，如果还剩下1对将，则能胡
}

bool can_hu(char* cards)
{
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