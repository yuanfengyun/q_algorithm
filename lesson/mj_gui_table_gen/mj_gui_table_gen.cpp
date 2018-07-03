// mj_gui_table_gen.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "table_mgr.h"

int main()
{
	TableMgr::init();
	printf("generate table begin...\n");
	gen_eye_table();
	gen_table();
	TableMgr::dump_table();

	printf("generate feng table begin...\n");
	gen_table();
	gen_eye_table();
	TableMgr::dump_feng_table();
    return 0;
}

