#include <stdio.h>
#include <string.h>
#include "set_table.h"

SetTable::SetTable()
{
}

SetTable::~SetTable()
{
}

bool SetTable::check(int number)
{
    std::set<int>::iterator it = m_tbl.find(number);
    return it != m_tbl.end();
}

void SetTable::add(int key)
{
    m_tbl.insert(key);
}

void SetTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");


    for(std::set<int>::iterator it=m_tbl.begin(); it!=m_tbl.end(); ++it)
    {
        fprintf(fp, "%d\n", *it);
    }

    fclose(fp);
}

void SetTable::load(char* name)
{
    FILE *fp = fopen(name, "rb");

    int key = 0;
    while(fscanf(fp, "%d\n", key)!=EOF)
    {
        m_tbl.insert(key);
    }

    printf("load %s, set_size = %d\n", name, m_tbl.size());
    fclose(fp);
}
