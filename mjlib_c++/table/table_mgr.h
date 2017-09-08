#pragma once

#include "table.h"

#define MAX_GUI_NUM 4

class TableMgr
{
public:
	static void init();

	static void destory();

	static void add(int key, int gui_num, bool eye, bool chi);

	static bool check(int key, int gui_num, bool eye, bool chi);

	static bool load();

	static bool dump_table();

	static bool dump_feng_table();

	static Table* m_check_table[MAX_GUI_NUM+1];
	static Table* m_check_eye_table[MAX_GUI_NUM + 1];
	static Table* m_check_feng_table[MAX_GUI_NUM + 1];
	static Table* m_check_feng_eye_table[MAX_GUI_NUM + 1];
};
