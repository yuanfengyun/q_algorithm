using System.Collections.Generic;
using System.IO;

namespace mjlib {
    class SetTable {
        private Dictionary<int, bool> m_tbl = new Dictionary<int, bool>();

        public bool check(int number)
        {
            return m_tbl.ContainsKey(number);
        }

        public void add(int key)
        {
            if (m_tbl.ContainsKey(key)) return;

            m_tbl.Add(key, true);
        }

        public void dump(string name)
        {
            System.IO.StreamWriter file = new System.IO.StreamWriter(name, true);

            foreach (var key in m_tbl)
            {
                file.WriteLine(key);
            }
            file.Close();
        }

        public void load(string name)
        {
            System.IO.StreamReader sr = new System.IO.StreamReader(name);
            string str;
            while ((str = sr.ReadLine()) != null)
            {
                m_tbl.Add(int.Parse(str), true);
            }
            sr.Close();
        }
    }
}