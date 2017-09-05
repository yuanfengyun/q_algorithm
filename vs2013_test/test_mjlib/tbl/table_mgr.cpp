#include "stdafx.h"
#include <stdio.h>
#include "table_mgr.h"
#include "set_table.h"
#include "hash_table.h"
#include "array_table.h"
#include "binary_array_table.h"

#define TABLECLASS BinaryArrayTable

TableMgr::TableMgr()
{
    for(int i=0; i<9; ++i)
    {
		m_check_table[i] = new TABLECLASS;
    }

    for(int i=0; i<9; ++i)
    {
		m_check_eye_table[i] = new TABLECLASS;
    }
 
    for(int i=0; i<9; ++i)
    {
		m_check_feng_table[i] = new TABLECLASS;
    }

    for(int i=0; i<9; ++i)
    {
		m_check_feng_eye_table[i] = new TABLECLASS;
    }
}

TableMgr::~TableMgr()
{
    for(int i=0; i<9; ++i)
    {
        delete m_check_table[i];
    }

    for(int i=0; i<9; ++i)
    {
        delete m_check_eye_table[i];
    }
 
    for(int i=0; i<9; ++i)
    {
        delete m_check_feng_table[i];
    }

    for(int i=0; i<9; ++i)
    {
        delete m_check_feng_eye_table[i];
    }
}

TableMgr* TableMgr::get_instance()
{
    static TableMgr mgr;
    return &mgr;
}

bool TableMgr::check(int key, int gui_num, bool eye, bool chi)
{
    Table* tbl = 0;

    if(chi)
    {
        if(eye)
        {
            tbl = m_check_eye_table[gui_num];
        }
        else
        {
            tbl = m_check_table[gui_num];
        }
    }
    else
    {
        if(eye)
        {
            tbl = m_check_feng_eye_table[gui_num];
        }
        else
        {
            tbl = m_check_feng_table[gui_num];
        }
    }

    return tbl->check(key);
}

void TableMgr::add(int key, int gui_num, bool eye, bool chi)
{
    Table* tbl = 0;

    if(chi)
    {
        if(eye)
        {
            tbl = m_check_eye_table[gui_num];
        }
        else
        {
            tbl = m_check_table[gui_num];
        }
    }
    else
    {
        if(eye)
        {
            tbl = m_check_feng_eye_table[gui_num];
        }
        else
        {
            tbl = m_check_feng_table[gui_num];
        }
    }

    tbl->add(key);
}

bool TableMgr::load()
{
    char path[256];
    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/table_%d.tbl", i);
        m_check_table[i]->load(path);
    }

    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/eye_table_%d.tbl", i);
        m_check_eye_table[i]->load(path);
    }
 
    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/feng_table_%d.tbl", i);
        m_check_feng_table[i]->load(path);
    }

    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/feng_eye_table_%d.tbl", i);
        m_check_feng_eye_table[i]->load(path);
    }
    return true;
}

bool TableMgr::dump_table()
{
    char path[256];
    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/table_%d.tbl", i);
        m_check_table[i]->dump(path);
    }

    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/eye_table_%d.tbl", i);
        m_check_eye_table[i]->dump(path);
    }
    return true;
}

bool TableMgr::dump_feng_table()
{
    char path[256];
    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/feng_table_%d.tbl", i);
        m_check_feng_table[i]->dump(path);
    }

    for(int i=0; i<9; ++i)
    {
        sprintf(path, "tbl/feng_eye_table_%d.tbl", i);
        m_check_feng_eye_table[i]->dump(path);
    }

    return true;
}

