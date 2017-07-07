using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using mjlib;

namespace test_hu
{
    class Program
    {
        static void print_cards(int[] cards)
        {
            for (int i = 0; i < 9; ++i)
            {
                System.Console.Write(cards[i]);
                System.Console.Write(",");
            }
            System.Console.WriteLine("");

            for (int i = 9; i < 18; ++i)
            {
                System.Console.Write(cards[i]);
                System.Console.Write(",");
            }
            System.Console.WriteLine("");
            for (int i = 18; i < 27; ++i)
            {
                System.Console.Write(cards[i]);
                System.Console.Write(",");
            }
            System.Console.WriteLine("");
            for (int i = 27; i < 34; ++i)
            {
                System.Console.Write(cards[i]);
                System.Console.Write(",");
            }
            System.Console.WriteLine("");
        }
        static HashSet<int> tested = new HashSet<int>();
        static void check_hu(int[] cards, int max_gui_index)
        {
           

            for (int i = 0; i < max_gui_index; ++i)
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

            for (int i = 0; i < max_gui_index; ++i)
            {
                if (!HuLib.getInstance().get_hu_info(cards, null, 0, 0))
                {
                    System.Console.Write("测试失败 i=%d\n", i);
                    print_cards(cards);
                }
            }
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
                    check_hu(cards, 18);
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

        static void test_two_color()
        {
            System.Console.WriteLine("测试两种花色\n");
            int[] cards = new int[34]{
                0,1,1,0,0,0,1,1,1,
                0,0,1,1,1,0,3,0,0,
                0,0,3,0,0,0,0,0,0,
                0,0,0,0,0,0,0,
            };

            for (int i = 0; i < 18; ++i)
            {
                cards[i] = 2;
                System.Console.Write("将 ");
                System.Console.WriteLine(i);
                gen_auto_table_sub(cards, 1);
                cards[i] = 0;
            }
        }

        static void test_one()
        {
            int[] cards = {
                0,0,0,0,1,0,0,0,0,
                1,1,1,2,2,0,2,2,2,
                0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0
            };

            System.Console.Write("测试1种\n");
            print_cards(cards);
            if (!HuLib.getInstance().get_hu_info(cards, null, 34, 34))
            {
                System.Console.Write("测试失败\n");
            }
            else
            {
                System.Console.Write("测试成功\n");
            }
        }

        static void Main()
        {
            System.Console.Write("test hulib begin...\n");

            TableMgr.getInstance().load();

            test_one();

            //    test_two_color();
            System.Console.ReadKey();
        }
    }
}
