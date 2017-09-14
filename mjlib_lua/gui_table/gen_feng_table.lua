local table_mgr = require "table_mgr"

local gui_tested = {}
local gui_eye_tested = {}
for i=0,8 do
    gui_tested[i] = {}
    gui_eye_tested[i] = {}
end

local function check_add(cards, gui_num, eye)
    local key = 0

    for i=1,7 do
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

    for i=1,7 do
        if cards[i] > 4 then
            return true
        end
    end
    table_mgr:add(key, gui_num, eye, false)
    return true
end

local function parse_table_sub(cards, num, eye)
    for i=1,7 do
        repeat
            if cards[i] == 0 then
                break
            end

            cards[i] = cards[i] - 1

            if not check_add(cards, num, eye) then
                cards[i] = cards[i] + 1
                break
            end
            if num < 8 then
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

local function gen_table_sub(cards, level, eye)
    for i=1,7 do
        repeat
            if cards[i] > 3 then
                break
            end
            cards[i] = cards[i] + 3
            parse_table(cards, eye)
            if level < 4 then
                gen_table_sub(cards, level + 1, eye)
            end
            cards[i] = cards[i] - 3
        until(true)
    end
end

local function gen_table()
    local t = {0,0,0,0,0,0,0}
    gen_table_sub(t, 1, false)
end

local function gen_eye_table()
    local t = {0,0,0,0,0,0,0}

    for i=1,7 do
        t[i] = 2
        parse_table(t, true)
        gen_table_sub(t, 1, true)
        t[i] = 0
    end
end

local function main()
    table_mgr:init()
    gen_table()
    gen_eye_table()
    table_mgr:dump_feng_table()
    print(total_key)
end

main()
