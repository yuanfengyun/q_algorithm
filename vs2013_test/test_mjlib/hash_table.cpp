#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "hash_table.h"

HashTable::HashTable()
{
}

HashTable::~HashTable()
{
}

bool HashTable::check(int number)
{
    std::unordered_map<int,int>::iterator it = m_tbl.find(number);
    return it != m_tbl.end();
}

void HashTable::add(int key)
{
    m_tbl[key] = 1;
}

void HashTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");

    int num = m_tbl.size();

    fwrite(&num, 4, 1, fp);

    for(std::unordered_map<int,int>::iterator it=m_tbl.begin(); it!=m_tbl.end(); ++it)
    {
        int key = it->second;
        fwrite(&key, 4, 1, fp);
    }

    fclose(fp);
}

void HashTable::load(char* name)
{
    FILE *fp = fopen(name, "rb");

    int num = 0;

    fread(&num, 4, 1, fp);

    for(int i=0; i<num; ++i)
    {
        int key = 0;
        fread(&key, 4, 1, fp);
        m_tbl[key]=1;
    }

    printf("load %s %d key, set_size = %d\n", name, num, m_tbl.size());
    fclose(fp);
}
