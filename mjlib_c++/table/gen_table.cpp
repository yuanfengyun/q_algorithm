#include <stdio.h>
#include <set>
#include <map>
#include "hulib.h"
#include "table_mgr.h"

std::map<int, bool> gui_tested[9];
std::map<int, bool> gui_eye_tested[9];

bool check_add(int cards[], int gui_num, bool eye)
{
    int key = 0;

    for (int i=0; i<9; i++) {
        key = key * 10 + cards[i];
    }

    if (key == 0)
    {
        return false;
    }

    std::map<int, bool>* m;
    if (!eye) {
        m = &gui_tested[gui_num];
    } else {
        m = &gui_eye_tested[gui_num];
    }
    if(m->find(key) != m->end()){
        return false;
    }

    (*m)[key] = true;

    for (int i=0; i<9; i++){
        if (cards[i] > 4) {
            return true;
        }
    }
    TableMgr::get_instance()->add(key, gui_num, eye, true);
    return true;
}

static void parse_table_sub(int cards[], int num, bool eye) {
    for (int i=0; i<9; i++) {
        if (cards[i] == 0) {
             continue;
        }

        cards[i]--;

        if (!check_add(cards, num, eye)) {
            cards[i]++;
            continue;
        }

        if (num < 8) {
            parse_table_sub(cards, num + 1, eye);
        }
        cards[i]++;
    }
}

static void parse_table(int cards[], bool eye) {
    if (!check_add(cards, 0, eye)) {
        return;
    }
    parse_table_sub(cards, 1, eye);
}

void gen_auto_table_sub(int cards[], int level, bool eye)
{
    for(int i=0;i<16;++i)
    {
        if(i <= 8)
        {
            if(cards[i] > 3) continue;
            cards[i] += 3;
        }
        else
        {
            int index = i - 9;
            if(cards[index]>5 || cards[index+1]>5 || cards[index+2]>5) continue;
            cards[index] += 1;
            cards[index+1] += 1;
            cards[index+2] += 1;
        }

        parse_table(cards, eye);
        if(level<4)
        {
            gen_auto_table_sub(cards, level + 1, eye);
        }

        if(i <= 8)
        {
            cards[i] -= 3;
        }
        else
        {
            int index = i - 9;
            cards[index] -= 1;
            cards[index + 1] -= 1;
            cards[index + 2] -= 1;
        }
    }
}

void gen_table()
{
    int cards[34] = {0};

    gen_auto_table_sub(cards, 1, false);
}

void gen_eye_table()
{
    int cards[34] = {0};

    for(int i=0; i<9; ++i)
    {
        printf("eye %d\n",i);
        cards[i] = 2;
        parse_table(cards, true);
        gen_auto_table_sub(cards, 1, true);
        cards[i] = 0;
    }
}

int main()
{
    printf("generate table begin...\n");
    gen_eye_table();
    gen_table();
    TableMgr::get_instance()->dump_table();

    return 0;
}
