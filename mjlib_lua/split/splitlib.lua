local utils = require "utils"

local M = {}
function M.get_hu_info(cards, gui_index)
	local gui_num = 0
	if gui_index >= 1 and gui_index <= 34 then 
		gui_num = cards[gui_index]
		cards[gui_index] = 0
	end
  
	local eye_tbl = {}
	local empty = -1
	--for循环选出将牌
	for i = 1, 34 do 
		--优化手段，三不靠的牌，必做将
		local min = math.floor((i-1) / 9) * 9 + 1
		local max = min + 8
		if min == 28 then
           max = 34
		end
		
		if cards[i] == 1 and
		   (i-2 < min or cards[i-2] == 0) and
		   (i-1 < min or cards[i-1] == 0) and
		   (i+1 > max or cards[i+1] == 0) and
		   (i+2 > max or cards[i+2] == 0) then
			if gui_num < 0 then
				return false
			end
		    eye_num = 1
			eye_tbl = {i}
			empty = -1
			break
		end
		if empty == -1 and cards[i] == 0 then
	       empty = i
        end

		if cards[i] > 0 and (cards[i] + gui_num >= 2) then
			table.insert(eye_tbl, i)
		end
	end
	if empty > 0 then
		table.insert(eye_tbl, 1, empty)
	end

	local hu = false
	local cache = { 0,0,0,0 }
	-- 减去将牌 3*n
	for _, eye in ipairs(eye_tbl) do
		if eye == empty then
			hu = M.foreach_eye(cards, gui_num - 2, gui_num, 1000, cache)
		else
			local n = cards[eye]
			--1-9 10-18 19-27 28-34
			local eye_color = math.floor((eye-1)/9)
			if n == 1 then
				cards[eye] = 0
				hu = M.foreach_eye(cards, gui_num - 1, gui_num, eye_color, cache)
			else
				cards[eye] = cards[eye] - 2
				hu = M.foreach_eye(cards, gui_num, gui_num, eye_color, cache)
			end
			cards[eye] = n
		end
		if hu then
			break
		end
	end

	if gui_num > 0 then
		cards[gui_index] = gui_num
	end
	return hu
end


function M.foreach_eye(cards, gui_num, max_gui, eye_color, cache)
	local left_gui = gui_num
	for i = 0, 2 do
		local cache_index = -1
		if eye_color ~= i then
			-- 代表将的花色
		    cache_index = i
		end
 
		local need_gui = M.check_normal(cards, i*9+1, (i*9+1)+8, max_gui, cache_index, cache)
		if cache_index >= 0 then
			cache[cache_index] = need_gui+1
		end
		left_gui = left_gui - need_gui
		if left_gui < 0 then
			return false
		end
	end

	local cache_index = -1
	if eye_color ~= 3 then 
	   cache_index = 3
	end
	local need_gui = M.check_zi(cards, max_gui, cache_index, cache)
	if cache_index> 1 then
		cache[4] = need_gui + 1
	end
	return left_gui >= need_gui
end

function M.check_normal(cards, from, to, max_gui, cache_index, cache)
	if cache_index >= 1 then
		local n = cache[cache_index]
		if n > 0 then
		   return n - 1
		end
	end

	local n = 0
	for i = from, to do
		n = n * 10 + cards[i]
	end
	
	if n == 0 then
	   return 0
	end

	--[[local n3 = false
	for i = 0, max_gui do
		if (n + i) % 3 == 0 then
			n3 = true
			break
		end
	end

	if not n3 then
		return max_gui + 1
	end]]--

	return M.next_split(n, 0, max_gui)
end

function M.next_split(n, need_gui, max_gui)
	local c = 0
	repeat
		if n == 0 then
		   return need_gui
		end
		
		while (n > 0) do
			c = n % 10
			n = math.floor(n / 10)
			if c ~= 0 then
			   break
			end
		end
		if c == 1 or c == 4 then
			return M.one(n, need_gui, max_gui)
		elseif c == 2 then
			return M.two(n, need_gui, max_gui)
		end
	until false
	return need_gui
end

function M.one(n, need_gui, max_gui)
	local c1 = n % 10
	local c2 = math.floor((n % 100) / 10)

	if c1 == 0 then
	    need_gui = need_gui + 1
	else 
		n = n - 1
    end

	if c2 == 0 then
	   need_gui = need_gui + 1
	else 
	   n = n - 10
	end

	if n == 0 then
	   return need_gui
	end

	if need_gui > max_gui then
	   return need_gui
	end

	return M.next_split(n, need_gui, max_gui)
end

function M.two(n, need_gui, max_gui)
	local c1 = n % 10
	local c2 = math.floor((n % 100) / 10)
	local c3 = math.floor((n % 1000) / 100)
	local c4 = math.floor((n % 10000) / 1000)
    
	local choose_ke = true
	if c1 == 0 then
		-- c1 == 0 全拆刻子
	elseif c1 == 1 then
		-- 刻子
		if c2 == 0 or c2 == 1 then
			
		elseif c2 == 2 then
			if c3 == 2 then
				if c4 == 2 then
					choose_ke = false
				end
			elseif c3 == 3 then
				if c4 ~= 2 then
					choose_ke = false
				end
			else
				choose_ke = false
			end
		elseif c2 == 3 then
			if c3 == 0 or c3 == 2 or c3 == 1 or c3 == 4 then
				choose_ke = false
			end
		elseif c2 == 4 then
			if c3 == 2 then
				if c4 == 2 or c4 == 3 or c4 == 4 then
					choose_ke = false
				end
			elseif c3 == 3 then
				choose_ke = false
			end
		end
	elseif c1 == 2 then
		choose_ke = false
	elseif c1 == 3 then
		if c2 == 2 then
			if c3 == 1 or c3 == 4 then
				choose_ke = false
			elseif c3 == 2 then
				if c4 ~= 2 then
					choose_ke = false
				end
			end
		end
		if c2 == 3 then
			choose_ke = false
		elseif c2 == 4 then
			if c3 == 2 then
				choose_ke = false
			end
		end
	elseif c1 == 4 then
		if c2 == 2 and c3 ~= 2 then
			choose_ke = false
		elseif c2 == 3 then
			if c3 == 0 or c3 == 1 or c3 == 2 then
				choose_ke = false
			end
		elseif c2 == 4 then
			if c3 == 2 then
				choose_ke = false
			end
		end
	end

	if choose_ke then
		need_gui = need_gui + 1
	else
		if c1 < 2 then
			need_gui = need_gui + (2 - c1)
			n = n - c1
		else 
			n = n - 2
		end

		if c2 < 2 then
			need_gui = need_gui + (2 - c2)
			n = n - c2*10
		else
			n = n - 20
		end
	end

	if n == 0 then
	   return need_gui
	end

	if need_gui > max_gui then
	   return need_gui
	end

	return M.next_split(n, need_gui, max_gui)
end

function M.check_zi(cards,  max_gui, cache_index, cache)
    
	if cache_index >= 1 then
		local n = cache[cache_index]
		if n > 0 then
		   return n - 1
		end
	end

	local need_gui = 0
    
	for i = 28, 34 do
		local c = cards[i]
		if c == 1 or c == 4 then
			need_gui = need_gui + 2
		elseif c == 2 then
			need_gui = need_gui + 1
		end
		if need_gui > max_gui then
		   return need_gui
		end
	end
	return need_gui
end

return M
