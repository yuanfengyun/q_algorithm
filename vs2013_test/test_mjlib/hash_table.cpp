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

    for(std::unordered_map<int,int>::iterator it=m_tbl.begin(); it!=m_tbl.end(); ++it)
    {
		fprintf(fp, "%d\n", *it);
    }

    fclose(fp);
}

void HashTable::load(char* name)
{
    FILE *fp = fopen(name, "rb");

	int key = 0;
	while (fscanf(fp, "%d", &key) != EOF)
	{
		m_tbl[key] = 1;
	}

    printf("load %s, set_size = %d\n", name, m_tbl.size());
    fclose(fp);
}
