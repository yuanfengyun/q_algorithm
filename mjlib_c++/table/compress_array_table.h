#pragma once

#include "table.h"
#include <set>

class CompressArrayTable : public Table {
private:
    int *m_tbl;
	int m_len;
	std::set<int> m_set;

public:
	CompressArrayTable();
	virtual ~CompressArrayTable();

public:
    virtual void add(int key);

	int transKey(int key);

    virtual bool check(int key);

    virtual void dump(char* name);

    virtual void load(char* name);
};
