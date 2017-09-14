local table_mgr = require "table_mgr"
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

function M.get_hu_info(hand_cards, waves, cur_card)
    local cards = {}
    for i,v in ipairs(hand_cards) do
        cards[i] = v
    end

    if cur_card then
        cards[cur_card] = cards[cur_card] + 1
    end

    local first_info = {
        eye = false,            -- 当前是否有将
    }

    -- 字牌（东西南北中发白)
    if not M.check_color(cards, 28, 34, first_info) then
        return false
    -- 万
    elseif not M.check_color_chi(cards, 1, 9, first_info) then
        return false
    -- 筒
    elseif not M.check_color_chi(cards, 10, 18, first_info) then
        return false
    -- 条
    elseif not M.check_color_chi(cards, 19, 27, first_info) then
        return false
    end
    return true
end

function M.check_color(cards, min, max, info)
    for i = min, max do
        local count = cards[i]

        if count == 1 or count == 4 then
            return false
        end

        if count == 2 then
            if info.eye then
                return false
            end

            info.eye = true
        end
    end

    return true
end

function M.check_color_chi(cards, min, max, info)
    local key = 0
    for i = min, max do
        local c = cards[i]
        if c > 0 then
            key = key * 10 + c
        elseif key > 0 then
            local eye = (key%3 == 2)
            if info.eye and eye then
                return false
            end

            if not table_mgr:check(key) then
                return false
            end
            key = 0
        end
    end

    return true
end

return M
