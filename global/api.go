package mjlib

import "qipai/mjlib_go"

var GlobalTableMgr *mjlib.TableMgr

func Init(){
	GlobalTableMgr=&mjlib.TableMgr{}
	GlobalTableMgr.Init()
	GlobalTableMgr.Gen()
}

func GetHuInfo(cards []int, curCard int, gui1 int, gui2 int) bool {
	return mjlib.GetHuInfo(GlobalTableMgr,cards,curCard,gui1,gui2)
}

func DumpTable(dir string,compress bool) error{
	err:=GlobalTableMgr.DumpTable(dir,compress)
	if err!=nil{
		return err
	}
	err=GlobalTableMgr.DumpFengTable(dir,compress)
	if err!=nil{
		return err
	}
	return nil
}

func LoadTable(dir string,compress bool) error{
	err:=GlobalTableMgr.LoadTable(dir,compress)
	if err!=nil{
		return err
	}
	err=GlobalTableMgr.LoadFengTable(dir,compress)
	if err!=nil{
		return err
	}
	return nil
}