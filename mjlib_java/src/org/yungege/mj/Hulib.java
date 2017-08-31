package org.yungege.mj;

public class Hulib
{
	static Hulib m_hulib = new Hulib();

	public boolean get_hu_info(int[] hand_cards, int curCard, int gui_index)
	{
		int[] hand_cards_tmp = new int[34];
		for (int i = 0 ; i < 34 ; ++i)
		{
			hand_cards_tmp[i] = hand_cards[i];
		}

		if (curCard < 34)
		{
			hand_cards_tmp[curCard]++;
		}
		int gui_num = 0;
		if (gui_index < 34)
		{
			gui_num = hand_cards_tmp[gui_index];
			hand_cards_tmp[gui_index] = 0;
		}

		ProbabilityItemTable ptbl = new ProbabilityItemTable();
		if (!split(hand_cards_tmp, gui_num, ptbl))
		{
			return false;
		}

		return check_probability(ptbl, gui_num);
	}

	public static Hulib getInstance()
	{
		return m_hulib;
	}

	boolean split(int[] cards, int gui_num, ProbabilityItemTable ptbl)
	{
		if (!_split(cards, gui_num, 0, 0, 8, true, ptbl))
			return false;
		if (!_split(cards, gui_num, 1, 9, 17, true, ptbl))
			return false;
		if (!_split(cards, gui_num, 2, 18, 26, true, ptbl))
			return false;
		if (!_split(cards, gui_num, 3, 27, 33, false, ptbl))
			return false;

		return true;
	}

	boolean _split(int[] cards, int gui_num, int color, int min, int max, boolean chi, ProbabilityItemTable ptbl)
	{
		int key = 0;
		int num = 0;

		for (int i = min ; i <= max ; ++i)
		{
			key = key * 10 + cards[i];
			num = num + cards[i];
		}

		if (num > 0)
		{
			if (!list_probability(color, gui_num, num, key, chi, ptbl))
			{
				return false;
			}
		}

		return true;
	}

	boolean list_probability(int color, int gui_num, int num, int key, boolean chi, ProbabilityItemTable ptbl)
	{
		boolean find = false;
		int anum = ptbl.array_num;
		for (int i = 0 ; i <= gui_num ; ++i)
		{
			int yu = (num + i) % 3;
			if (yu == 1)
				continue;
			boolean eye = (yu == 2);
			if (find || TableMgr.getInstance().check(key, i, eye, chi))
			{
				ProbabilityItem item = ptbl.m[anum][ptbl.m_num[anum]];
				ptbl.m_num[anum]++;

				item.eye = eye;
				item.gui_num = i;
				find = true;
			}
		}

		if (ptbl.m_num[anum] <= 0)
		{
			return false;
		}

		ptbl.array_num++;
		return true;
	}

	boolean check_probability(ProbabilityItemTable ptbl, int gui_num)
	{
		// 全是鬼牌
		if (ptbl.array_num == 0)
		{
			return gui_num >= 2;
		}

		// 只有一种花色的牌的鬼牌
		if (ptbl.array_num == 1)
			return true;

		// 尝试组合花色，能组合则胡
		for (int i = 0 ; i < ptbl.m_num[0] ; ++i)
		{
			ProbabilityItem item = ptbl.m[0][i];
			boolean eye = item.eye;

			int gui = gui_num - item.gui_num;
			if (check_probability_sub(ptbl, eye, gui, 1, ptbl.array_num))
			{
				return true;
			}
		}
		return false;
	}

	boolean check_probability_sub(ProbabilityItemTable ptbl, boolean eye, int gui_num, int level, int max_level)
	{
		for (int i = 0 ; i < ptbl.m_num[level] ; ++i)
		{
			ProbabilityItem item = ptbl.m[level][i];

			if (eye && item.eye)
				continue;

			if (gui_num < item.gui_num)
				continue;

			if (level < max_level - 1)
			{
				if (check_probability_sub(ptbl, eye || item.eye, gui_num - item.gui_num, level + 1, ptbl.array_num))
				{
					return true;
				}
				continue;
			}

			if (!eye && !item.eye && !item.eye && item.gui_num + 2 > gui_num)
				continue;
			return true;
		}

		return false;
	}

	boolean check_7dui(int[] cards)
	{
		int c = 0;
		for (int i = 0 ; i < 34 ; ++i)
		{
			if (cards[i] % 2 != 0)
				return false;
			c += cards[i];
		}

		if (c != 34)
			return false;

		return true;
	}
}

class ProbabilityItem
{
	public boolean eye;
	public int gui_num;

	public ProbabilityItem()
	{
		eye = false;
		gui_num = 0;
	}
};

class ProbabilityItemTable
{
	ProbabilityItem[][] m = new ProbabilityItem[4][5];

	public int array_num;
	public int[] m_num;

	public ProbabilityItemTable()
	{
		for (int i = 0 ; i < m.length ; i++)
		{
			for (int j = 0 ; j < m[i].length ; j++)
			{
				m[i][j] = new ProbabilityItem();
			}
		}

		array_num = 0;
		m_num = new int[] {
				0, 0, 0, 0
		};

	}
}
