package gen_feng_table;

import java.util.HashMap;

public class Program
{
	static HashMap<Integer, Boolean>[] gui_tested = new HashMap[9];
	static HashMap<Integer, Boolean>[] gui_eye_tested = new HashMap[9];

	static void init_cache()
	{
		for (int i = 0 ; i < 9 ; i++)
		{
			gui_tested[i] = new HashMap<Integer, Boolean>();
			gui_eye_tested[i] = new HashMap<Integer, Boolean>();
		}
	}

	static boolean check_add(int[] cards, int gui_num, boolean eye)
	{
		int key = 0;

		for (int i = 0 ; i < 7 ; i++)
		{
			key = key * 10 + cards[i];
		}

		if (key == 0)
		{
			return false;
		}

		HashMap<Integer, Boolean> m;
		if (!eye)
		{
			m = gui_tested[gui_num];
		}
		else
		{
			m = gui_eye_tested[gui_num];
		}

		if (m.containsKey(key))
		{
			return false;
		}

		m.put(key, true);

		for (int i = 0 ; i < 7 ; i++)
		{
			if (cards[i] > 4)
			{
				return true;
			}
		}
		TableMgr.getInstance().add(key, gui_num, eye, false);
		return true;
	}

	static void parse_table_sub(int[] cards, int num, boolean eye)
	{
		for (int i = 0 ; i < 7 ; i++)
		{
			if (cards[i] == 0)
			{
				continue;
			}

			cards[i]--;

			if (!check_add(cards, num, eye))
			{
				cards[i]++;
				continue;
			}

			if (num < 8)
			{
				parse_table_sub(cards, num + 1, eye);
			}
			cards[i]++;
		}
	}

	static void parse_table(int[] cards, boolean eye)
	{
		if (!check_add(cards, 0, eye))
		{
			return;
		}
		parse_table_sub(cards, 1, eye);
	}

	static void gen_auto_table_sub(int[] cards, int level, boolean eye)
	{
		for (int i = 0 ; i < 7 ; ++i)
		{
			if (cards[i] > 3)
			{
				continue;
			}
			cards[i] += 3;
			parse_table(cards, eye);
			if (level < 4)
			{
				gen_auto_table_sub(cards, level + 1, eye);
			}
			cards[i] -= 3;
		}
	}

	static void gen_table()
	{
		int[] cards = new int[34];
		for (int i = 0 ; i < 34 ; ++i)
		{
			cards[i] = 0;
		}

		gen_auto_table_sub(cards, 1, false);
	}

	static void gen_eye_table()
	{
		int[] cards = new int[34];
		for (int i = 0 ; i < 34 ; ++i)
		{
			cards[i] = 0;
		}

		for (int i = 0 ; i < 7 ; ++i)
		{
			cards[i] = 2;
			parse_table(cards, true);
			gen_auto_table_sub(cards, 1, true);
			cards[i] = 0;
		}
	}

	public static void main(String[] args)
	{
		System.out.println("generate feng table begin...");
		init_cache();
		gen_table();
		gen_eye_table();
		TableMgr.getInstance().dump_feng_table();
	}
}
