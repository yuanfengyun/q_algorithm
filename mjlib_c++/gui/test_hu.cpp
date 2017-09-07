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

    return 0;
}
