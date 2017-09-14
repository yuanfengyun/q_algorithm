local split = require "splitlib"

function test_success()
	local cards = {
		0,1,1,0,0,0,0,0,0,	-- 1-9万
		0,0,0,1,0,4,3,1,1,	-- 1-9条
		0,0,0,0,0,0,0,0,0,	-- 1-9筒
		0,0,0,0,0,0,2		-- 东南西北中发白
	}

	if split.get_hu_info(cards, 34) then
		print("test_success success")
	else
		print("test_success fail")
	end
end

function test_fail()
	local cards = {
		0,1,1,0,0,0,0,0,0,
		1,1,0,1,0,4,1,1,1,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,2
	}
	
	if not split.get_hu_info(cards, 34) then
		print("test_fail success")
	else
		print("test_fail fail")
	end
end

test_success()
test_fail()
