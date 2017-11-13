import java.util.HashMap;

public class test
{
	static HashMap<Integer, Boolean>[] gui_tested = new HashMap[5];
	static HashMap<Integer, Boolean>[] gui_eye_tested = new HashMap[5];
	static int[] test_cards = new int[]{
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0
	};

	static void init_cache()
	{
		for (int i = 0 ; i < 5 ; i++)
		{
			gui_tested[i] = new HashMap<Integer, Boolean>();
			gui_eye_tested[i] = new HashMap<Integer, Boolean>();
		}
	}

	static Boolean check_add(int[] cards, int gui_num, Boolean eye)
	{
		int key = 0;

		for (int i = 0 ; i < 9 ; i++)
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

		for (int i = 0 ; i < 9 ; i++)
		{
			if (cards[i] > 4)
			{
				return true;
			}
			test_cards[i] = cards[i];
		}
		test_cards[33] = gui_num;
		if(!split.get_hu_info(test_cards, 34, 33)){
			System.out.println("error: cant't hu");
		}
		return true;
	}

	static void parse_table_sub(int[] cards, int num, boolean eye)
	{
		for (int i = 0 ; i < 9 ; i++)
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

			if (num < 4)
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
		for (int i = 0 ; i < 16 ; ++i)
		{
			if (i <= 8)
			{
				if (cards[i] > 3)
				{
					continue;
				}
				cards[i] += 3;
			}
			else
			{
				int index = i - 9;
				if (cards[index] > 5 || cards[index + 1] > 5 || cards[index + 2] > 5)
				{
					continue;
				}
				cards[index] += 1;
				cards[index + 1] += 1;
				cards[index + 2] += 1;
			}

			parse_table(cards, eye);
			if (level < 4)
			{
				gen_auto_table_sub(cards, level + 1, eye);
			}

			if (i <= 8)
			{
				cards[i] -= 3;
			}
			else
			{
				int index = i - 9;
				cards[index] -= 1;
				cards[index + 1] -= 1;
				cards[index + 2] -= 1;
			}
		}
	}

	static void gen_eye_table()
	{
		int[] cards = new int[34];
		for (int i = 0 ; i < 34 ; ++i)
		{
			cards[i] = 0;
		}

		for (int i = 0 ; i < 9 ; ++i)
		{
			System.out.println("jiang");
			cards[i] = 2;
			parse_table(cards, true);
			gen_auto_table_sub(cards, 1, true);
			cards[i] = 0;
		}
	}

	public static void main(String[] args)
	{
		System.out.println("test single color begin...");
		init_cache();
		gen_eye_table();
	}
}
