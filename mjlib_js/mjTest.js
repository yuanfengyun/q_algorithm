"use strict";

let mjlib = require( './api.js' );


function  init(){
	mjlib.Init();
	mjlib.MTableMgr.LoadTable();
	mjlib.MTableMgr.LoadFengTable();
}

console.log("initing");
init();
console.log("init  end");

function test_one_success() {
	//定义34中牌型
	var cards = [
		0, 0, 0, 0, 0, 0, 0, 0, 0, 
		1, 1, 1, 2, 3, 0, 0, 0, 0,
		0, 4, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0,
	];

	// 筒  万 条 
	//  混儿  1张 2筒
	console.log("测试能胡的牌型");
	// print_cards(cards)
	if (mjlib.MHulib.get_hu_info(cards, 34, 19, 34) ){
		console.log("测试通过：胡牌")
	} else {
		console.log("测试失败：能胡的牌型判断为不能胡牌")
	}
}

function test_one_fail() {
	var cards = [
		0, 1, 1, 1, 0, 0, 1, 0, 1,
		0, 1, 1, 1, 0, 0, 2, 2, 2,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
	]

    if (!mjlib.MHulib.get_hu_info(cards, 34, 34, 34) ){
		console.log("测试通过：不能胡牌")
	} else {
		console.log("测试失败：不能胡的牌型判断为能胡牌")
	}
}


console.log("Start  testing");

test_one_success();
test_one_fail();


console.log("End testing");