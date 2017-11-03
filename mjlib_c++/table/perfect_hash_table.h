#pragma once

#include "table.h"
#include <hash_map>

class HashTable : public Table {
private:
    std::hash_map<int,int> m_tbl;

public:
    HashTable();
    virtual ~HashTable();

public:
    virtual void add(int key);

    virtual bool check(int key);

    virtual void dump(char* name);

    virtual void load(char* name);
};
