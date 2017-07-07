package.path = "../../lualib/?.lua;"..package.path
local utils = require "utils"
local wave_table = require "auto_table"
local wave_table_eye = require "auto_table_with_eye"
local mjlib = require "mjlib"

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

function M.get_hu_info(hand_cards, waves, self_card, other_card)
    local hand_cards_tmp = {}
    for i,v in ipairs(hand_cards) do
        hand_cards_tmp[i] = v
    end

    if other_card then
        hand_cards_tmp[other_card] = hand_cards_tmp[other_card] + 1
    end

    local first_info = {
        eye = false,            -- 当前是否有将
        dui_array = {},
    }

    for color, cfg in pairs(mjlib.CardType) do
        if cfg.chi and not M.check_color_chi(hand_cards_tmp, cfg, first_info) then
            return false
        elseif not cfg.chi and not M.check_color(hand_cards_tmp, cfg, first_info) then
            return false
        end
    end

    return true
end

function M.check_color(cards, cfg, info)
    for i = cfg.min, cfg.max do
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

function M.check_color_chi(cards, cfg, info)
    local tbl = {}
    for i = cfg.min, cfg.max do
        repeat
            local count = cards[i]
            if count > 0 then
                table.insert(tbl, count)
            else
                if #tbl == 0 then
                    break
                end

                if not M.check_sub(tbl, info) then
                    return false
                end
                tbl = {}
            end
        until(true)
    end

    return true
end

function M.check_sub(tbl, info)
    local count = 0
    for _,v in ipairs(tbl) do
        count = count + v
    end
    local yu = (count % 3)

    if yu == 1 then
        return false
    elseif yu == 2 then
        if info.eye then
            return false
        end

        return M.check_wave_and_eye(tbl)
    end

    return M.check_wave(tbl)
end

function M.check_wave(tbl)
    local num = 0
    for _,c in ipairs(tbl) do
        num = num * 10 + c
    end

    if wave_table[num] then
        return true
    end

    --wave_table[num] = true
    return false
end

-- 检查是否匹配3*n + 2
function M.check_wave_and_eye(tbl)
    if #tbl == 1 then
        return true
    end

    local num = 0
    for _,c in ipairs(tbl) do
        num = num * 10 + c
    end

    if wave_table_eye[num] then
        return true
    end

    local len = #tbl
    -- 拆出可能的眼位，再判断
    for i,v in ipairs(tbl) do
        repeat
            if v < 2 then
                break
            end

            local tmp_tbl_1 = {}
            local tmp_tbl_2 = {}
            for ii,vv in ipairs(tbl) do
                table.insert(tmp_tbl_1, vv)
            end

            if v > 2 then
                tmp_tbl_1[i] = v - 2
            else
                if i == 1 then
                    table.remove(tmp_tbl_1, 1)
                elseif i == len then
                    table.remove(tmp_tbl_1)
                else
                    for ii = i + 1, len do
                        table.insert(tmp_tbl_2, tbl[ii])
                    end
                    tmp_tbl_1[i] = nil
                end
            end

            if not M.check_wave(tmp_tbl_1) then
                break
            end

            if next(tmp_tbl_2) then
                if not M.check_wave(tmp_tbl_2) then
                    break
                end
            end

            return true
        until(true)
    end

    return false
end

return M
