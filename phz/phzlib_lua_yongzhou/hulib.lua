local table_mgr = require "table_mgr"
local gen_jiao = require "gen_jiao"
local M = {}

function M:init()
	table_mgr:load()
	gen_jiao.gen_jiao()
end

function M:get_hu_info(cards,gui_num)
	local small_cards = 0
	local big_cards = 0
	local small_num = 0
	local big_num = 0
	
	local valid_jiaos = {}

	for i=1,10 do
		local bit = (i-1)*2
		local small = cards[i]
		if small > 0 then
			small_cards = small_cards | (small << bit)
			small_num = small_num + small
		end

		local big = cards[i+10]
		if big > 0 then
			big_cards = big_cards | (big << bit)
			big_num = big_num + big
		end

		-- 过滤掉本身不能做绞的可能
		if (small == 1 or small == 2) and (big == 1 or big == 2) and gui_num >= 3 - (small+big) then
			valid_jiaos[i] = true
			valid_jiaos[i+10] = true
		end
	end

	-- 全是大牌
	if small_num == 0 then
		return self:get_single_shun()
	-- 全是小牌
	elseif big_num == 0 then
		return self:get_single_shun()
	end
	
	return self:get_huxi(big_cards,big_num,small_cards,small_num,gui_num,valid_jiaos)
end

-- 只有一种牌
function M:get_single_shun()
	return
end

-- 遍历所有绞的组合，找出最大胡息
function M:get_huxi(big_cards,big_num,small_cards,small_num,gui_num,valid_jiaos)
	local max_huxi
	local jiao_tbl = gen_jiao.get_table()
	local tbl = table_mgr:get_table()
	local eye_tbl = table_mgr:get_eye_table()
	
	for _,v in ipairs(jiao_tbl) do
		local big = big_cards
		local small = small_cards
		local big_n = big_num
		local small_n = small_cards

		local pass = false
		local need_gui = 0
		
		for _,jiao in ipairs(v) do
			if not valid_jiaos[jiao] then
				pass = true
				break
			end
		end
		
		if not pass then
			for _,jiao in ipairs(v) do
				local need_da = 1
				local need_xiao = 2
				if jiao > 10 then
					need_da = 2
					need_xiao = 1
					jiao = jiao - 10
				end
				
				local bit = (jiao-1)*2
				local mask = 3 << bit
				local n_big = (big & mask ) >> bit
				local lack_da = 0
				if n_big < need_da then
					lack_da = need_da - n_big
				end
				
				if lack_da == 2 or lack_da + need_gui > gui_num then
					pass = true
					break
				elseif n_big > 0 then
					local use = need_da-lack_da
					big = big - (use << bit)
					big_n = big_n - use
				end

				local n_small = (small & mask ) >> bit
				local lack_xiao = 0
				if n_small < need_xiao then
					lack_xiao = need_xiao - n_small
				end
				
				if lack_xiao == 2 then
					pass = true
					break
				elseif n_small > 0 then
					local use = n_small-lack_xiao
					small = small - (use << bit)
					small_n = small_n - use
				end
				
				need_gui = need_gui + lack_da + lack_xiao
				if need_gui > gui_num then
					pass = true
					break
				end
			end
		end
			
		if not pass then
			-- 大小牌分别根据赖子个数查表
			local big_huxi
			if big_n == 0 then
				big_huxi = 0
			else
				local yu = big_n % 3
				if yu then
					big_huxi = tbl[big]
				elseif yu then
					big_huxi = eye_tbl[big]
				end
			end

			if big_huxi then
				local small_huxi
				if small_n == 0 then
					small_huxi = 0
				else
					local yu = small_n % 3
					if yu then
						small_huxi = tbl[small]
					elseif yu then
						small_huxi = eye_tbl[small]
					end
				end
				
				if small_huxi then
					local sum = small_huxi*3 + big_huxi*6
					if not max_huxi or sum > max_huxi then
						max_huxi = sum
					end
				end
			end
		end
	end
	return max_huxi
end

return M

