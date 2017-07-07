package.path = "../../lualib/?.lua;"..package.path
local utils = require "utils"
local mjlib = require "mjlib"

local split_table = {
    {min = 1,  max = 9,  chi = true},
    {min = 10, max = 18, chi = true},
    {min = 19, max = 27, chi = true},
    {min = 28, max = 34, chi = false}
}

local check_table = {
    [0] = require "no_gui_table",
    [1] = require "one_gui_table",
    [2] = require "two_gui_table",
    [3] = require "three_gui_table",
    [4] = require "four_gui_table"
}

local check_eye_table = {
    [0] = require "no_gui_eye_table",
    [1] = require "one_gui_eye_table",
    [2] = require "two_gui_eye_table",
    [3] = require "three_gui_eye_table",
    [4] = require "four_gui_eye_table"
}

local check_feng_table = {
    [0] = require "no_gui_feng_table",
    [1] = require "one_gui_feng_table",
    [2] = require "two_gui_feng_table",
    [3] = require "three_gui_feng_table",
    [4] = require "four_gui_feng_table"
}

local check_feng_eye_table = {
    [0] = require "no_gui_feng_eye_table",
    [1] = require "one_gui_feng_eye_table",
    [2] = require "two_gui_feng_eye_table",
    [3] = require "three_gui_feng_eye_table",
    [4] = require "three_gui_feng_eye_table",
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

function M.get_hu_info(hand_cards, waves, gui_index)
    local hand_cards_tmp = {}
    for i,v in ipairs(hand_cards) do
        hand_cards_tmp[i] = v
    end

    local gui_num = hand_cards_tmp[gui_index]
    hand_cards_tmp[gui_index] = 0

    local splited_tbl = M.split_info(hand_cards_tmp, gui_num)
    if not splited_tbl then
        return false
    end

    return M.check_probability(splited_tbl, gui_num)
end

function M.check_table(key, gui_num, eye, chi)
    local tbl
    if chi then
        if eye then
            tbl = check_eye_table[gui_num]
        else
            tbl = check_table[gui_num]
        end
    else
        if eye then
            tbl = check_feng_eye_table[gui_num]
        else
            tbl = check_feng_table[gui_num]
        end
    end

    if tbl then
        return tbl[key]
    end
end

function M.list_probability(gui_num, num, key, chi)
    --nprint("gui_num", gui_num, "num",num, "key", key)
    local t = {}
    for i=0, gui_num do
        local yu = (num + i)%3
        if yu == 0 then
            if M.check_table(key, i, false, chi) then
                table.insert(t, {eye = false, gui_num = i})
            end
        elseif yu == 2 then
            if M.check_table(key, i, true, chi) then
                table.insert(t, {eye = true, gui_num = i})
            end
        end
    end

    --utils.print_array(t)
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
    --utils.print_array(ret)
    return ret
end

function M.check_probability_sub(splited_table, info, level)
    for _,v in ipairs(splited_table[level]) do
        repeat
            if info.eye and v.eye then
                break
            end

            if info.gui_num < v.gui_num then
                break
            end

            if level < info.c then
                info.gui_num = info.gui_num - v.gui_num
                local old_eye = info.eye
                info.eye = old_eye or v.eye
                if M.check_probability_sub(splited_table, info, level + 1) then
                    return true
                end
                info.eye = old_eye
                info.gui_num = info.gui_num + v.gui_num
                break
            end

            if not info.eye and not v.eye and info.gui_num < 2 + v.gui_num then
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
        local info = {
            eye = v.eye,
            gui_num = gui_num - v.gui_num,
            c = c
        }

        local ret = M.check_probability_sub(splited_table, info, 2)
        if ret then
            return true
        end
    end

    return false
end

return M
