#include <stdio.h>
#include <set>
#include "hulib.h"
#include "wave_table.h"
#include "wave_eye_table.h"

void print_cards(char* cards)
{
    for(int i=0;i<34;++i)
    {
          printf("%d,", cards[i]);
    }
    printf("\n");
}

void check_hu(char* cards)
{
    static std::set<int> tested;

    for(int i=0;i<18;++i)
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

    if(!HuLib::get_hu_info(cards, NULL, 0, 0))
    {
        printf("测试失败 \n");
        print_cards(cards);
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
            check_hu(cards);
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
        0,1,1,3,0,0,0,0,0,
        0,1,1,1,0,0,1,1,1,
        0,0,0,0,0,1,1,1,0,
        0,0,0,0,0,0,0
    };

    if(!HuLib::get_hu_info(cards, NULL, 0, 0))
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
    WaveTable::getInstance()->load("wave_table.data");
    WaveEyeTable::getInstance()->load("wave_eye_table.data");

    //test_two_color();
    test_one();
    return 0;
}
