package mjlib

import (
	"fmt"
	"sync"
)

type TableMgr struct {
	rw         sync.RWMutex
	tbl        [9]*table
	eyeTbl     [9]*table
	fengTbl    [9]*table
	fengEyeTbl [9]*table
	//TODO cache 在gen过程中会利用这两个缓存，gen方法结束会清空
	cache    [9]map[int]bool
	eyeCache [9]map[int]bool
}

func (t *TableMgr) Init() {
	t.rw.Lock()
	defer t.rw.Unlock()
	for i := 0; i < 9; i++ {
		t.tbl[i] = &table{}
		t.tbl[i].init()

		t.eyeTbl[i] = &table{}
		t.eyeTbl[i].init()

		t.fengTbl[i] = &table{}
		t.fengTbl[i].init()

		t.fengEyeTbl[i] = &table{}
		t.fengEyeTbl[i].init()
	}
}

func (t *TableMgr) getTable(guiNum int, eye bool, feng bool) *table {
	var tbl *table
	if feng {
		if eye {
			tbl = t.fengEyeTbl[guiNum]
		} else {
			tbl = t.fengTbl[guiNum]
		}
	} else {
		if eye {
			tbl = t.eyeTbl[guiNum]
		} else {
			tbl = t.tbl[guiNum]
		}
	}
	return tbl
}

func (t *TableMgr) Add(key int, guiNum int, eye bool, feng bool) {
	t.rw.Lock()
	defer t.rw.Unlock()
	tbl := t.getTable(guiNum, eye, feng)
	tbl.add(key)
}

func (t *TableMgr) check(key int, guiNum int, eye bool, feng bool) bool {
	t.rw.RLock()
	defer t.rw.RUnlock()
	tbl := t.getTable(guiNum, eye, feng)
	return tbl.check(key)
}

func (t *TableMgr) LoadTable(dir string, compress bool) error {
	t.rw.Lock()
	defer t.rw.Unlock()
	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/table_%d.tbl", dir, i)
		err := t.tbl[i].load(name, compress)
		if err != nil {
			return err
		}
	}

	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/eye_table_%d.tbl", dir, i)
		err := t.eyeTbl[i].load(name, compress)
		if err != nil {
			return err
		}
	}
	return nil
}

func (t *TableMgr) DumpTable(dir string, compress bool) error {
	t.rw.RLock()
	defer t.rw.RUnlock()
	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/table_%d.tbl", dir, i)
		err := t.tbl[i].dump(name, compress)
		if err != nil {
			return err
		}
	}

	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/eye_table_%d.tbl", dir, i)
		err := t.eyeTbl[i].dump(name, compress)
		if err != nil {
			return err
		}
	}
	return nil
}

func (t *TableMgr) LoadFengTable(dir string, compress bool) error {
	t.rw.Lock()
	defer t.rw.Unlock()
	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/feng_table_%d.tbl", dir, i)
		err := t.fengTbl[i].load(name, compress)
		if err != nil {
			return err
		}
	}

	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/feng_eye_table_%d.tbl", dir, i)
		err := t.fengEyeTbl[i].load(name, compress)
		if err != nil {
			return err
		}
	}
	return nil
}

func (t *TableMgr) DumpFengTable(dir string, compress bool) error {
	t.rw.RLock()
	defer t.rw.RUnlock()
	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/feng_table_%d.tbl", dir, i)
		err := t.fengTbl[i].dump(name, compress)
		if err != nil {
			return err
		}
	}

	for i := 0; i < 9; i++ {
		name := fmt.Sprintf("%s/feng_eye_table_%d.tbl", dir, i)
		err := t.fengEyeTbl[i].dump(name, compress)
		if err != nil {
			return err
		}
	}
	return nil
}

func (t *TableMgr) add(cards []int, guiNum int, eye bool) bool {
	key := 0
	for i := 0; i < len(cards); i++ {
		key = key*10 + cards[i]
	}

	var c map[int]bool
	if eye {
		c = t.eyeCache[guiNum]
	} else {
		c = t.cache[guiNum]
	}

	if _, ok := c[key]; ok { // TODO 需要放在牌数判断前面，剪枝
		return false
	} else {
		c[key] = true
	}

	for i := 0; i < len(cards); i++ { //TODO 剪枝
		if cards[i] > 4 {
			return true
		}
	}
	feng := len(cards) == 7
	tbl := t.getTable(guiNum, eye, feng)
	tbl.add(key)
	return true
}

func (t *TableMgr) genDp(cards []int, eye bool, dpNum int, dpIndex int) {
	if dpNum == 4 {
		t.genGui(cards, eye)
		return
	}
	if dpIndex == len(cards) {
		return
	}
	i := dpIndex
	for k := 0; k <= 2; k++ {
		for s := 0; s <= (2-k)*2; s++ {
			addNum := 3*k + s
			kb := (cards[i] + addNum) <= 6
			sb := len(cards) == 9 && i < 7 && (cards[i]+addNum) <= 6 && (cards[i+1]+addNum) <= 6 && (cards[i+2]+addNum) <= 6
			if kb {
				cards[i] += 3 * k
			}
			if sb {
				cards[i] += s
				cards[i+1] += s
				cards[i+2] += s
			}
			if (kb || sb) && (dpNum+k+s) <= 4 {
				t.genDp(cards, eye, dpNum+k+s, i+1)
			}
			if kb {
				cards[i] -= 3 * k
			}
			if sb {
				cards[i] -= s
				cards[i+1] -= s
				cards[i+2] -= s
			}
		}
	}
}

func (t *TableMgr) genGui(cards []int, eye bool) {
	if !t.add(cards, 0, eye) {
		return
	}
	t.genGuidp(cards, eye, 1)
}

func (t *TableMgr) genGuidp(cards []int, eye bool, dpNum int) {
	for i := 0; i < len(cards); i++ {
		if cards[i] == 0 {
			continue
		}
		cards[i]--
		if !t.add(cards, dpNum, eye) { //TODO 剪枝
			cards[i]++
			continue
		}
		if dpNum < 8 {
			t.genGuidp(cards, eye, dpNum+1)
		}
		cards[i]++
	}
}

func (t *TableMgr) Gen() {
	t.rw.Lock()
	defer t.rw.Unlock()
	for i := 0; i < 9; i++ {
		t.cache[i] = map[int]bool{}
		t.eyeCache[i] = map[int]bool{}
	}
	cards := make([]int, 9)
	// 不带将
	t.genDp(cards, false, 0, 0)
	// 带将
	for i := 0; i < len(cards); i++ {
		cards[i] = 2
		t.genGui(cards, true)
		t.genDp(cards, true, 0, 0)
		cards[i] = 0
	}

	// 风牌表
	// 不带将
	for i := 0; i < 9; i++ { //clean
		t.cache[i] = map[int]bool{}
		t.eyeCache[i] = map[int]bool{}
	}
	cards = cards[:7]
	t.genDp(cards, false, 0, 0)
	// 带将
	for i := 0; i < len(cards); i++ {
		cards[i] = 2
		t.genDp(cards, true, 0, 0)
		cards[i] = 0
	}
	// 去除赖子不能符合规则的情况
	for i := 0; i < 9; i++ {
		if i == 0 || i%3 != 0 {
			delete(t.tbl[i].tbl, 0)
			delete(t.fengTbl[i].tbl, 0)
		}
	}
	t.cache = [9]map[int]bool{}
	t.eyeCache = [9]map[int]bool{}
}
