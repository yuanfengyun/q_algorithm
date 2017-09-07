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

bool ArrayTable::check(int key)
{
	unsigned char v = m_tbl[key >> 3];
	return v & (1 << (key & 7));
}

void ArrayTable::add(int key)
{
	unsigned char old = m_tbl[key>>3];
	old |= (1 << (key & 7));
	m_tbl[key >> 3] = old;
}

void ArrayTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");


    for(int i=0;i<444444444;i++)
    {
        fprintf(fp, "%d\n", m_tbl[i]);
    }

    fclose(fp);
}

void ArrayTable::load(char* name)
{
	printf("load tbl %s\n", name);
    FILE *fp = fopen(name, "rb");

    int num = 0;
    int key = 0;
    while(fscanf(fp, "%d\n", &key)!=EOF)
    {
//		printf("load key %d\n", key);
        num++;
        add(key);
    }

    printf("load %s, set_size = %d\n", name, num);
    fclose(fp);
}
