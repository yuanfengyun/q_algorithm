package mjlib

import (
)

const MAX_CARD = 34

type HuLib struct {
}

func (this *HuLib) GetHuInfo(cards []int, cur_card int, gui_1 int, gui_2 int) bool {
    if cur_card != MAX_CARD {
        cards[cur_card]++
    }

    gui_num_1 := 0
	gui_num_2 := 0
    if gui_1 != MAX_CARD {
        gui_num_1 = cards[gui_1]
        cards[gui_1] = 0
    }

    if gui_2 != MAX_CARD {
        gui_num_2 = cards[gui_2]
        cards[gui_2] = 0
    }

    hu := this.split(cards, gui_num_1 + gui_num_2)

	if gui_1 != MAX_CARD {
        cards[gui_1] = gui_num_1
    }

    if gui_2 != MAX_CARD {
        cards[gui_2] = gui_num_2
    }

	if cur_card != MAX_CARD {
        cards[cur_card]--
    }

    return hu
}

func check(gui int, eye_num int, gui_num int, gui_sum int) (bool, int) {
	if gui < 0 {
		return false, 0
	}

	gui_sum += gui
	if gui_sum > gui_num {
		return false, 0
	}

	if eye_num == 0{
		return true, gui_sum
	}

	return gui_sum + (eye_num - 1) <= gui_num, gui_sum
}

func (this *HuLib) split(cards []int, gui_num int) bool {
    eye_num := 0
	gui_sum := 0
	gui := 0
	ret := false
	
	gui, eye_num = this._split(cards, gui_num, 0, 8, true, eye_num)
	ret, gui_sum = check(gui, eye_num, gui_num, gui_sum)
	if ret == false {
		return false
	}
	
	gui, eye_num = this._split(cards, gui_num-gui_sum, 9, 17, true, eye_num)
	ret, gui_sum = check(gui, eye_num, gui_num, gui_sum)
	if ret == false {
		return false
	}

	gui, eye_num = this._split(cards, gui_num-gui_sum, 18, 26, true, eye_num)
	ret, gui_sum = check(gui, eye_num, gui_num, gui_sum)
	if ret == false {
		return false
	}

	gui, eye_num = this._split(cards, gui_num-gui_sum, 27, 33, false, eye_num)
	ret, gui_sum = check(gui, eye_num, gui_num, gui_sum)
	if ret == false {
		return false
	}

	if eye_num == 0{
		return gui_sum + 2 <= gui_num
	}

	return true
}

func (this *HuLib) _split(cards []int, gui_num int, min int, max int, chi bool, eye_num int) (int, int) {
    key := 0
    num := 0

    for i := min; i <= max; i++ {
        key = key*10 + cards[i]
        num = num + cards[i]
    }

	if num == 0 {
		return 0, eye_num
	}

	for i := 0; i <= gui_num; i++ {
		yu := (num + i) % 3
		if yu == 1 {
			continue
		}
		eye := (yu == 2)
		if MTableMgr.check(key, i, eye, chi){
			if eye {
				eye_num++
			}
			return i, eye_num
		}
	}

	return -1, 0
}

func (this *HuLib) check_7dui(cards []int, gui_num int) bool {
    need := 0
    for i := 0; i < 34; i++ {
        if cards[i]%2 != 0 {
            need = need + 1
        }
    }

    if need > gui_num {
        return false
    }

    return true
}
