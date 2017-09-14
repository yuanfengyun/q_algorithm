local split = require "splitlib"

local hu_cache = {
	[0] = {},
	[1] = {},
	[2] = {},
	[3] = {},
	[4] = {},
}
local gui_tested = {}
local gui_eye_tested = {}
for i=0,8 do
    gui_tested[i] = {}
    gui_eye_tested[i] = {}
end
local test_tbl = {
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
}

function print_cards(cards)
	local str = ""
	for i=1,34 do
		str = str .. cards[i]..","
		if i == 9 or i == 18 or i == 27 then
			str = str .. "  "
		end
	end
	print(str)
end

local CANT_HU_COUNT = 0

local function check_add(cards, gui_num, eye)
    local key = 0

    for i=1,9 do
        key = key * 10 + cards[i]
    end

    if key == 0 then
        return false
    end

    local m
    if not eye then
        m = gui_tested[gui_num]
    else
        m = gui_eye_tested[gui_num]
    end

    if m[key] then
        return false
    end

    m[key] = true

    for i=1,9 do
        if cards[i] > 4 then
            return true
        end
    end

	hu_cache[gui_num][key] = true
	
    return true
end

local function parse_table_sub(cards, num, eye)
    for i=1,9 do
        repeat
            if cards[i] == 0 then
                break
            end

            cards[i] = cards[i] - 1

            if not check_add(cards, num, eye) then
                cards[i] = cards[i] + 1
                break
            end
            if num < 4 then
                parse_table_sub(cards, num + 1, eye)
            end
            cards[i] = cards[i] + 1
        until(true)
    end
end

local function parse_table(cards, eye)
    if not check_add(cards, 0, eye) then
        return
    end
    parse_table_sub(cards, 1, eye)
end

local function gen_table_sub(t, level, eye)
    for j=1,16 do
        repeat
            if j <= 9 then
                if t[j] > 3 then
                    break
                end
                t[j] = t[j] + 3
            elseif j<= 16 then
                local index = j - 9
                if t[index] >= 4 or t[index+1] >= 4 or t[index+2] >= 4 then
                    break
                end
                t[index] = t[index] + 1
                t[index + 1] = t[index + 1] + 1
                t[index + 2] = t[index + 2] + 1
            end
            parse_table(t, eye)
            if level < 4 then
                gen_table_sub(t, level + 1, eye)
            end

            if j<= 9 then
                t[j] = t[j] - 3
            else
                local index = j - 9
                t[index] = t[index] - 1
                t[index + 1] = t[index + 1] - 1
                t[index + 2] = t[index + 2] - 1
            end
        until(true)
    end
end

local function collect_hu()
    local t = {0,0,0,0,0,0,0,0,0}

    for i=1,9 do
		print("check jiang",i)
        t[i] = 2
        parse_table(t, true)
        gen_table_sub(t, 1, true)
        t[i] = 0
    end
end

local tested
local tested_total

function test_sub(cards, level, max_level)
	for i=1,9 do
		repeat
			local n = cards[i]
			if n > 3 then
				break
			end
			
			cards[i] = n + 1
			
			local key = 0
			for k=1,9 do
				key = key * 10 + cards[k]
			end
			
			if tested[level][key] then
				cards[i] = n
				break
			end
			
			tested[level][key] = true
			
			
			if level == max_level then
				tested_total = tested_total + 1
				if split.get_hu_info(cards, 34) then
					if not hu_cache[0][key] then
						print("不能胡的牌型胡了")
					end
				end
			elseif level == max_level - 1 then
				cards[34] = 1
				if split.get_hu_info(cards, 34) then
					if not hu_cache[1][key] then
						print("1个赖子，不能胡的牌型胡了")
					end
				end 
				cards[34] = 0
			elseif level == max_level - 2 then
				cards[34] = 2
				if split.get_hu_info(cards, 34) then
					if not hu_cache[2][key] then
						print("2个赖子，不能胡的牌型胡了")
					end
				end 
				cards[34] = 0
			elseif level == max_level - 3 then
				cards[34] = 3
				if split.get_hu_info(cards, 34) then
					if not hu_cache[3][key] then
						print("3个赖子，不能胡的牌型胡了")
					end
				end 
				cards[34] = 0
			elseif level == max_level - 4 then
				cards[34] = 4
				if split.get_hu_info(cards, 34) then
					if not hu_cache[4][key] then
						print("4个赖子，不能胡的牌型胡了")
					end
				end 
				cards[34] = 0
			end
			
			if level ~= max_level then
				test_sub(cards, level+1, max_level)
			end
			cards[i] = n
		until true
	end
end

function test(max_level)
	local cards = {
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0
	}
	tested = {}
	tested_total = 0
	for i=1,14 do
		table.insert(tested,{})
	end
	print("test cards num = ",max_level)
	test_sub(cards, 1, max_level)
	print("total num =",tested_total)
end

collect_hu()
test(2)
test(5)
test(8)
test(11)
test(14)