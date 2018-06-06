local table_mgr = require "table_mgr"
local M = {}

local SMALL_SHUN_HUXI = 3   --小牌顺子-2 7 10 胡息
local SMALL_KAN_HUXI = 3    --小牌坎-2 7 10 胡息

local BIG_SHUN_HUXI = 6		--小牌顺子-2 7 10 胡息
local BIG_KAN_HUXI = 6      --大牌坎-2 7 10 胡息

function M:init()
	table_mgr:load()
end

function M:get_hu_info(cards,need_huxi)
    -- 选出3个的牌
	local xiao_kan = 0
	local da_kan = 0
	local small_cards = 0
	local big_cards = 0
	local small_num = 0
	local big_num = 0
	for i=1,10 do
		local small = cards[i]
		if small == 3 then
			xiao_kan = xiao_kan + 1
		elseif small > 0 then
			small_cards = small_cards | (small << ((i-1)*2))
			small_num = small_num + small
		end

		local big = cards[i+10]
		if big == 3 then
			da_kan = da_kan + 1
		elseif big > 0 then
			big_cards = big_cards | (big << ((i-1)*2))
			big_num = big_num + big
		end
	end

	local kan_huxi = xiao_kan*SMALL_KAN_HUXI + da_kan*BIG_KAN_HUXI
	-- 全是坎
	if small_num == 0 and big_num == 0 then
		return kan_huxi
	end
	
	local tbl = table_mgr:get_table()
	local eye_tbl = table_mgr:get_eye_table()

	-- 全是大牌
	if small_num == 0 then
		return self:get_single_shun(big_cards, big_num, BIG_SHUN_HUXI, tbl, eye_tbl)
	-- 全是小牌
	elseif big_num == 0 then
		return self:get_single_shun(small_cards, small_num, SMALL_SHUN_HUXI, tbl, eye_tbl)
	end

	-- 不需要将
	if (small_num + big_num) % 3 == 0 then
		local shun_huxi = self:gethuxi(small_cards,small_num,SMALL_SHUN_HUXI,big_cards,big_num,BIG_SHUN_HUXI,tbl,eye_tbl)
		if not shun_huxi then
			return
		end
		return shun_huxi + kan_huxi
	end

	-- 需要将
	local huxi1 = self:gethuxi(small_cards,small_num,SMALL_SHUN_HUXI,big_cards,big_num,BIG_SHUN_HUXI,tbl,eye_tbl)
	local huxi2 = self:gethuxi(big_cards,big_num,BIG_SHUN_HUXI,small_cards,small_num,SMALL_SHUN_HUXI,tbl,eye_tbl)
	local shun_huxi = huxi1
	if not huxi1 or (huxi2 and huxi2 > huxi1) then
		shun_huxi = huxi2
	end
	
	if not shun_huxi then
		return
	end
	
	return shun_huxi + kan_huxi
end

function M:get_single_shun(cards, num, shun_huxi, tbl, eye_tbl)
	local v
	if num % 3 == 0 then
		v = tbl[cards]
	else
		v = eye_tbl[cards]
	end

	if not v then
		return 
	end

	local n = (v & 0xff00) >> 8
	return n * shun_huxi
end

function M:gethuxi(cards1,cards1_num,shun_huxi1,cards2,cards2_num,shun_huxi2,tbl,eye_tbl)
	local max_huxi
    for k,v in pairs(tbl) do
		local n = v & 255
		if n <= cards1_num then
			local c1 = cards1
			local c2 = cards2
			local c2_n = cards2_num
			local bit = 0
			local mask = 3
			local fail = false
			for i=1,10 do
				local num_target = (k & mask) >> bit
			    local num = (c1 & mask) >> bit
				if num_target > num then
					fail = true
					break
				elseif num_target < num then
					local num2 = (c2 & mask) >> bit
					local left = num - num_target
					if left + num2 < 3 then
						fail = true
						break
					end
					c2 = c2 - ((3-left) << bit)
					c2_n = c2_n - (3-left)
				end
				
				bit = bit + 2
				mask = mask << 2
			end

			if not fail then
				--print("not fail",k,c2,ca)
				local shun1 = (v & 0xff00) >> 8
				local shun2
				if c2_n == 0 then
					shun2 = 0
				else
					if c2_n % 3 == 0 then
						shun2 = tbl[c2]
					elseif c2_n % 3 == 2 then
						shun2 = eye_tbl[c2]
					end
				end
				if shun2 then
					shun2 = (shun2 & 0xff00) >> 8
					local cur_huxi = shun1 * shun_huxi1 + shun2 * shun_huxi2
					if not max_huxi or max_huxi > cur_huxi then
						max_huxi = cur_huxi
					end
				end
			end
		end
	end
	return max_huxi
end

return M

