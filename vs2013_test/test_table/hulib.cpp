#include <string.h>
#include "hulib.h"
#include "table_mgr.h"

bool HuLib::get_hu_info(char* const hand_cards, Wave* const waves, char self_card_index, char other_card_index, int gui_index)
{
    char hand_cards_tmp[34];
    memcpy(hand_cards_tmp, hand_cards, 34);

    if(self_card_index != 34)
    {
        hand_cards_tmp[self_card_index]++;
    }
    else if(other_card_index != 34)
    {
        hand_cards_tmp[other_card_index]++;
    }

    int gui_num = hand_cards_tmp[gui_index];
    hand_cards_tmp[gui_index] = 0;

	return split(hand_cards_tmp, gui_num);
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
	
	gui = _split(cards, gui_num, 0, 8, true, eye_num);
	if (!check(gui, eye_num, gui_num, gui_sum)) return false;
	
	gui = _split(cards, gui_num-gui_sum, 9, 17, true, eye_num);
	if (!check(gui, eye_num, gui_num, gui_sum)) return false;

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
		if (TableMgr::get_instance()->check(key, i, eye, chi))
		{
			if (eye) eye_num++;
			return i;
		}
	}

    return -1;
}

