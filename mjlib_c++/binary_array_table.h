#pragma once

#include "table.h"
#include <set>

class BinaryArrayTable : public Table {
private:
    int *m_tbl;
	int m_len;
	std::set<int> m_set;

public:
	BinaryArrayTable();
	virtual ~BinaryArrayTable();

public:
    virtual void add(int key);

    virtual bool check(int key);

    virtual void dump(char* name);

    virtual void load(char* name);
};
