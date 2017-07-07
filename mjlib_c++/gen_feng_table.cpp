#include <stdio.h>
#include <set>
#include "hulib.h"
#include "table_mgr.h"

void add(char* cards, int gui_num, bool eye)
{
    int count = 0;
    int key = 0;
    for(int i=9; i<16; ++i)
    {
        count += cards[i];
        key = key * 10 + cards[i];
    }

    TableMgr::get_instance()->add(key, gui_num, eye, false);
}

void parse_table_sub(char* cards, int num, bool eye)
{
    for(int i=9;i<16;++i)
    {
        if(cards[i] == 0) continue;

        --cards[i];

        add(cards, num, eye);

        if(num < 4)
        {
            parse_table_sub(cards, num+1, eye);
        }
        ++cards[i];
    }
}

void parse_table(char* cards)
{
    int count = 0;
    for(int i=9;i<16;++i)
    {
        count += cards[i];
    }

    bool eye = false;
    if(count%3 != 0)
    {
        eye = true;
    }

    add(cards, 0, eye);
    parse_table_sub(cards, 1, eye);
}

void check_hu(char* cards)
{
    static std::set<int> tested;

    for(int i=0;i<16;++i)
    {
        if(cards[i] > 4) return;
    }

    int num = 0;
    for(int i=9;i<16;++i)
    {
        num = num * 10 + cards[i];
    }

    if(tested.find(num) != tested.end())
    {
        return;
    }

    tested.insert(num);

    parse_table(cards);
}

void gen_auto_table_sub(char* cards, int level)
{
    for(int i=0;i<23;++i)
    {
        int index = -1;
        if(i < 16)
        {
            cards[i] += 3;
        }
        else
        {
            index = i - 16;
        }

        if(index >= 0)
        {
            cards[index] += 1;
            cards[index + 1] += 1;
            cards[index + 2] += 1;
        }

        if(level == 4)
        {
            check_hu(cards);
        }
        else
        {
            gen_auto_table_sub(cards, level + 1);
        }

        if(i < 16)
        {
            cards[i] -= 3;
        }
        else
        {
            cards[index] -= 1;
            cards[index + 1] -= 1;
            cards[index + 2] -= 1;
        }
    }
}

void gen_auto_table()
{
    char cards[34] = {0};

    for(int i=0; i<16; ++i)
    {
        cards[i] = 2;
        printf("将 %d\n", i+1);
        gen_auto_table_sub(cards, 1);
        cards[i] = 0;
    }

    TableMgr::get_instance()->dump_feng_table();
}

int main()
{
  printf("generate feng table begin...\n");
  gen_auto_table();

  return 0;
}
