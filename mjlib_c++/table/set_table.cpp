#include <stdio.h>
#include <string.h>
#include "set_table.h"

using namespace std;

SetTable::SetTable()
{
}

SetTable::~SetTable()
{
}

bool SetTable::check(int number)
{
	return m_tbl.find(number) != m_tbl.end();
}

void SetTable::add(int key)
{
    m_tbl.insert(key);
}

void SetTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");

    int num = m_tbl.size();

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
	int num = 0;
	while (fscanf(fp, "%d\n", &key) != EOF)
	{
		num++;
		add(key);
	}

    printf("load %s %d key, set_size = %d\n", name, num, m_tbl.size());
    fclose(fp);
}
