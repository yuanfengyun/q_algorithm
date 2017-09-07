#include <stdio.h>
#include <string.h>
#include "array_table.h"

ArrayTable::ArrayTable()
{
    memset(m_tbl, 0, sizeof(m_tbl));
}

ArrayTable::~ArrayTable()
{
}

bool ArrayTable::check(int number)
{
    return m_tbl;
}

void ArrayTable::add(int key)
{
    m_tbl[key] = true;
}

void ArrayTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");

    for(int i=0;i<(2<<27);i++)
    {
        if(m_tbl[i]){
            fprintf(fp, "%d\n", m_tbl[i]);
        }
    }

    fclose(fp);
}

void ArrayTable::load(char* name)
{
    FILE *fp = fopen(name, "rb");

    int num = 0;
    int key = 0;
    while(fscanf(fp, "%d\n", &key)!=EOF)
    {
        num++;
        add(key);
    }

    printf("load %s, set_size = %d\n", name, num);
    fclose(fp);
}
