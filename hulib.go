package mjlib

const MaxCard = 34

func GetHuInfo(tblMgr *TableMgr, cards []int, curCard int, gui1 int, gui2 int) bool {
	if curCard != MaxCard {
		cards[curCard]++
	}

	guiNum1 := 0
	guiNum2 := 0
	if gui1 != MaxCard {
		guiNum1 = cards[gui1]
		cards[gui1] = 0
	}

	if gui2 != MaxCard {
		guiNum2 = cards[gui2]
		cards[gui2] = 0
	}

	hu := split(tblMgr, cards, guiNum1+guiNum2)

	if gui1 != MaxCard {
		cards[gui1] = guiNum1
	}

	if gui2 != MaxCard {
		cards[gui2] = guiNum2
	}

	if curCard != MaxCard {
		cards[curCard]--
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

	if eye_num == 0 {
		return true, gui_sum
	}

	return gui_sum+(eye_num-1) <= gui_num, gui_sum
}

func split(tblMgr *TableMgr, cards []int, guiNum int) bool {
	eyeNum := 0
	guiSum := 0
	gui := 0
	ret := false

	gui, eyeNum = subSplit(tblMgr, cards, guiNum, 0, 8, false, eyeNum)
	ret, guiSum = check(gui, eyeNum, guiNum, guiSum)
	if ret == false {
		return false
	}

	gui, eyeNum = subSplit(tblMgr, cards, guiNum-guiSum, 9, 17, false, eyeNum)
	ret, guiSum = check(gui, eyeNum, guiNum, guiSum)
	if ret == false {
		return false
	}

	gui, eyeNum = subSplit(tblMgr, cards, guiNum-guiSum, 18, 26, false, eyeNum)
	ret, guiSum = check(gui, eyeNum, guiNum, guiSum)
	if ret == false {
		return false
	}

	gui, eyeNum = subSplit(tblMgr, cards, guiNum-guiSum, 27, 33, true, eyeNum)
	ret, guiSum = check(gui, eyeNum, guiNum, guiSum)
	if ret == false {
		return false
	}

	if eyeNum == 0 {
		return guiSum+2 <= guiNum
	}

	return true
}

func subSplit(tblMgr *TableMgr, cards []int, guiNum int, min int, max int, feng bool, eyeNum int) (int, int) {
	key := 0
	num := 0

	for i := min; i <= max; i++ {
		key = key*10 + cards[i]
		num = num + cards[i]
	}

	if num == 0 {
		return 0, eyeNum
	}

	for i := 0; i <= guiNum; i++ {
		yu := (num + i) % 3
		if yu == 1 {
			continue
		}
		eye := yu == 2
		if tblMgr.check(key, i, eye, feng) {
			if eye {
				eyeNum++
			}
			return i, eyeNum
		}
	}

	return -1, 0
}

func check_7dui(cards []int, gui_num int) bool {
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
