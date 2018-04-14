package mjlib

import (
	"fmt"
	"testing"
	"time"
    "math/rand"
)

func printCards(cards []int) {
    for i := 0; i < 9; i++ {
        fmt.Printf("%d,", cards[i])
    }
    fmt.Printf("\n")

    for i := 9; i < 18; i++ {
        fmt.Printf("%d,", cards[i])
    }
    fmt.Printf("\n")

    for i := 18; i < 27; i++ {
        fmt.Printf("%d,", cards[i])
    }
    fmt.Printf("\n")

    for i := 27; i < 34; i++ {
        fmt.Printf("%d,", cards[i])
    }
    fmt.Printf("\n")
}

var tested = map[int]bool{}

func checkHu(cards []int, max int) {
    for i := 0; i < max; i++ {
        if cards[i] > 4 {
            return
        }
    }

    num := 0
    for i := 0; i < 9; i++ {
        num = num*10 + cards[i]
    }

    _, ok := tested[num]
    if ok {
        return
    }

    tested[num] = true

    for i := 0; i < max; i++ {
        if !GetHuInfo(&tm,cards, 34, 34, 34) {
            fmt.Printf("测试失败 i=%d\n", i)
			printCards(cards)
        }
    }
}

func genAutoTableSub(cards []int, level int) {
    for i := 0; i < 32; i++ {
        index := -1
        if i <= 17 {
            cards[i] += 3
        } else if i <= 24 {
            index = i - 18
        } else {
            index = i - 16
        }

        if index >= 0 {
            cards[index] += 1
            cards[index+1] += 1
            cards[index+2] += 1
        }

        if level == 4 {
			checkHu(cards, 18)
        } else {
			genAutoTableSub(cards, level+1)
        }

        if i <= 17 {
            cards[i] -= 3
        } else {
            cards[index] -= 1
            cards[index+1] -= 1
            cards[index+2] -= 1
        }
    }
}

func testTwoColor() {
    fmt.Println("测试两种花色")
    cards := []int{
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    }

    for i := 0; i < 18; i++ {
        cards[i] = 2
        fmt.Printf("将 %d\n", i+1)
		genAutoTableSub(cards, 1)
        cards[i] = 0
    }
}

func testOneSuccess() {
    cards := []int{
        0, 0, 0, 0, 0, 1, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 4, 4,
    }

    fmt.Println("测试1种能胡的牌型")
	printCards(cards)
    if GetHuInfo(&tm,cards, 34, 32, 33) {
        fmt.Println("测试通过：胡牌")
    } else {
        fmt.Println("测试失败：能胡的牌型判断为不能胡牌")
    }
}

func testOneFail() {
    cards := []int{
        0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 1, 1, 1, 0, 0, 2, 2, 2,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    }

    fmt.Println("测试1种不能胡的牌型")
	printCards(cards)
    if !GetHuInfo(&tm,cards, 34, 34, 34) {
        fmt.Println("测试通过：不能胡牌")
    } else {
        fmt.Println("测试失败：不能胡牌的牌型判断为胡了")
    }
}

func testTime(count int) {
    cards := []int{
        0, 0, 0, 0, 0, 1, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 4, 4,
    }

	printCards(cards)
    start := time.Now()
    for i:=0;i<count;i++{
        GetHuInfo(&tm,cards, 34, 32, 33)
    }
	printCards(cards)
    fmt.Println("count=",count,"use time=",time.Now().Sub(start).Nanoseconds())
}

func TestGetHuInfo(t *testing.T) {
    fmt.Println("test hulib begin...")
	tm.LoadTable("tbl",false)
	tm.LoadFengTable("tbl",false)
    testOneSuccess()
    testOneFail()
    //testTime(100000000)
    //    test_two_color()
}

//
func randomCards(guiNum int) ([]int,int){
    cards := []int{
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    }
    r:=rand.New(rand.NewSource(time.Now().UnixNano()))
    for i:=0;i<(14-guiNum);i++{
        for{
            x:=r.Intn(MaxCard)
            if cards[x]>=4{

            }else{
                cards[x]++
                break
            }
        }
    }
    guiIndex:=MaxCard
    if guiNum>0{
        for i:=0;i<MaxCard;i++{
            if cards[i]==0{
                cards[i]=guiNum
                guiIndex=i
            }
        }
    }
    return cards,guiIndex
}

func TestRandomCards(t *testing.T){
    for i:=0;i<1000;i++{
        randomCards(0)
    }
}

func benchmarkGetHuInfo(b *testing.B,guiNum int) {
    for i:=0;i<b.N;i++{
        b.StopTimer()
        cards,guiIndex:=randomCards(guiNum)
        b.StartTimer()
        GetHuInfo(&tm,cards, MaxCard, guiIndex, MaxCard)
    }
}

func BenchmarkGetHuInfo0(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,0)
}

func BenchmarkGetHuInfo1(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,1)
}

func BenchmarkGetHuInfo2(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,2)
}

func BenchmarkGetHuInfo3(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,3)
}

func BenchmarkGetHuInfo4(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,4)
}

func BenchmarkGetHuInfo5(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,5)
}

func BenchmarkGetHuInfo6(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,6)
}

func BenchmarkGetHuInfo7(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,7)
}

func BenchmarkGetHuInfo8(b *testing.B) {
    b.StopTimer()
    tm.Gen()
    b.StartTimer()
    benchmarkGetHuInfo(b,8)
}
