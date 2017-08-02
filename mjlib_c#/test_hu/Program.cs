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
        static void test_one()
        {
            int[] cards = {
                0,0,0,0,2,0,0,0,0,
                1,2,2,1,0,0,2,2,2,
                0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0
            };

            System.Console.Write("测试1种\n");
            print_cards(cards);
            if (!HuLib.getInstance().get_hu_info(cards, null, 34, 34, 34))
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

            System.Console.ReadKey();
        }
    }
}
