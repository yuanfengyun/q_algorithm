#pragma once
class split2
{
public:
	static bool get_hu_info(char* cards, char cur_card, char gui_index);
private:
	static bool foreach_color(char* cards, char max_gui, int* cache, int* counter);

	static int check_normal(char* cards, int from, int max_gui);

	static int next_split(int n, int gui_num, int max_gui);

	static int one(int n, int gui_num, int max_gui);
	static int two(int n, int gui_num, int max_gui);

	static inline int check_zi(char* cards, int max_gui);
};
