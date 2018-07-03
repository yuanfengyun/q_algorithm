#include "stdafx.h"

#include <string.h>
#include "hulib.h"
#include "table_mgr.h"

bool HuLib::get_hu_info(char* cards, int gui_index1,int gui_index2)
{
	int gui_num1 = 0;
	int gui_num2 = 0;
	if (gui_index1 != 34) gui_num1 = cards[gui_index1];
	cards[gui_index1] = 0;

	if (gui_index2 != 34) gui_num2 = cards[gui_index2];
	cards[gui_index2] = 0;

	bool hu = split(cards, gui_num1+ gui_num2);

	if (gui_index1 != 34) cards[gui_index1] = gui_num1;
	if (gui_index2 != 34) cards[gui_index2] = gui_num2;
	return hu;
}

bool check(int gui, int eye_num, int gui_num, int& gui_sum) {
	if (gui < 0) return false;

	gui_sum += gui;
	if (gui_sum > gui_num) return false;

	if (eye_num == 0) return true;

	return gui_sum + (eye_num - 1) <= gui_num;
}

bool HuLib::split(char* const cards, int gui_num)
{
	int eye_num = 0;
	int gui_sum = 0;
	int gui = 0;
	// 检查万
	gui = _split(cards, gui_num, 0, 8, true, eye_num);
	if (!check(gui, eye_num, gui_num, gui_sum)) return false;
	// 检查筒
	gui = _split(cards, gui_num-gui_sum, 9, 17, true, eye_num);
	if (!check(gui, eye_num, gui_num, gui_sum)) return false;
	// 检查条
	gui = _split(cards, gui_num-gui_sum, 18, 26, true, eye_num);
	if (!check(gui, eye_num, gui_num, gui_sum)) return false;

	gui = _split(cards, gui_num-gui_sum, 27, 33, false, eye_num);
	if (!check(gui, eye_num, gui_num, gui_sum)) return false;

	if (eye_num == 0)
	{
		return gui_sum + 2 <= gui_num;
	}

	return true;
}

int HuLib::_split(char* const cards, int gui_num, int min, int max, bool chi, int& eye_num)
{
    int key = 0;
    int num = 0;

    for(int i = min; i <= max; ++i)
    {
        key = key*10 + cards[i];
        num = num + cards[i];
    }

	if (num == 0) return 0;

	for (int i = 0; i <= gui_num; ++i)
	{
		int yu = (num + i) % 3;
		if (yu == 1) continue;
		bool eye = (yu == 2);
		if (TableMgr::check(key, i, eye, chi))
		{
			if (eye) eye_num++;
			return i;
		}
	}

    return -1;
}

