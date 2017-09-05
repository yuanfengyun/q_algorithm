#include <stdio.h>
#include <string.h>
#include "compress_array_table.h"


CompressArrayTable::CompressArrayTable()
{
	m_tbl = NULL;
	m_len = 0;
}

CompressArrayTable::~CompressArrayTable()
{
	delete []m_tbl;
	m_len = 0;
}

int CompressArrayTable::transKey(int key)
{

}

bool CompressArrayTable::check(int key)
{
	
	return false;
}

void CompressArrayTable::add(int key)
{
	m_set.insert(key);
}

void CompressArrayTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");

	for (std::set<int>::iterator it = m_set.begin(); it != m_set.end();++it)
    {
        fprintf(fp, "%d\n", *it);
    }

    fclose(fp);
}

void CompressArrayTable::load(char* name)
{
	printf("load tbl %s\n", name);
    FILE *fp = fopen(name, "rb");

    int key = 0;
	int num = 0;
    while(fscanf(fp, "%d\n", &key)!=EOF)
    {
		num++;
		add(key);
    }
	printf("load %s, set_size = %d\n", name, num);
    fclose(fp);
}
