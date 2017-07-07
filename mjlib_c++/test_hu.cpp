#include <stdio.h>
#include <stdlib.h>
#include <set>
#include "hulib.h"
#include "table_mgr.h"

void print_cards(char* cards)
{
    for(int i=0;i<9;++i)
    {
          printf("%d,", cards[i]);
    }
    printf("\n");

    for(int i=9;i<18;++i)
    {
          printf("%d,", cards[i]);
    }
    printf("\n");
    for(int i=18;i<27;++i)
    {
          printf("%d,", cards[i]);
    }
    printf("\n");
    for(int i=27;i<34;++i)
    {
          printf("%d,", cards[i]);
    }
    printf("\n");
}

void check_hu(char* cards, int max_gui_index)
{
    static std::set<int> tested;

    for(int i=0;i<max_gui_index;++i)
    {
        if(cards[i] > 4) return;
    }

    int num = 0;
    for(int i=0;i<9;++i)
    {
        num = num * 10 + cards[i];
    }

    if(tested.find(num) != tested.end())
    {
        return;
    }

    tested.insert(num);

    for(int i=0; i<max_gui_index; ++i)
    {
        if(!HuLib::get_hu_info(cards, NULL, 0, 0, i))
        {
            printf("测试失败 i=%d\n",i);
            print_cards(cards);
            abort();
        }
    }
}

void gen_auto_table_sub(char* cards, int level)
{
    for(int i=0;i<32;++i)
    {
        int index = -1;
        if(i <= 17)
        {
            cards[i] += 3;
        }
        else if(i <= 24)
        {
            index = i - 18;
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
            check_hu(cards, 18);
        }
        else
        {
            gen_auto_table_sub(cards, level + 1);
        }

        if(i <= 17)
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

void test_two_color()
{
    printf("测试两种花色\n");
    char cards[34] = {0};

    for(int i=0; i<18; ++i)
    {
        cards[i] = 2;
        printf("将 %d\n", i+1);
        gen_auto_table_sub(cards, 1);
        cards[i] = 0;
    }
}

void test_one()
{
    char cards[34] = {
        0,0,0,0,0,0,0,0,0,  // 万
        0,1,1,1,0,0,2,0,0,  // 条
        0,1,1,1,3,0,0,0,0,  // 筒
        0,0,0,0,0,0,0
    };

    printf("测试1种\n");
    print_cards(cards);
    if(!HuLib::get_hu_info(cards, NULL, 34, 34, 31))
    {
        printf("测试失败\n");
    }
    else
    {
        printf("测试成功\n");
    }
}

int main()
{
    printf("test hulib begin...\n");

    TableMgr::get_instance()->load();

    test_one();

//    test_two_color();

    return 0;
}
