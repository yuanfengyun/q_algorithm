-- 包含各种胡牌牌型检测

local M = {}

-- 判断七对，不带鬼牌
function M.is_7_dui(cards)
    local sum = 0
    for _,v in ipairs(cards) do
        sum = sum + v
        if v == 1 or v == 3 then
            return false
        end 
    end

    return sum == 14
end

-- 判断七对，带鬼牌
function M.is_7_dui_with_gui(cards, gui_index)
    local sum = 0
    local gui_num = cards[gui_index]
    cards[gui_index] = 0
    local need_gui = 0
    for i,v in ipairs(cards) do
        sum = sum + v
        if v == 1 or v == 3 then
            need_gui = need_gui + 1
        end 
    end
    cards[gui_index] = gui_num
    return sum + gui_num == 14 and gui_num >= need_gui
end

--- 判断十三幺
function M.is_13_19(cards)
    local sum = 0
    local index_tbl = {1,9,10,18,19,27,28,29,30,31,32,33,34}
    for _, i in ipairs(index_tbl) do
        local c = cards[i]
        if c ~= 1 and c ~= 2 then
            return false
        end
        sum = sum + c
    end
    return sum == 14
end

return M
