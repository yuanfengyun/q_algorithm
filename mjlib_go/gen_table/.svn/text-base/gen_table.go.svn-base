package main

import (
    "fmt"
    "mjlib"
)

var gui_tested = [9]*map[int] bool{}
var gui_eye_tested = [9]*map[int] bool{}

func check_add(cards []int, gui_num int, eye bool) bool {
    key := 0

    for i:=0; i<9; i++ {
        key = key * 10 + cards[i]
    }

    var m *map[int] bool
    if !eye {
        m = gui_tested[gui_num]
    } else {
        m = gui_eye_tested[gui_num]
    }
    _, ok := (*m)[key]

    if ok {
        return false
    }

    (*m)[key] = true
    mjlib.MTableMgr.Add(key, gui_num, eye, true)
    return true
}

func parse_table_sub(cards []int, num int, eye bool) {
    for i:=0; i<9; i++{
        if cards[i] == 0 {
             continue
        }

        cards[i]--

        if !check_add(cards, num, eye) {
            cards[i]++
            continue
        }

        if num < 8 {
            parse_table_sub(cards, num+1, eye)
        }
        cards[i]++
    }
}

func parse_table(cards []int, eye bool) {
    if !check_add(cards, 0, eye) {
        return
    }
    parse_table_sub(cards, 1, eye)
}

func gen_111_3(cards []int, level int, eye bool) {
    for i:=0; i<16; i++{
        if i <= 8 {
            if cards[i] > 1 {
                continue
            }
            cards[i] += 3
        } else {
            index := i - 9
            if cards[index] > 3 || cards[index + 1] > 3 || cards[index + 2] > 3 {
                continue
            }
            cards[index] += 1
            cards[index + 1] += 1
            cards[index + 2] += 1
        }

        parse_table(cards, eye)
        if level < 4 {
            gen_111_3(cards, level + 1, eye)
        }

        if i <= 8 {
            cards[i] -= 3
        } else {
            index := i - 9
            cards[index] -= 1
            cards[index + 1] -= 1
            cards[index + 2] -= 1
        }
    }
}

func gen_table(){
    cards := []int{
        0,0,0,0,0,0,0,0,0,
    }

    // 无眼
    fmt.Printf("无眼表生成开始\n")
    gen_111_3(cards, 1, false)
    fmt.Printf("无眼表生成结束\n")

    // 有眼
    fmt.Printf("有眼表生成开始\n")
    for i:=0; i<9; i++{
        cards[i] = 2
        fmt.Printf("将 %d \n", i)
        gen_111_3(cards, 1, true)
        cards[i] = 0
    }
    fmt.Printf("有眼表生成结束\n")

    fmt.Printf("表数据存储开始\n")
    mjlib.MTableMgr.DumpTable()
    fmt.Printf("表数据存储结束\n")
}

func main(){
    for i:=0; i<9; i++{
        gui_tested[i] = &map[int] bool{}
        gui_eye_tested[i] = &map[int] bool{}
    }

    fmt.Println("generate table begin...")

    mjlib.Init()
    gen_table()
}
