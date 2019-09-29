public class split {
    public static boolean get_hu_info(int[] hand_cards, int curCard, int gui_index) {
        int[] cards = new int[34];
		System.arraycopy(hand_cards, 0, cards, 0, 34);

        if (curCard < 34) {
            cards[curCard]++;
        }
        int gui_num = 0;
        if (gui_index < 34) {
            gui_num = cards[gui_index];
            cards[gui_index] = 0;
        }

        int[] eye_tbl = new int[34];
        int eye_num = 0;
        int empty = -1;
        for (int i = 0; i < 34; ++i) {
            // 优化手段，三不靠的牌，必做将
            int min = (i / 9) * 9;
            int max = min + 8;
            if (max == 35) max = 33;
            if (cards[i] == 1 &&
                    (i - 2 < min || cards[i - 2] == 0) &&
                    (i - 1 < min || cards[i - 1] == 0) &&
                    (i + 1 > max || cards[i + 1] == 0) &&
                    (i + 2 > max || cards[i + 2] == 0)) {
                if (gui_num < 0) {
                    return false;
                }
                eye_num = 1;
                eye_tbl[0] = i;
                empty = -1;
                break;
            }
            if (empty == -1 && cards[i] == 0) empty = i;
            if (cards[i] > 0 && cards[i] + gui_num >= 2) {
                eye_tbl[eye_num++] = i;
            }
        }
        if (empty > 0) {
            eye_tbl[eye_num++] = empty;
        }

        boolean hu = false;
        int[] cache = {0, 0, 0, 0};
        for (int i = 0; i < eye_num; i++) {
            int eye = eye_tbl[i];
            if (eye == empty) {
                hu = foreach_eye(cards, gui_num - 2, gui_num, 1000, cache);
            } else {
                int n = cards[eye];
                if (n == 1) {
                    cards[eye] = 0;
                    hu = foreach_eye(cards, gui_num - 1, gui_num, eye / 9, cache);
                } else {
                    cards[eye] -= 2;
                    hu = foreach_eye(cards, gui_num, gui_num, eye / 9, cache);
                }
                cards[eye] = n;
            }
            if (hu) {
                break;
            }
        }

        if (gui_num > 0) {
            cards[gui_index] = gui_num;
        }
        return hu;
    }

    public static boolean foreach_eye(int[] cards, int gui_num, int max_gui, int eye_color, int[] cache) {
        int left_gui = gui_num;
        for (int i = 0; i < 3; i++) {
            int cache_index = -1;
            if (eye_color != i) cache_index = i;
            int need_gui = check_normal(cards, i * 9, i * 9 + 8, max_gui, cache_index, cache);
            if (cache_index > 0) {
                cache[i] = need_gui + 1;
            }
            left_gui -= need_gui;
            if (left_gui < 0) {
                return false;
            }
        }

        int cache_index = -1;
        if (eye_color != 3) cache_index = 3;
        int need_gui = check_zi(cards, max_gui, cache_index, cache);
        if (cache_index > 0) {
            cache[3] = need_gui + 1;
        }
        return left_gui >= need_gui;
    }

    public static int check_normal(int[] cards, int from, int to, int max_gui, int cache_index, int[] cache) {
        if (cache_index >= 0) {
            int n = cache[cache_index];
            if (n > 0) return n - 1;
        }

        int n = 0;
        for (int i = from; i <= to; i++) {
            n = n * 10 + cards[i];
        }

        if (n == 0) return 0;

        boolean n3 = false;
        for (int i = 0; i <= max_gui; i++) {
            if ((n + i) % 3 == 0) {
                n3 = true;
                break;
            }
        }

        if (!n3) {
            return max_gui + 1;
        }

        return next_split(n, 0, max_gui);
    }

    public static int next_split(int n, int need_gui, int max_gui) {
        int c = 0;
        while (true) {
            if (n == 0) return need_gui;

            while (n > 0) {
                c = n % 10;
                n = n / 10;
                if (c != 0) break;
            }
            if (c == 1 || c == 4) {
                return one(n, need_gui, max_gui);
            } else if (c == 2) {
                return two(n, need_gui, max_gui);
            }
        }
    }

    public static int one(int n, int need_gui, int max_gui) {
        int c1 = n % 10;
        int c2 = (n % 100) / 10;

        if (c1 == 0) ++need_gui;
        else n -= 1;

        if (c2 == 0) ++need_gui;
        else n -= 10;

        if (n == 0) return need_gui;

        if (need_gui > max_gui) return need_gui;

        return next_split(n, need_gui, max_gui);
    }

    public static int two(int n, int need_gui, int max_gui) {
        int c1 = n % 10;
        int c2 = (n % 100) / 10;
        int c3 = (n % 1000) / 100;
        int c4 = (n % 10000) / 1000;

        boolean choose_ke = true;
		if (c1 != 0) {
			if (c1 == 1) {
				// 刻子
				if (c2 != 0 && c2 != 1) {
					if (c2 == 2) {
						if (c3 == 2) {
							if (c4 == 2) choose_ke = false;
						} else if (c3 == 3) {
							if (c4 != 2) choose_ke = false;
						} else {
							choose_ke = false;
						}
					} else if (c2 == 3) {
						if (c3 != 3) {
							choose_ke = false;
						}
					} else if (c2 == 4) {
						if (c3 == 2) {
							if (c4 == 2 || c4 == 3 || c4 == 4) choose_ke = false;
						}
						if (c3 == 3) {
							choose_ke = false;
						}
					}
				}
			} else if (c1 == 2) {
				choose_ke = false;
			} else if (c1 == 3) {
				if (c2 == 2) {
					if (c3 == 1 || c3 == 4) {
						choose_ke = false;
					} else if (c3 == 2) {
						if (c4 != 2) choose_ke = false;
					}
				}
				if (c2 == 3) {
					choose_ke = false;
				} else if (c2 == 4) {
					if (c3 == 2) {
						choose_ke = false;
					}
				}
			} else if (c1 == 4) {
				if (c2 == 2 && c3 != 2) {
					choose_ke = false;
				} else if (c2 == 3) {
					if (c3 == 0 || c3 == 1 || c3 == 2) {
						choose_ke = false;
					}
				} else if (c2 == 4) {
					if (c3 == 2) choose_ke = false;
				}
			}
		}  // c1 == 0 全拆刻子


		if (choose_ke) {
            need_gui += 1;
        } else {
            if (c1 < 2) {
                need_gui += (2 - c1);
                n -= c1;
            } else {
                n -= 2;
            }

            if (c2 < 2) {
                need_gui += (2 - c2);
                n -= c2;
            } else {
                n -= 20;
            }
        }

        if (n == 0) return need_gui;

        if (need_gui > max_gui) return need_gui;

        return next_split(n, need_gui, max_gui);
    }

    public static int check_zi(int[] cards, int max_gui, int cache_index, int[] cache) {
        if (cache_index >= 0) {
            int n = cache[cache_index];
            if (n > 0) return n - 1;
        }

        int need_gui = 0;
        for (int i = 27; i < 34; i++) {
            int c = cards[i];
            if (c == 0) continue;
            if (c == 1 || c == 4) {
                need_gui = need_gui + 2;
            } else if (c == 2) {
                need_gui = need_gui + 1;
            }
            if (need_gui > max_gui) return need_gui;
        }

        return need_gui;
    }
}
