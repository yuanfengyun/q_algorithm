local hulib = require "hulib"
local utils = require "utils"

-- 测试胡牌
function test_hu()
	-- 吃、碰、偎、跑、提的牌
	-- 结构 {t = "左吃、中吃、右吃、大大小绞，大小小绞、碰、偎、跑、提",c="中心牌"}
	local groups = {
		{t = "wei",card = 9} -- 偎九
	}

	--  手牌
	--  1,2,3,4,5,6,7,8,9,0
	local cards = {
		--0,0,0,0,0,1,1,1,0,0,
		--1,2,1,0,0,0,1,0,2,1,
		1,2,0,0,0,0,1,0,0,1,
		0,2,0,0,0,0,2,0,0,2
	}
	-- 王
	cards[21] = 0
	print("======= test hu =======")
	local begin = os.clock()
	
	-- 判断自已摸的牌是否能胡
	local self_huxi
	local other_huxi
	for i = 1,1 do
		self_huxi = hulib.get_huxi_self(cards,groups,3)
	--	other_huxi = hulib.get_huxi_other(cards,groups,9)
	end

	print("time",os.clock()-begin,"seconds")
	print("self huxi:",self_huxi)
	print("other huxi:",other_huxi)
end

-- 测试获取听的牌
function test_ting()
	-- 吃、碰、偎、跑、提的牌
	-- 结构 {t = "左吃、中吃、右吃、大大小绞，大小小绞、碰、偎、跑、提",c="中心牌"}
	local groups = {
		{t = "wei",card = 9} -- 偎九
	}

	--  手牌
	--  1,2,3,4,5,6,7,8,9,0
	local cards = {
		0,0,0,0,1,1,2,0,0,2,
		1,2,1,0,0,0,2,0,0,2,
	}

	print("======= test ting =======")
	local begin = os.clock()
	local ting_cards = hulib.get_ting_cards(cards,groups)
	print("time",os.clock()-begin,"seconds")
	print("ting cards:",utils.table_2_str(ting_cards))
end

test_hu()
--test_ting()
