package org.yungege.mj;

public class TableMgr {
	public static TableMgr mgr = new TableMgr();
	public SetTable[] m_check_table = new SetTable[9];
	public SetTable[] m_check_eye_table = new SetTable[9];
	public SetTable[] m_check_feng_table = new SetTable[9];
	public SetTable[] m_check_feng_eye_table = new SetTable[9];

	public TableMgr() {
		for (int i = 0; i < 9; ++i) {
			m_check_table[i] = new SetTable();
			m_check_eye_table[i] = new SetTable();
			m_check_feng_table[i] = new SetTable();
			m_check_feng_eye_table[i] = new SetTable();
		}
	}

	public static TableMgr getInstance() {
		return mgr;
	}

	public boolean check(int key, int gui_num, boolean eye, boolean chi) {
		SetTable tbl;

		if (chi) {
			if (eye) {
				tbl = m_check_eye_table[gui_num];
			} else {
				tbl = m_check_table[gui_num];
			}
		} else {
			if (eye) {
				tbl = m_check_feng_eye_table[gui_num];
			} else {
				tbl = m_check_feng_table[gui_num];
			}
		}

		return tbl.check(key);
	}

	public void add(int key, int gui_num, boolean eye, boolean chi) {
		SetTable tbl;

		if (chi) {
			if (eye) {
				tbl = m_check_eye_table[gui_num];
			} else {
				tbl = m_check_table[gui_num];
			}
		} else {
			if (eye) {
				tbl = m_check_feng_eye_table[gui_num];
			} else {
				tbl = m_check_feng_table[gui_num];
			}
		}

		tbl.add(key);
	}

	public boolean load() {
		for (int i = 0; i < 9; ++i) {
			String path = "tbl/table_";
			m_check_table[i].load(path + i + ".tbl");
		}

		for (int i = 0; i < 9; ++i) {
			String path = "tbl/eye_table_";
			m_check_eye_table[i].load(path + i + ".tbl");
		}

		for (int i = 0; i < 9; ++i) {
			String path = "tbl/feng_table_";
			m_check_feng_table[i].load(path + i + ".tbl");
		}

		for (int i = 0; i < 9; ++i) {
			String path = "tbl/feng_eye_table_";
			m_check_feng_eye_table[i].load(path + i + ".tbl");
		}
		return true;
	}

	public boolean dump_table() {
		for (int i = 0; i < 9; ++i) {
			String path = "tbl/table_";
			m_check_table[i].dump(path + i + ".tbl");
		}

		for (int i = 0; i < 9; ++i) {
			String path = "tbl/eye_table_";
			m_check_eye_table[i].dump(path + i + ".tbl");
		}
		return true;
	}

	public boolean dump_feng_table() {
		for (int i = 0; i < 9; ++i) {
			String path = "tbl/feng_table_";
			m_check_feng_table[i].dump(path + i + ".tbl");
		}

		for (int i = 0; i < 9; ++i) {
			String path = "tbl/feng_eye_table_";
			m_check_feng_eye_table[i].dump(path + i + ".tbl");
		}

		return true;
	}
}
