#pragma once

class WaveTable {
private:
    static WaveTable* m_instance;
    char*  m_keys;
    int    m_key_num;
    bool   m_collect;

    WaveTable();
    ~WaveTable();

public:
    static WaveTable* getInstance();

    void setCollect();

    bool check(int key);

    void dump(char* name);

    void load(char* name);
private:
    int getKey(int number);

    void add(int key);
};
