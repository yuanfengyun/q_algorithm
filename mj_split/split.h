#pragma once
class split
{
public:
	static bool get_hu_info(char* cards, char cur_card, char gui_index);
private:
	static bool foreach_eye(char* cards, char gui_num);

	static int check_normal(char* cards, int from, int to, int left_gui);

	static int next_split(int n, int gui_num);

	static int one(int n, int gui_num);
	static int two(int n, int gui_num);
	static int three(int n, int gui_num);
	static int four(int n, int gui_num);

	static int check_zi(char* cards, int gui_num);
};
