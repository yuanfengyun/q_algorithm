local utils = require "utils"
local tbl_huxi = require "tbl_huxi"
local tbl_split = require "tbl_split"

-- 默认可以胡牌的胡息
local HU_HUXI = 15

local M = {}

-- 获取特定类型，特定牌胡息
function M.get_group_huxi(t,card)
	if t == "peng" then		-- 碰
		if card <= 10 then
			return 1
		else
			return 3
		end
	elseif t == "wei" then	-- 偎
		if card <= 10 then
			return 3
		else
			return 6
		end
	elseif t == "pao" then	-- 跑
		if card <= 10 then
			return 6
		else
			return 9
		end
	elseif t == "ti" then	-- 提
		if card <= 10 then
			return 9
		else
			return 12
		end
	end
	return 0
end

-- 获取坎外手牌、坎牌胡息、桌面牌胡息
function M.get_huxi_pre(cards,groups)
	local tcards = {
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	}

	local kan_huxi = 0
	-- 计算牌张数、坎的胡息
	for i,v in pairs(cards) do
		if v == 3 then
			kan_huxi = kan_huxi + M.get_group_huxi("wei",i)
		else
			tcards[i] = v
		end
	end

	local tipao = false
	local groups_huxi = 0
	-- 获取桌上吃、碰、偎、跑、提的总胡息
	for _,group in ipairs(groups) do
		groups_huxi = groups_huxi + M.get_group_huxi(group.t,group.card)
		if group.t == "pao" or group.t == "ti" then
			tipao = true
		end
	end
	return tcards,kan_huxi,groups_huxi,tipao
end

-- 获取所有能听的牌
function M.get_ting_cards(cards,groups)
	local t = {}
	for i=1,20 do
		if M.get_huxi_self(cards,groups,i) > 0 or M.get_huxi_other(cards,groups,i) > 0 then
			table.insert(t,i)
		end
	end
	return t
end

-- 若能胡，返回胡息，若不能胡，返回-1
-- cards 表示手牌
-- groups 表示吃、碰、跑、提的牌
-- c表示自己摸到的牌
function M.get_huxi_self(cards,groups,c)
	local tcards,kan_huxi,groups_huxi,tipao = M.get_huxi_pre(cards,groups)

	-- 提胡
	if not tipao and cards[c] == 3 then
		local add_huxi = M.get_group_huxi("ti",c) - M.get_group_huxi("wei",c)
		return M.get_huinfo(tcards,kan_huxi+groups_huxi+add_huxi)
	end

	-- 跑胡、破跑胡
	for _,group in pairs(groups) do
		if (group.t == "peng" or group.t == "wei") and group.card == c then
			-- 跑胡
			if not tipao then
				local add_huxi = M.get_group_huxi("pao",c)-M.get_group_huxi(group.t,c)
				local r = M.get_huinfo(tcards,kan_huxi+groups_huxi+add_huxi)
				if r > 0 then
					return r
				end
			end
			-- 破跑胡
			tcards[c] = 1
			return M.get_huinfo(tcards,kan_huxi+groups_huxi)
		end
	end

	-- 偎胡
	if cards[c] == 2 then
		tcards[c] = 0
		local add_huxi = M.get_group_huxi("wei",c)
		return M.get_huinfo(tcards,kan_huxi+groups_huxi+add_huxi)
	end

	-- 吃胡
	tcards[c] = tcards[c] + 1
	return M.get_huinfo(tcards,kan_huxi+groups_huxi)
end

-- 若能胡，返回胡息，若不能胡，返回-1
-- cards 表示手牌
-- groups 表示吃、碰、跑、提的牌
-- c表示别人摸、打的牌
function M.get_huxi_other(cards,groups,c)
	local tcards,kan_huxi,groups_huxi,tipao = M.get_huxi_pre(cards,groups)

	-- 跑胡、破跑胡
	if cards[c] == 3 then
		if not tipao then
			local add_huxi = M.get_group_huxi("pao",c) - M.get_group_huxi("wei",c)
			local r = M.get_huinfo(tcards,kan_huxi+groups_huxi+add_huxi)
			if r > 0 then
				return r
			end
		end
		-- 破跑胡
		tcards[c] = 1
		return M.get_huinfo(tcards,kan_huxi+groups_huxi)
	end

	-- 跑胡、破跑胡
	for _,group in pairs(groups) do
		if (group.t == "peng" or group.t == "wei") and group.card == c then
			-- 跑胡
			if not tipao then
				local add_huxi = M.get_group_huxi("pao",c)-M.get_group_huxi(group.t,c)
				local r = M.get_huinfo(tcards,kan_huxi+groups_huxi+add_huxi)
				if r > 0 then
					return r
				end
			end
			-- 破跑胡
			tcards[c] = 1
			return M.get_huinfo(tcards,kan_huxi+groups_huxi)
		end
	end

	-- 碰胡
	if cards[c] == 2 then
		tcards[c] = 0
		local add_huxi = M.get_group_huxi("wei",c)
		local r = M.get_huinfo(tcards,kan_huxi+groups_huxi+add_huxi)
		tcards[c] = 2
		if r > 0 then
			return r
		end
	end

	-- 吃胡
	tcards[c] = tcards[c] + 1
	return M.get_huinfo(tcards,kan_huxi+groups_huxi)
end

-- 判断剩余的手牌是否能胡
function M.get_huinfo(cards,huxi)
	local need_huxi = HU_HUXI - huxi
	
	local sum = 0
	for _,n in ipairs(cards) do
		sum = sum + n
	end
	if sum == 0 then
		return huxi
	end
	-- 不需要将的情况
	local need_eye = (sum % 3 == 2)

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

			local canhu = M.can_hu(cards,1,need_eye)

			-- 加回相应牌
			for card,num in pairs(v) do
				if type(card) == "number" then
					cards[card] = cards[card] + num
				end
			end
			
			if canhu then
				return v.huxi + huxi
			end
		end
	end
	if need_huxi > 0 then
		return -1
	end
	if M.can_hu(cards,1,need_eye) then
		return huxi
	end
	return -1
end

-- 尝试各种拆分方式，只要有一种能胡，则能胡
function M.can_hu(cards,i,need_eye)
	if i > 10 then
		return true
	end
	while cards[i] + cards[i+10] == 0 do
		if i == 10 then
			return true
		end
		i = i + 1
	end
	for sk,split in ipairs(tbl_split) do
		if (not split.eye or need_eye) and split[0]==cards[i] and split[10]==cards[i+10] then
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
					if type(k)=="number" then
						cards[i+k] = cards[i+k] - v
					end
				end
				local canhu = M.can_hu(cards,i+1,need_eye)
				-- 加回相关牌
				for k,v in pairs(split) do
					if type(k)=="number" then
						cards[i+k] = cards[i+k] + v
					end
				end
				if canhu then
					return true
				end
			end
		end
	end

	return false
end

return M





