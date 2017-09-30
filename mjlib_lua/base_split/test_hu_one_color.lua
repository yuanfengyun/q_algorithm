local split = require "base_split"

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
	
	for i=1,9 do
		test_tbl[i] = cards[i]
	end
	test_tbl[34] = gui_num
    if not split.get_hu_info(test_tbl) then
		print_cards(test_tbl)
		CANT_HU_COUNT = CANT_HU_COUNT + 1
	end
    return true
end

local function parse_table(cards, eye)
    check_add(cards, 0, eye)
end

local function gen_table_sub(t, level, eye)
    for j=1,16 do
        repeat
            if j <= 9 then
                if t[j] > 1 then
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

local function test()
    local t = {0,0,0,0,0,0,0,0,0}

    for i=1,9 do
		print("check jiang",i)
        t[i] = 2
        parse_table(t, true)
        gen_table_sub(t, 1, true)
        t[i] = 0
    end
end

test()
if CANT_HU_COUNT == 0 then
	print("all check success.")
else
	print("check failed ", CANT_HU_COUNT, "times")
end
