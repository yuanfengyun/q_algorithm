local utils = require "utils"
local mjlib = require "mjlib"

local M = {}

function M.get_tips(cards)
    local begin_tbl = {}
    local ting_tbl = M.get_ting_cards(cards)
    for card,_ in pairs(ting_tbl) do
        local begin = math.floor((card-1)/9)*9+1
        begin_tbl[begin] = true
    end

    if not next(ting_tbl) then
        assert(false, "不能听牌")
        return {}
    end

    local ret = {}
    for begin,_ in pairs(begin_tbl) do
        if begin == 28 then
            M.get_feng_tips(cards, ting_tbl, ret_tbl)
        else
            M.get_color_tips(cards, ting_tbl, begin, ret)
        end
    end

    return ret
end

function M.get_feng_tips(cards,ting_tbl,ret_tbl)
    for i=28,34 do
        if ting_tbl[i] then
            local n = cards[i]
            while n > 0 do
                table.insert(ret_tbl, i)
                n = n - 1
            end
        end
    end
end

function M.get_color_tips(cards, ting_tbl, begin, ret_tbl)
    local color_cards = {}
    for _,v in ipairs(cards) do
        table.insert(color_cards, v)
    end

    -- 遍历所有的可以组合的牌
    local ke_tbl, shun_tbl, dui_tbl = M.get_split(color_cards, begin, begin+8)

    -- 遍历能拆出的刻子
    for c,_ in pairs(ke_tbl) do
        M.sub_ke(color_cards, c, ting_tbl)
    end

    -- 遍历可能拆出的顺子
    for c,_ in pairs(shun_tbl) do
        M.sub_shun(color_cards, c, ting_tbl)
    end

    -- 遍历可能拆出的对子
    for c,_ in pairs(dui_tbl) do
        M.sub_dui(color_cards, c, ting_tbl)
    end

    -- 收集剩余胡牌，即为需要展示的牌
    for i=begin, begin+8 do
        local v = color_cards[i]
        while v>0 do
            table.insert(ret_tbl, i)
            v = v - 1
        end
    end
end

function M.sub_ke(cards, i, ting_tbl)
    if cards[i] < 3 then
        return false
    end

    cards[i] = cards[i] - 3
    for card,_ in pairs(ting_tbl) do
        if not M.check_hu(cards, card) then
            cards[i] = cards[i] + 3
            return false
        end
    end

    return true
end

function M.sub_shun(cards, i, ting_tbl)
    if cards[i] < 1 or cards[i+1] < 1 or cards[i+2] < 1 then
        return false
    end

    cards[i] = cards[i] - 1
    cards[i+1] = cards[i+1] - 1
    cards[i+2] = cards[i+2] - 1
    for c,_ in pairs(ting_tbl) do
        if not M.check_hu(cards, c) then
            cards[i] = cards[i] + 1
            cards[i+1] = cards[i+1] + 1
            cards[i+2] = cards[i+2] + 1
            return false
        end
    end
    return true
end

function M.sub_dui(cards, i, ting_tbl)
    if cards[i] < 2 then
        return false
    end

    cards[i] = cards[i] - 2
    for card,_ in pairs(ting_tbl) do
        if not M.check_hu(cards, card) then
            cards[i] = cards[i] + 2
            return false
        end
    end

    return true
end

function M.check_hu(cards, card)
    cards[card] = cards[card] + 1
    local count = 0
    for _,c in ipairs(cards) do
        count = count + c
    end

    local yu = count % 3
    if yu == 1 then
        cards[card] = cards[card] - 1
        return false
    end

    local add_eye
    if yu == 0 then
        for i=28,34 do
            if cards[cards] == 0 then
                cards[cards] = 2
                add_eye = i
            end
        end
    end

    local ret = mjlib.check_hu(cards)
    cards[card] = cards[card] - 1
    if add_eye then
        cards[add_eye] = cards[add_eye] - 2
    end
    return ret
end

-- 获取能拆出的牌
function M.get_split(cards, from, to)
    local ke_tbl = {}
    local shun_tbl = {}
    local dui_tbl = {}
    for i=from, to do
        local c = cards[i]
        if c >= 2 then
            dui_tbl[i] = true
            if c >= 3 then
                ke_tbl[i] = true
            end
        end

        if c >= 1 and i+2 <= to and cards[i+1] >= 1 and cards[i+2] >= 1 then
            shun_tbl[i] = true
        end
    end
    return ke_tbl, shun_tbl, dui_tbl
end

function M.get_ting_cards(cards)
    local t = {}
    for i=1,34 do
       if cards[i] < 4 and mjlib.check_hu(cards, i) then
            t[i] = true
       end
    end

    return t
end

return M
