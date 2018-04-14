package mjlib

import (
	"fmt"
	"testing"
	"time"
)

var tm TableMgr

func init() {
	tm = TableMgr{}
	tm.Init()
}

func TestGenGui(t *testing.T) {
	for i := 0; i < 9; i++ {
		tm.cache[i] = map[int]bool{}
	}
	startTime := time.Now()
	tm.genGui([]int{3, 3, 3, 3, 0, 0, 0, 0, 0}, false)
	fmt.Println("cost: ", time.Now().Sub(startTime).Nanoseconds())
	//fmt.Println(genGuiCount)
	//fmt.Println(genGuiDpCount)
	//fmt.Println(genGuiDpTimeCount/int64(genGuiDpCount))
	//fmt.Println(addCount)
	//fmt.Println(realAddCount)
	//for i:=0;i<9;i++{
	//	fmt.Printf("tab[%d] len[%d]\n",i,len(MTableMgr.tbl[i].tbl))
	//}
}

func TestGenTable(t *testing.T) {
	startTime := time.Now()
	tm.Gen()
	fmt.Println("cost: ", time.Now().Sub(startTime).Nanoseconds())
	//fmt.Println(genGuiCount)
	//fmt.Println(genGuiDpCount)
	//fmt.Println(genGuiDpTimeCount/int64(genGuiDpCount))
	//for i:=0;i<9;i++{
	//	fmt.Printf("tab[%d] len[%d]\n",i,len(MTableMgr.tbl[i].tbl))
	//}
}

func BenchmarkGenTable(b *testing.B) {
	for i := 0; i < b.N; i++ {
		tm.Gen()
	}
}

func TestGenTableTabCorrect(t *testing.T) {
	compMTableMgr := &TableMgr{}
	compMTableMgr.Init()
	err := compMTableMgr.LoadTable("tbl", false)
	if err != nil {
		panic(err)
	}
	correct := true
	tm.Gen()
	for i := 0; i < 9; i++ {
		//i:=1
		t1 := compMTableMgr.tbl[i]
		fmt.Printf("tab[%d] len[%d]\n", i, len(compMTableMgr.tbl[i].tbl))
		t2 := tm.tbl[i]
		fmt.Printf("r-tab[%d] len[%d]\n", i, len(tm.tbl[i].tbl))
		for k := range t1.tbl {
			if _, ok := t2.tbl[k]; !ok {
				correct = false
				//fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n",k,i)
			}
		}
		for k := range t2.tbl {
			if _, ok := t1.tbl[k]; !ok {
				correct = false
				//fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n",k,i)
			}
		}
		fmt.Println()
	}
	fmt.Println(correct)
	if !correct{
		panic("unmatch")
	}
}

func TestGenTableEyeTabCorrect(t *testing.T) {
	compMTableMgr := &TableMgr{}
	compMTableMgr.Init()
	compMTableMgr.LoadTable("tbl", false)
	correct := true
	tm.Gen()
	for i := 0; i < 9; i++ {
		//i:=1
		t1 := compMTableMgr.eyeTbl[i]
		fmt.Printf("eyeTbl[%d] len[%d]\n", i, len(compMTableMgr.eyeTbl[i].tbl))
		t2 := tm.eyeTbl[i]
		fmt.Printf("r-eyeTbl[%d] len[%d]\n", i, len(tm.eyeTbl[i].tbl))
		for k := range t1.tbl {
			if _, ok := t2.tbl[k]; !ok {
				correct = false
				//fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n",k,i)
			}
		}
		for k := range t2.tbl {
			if _, ok := t1.tbl[k]; !ok {
				correct = false
				//fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n",k,i)
			}
		}
		fmt.Println()
	}
	fmt.Println(correct)
	if !correct{
		panic("unmatch")
	}
}

func TestGenTableFengTabCorrect(t *testing.T) {
	compMTableMgr := &TableMgr{}
	compMTableMgr.Init()
	compMTableMgr.LoadFengTable("tbl", false)
	correct := true
	tm.Gen()
	for i := 0; i < 9; i++ {
		//i:=0
		t1 := compMTableMgr.fengTbl[i]
		fmt.Printf("fengTbl[%d] len[%d]\n", i, len(compMTableMgr.fengTbl[i].tbl))
		t2 := tm.fengTbl[i]
		fmt.Printf("r-fengTbl[%d] len[%d]\n", i, len(tm.fengTbl[i].tbl))
		for k := range t1.tbl {
			if _, ok := t2.tbl[k]; !ok {
				correct = false
				fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n", k, i)
			}
		}
		for k := range t2.tbl {
			if _, ok := t1.tbl[k]; !ok {
				correct = false
				fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n", k, i)
			}
		}
		fmt.Println()
	}
	fmt.Println(correct)
	if !correct{
		panic("unmatch")
	}
}

