#include <stdio.h>
#include <string.h>
#include "binary_array_table.h"


BinaryArrayTable::BinaryArrayTable()
{
	m_tbl = NULL;
	m_len = 0;
}

BinaryArrayTable::~BinaryArrayTable()
{
	delete []m_tbl;
	m_len = 0;
}

bool BinaryArrayTable::check(int key)
{
	if (m_len == 0) return false;
	if (key < m_tbl[0]) return false;
	if (key > m_tbl[m_len-1]) return false;

	int min = 0;
	int max = m_len-1;
	int mid = 0;
	while (min < max){
		if (key == m_tbl[min]) return true;
		if (key == m_tbl[max]) return true;
		mid = (min + max) >> 1;
		if (key == m_tbl[mid]) return true;
		else if (key < m_tbl[mid]) {
			max = mid - 1;
		}
		else{
			min = mid + 1;
		}
	}
	return false;
}

void BinaryArrayTable::add(int key)
{
	m_set.insert(key);
}

void BinaryArrayTable::dump(char* name)
{
    FILE *fp = fopen(name, "wb+");

	for (std::set<int>::iterator it = m_set.begin(); it != m_set.end();++it)
    {
        fprintf(fp, "%d\n", *it);
    }

    fclose(fp);
}

void BinaryArrayTable::load(char* name)
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

	num = 0;

	m_len = m_set.size();
	m_tbl = new int[m_len];
	for (std::set<int>::iterator it = m_set.begin(); it != m_set.end(); ++it)
	{
		m_tbl[num++] = *it;
	}

    fclose(fp);
}
