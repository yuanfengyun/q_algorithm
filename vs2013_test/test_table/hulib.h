#pragma once

#include <string.h>

struct Wave
{
};

class HuLib
{
    struct ProbabilityItem
    {
        bool eye;
        int gui_num;
    };

    struct ProbabilityItemTable
    {
        ProbabilityItem m[4][9];
        int array_num;
        int m_num[4];
        ProbabilityItemTable()
        {
            memset(this, 0, sizeof(*this));
        }
    };
    
public:
    static bool get_hu_info(char* const hand_cards, Wave* const waves, char self_card, char other_card, int gui_index);
private:

    static bool list_probability(int color, int gui_num, int num, int key, bool chi, ProbabilityItemTable& ptbl);
	static bool split(char* const cards, int gui_num);

	static int _split(char* const cards, int gui_num, int min, int max, bool chi, int& eye_num);
};
