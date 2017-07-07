using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using mjlib;

namespace gen_table
{
    class Program
    {
        static void add(int[] cards, int gui_num, bool eye)
        {
            int count = 0;
            int key = 0;
            for (int i = 0; i < 9; ++i)
            {
                count += cards[i];
                key = key * 10 + cards[i];
            }

            TableMgr.getInstance().add(key, gui_num, eye, true);
        }

        static void parse_table_sub(int[] cards, int num, bool eye)
        {
            for (int i = 0; i < 9; ++i)
            {
                if (cards[i] == 0) continue;

                --cards[i];

                add(cards, num, eye);

                if (num < 8)
                {
                    parse_table_sub(cards, num + 1, eye);
                }
                ++cards[i];
            }
        }

        static void parse_table(int[] cards)
        {
            int count = 0;
            for (int i = 0; i < 9; ++i)
            {
                count += cards[i];
            }

            bool eye = false;
            if (count % 3 != 0)
            {
                eye = true;
            }

            add(cards, 0, eye);
            parse_table_sub(cards, 1, eye);
        }

        static HashSet<int> tested = new HashSet<int>();
        static void check_hu(int[] cards)
        {
           

            for (int i = 0; i < 18; ++i)
            {
                if (cards[i] > 4) return;
            }

            int num = 0;
            for (int i = 0; i < 9; ++i)
            {
                num = num * 10 + cards[i];
            }

            if (tested.Contains(num))
            {
                return;
            }

            tested.Add(num);

            parse_table(cards);
        }

        static void gen_auto_table_sub(int[] cards, int level)
        {
            for (int i = 0; i < 32; ++i)
            {
                int index = -1;
                if (i <= 17)
                {
                    cards[i] += 3;
                }
                else if (i <= 24)
                {
                    index = i - 18;
                }
                else
                {
                    index = i - 16;
                }

                if (index >= 0)
                {
                    cards[index] += 1;
                    cards[index + 1] += 1;
                    cards[index + 2] += 1;
                }

                if (level == 4)
                {
                    check_hu(cards);
                }
                else
                {
                    gen_auto_table_sub(cards, level + 1);
                }

                if (i <= 17)
                {
                    cards[i] -= 3;
                }
                else
                {
                    cards[index] -= 1;
                    cards[index + 1] -= 1;
                    cards[index + 2] -= 1;
                }
            }
        }

        static void gen_auto_table()
        {
            int[] cards = new int[34];
            for(int i=0; i<34; ++i)
            {
                cards[i] = 0;
            }

            for (int i = 0; i < 18; ++i)
            {
                cards[i] = 2;
                System.Console.WriteLine("将 %d", i + 1);
                gen_auto_table_sub(cards, 1);
                cards[i] = 0;
            }

            TableMgr.getInstance().dump_table();
        }

        static void Main(string[] args)
        {
            System.Console.WriteLine("generate table begin...");
            gen_auto_table();
        }
    }
}
