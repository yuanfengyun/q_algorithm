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
    std::hash_map<int,int>::iterator it = m_tbl.find(number);
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

	for (std::hash_map<int, int>::iterator it = m_tbl.begin(); it != m_tbl.end(); ++it)
    {
        int key = it->second;
        fwrite(&key, 4, 1, fp);
    }

    fclose(fp);
}

void HashTable::load(char* name)
{
    FILE *fp = fopen(name, "rb");

	int key = 0;
	int num = 0;
	while (fscanf(fp, "%d\n", &key) != EOF)
	{
		num++;
		add(key);
	}

    printf("load %s %d key, set_size = %d\n", name, num, m_tbl.size());
    fclose(fp);
}
