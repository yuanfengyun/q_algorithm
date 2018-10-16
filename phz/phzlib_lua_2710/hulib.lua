local M = {}

local tbl_huxi = {
	-- 大2小2  1-9
	{[11]=2,[12]=2,[13]=2,[1]=2,[2]=2,[3]=2,huxi=18},
	{[11]=2,[12]=2,[13]=2,[2]=2,[7]=2,[10]=2,huxi=18},
	{[11]=2,[12]=2,[13]=2,[1]=1,[2]=2,[3]=1,[7]=1,[10]=1,huxi=18},
	{[12]=2,[17]=2,[20]=2,[1]=2,[2]=2,[3]=2,huxi=18},
	{[12]=2,[17]=2,[20]=2,[2]=2,[7]=2,[10]=2,huxi=18},
	{[12]=2,[17]=2,[20]=2,[1]=1,[2]=2,[3]=1,[7]=1,[10]=1,huxi=18},
	{[11]=1,[12]=2,[13]=1,[17]=1,[20]=1,[1]=2,[2]=2,[3]=2,huxi=18},
	{[11]=1,[12]=2,[13]=1,[17]=1,[20]=1,[2]=2,[7]=2,[10]=2,huxi=18},
	{[11]=1,[12]=2,[13]=1,[17]=1,[20]=1,[1]=1,[2]=2,[3]=1,[7]=1,[10]=1,huxi=18},
	-- 大2小1  10-15
	{[11]=2,[12]=2,[13]=2,[1]=1,[2]=1,[3]=1,huxi=15},
	{[11]=2,[12]=2,[13]=2,[2]=1,[7]=1,[10]=1,huxi=15},
	{[12]=2,[17]=2,[20]=2,[1]=1,[2]=1,[3]=1,huxi=15},
	{[12]=2,[17]=2,[20]=2,[2]=1,[7]=1,[10]=1,huxi=15},
	{[11]=1,[12]=2,[13]=1,[17]=1,[20]=1,[1]=1,[2]=1,[3]=1,huxi=15},
	{[11]=1,[12]=2,[13]=1,[17]=1,[20]=1,[2]=1,[7]=1,[10]=1,huxi=15},
	-- 大2小0 16-18
	{[11]=2,[12]=2,[13]=2,huxi=12},
	{[12]=2,[17]=2,[20]=2,huxi=12},
	{[11]=1,[12]=2,[13]=1,[17]=1,[20]=1,huxi=12},
	-- 大1小2 19-24
	{[11]=1,[12]=1,[13]=1,[1]=2,[2]=2,[3]=2,huxi=12},
	{[11]=1,[12]=1,[13]=1,[2]=1,[7]=1,[10]=1,huxi=12},
	{[11]=1,[12]=1,[13]=1,[1]=1,[2]=2,[3]=1,[7]=1,[10]=1,huxi=12},
	{[12]=1,[17]=1,[20]=1,[1]=2,[2]=2,[3]=2,huxi=12},
	{[12]=1,[17]=1,[20]=1,[2]=2,[7]=2,[10]=2,huxi=12},
	{[12]=1,[17]=1,[20]=1,[1]=1,[2]=2,[3]=1,[7]=1,[10]=1,huxi=12},
	-- 大1小1  25-28
	{[11]=1,[12]=1,[13]=1,[1]=1,[2]=1,[3]=1,huxi=9},
	{[11]=1,[12]=1,[13]=1,[2]=1,[7]=1,[10]=1,huxi=9},
	{[12]=1,[17]=1,[20]=1,[1]=1,[2]=1,[3]=1,huxi=9},
	{[12]=1,[17]=1,[20]=1,[2]=1,[7]=1,[10]=1,huxi=9},
	-- 大1小0  29-30
	{[11]=1,[12]=1,[13]=1,huxi=6},
	{[12]=1,[17]=1,[20]=1,huxi=6},
	-- 大0小2  31-33
	{[1]=2,[2]=2,[3]=2,huxi=6},
	{[2]=2,[7]=2,[10]=2,huxi=6},
	{[1]=1,[2]=2,[3]=1,[7]=1,[10]=1,huxi=6},
	-- 大0小1  34-35
	{[1]=1,[2]=1,[3]=1,huxi=3},
	{[2]=1,[7]=1,[10]=1,huxi=3},
	-- 大0小0
}

