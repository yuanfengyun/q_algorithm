#pragma once
class split2
{
public:
	static bool get_hu_info(char* cards, char cur_card, char gui_index);
private:
	static bool get_cache(char* cards, char max_gui, int* cache);

	static bool check_color(char* cards, char from, char gui_num);

	static int check_normal(char* cards, int from, int max_gui, int used_gui);

	static int next_split(char* cards, int gui_num, int max_gui, int used_gui);

	static void one(char* cards, int& index, int& need_gui, int max_gui, int used_gui);
	static void two(char* cards, int& index, int& need_gui, int max_gui, int used_gui);

	static inline int check_zi(char* cards);
};
