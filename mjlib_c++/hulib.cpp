#include <stdio.h>
#include <string.h>
#include "hulib.h"
#include "table_mgr.h"

#define LOG printf
//#define LOG log

void log(char*, ...)
{
}

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

    ProbabilityItemTable ptbl;
    if(!split(hand_cards_tmp, gui_num, ptbl))
    {
        return false;
    }

    return check_probability(ptbl, gui_num);
}

bool HuLib::split(char* const cards, int gui_num, ProbabilityItemTable& ptbl)
{
    if(!_split(cards, gui_num, 0,  0,  8, true,  ptbl)) return false;
    if(!_split(cards, gui_num, 1,  9, 17, true,  ptbl)) return false;
    if(!_split(cards, gui_num, 2, 18, 26, true,  ptbl)) return false;
    if(!_split(cards, gui_num, 3, 27, 33, false, ptbl)) return false;

    return true;
}

bool HuLib::_split(char* const cards, int gui_num, int color, int min, int max, bool chi, ProbabilityItemTable& ptbl)
{
    int key = 0;
    int num = 0;

    for(int i = min; i <= max; ++i)
    {
        key = key*10 + cards[i];
        num = num + cards[i];
    }


    if(num > 0)
    {
        if(!list_probability(color, gui_num, num, key, chi, ptbl))
        {
            return false;
        }
    }

    return true;
}

bool HuLib::list_probability(int color, int gui_num, int num, int key, bool chi, ProbabilityItemTable& ptbl)
{
    int anum = ptbl.array_num;
    for(int i=0; i<=gui_num; ++i)
    {
        int yu = (num + i)%3;
        if(yu == 1) continue;
        bool eye = (yu == 2);
        if(TableMgr::get_instance()->check(key, i, eye, chi))
        {
            ProbabilityItem& item = ptbl.m[anum][ptbl.m_num[anum]];
            ptbl.m_num[anum]++;

            item.eye = eye;
            item.gui_num = i;
        }
    }

    LOG("gui_num = %d, color = %d key = %d num = %d pra_num=%d\n", gui_num, color, key, num, ptbl.m_num[anum]);

    if(ptbl.m_num[anum] <= 0)
    {
        return false;
    }

    ptbl.array_num++;
    return true;
}

bool HuLib::check_probability(ProbabilityItemTable& ptbl, int gui_num)
{
    LOG("组合 array_num = %d\n", ptbl.array_num);
    // 全是鬼牌
    if(ptbl.array_num == 0)
    {
        return gui_num >= 2;
    }

    // 只有一种花色的牌的鬼牌
    if(ptbl.array_num == 1) return true;

    // 尝试组合花色，能组合则胡
    for(int i=0; i<ptbl.m_num[0]; ++i)
    {
        ProbabilityItem& item = ptbl.m[0][i];
        bool eye = item.eye;
        int gui = gui_num - item.gui_num;
        if(check_probability_sub(ptbl, eye, gui, 1, ptbl.array_num))
        {
            return true;
        }
    }
    return false;
}

bool HuLib::check_probability_sub(ProbabilityItemTable& ptbl, bool& eye, int& gui_num, int level, int max_level)
{
    for(int i=0; i<ptbl.m_num[level]; ++i)
    {
        ProbabilityItem& item = ptbl.m[level][i];

        if(eye && item.eye) continue;

        if(gui_num < item.gui_num) continue;

        if(level < max_level - 1)
        {
            int old_gui_num = gui_num;
            bool old_eye = eye;
            gui_num -= item.gui_num;
            eye = eye || item.eye;

            if(check_probability_sub(ptbl, eye, gui_num, level + 1, ptbl.array_num))
            {
                return true;
            }

            eye = old_eye;
            gui_num = old_gui_num;
            continue;
        }

        if(!eye && !item.eye && item.gui_num + 2 > gui_num) continue;
        return true;
    }

    return false;
}

bool HuLib::check_7dui(char* const cards)
{
    int c = 0;
    for(int i=0;i<34;++i)
    {
        if(cards[i] % 2 != 0) return false;
        c += cards[i];
    }

    if(c != 34) return false;

    return true;
}

bool HuLib::check_pengpeng(char* const cards, Wave* const waves, char self_card, char other_card)
{
    bool eye = false;

    for(int i=0;i<34;++i)
    {
        int c = cards[i];
        if(c == 1 || c == 4) return false;

        if(c == 2)
        {
            if(eye) return false;
            eye = true;
        }
    }

    // 检查吃碰杠的牌

    return true;
}

