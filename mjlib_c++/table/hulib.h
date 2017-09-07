#pragma once

#include <string.h>

struct Wave
{
};

class HuLib
{
public:
    static bool get_hu_info(char* const hand_cards, char cur_card, int gui_index);
private:
	static bool split(char* const cards, int gui_num);
	static int _split(char* const cards, int gui_num, int min, int max, bool chi, int& eye_num);
};
