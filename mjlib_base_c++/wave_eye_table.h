#pragma once

class WaveEyeTable {
private:
    static WaveEyeTable* m_instance;
    char*  m_keys;
    int    m_key_num;
    bool   m_collect;

    WaveEyeTable();
    ~WaveEyeTable();

public:
    static WaveEyeTable* getInstance();

    int getKey(int number);

    void setCollect();

    bool check(int key);

    void add(int key);

    void dump(char* name);

    void load(char* name);
};
