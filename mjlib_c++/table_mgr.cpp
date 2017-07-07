#include <stdio.h>
#include "table_mgr.h"
#include "set_table.h"


TableMgr::TableMgr()
{
    for(int i=0; i<5; ++i)
    {
        m_check_table[i] = new SetTable;
    }

    for(int i=0; i<5; ++i)
    {
        m_check_eye_table[i] = new SetTable;
    }
 
    for(int i=0; i<5; ++i)
    {
        m_check_feng_table[i] = new SetTable;
    }

    for(int i=0; i<5; ++i)
    {
        m_check_feng_eye_table[i] = new SetTable;
    }
}

TableMgr::~TableMgr()
{
    for(int i=0; i<5; ++i)
    {
        delete m_check_table[i];
    }

    for(int i=0; i<5; ++i)
    {
        delete m_check_eye_table[i];
    }
 
    for(int i=0; i<5; ++i)
    {
        delete m_check_feng_table[i];
    }

    for(int i=0; i<5; ++i)
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
    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_table_%d", i);
        m_check_table[i]->load(path);
    }

    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_eye_table_%d", i);
        m_check_eye_table[i]->load(path);
    }
 
    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_feng_table_%d", i);
        m_check_feng_table[i]->load(path);
    }

    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_feng_eye_table_%d", i);
        m_check_feng_eye_table[i]->load(path);
    }
    return true;
}

bool TableMgr::dump_table()
{
    char path[256];
    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_table_%d", i);
        m_check_table[i]->dump(path);
    }

    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_eye_table_%d", i);
        m_check_eye_table[i]->dump(path);
    }
    return true;
}

bool TableMgr::dump_feng_table()
{
    char path[256];
    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_feng_table_%d", i);
        m_check_feng_table[i]->dump(path);
    }

    for(int i=0; i<5; ++i)
    {
        sprintf(path, "tbl/check_feng_eye_table_%d", i);
        m_check_feng_eye_table[i]->dump(path);
    }

    return true;
}