func TestGenTableFengEyeTabCorrect(t *testing.T) {
	compMTableMgr := &TableMgr{}
	compMTableMgr.Init()
	compMTableMgr.LoadFengTable("tbl", false)
	correct := true
	tm.Gen()
	for i := 0; i < 9; i++ {
		//i:=0
		t1 := compMTableMgr.fengEyeTbl[i]
		fmt.Printf("fengEyeTbl[%d] len[%d]\n", i, len(compMTableMgr.fengEyeTbl[i].tbl))
		t2 := tm.fengEyeTbl[i]
		fmt.Printf("r-fengEyeTbl[%d] len[%d]\n", i, len(tm.fengEyeTbl[i].tbl))
		for k := range t1.tbl {
			if _, ok := t2.tbl[k]; !ok {
				correct = false
				fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n", k, i)
			}
		}
		for k := range t2.tbl {
			if _, ok := t1.tbl[k]; !ok {
				correct = false
				fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n", k, i)
			}
		}
		fmt.Println()
	}
	fmt.Println(correct)
	if !correct{
		panic("unmatch")
	}
}

func TestDump(t *testing.T) {
	tm.Gen()
	err := tm.DumpTable("tbl1", false)
	if err != nil {
		panic(err)
	}
	err = tm.DumpFengTable("tbl1", false)
	if err != nil {
		panic(err)
	}
}

func TestDumpCompress(t *testing.T) {
	tm.Gen()
	err := tm.DumpTable("tbl1", true)
	if err != nil {
		panic(err)
	}
	err = tm.DumpFengTable("tbl1", true)
	if err != nil {
		panic(err)
	}
}

func TestLoad(t *testing.T) {
	tm.Gen()
	err := tm.DumpTable("tbl1", false)
	if err != nil {
		panic(err)
	}
	err = tm.DumpFengTable("tbl1", false)
	if err != nil {
		panic(err)
	}
	testLoad("tbl", false)
}

func TestLoadCompress(t *testing.T) {
	tm.Gen()
	err := tm.DumpTable("tbl1", true)
	if err != nil {
		panic(err)
	}
	err = tm.DumpFengTable("tbl1", true)
	if err != nil {
		panic(err)
	}
	testLoad("tbl1", true)
}

func testLoad(dir string, compress bool) {
	compMTableMgr := &TableMgr{}
	compMTableMgr.Init()
	err := compMTableMgr.LoadTable(dir, compress)
	if err != nil {
		panic(err)
	}
	err = compMTableMgr.LoadFengTable(dir, compress)
	if err != nil {
		panic(err)
	}
	correct := true
	tm.Gen()
	for i := 0; i < 9; i++ {
		{
			t1 := compMTableMgr.tbl[i]
			fmt.Printf("tab[%d] len[%d]\n", i, len(compMTableMgr.tbl[i].tbl))
			t2 := tm.tbl[i]
			fmt.Printf("r-tab[%d] len[%d]\n", i, len(tm.tbl[i].tbl))
			for k := range t1.tbl {
				if _, ok := t2.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n",k,i)
				}
			}
			for k := range t2.tbl {
				if _, ok := t1.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n",k,i)
				}
			}
			fmt.Println()
		}
		{
			t1 := compMTableMgr.eyeTbl[i]
			fmt.Printf("eyeTbl[%d] len[%d]\n", i, len(compMTableMgr.eyeTbl[i].tbl))
			t2 := tm.eyeTbl[i]
			fmt.Printf("r-eyeTbl[%d] len[%d]\n", i, len(tm.eyeTbl[i].tbl))
			for k := range t1.tbl {
				if _, ok := t2.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n",k,i)
				}
			}
			for k := range t2.tbl {
				if _, ok := t1.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n",k,i)
				}
			}
			fmt.Println()
		}
		{
			t1 := compMTableMgr.fengEyeTbl[i]
			fmt.Printf("fengEyeTbl[%d] len[%d]\n", i, len(compMTableMgr.fengEyeTbl[i].tbl))
			t2 := tm.fengEyeTbl[i]
			fmt.Printf("r-fengEyeTbl[%d] len[%d]\n", i, len(tm.fengEyeTbl[i].tbl))
			for k := range t1.tbl {
				if _, ok := t2.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n",k,i)
				}
			}
			for k := range t2.tbl {
				if _, ok := t1.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n",k,i)
				}
			}
			fmt.Println()
		}
		{
			t1 := compMTableMgr.fengTbl[i]
			fmt.Printf("fengTbl[%d] len[%d]\n", i, len(compMTableMgr.fengTbl[i].tbl))
			t2 := tm.fengTbl[i]
			fmt.Printf("r-fengTbl[%d] len[%d]\n", i, len(tm.fengTbl[i].tbl))
			for k := range t1.tbl {
				if _, ok := t2.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t1->1 t2->0 k[%v] i[%v]\n",k,i)
				}
			}
			for k := range t2.tbl {
				if _, ok := t1.tbl[k]; !ok {
					correct = false
					//fmt.Printf("incorrect t2->1 t1->0 k[%v] i[%v]\n",k,i)
				}
			}
			fmt.Println()
		}
	}
	fmt.Println(correct)
	if !correct{
		panic("unmatch")
	}
}
