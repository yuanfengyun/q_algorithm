local utils = require "utils"
local tbl_split = require "tbl_split"
local tbl_split1 = require "tbl_split1"
local tbl_split2 = require "tbl_split2"
local tbl_split3 = require "tbl_split3"

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
	
	for i=1,20 do
		tcards[i] = cards[i]
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
	local gui = cards[21]
	return tcards,gui,groups_huxi,tipao
end

-- 获取所有能听的牌
function M.get_ting_cards(cards,groups)
	local t = {}
	for i=1,20 do
		if M.get_huxi_self(cards,groups,i) > 0 or M.get_huxi_other(cards,groups,i) > 0 then
			table.insert(t,i)
		end
	end
	-- 判断是否胡王
	if M.get_huxi_self(cards,groups,21) then
		table.insert(t,21)
	end
	return t
end

-- 若能胡，返回胡息，若不能胡，返回-1
-- cards 表示手牌
-- groups 表示吃、碰、跑、提的牌
-- c表示自己摸到的牌
function M.get_huxi_self(cards,groups,c)
	local paohu_huxi = -1
	local tihu_huxi = -1
	local tcards,gui,groups_huxi,tipao = M.get_huxi_pre(cards,groups)

	if c == 21 then
		gui = gui + 1
	else
		-- 提胡
		if not tipao and tcards[c] == 3 then
			groups_huxi = groups_huxi + M.get_group_huxi("ti",c)
			return M.get_huinfo(tcards,groups_huxi,gui)
		end
		-- 提胡
		for _,group in pairs(groups) do
			if not tipao and group.t == "wei" and group.card == c then
				-- 提胡
				local add_huxi = M.get_group_huxi("ti",c)-M.get_group_huxi(group.t,c)
				return M.get_huinfo(tcards,groups_huxi+add_huxi,gui)
			end
		end
		
		-- 跑胡、破跑胡
		for _,group in pairs(groups) do
			if not tipao and group.t == "peng" and group.card == c then
				-- 跑胡
				local add_huxi = M.get_group_huxi("pao",c)-M.get_group_huxi(group.t,c)
				paohu_huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,gui)
			end
		end

		tcards[c] = tcards[c] + 1
	end

	-- 带赖子的提胡、跑胡
	if not tipao then
		-- 提胡、跑胡
		for _,group in pairs(groups) do
			if group.t == "wei" then				-- 提胡
				local add_huxi = M.get_group_huxi("ti",group.card)-M.get_group_huxi(group.t,group.card)
				local huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,gui-1)
			elseif group.t == "peng" then			-- 跑胡
				local add_huxi = M.get_group_huxi("ti",group.card)-M.get_group_huxi(group.t,group.card)
				local huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,gui-1)
			end
		end

		-- 提胡
		for i=1,20 do
			local old = tcards[i]
			if old + gui >= 4 then
				tcards[i] = 0
				local newgui = gui
				if old < 4 then
					newgui = gui - (4-old)
				end
				print(old,gui)
				local add_huxi = 0
				local huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,newgui)
				tcards[i] = old
			end
		end
	end

	-- 偎胡、吃胡
	local other_huxi = M.get_huinfo(tcards,groups_huxi,gui)

	return math.max(tihu_huxi,paohu_huxi,other_huxi)
end

