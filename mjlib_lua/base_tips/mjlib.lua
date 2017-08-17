local utils = require "utils"

local M = {}

function M.check_hu(hand_cards, card)
    local cards = {}
    for _,v in ipairs(hand_cards) do
        table.insert(cards, v)
    end

    if card then
        cards[card] = cards[card] + 1
    end

    local ret, eye_begin = M.check_eye(cards)
    if not ret then
        return false
    end

    if not M.check_color(cards, 1, eye_begin == 1) then
        return false
    end

    if not M.check_color(cards, 10, eye_begin == 10) then
        return false
    end

    if not M.check_color(cards, 19, eye_begin == 19) then
        return false
    end

--    if not M.check_feng(cards, 28, eye_begin == 28) then
--        return false
--    end

    return true
end

-- 检查眼
function M.check_eye(cards)
    local eye_begin
    local begin = 1
    local ret, eye = M._check_eye(cards, begin, begin+8, false)
    if not ret then
        return false
    end
    if eye then
        eye_begin = begin
    end

    begin = 10
    ret, eye = M._check_eye(cards, begin, begin+8, eye_begin ~= nil)
    if not ret then
        return false
    end
    if eye then
        eye_begin = begin
    end

    begin = 19
    ret, eye = M._check_eye(cards, begin, begin+8, eye_begin ~= nil)
    if not ret then
        return false
    end
    if eye then
        eye_begin = begin
    end

    begin = 28
    ret, eye = M._check_feng_eye(cards, begin, begin+6, eye_begin ~= nil)
    if not ret then
        return false
    end

    if eye then
        eye_begin = begin
    end

    return true, eye_begin
end

-- 检查单色眼
function M._check_eye(cards, from, to, eye)
    local sum = 0
    local dui
    for i=from,to do
        sum = sum + cards[i]
        if cards[i] >= 2 then
            dui = true
        end
    end

    local yu = sum % 3
    if yu == 1 then
        return false
    elseif yu == 2 then
        if not dui or eye then
            return false
        end
        return true, true
    elseif yu == 0 then
        return true, false
    end

    return false
end

function M._check_feng_eye(cards, from, to, eye)
    if eye then
        for i=from,to do
            if cards[i] >0 and cards[i] ~= 3 then
                return false
            end
        end
        return true, false
    else
        local eye_count = 0
        for i=from,to do
            if cards[i] == 2 then
                eye_count = eye_count + 1
            end
        end
        return eye_count == 1, true
    end
end

function M.check_feng(cards, begin, eye)
    local eye_count = 0
    for i=begin,begin+6 do
        local c = cards[i]
        if c == 2 then
            eye_count = eye_count + 1
        elseif c == 4 or c == 1 then
            return false
        end
    end

    if eye_count > 1 then
        return
    end

    if not eye then
        return eye_count == 0
    else
        return eye_count == 1
    end
end

function M.check_color(cards, begin, eye)
    if eye then
        return M._check_color_eye(cards, begin)
    else
        return M._check_color(cards, begin)
    end
end

function M._check_color_eye(cards, begin)
    -- 找出能做将的牌
    local eye_tbl = M.get_eye(cards, begin)
    local hu = false
    for i,_ in pairs(eye_tbl) do
        cards[i] = cards[i] - 2
        hu = M._check_color(cards, begin)
        cards[i] = cards[i] + 2
        if hu then
            break
        end
    end
    return hu
end

function M.get_eye(cards, begin)
    local eye_tbl = {}
    local sum = 0
    for i=begin,begin+8 do
        sum = sum + cards[i]
    end

    if sum % 3 ~= 2 then
        return eye_tbl
    end
    local key = 0
    local t = {}
    local to = begin + 8
    for i=begin,to do
        local c = cards[i]
        if c > 0 then
            key = key * 10 + c
            if c >= 2 then
                t[i] = true
            end
        end


        if c == 0 or i == to then
            if (key%3) == 2 then
                for k,_ in pairs(t) do
                    eye_tbl[k] = true
                end
            end
            if key > 0 and next(t) then
                t = {}
            end
        end
    end
    return eye_tbl
end

-- 检查单一花色
function M._check_color(cards, min)
    local max = min + 8
    local t = {}
    for _,v in ipairs(cards) do
        table.insert(t, v)
    end
    local i = min
    while i <= max do
        if (t[i] == 1) or (t[i] == 2) or (t[i] == 4) then
            if (i+2 > max) or (t[i+1] == 0) or (t[i+2] == 0) then
                return false
            end
            t[i] = t[i] - 1
            t[i+1] = t[i+1] - 1
            t[i+2] = t[i+2] - 1
        elseif t[i] == 3 then
            t[i] = 0
        end

        if t[i] == 0 then
            i = i + 1
        end
    end

    return true
end

return M
