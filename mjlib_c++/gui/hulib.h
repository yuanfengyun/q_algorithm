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
        ProbabilityItem m[4][5];
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

    static bool split(char* const cards, int gui_num, ProbabilityItemTable& ptbl);
    static bool _split(char* const cards, int gui_num, int color, int min, int max, bool chi, ProbabilityItemTable& ptbl);

    static bool check_probability(ProbabilityItemTable& ptbl, int gui_num);

    static bool check_probability_sub(ProbabilityItemTable& ptbl, bool& eye, int& gui_num, int level, int max_level);
    static bool check_7dui(char* const hand_cards);
    static bool check_pengpeng(char* const hand_cards, Wave* const waves, char self_card, char other_card);
};
