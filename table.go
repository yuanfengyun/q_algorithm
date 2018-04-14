package mjlib

import (
	"bufio"
	"encoding/gob"
	"fmt"
	"io"
	"os"
	"strconv"
)

type table struct {
	tbl map[int]int
}

func (t *table) init() {
	t.tbl = make(map[int]int)
}

func (t *table) check(key int) bool {
	_, ok := t.tbl[key]
	return ok
}

func (t *table) add(key int) {
	t.tbl[key] = 1
}

func (t *table) dump(name string, compress bool) error {
	//if t.tbl == nil {
	//	panic("table.tbl not init") //内部异常直接panic
	//}
	file, err := os.OpenFile(name, os.O_WRONLY|os.O_CREATE, 0666)
	if err != nil {
		return err
	}
	//先判断err，否则这里有panic风险
	defer file.Close()
	if compress {
		e := gob.NewEncoder(file)
		err = e.Encode(t.tbl)
		if err != nil {
			return err
		}
	} else {
		buf := bufio.NewWriter(file)
		for key := range t.tbl {
			n := int(key)
			fmt.Fprintf(buf, "%d\n", n)
		}
		buf.Flush()
	}
	return nil
}

func (t *table) load(name string, compress bool) error {
	file, err := os.Open(name)
	if err != nil {
		return err
	}
	//先判断err，否则这里有panic风险
	defer file.Close()
	reader := bufio.NewReader(file)
	if compress {
		t.init()
		d := gob.NewDecoder(file)
		err = d.Decode(&t.tbl)
		if err != nil {
			return err
		}
	} else {
		for {
			buf, _, err := reader.ReadLine()
			if err == io.EOF {
				break
			}
			str := string(buf)
			key, err := strconv.Atoi(str)
			if err != nil {
				return err
			}
			t.tbl[key] = 1
		}
	}
	return nil
}