local tbl_split = {
	-- 1顺子
	{[0]=0,[10]=1,[11]=1,[12]=1},
	{[0]=1,[1]=1,[2]=1,[10]=0},
	-- 2顺子
	{[0]=0,[10]=2,[11]=2,[12]=2},
	{[0]=2,[1]=2,[2]=2,[10]=0},
	{[0]=1,[1]=1,[2]=1,[10]=1,[11]=1,[12]=1},
	-- 3顺子
	{[0]=1,[1]=1,[2]=1,[10]=2,[11]=2,[12]=2},
	{[0]=2,[1]=2,[2]=2,[10]=1,[11]=1,[12]=1},
	-- 1绞
	{[0]=1,[10]=2},
	{[0]=2,[10]=1},
	-- 1绞一顺
	{[0]=2,[10]=2,[11]=1,[12]=1},
	{[0]=2,[1]=1,[2]=1,[10]=2}
}

local function can_hu(cards,i)
	while cards[i] == 0 do
		if i == 10 then
			return true
		end
		i = i + 1
	end
	for _,split in ipairs(tbl_split) do
		if split[0]==cards[i] and split[10]==cards[i+10] then
			local can = true
			if (split[2] and (i+2>10 or cards[i+2]<split[2])) or (split[1] and (i+1>10 or cards[i+1]<split[1])) then
				can = false
			end
			
			if (split[12] and (i+12>20 or cards[i+12]<split[12])) or (split[11] and (i+11>20 or cards[i+11]<split[11])) then
				can = false
			end
			if can then
				-- 扣除相关牌
				for k,v in pairs(split) do
					cards[i+k] = cards[i+k] - v
				end
				if can_hu(cards,i+1) then
					return true
				end
				-- 加回相关牌
				for k,v in pairs(split) do
					cards[i+k] = cards[i+k] + v
				end
			end
		end
	end

	return false
end

function M.get_huinfo(cards,need_huxi)
	local tcards = {
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	}
	local sum = 0
	local kan_huxi = 0
	for i,v in pairs(cards) do
		if v < 3 then
			tcards[i] = v
		else
			kan_huxi = kan_huxi + 3
			if i > 10 then
				kan_huxi = kan_huxi + 3
			end
		end
		sum = sum + v
	end
	
	if sum % 3 == 0 then
		local ret = M._get_huinfo(tcards,need_huxi)
		if ret >= 0 then
			return ret + kan_huxi
		end
		return -1
	end

	-- 有将的情况
	for i=10,1,-1 do
		if tcards[i] == 2 then
			tcards[i] = 0
			local ret = M._get_huinfo(tcards,need_huxi)
			if ret >= 0 then
				return ret + kan_huxi
			end
			tcards[i] = 2
		end
	end
	
	for i=20,11,-1 do
		if tcards[i] == 2 then
			tcards[i] = 0
			local ret = M._get_huinfo(tcards,need_huxi)
			if ret >= 0 then
				return ret + kan_huxi
			end
			tcards[i] = 2
		end
	end
	
	return -1
end

function M._get_huinfo(cards,need_huxi)
	for k,v in ipairs(tbl_huxi) do
		if v.huxi < need_huxi then
			return -1
		end
		-- 检查相应牌
		local check = true
		for card,num in pairs(v) do
			if type(card)=="number" and cards[card] < num then
				check = false
				break
			end
		end
		
		if check then
			-- 扣除相应牌
			for card,num in pairs(v) do
				if type(card) == "number" then
					cards[card] = cards[card] - num
				end
			end
			
			if can_hu(cards,1) then
				return v.huxi
			end

			-- 加回相应牌
			for card,num in pairs(v) do
				if type(card) == "number" then
					cards[card] = cards[card] + num
				end
			end
		end
	end
	if can_hu(cards,1) then
		return 0
	end
	return -1
end

return M





