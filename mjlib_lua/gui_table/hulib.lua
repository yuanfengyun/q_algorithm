local mjlib = require "mjlib"
local table_mgr = require "table_mgr"

local split_table = {
    {min = 1,  max = 9,  chi = true},
    {min = 10, max = 18, chi = true},
    {min = 19, max = 27, chi = true},
    {min = 28, max = 34, chi = false}
}

local M = {}
function M.check_7dui(hand_cards, waves)
    if #waves > 0 then return false end

    for _,c in ipairs(hand_cards) do
        if c % 2 ~= 0 then
            return false
        end
    end

    return true
end

function M.check_pengpeng()

end

function M.get_hu_info(cards, waves, gui_index)
    local hand_cards = {}
    for i,v in ipairs(cards) do
        hand_cards[i] = v
    end

    local gui_num = 0
    if gui_index > 0 then
        gui_num = hand_cards[gui_index]
        hand_cards[gui_index] = 0
    end

    local splited_tbl = M.split_info(hand_cards, gui_num)
    if not splited_tbl then
        return false
    end

    return M.check_probability(splited_tbl, gui_num)
end

function M.list_probability(gui_num, num, key, chi)
    local find = false
    local t = {}
    for i=0, gui_num do
        local yu = (num + i)%3
        if yu ~= 1 then
            local eye = (yu == 2)
            if find or table_mgr:check(key, i, eye, chi) then
                table.insert(t, {eye = eye, gui_num = i})
                find = true
            end
        end
    end

    return t
end

-- 根据花色切分
function M.split_info(t, gui_num)
    local ret = {}
    for _,v in ipairs(split_table) do
        local key = 0
        local num = 0
        for i=v.min,v.max do
            key = key*10 + t[i]
            num = num + t[i]
        end
        if num > 0 then
            local t = M.list_probability(gui_num, num, key, v.chi)
            if #t == 0 then
                return false
            end
            table.insert(ret, t)
        end
    end
    return ret
end

function M.check_probability_sub(splited_table, eye, gui_num, level, max_level)
    for _,v in ipairs(splited_table[level]) do
        repeat
            if eye and v.eye then
                break
            end

            if gui_num < v.gui_num then
                break
            end

            if level < max_level then
                if M.check_probability_sub(
                    splited_table,
                    eye or v.eye,
                    gui_num - v.gui_num,
                    level + 1,
                    max_level) then
                    return true
                end
                break
            end

            if not eye and not v.eye and gui_num < 2 + v.gui_num then
                break
            end

            return true
       until(true)
    end

    return false
end

function M.check_probability(splited_table, gui_num)
    local c = #splited_table
    -- 全是鬼牌
    if c == 0 then
        return true
    end

    -- 只有一种花色的牌和鬼牌
    if c == 1 then
        return true
    end

    -- 组合花色间的组合，如果能满足组合条件，则胡
    for i,v in ipairs(splited_table[1]) do
        local ret = M.check_probability_sub(splited_table, v.eye, gui_num - v.gui_num, 2, c)
        if ret then
            return true
        end
    end

    return false
end

return M
