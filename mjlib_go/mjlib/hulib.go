package mjlib

const MAX_CARD = 34

type ProbabilityItem struct {
    eye bool
    gui_num int
}

type ProbabilityItemTable struct{
    m [4][5]ProbabilityItem
    array_num int
    m_num [4]int
}

type HuLib struct{
    gui_index1 int
    gui_index2 int
}

// 初始化
func (this *HuLib) init(){

}

func (this *HuLib) get_hu_info(cards []int, cur_card int) bool {
    var tmp_cards = cards

    if cur_card != MAX_CARD {
        tmp_cards[cur_card]++
    }

    var gui_index1 = this.gui_index1
    var gui_index2 = this.gui_index2
    var gui_num = 0
    if gui_index1 != MAX_CARD {
        gui_num = tmp_cards[gui_index1]
        tmp_cards[gui_index1] = 0
    }

    if gui_index2 != MAX_CARD {
        gui_num = gui_num + tmp_cards[gui_index2]
        tmp_cards[gui_index2] = 0
    }

    var ptbl ProbabilityItemTable
    if this._split(tmp_cards, gui_num, &ptbl) {
        return false
    }

    return true
}

func (this *HuLib) _split(cards []int, gui_num int, ptbl *ProbabilityItemTable) bool{
    if !this._split_color(cards, gui_num, 0, 0, 8, true, ptbl) {
        return false
    }
    if !this._split_color(cards, gui_num, 1, 9, 17, true, ptbl) {
        return false
    }
    if !this._split_color(cards, gui_num, 2, 18, 26, true, ptbl) {
        return false
    }
    if !this._split_color(cards, gui_num, 3, 27, 33, false, ptbl) {
        return false
    }

    return true
}

func (this *HuLib) _split_color(cards []int, gui_num int, color int, min int, max int, chi bool, ptbl *ProbabilityItemTable) bool{
    key := 0
    num := 0

    for i := min; i<=max; i++ {
        key = key*10 + cards[i]
        num = num + cards[i]
    }

    if num == 0 {
        return true
    }

    if !this.list_probability(color, gui_num, num, key, chi, ptbl) {
        return false
    }

    return true
}

func (this *HuLib) list_probability(color int, gui_num int, num int, key int, chi bool, ptbl *ProbabilityItemTable) bool {
    anum := ptbl.array_num
    for i:=0; i<=gui_num; i++ {
        eye := false
        var yu int = (num + i)%3
        if yu == 1 {
            continue
        } else if yu == 2 {
            eye = true
        }
        if MTableMgr.check(key, i, eye, chi) {
            item := &(ptbl.m[anum][ptbl.m_num[anum]])
            ptbl.m_num[anum]++

            item.eye = eye
            item.gui_num = i
        }
    }

    if ptbl.m_num[anum] <= 0 {
        return false
    }

    ptbl.array_num++

    return true
}

func (this *HuLib)check_probability(ptbl *ProbabilityItemTable, gui_num int) bool {
    // 全是鬼牌
    if ptbl.array_num == 0 {
        return gui_num >= 2
    }

    // 只有一种花色的牌的鬼牌
    if ptbl.array_num == 1 {
        return true
    }

    // 尝试组合花色，能组合则胡
    for i:=0; i<ptbl.m_num[0]; i++ {
        item := &ptbl.m[0][i]
        eye := item.eye
        gui := gui_num - item.gui_num
        if this.check_probability_sub(ptbl, &eye, &gui, 1, ptbl.array_num) {
            return true
        }
    }
    return false
}

func (this *HuLib) check_probability_sub(ptbl *ProbabilityItemTable, eye *bool, gui_num *int, level int, max_level int) bool {
    for i:=0; i<ptbl.m_num[level]; i++ {
        item := &ptbl.m[level][i]

        if *eye && item.eye {
            continue
        }

        if *gui_num < item.gui_num {
            continue
        }

        if level < max_level - 1 {
            old_gui_num := gui_num
            old_eye := eye
            *gui_num -= item.gui_num
            *eye = *eye || item.eye

            if this.check_probability_sub(ptbl, eye, gui_num, level + 1, ptbl.array_num) {
                return true
            }

            eye = old_eye
            gui_num = old_gui_num
            continue
        }

        if !*eye && !item.eye && item.gui_num + 2 > *gui_num {
            continue
        }
        return true
    }

    return false
}

func (this *HuLib) check_7dui(cards []int, gui_num int) bool {
    need := 0
    for i:=0; i<34; i++ {
        if cards[i] % 2 != 0 {
            need = need + 1
        }
    }

    if need > gui_num {
        return false
    }

    return true
}
