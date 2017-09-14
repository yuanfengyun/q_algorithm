package.path = "../../lualib/?.lua;"..package.path

local utils = require "utils"
local wave_table = require "auto_table"
local wave_table_eye = require "auto_table_with_eye"

local CardType = {
    [0x10] = {min = 1, max = 9, chi = true},
    [0x20] = {min = 10, max = 18, chi = true},
    [0x30] = {min = 19, max = 27, chi = true},
    [0x40] = {min = 28, max = 34, chi = false},
}

local CardDefine = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, -- 万
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, -- 筒
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, -- 条
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, -- 东、南、西、北、中、发、白
}

local M = {}

M.COLOR_WAN = 0x10
M.COLOR_TONG = 0x20
M.COLOR_TIAO = 0x30
M.COLOR_ZI = 0x40
M.COLOR_HUA = 0x50

function M.get_card_str(index)
    if index >= 1 and index <= 9 then
        return index .. "万"
    elseif index >= 10 and index <= 18 then
        return (index - 9) .. "筒"
    elseif index >= 19 and index <= 27 then
        return (index - 18) .. "条"
    end

    local t = {"东","西","南","北","中","发","白"}
    return t[index - 27]
end

function M.print(tbl)
    local str = ""
    local card_str
    for i=1,34 do
        if tbl[i] > 0 then
            card_str = M.get_card_str(i)
        end

        if tbl[i] == 1 then
            str = str .. card_str
        elseif tbl[i] == 2 then
            str = str .. card_str .. card_str
        elseif tbl[i] == 3 then
            str = str .. card_str .. card_str .. card_str
        elseif tbl[i] == 4 then
            str = str .. card_str .. card_str .. card_str .. card_str
        end
    end

    print(str)
end

-- 创建一幅牌,牌里存的不是牌本身，而是牌的序号
function M.create(zi)
    local t = {}

    local num = 3*9

    if zi then
        num = num + 7
    end

    for i=1,num do
        for _=1,4 do
            table.insert(t, i)
        end
    end

    return t
end

-- 洗牌
function M.shuffle(t)
    for i=#t,2,-1 do
        local tmp = t[i]
        local index = math.random(1, i - 1)
        t[i] = t[index]
        t[index] = tmp
    end
end

function M.can_peng(hand_cards, card)
    return hand_cards[card] >= 2
end

function M.can_angang(hand_cards, card)
    return hand_cards[card] == 4
end

function M.can_diangang(hand_cards, card)
    return hand_cards[card] == 3
end

function M.can_chi(hand_cards, card1, card2)
    if not hand_cards[card1] or not hand_cards[card2] then
        return false
    end

    if hand_cards[card1] == 0 or  hand_cards[card2] == 0 then
        return false
    end

    local color1 = CardDefine[hand_cards[card1]] & 0xf0
    local color2 = CardDefine[hand_cards[card2]] & 0xf0

    if color1 ~= color2 then
        return false
    end

    -- 本种花色不能吃
    if not CardType[color1].chi then
        return false
    end

    return true
end

function M.can_left_chi(hand_cards, card)
    return M.can_chi(hand_cards, card + 1, card + 2)
end

function M.can_middle_chi(hand_cards, card)
    return M.can_chi(hand_cards, card - 1, card + 1)
end

function M.can_right_chi(hand_cards, card)
    return M.can_chi(hand_cards, card - 2, card - 1)
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

    for color, cfg in pairs(CardType) do
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