-- 若能胡，返回胡息，若不能胡，返回-1
-- cards 表示手牌
-- groups 表示吃、碰、跑、提的牌
-- c表示别人摸、打的牌
function M.get_huxi_other(cards,groups,c)
	local tcards,gui,groups_huxi,tipao = M.get_huxi_pre(cards,groups)

	-- 跑胡、破跑胡
	if not tipao and tcards[c] == 3 then
		-- 破跑胡
		tcards[c] = tcards[c] + 1
		local popao_huxi = M._get_huxi_other(tcards,groups_huxi)

		-- 跑胡
		tcards[c] = 0
		local add_huxi = M.get_group_huxi("pao",i) - M.get_group_huxi("wei",i)
		local paohu_huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,gui)
		return math.max(popao_huxi,paohu_huxi)
	end

	-- 跑胡、破跑胡
	for _,group in pairs(groups) do
		if not tipao and (group.t == "peng" or group.t == "wei") and group.card == c then
			-- 破跑胡
			tcards[c] = 1
			local popaohu_huxi = M._get_huxi_other(tcards,groups_huxi)
			-- 跑胡
			local add_huxi = M.get_group_huxi("pao",c)-M.get_group_huxi(group.t,c)
			local paohu_huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,gui)
			return math.max(popaohu_huxi,paohu_huxi)			
		end
	end

	return M._get_huxi_other(tcards,groups,tipao,gui)
end

function M._get_huxi_other(cards,groups,tipao,gui)
	local paohu_huxi = 0
	local tihu_huxi = 0
	-- 带赖子的提胡、跑胡
	if not tipao and gui >= 1 then
		-- 跑胡
		for _,group in pairs(groups) do
			if group.t == "wei" or group.t == "peng" then -- 跑胡
				local add_huxi = M.get_group_huxi("ti",group.card)-M.get_group_huxi(group.t,group.card)
				local huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,gui-1)
			end
		end

		-- 提胡、跑胡
		for i=1,20 do
			local old = tcards[i]
			if old + gui >= 4 then
				tcards[i] = 0
				local newgui = gui
				if old < 4 then
					newgui = gui - (4-old)
				end
				local huxi = M.get_huinfo(tcards,groups_huxi+add_huxi,newgui)
				tcards[i] = old
			end
		end
	end

	-- 偎胡、吃胡
	local other_huxi = M.get_huinfo(tcards,groups_huxi,gui)

	return math.max(tihu_huxi,paohu_huxi,other_huxi)
end

-- 判断剩余的手牌是否能胡
function M.get_huinfo(cards,huxi,total_gui)
	local sum = 0
	for _,n in ipairs(cards) do
		sum = sum + n
	end
	if sum == 0 then
		return 0
	end
	-- 不需要将的情况
	local need_eye = (sum % 3 == 2)

	if M.can_hu(cards,1,need_eye,false,total_gui) then
		return 0
	end
	return -1
end

-- 尝试各种拆分方式，只要有一种能胡，则能胡
function M.can_hu(cards,i,needeye,eye,totalgui)
	if i > 10 then
		return true
	end
	while cards[i] + cards[i+10] == 0 do
		if i == 10 then
			return true
		end
		i = i + 1
	end
	local canhu = false
	local tbl = tbl_split2
	if i == 1 then
		tbl = tbl_split
	elseif i == 2 then
		tbl = tbl_split1
	elseif i > 8 then
		tbl = tbl_split3
	end
	for sk,split in ipairs(tbl) do
		repeat
			if (not needeye or eye) and split.eye then
				break
			end
			if cards[i] > split[1] or cards[i+10] > split[11] then
				break
			end
			local huxi = split.x
			if i == 1 then
				huxi = split.x
			end
			local gui = 0
			-- 检查够不够扣
			for c,n in pairs(split) do
				if type(c)=="number" and n > 0 then
					if cards[i+c-1] < n then
						gui = gui + n - cards[i+c-1]
					end
				end
			end
			-- 王不够
			if gui > totalgui then
				break
			end
			-- 扣除相关牌
			for c,n in pairs(split) do
				if type(c)=="number" and n > 0 then
					if cards[i+c-1] >= n then
						cards[i+c-1] = cards[i+c-1] - n
					else
						cards[i+c-1] = 0
					end
				end
			end
			canhu = M.can_hu(cards,i+1,need_eye,eye or split.eye,totalgui-gui) or canhu
			-- 加回相关牌
			for c,n in pairs(split) do
				if type(c)=="number" then
					cards[i+c-1] = cards[i+c-1] + n
				end
			end

		until true
	end

	return canhu
end

return M





