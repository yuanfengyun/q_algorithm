#pragma once
class split
{
public:
	static bool get_hu_info(char* cards, char cur_card, char gui_index);
private:
	static bool foreach_eye(char* cards, char gui_num, char max_gui, int eye_color, int* cache);

	static int check_normal(char* cards, int from, int to, int max_gui, int cache_index, int* cache);

	static int next_split(int n, int gui_num, int max_gui);

	static int one(int n, int gui_num, int max_gui);
	static int two(int n, int gui_num, int max_gui);

	static inline int check_zi(char* cards, int max_gui, int cache_index, int* cache);
};
