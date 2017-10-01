local splitlib = require "mjlib.base_split.splitlib"

local M = {}

-- 是否七对
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

-- 是否碰碰胡
function M.is_pengpeng(cards, weaves)
    for _, v in pairs(weaves) do
        if v.weaveKind & (WIK_PENG | WIK_GONG_GANG | WIK_MING_GANG | WIK_AN_GANG) == 0 then
            return false
        end
    end
    local pairsNum = 0
    for _, v in pairs(cards) do
        if v == 4 or v == 1 then
            return false
        end
        if v == 2 then
            pairsNum = pairsNum + 1
        end
    end
    return pairsNum == 1
end

-- 判断十三幺(东西南北中发白，1万9万1筒9筒1条9条全齐)
function M.is_13_19(cards)
    local sum = 0
    local tbl_13_1_9 = {1,9,10,18,19,27,28,29,30,31,32,33,34}
    for _, i in ipairs(tbl_13_1_9) do
        local c = cards[i]
        if c ~= 1 and c ~= 2 then
            return false
        end
        sum = sum + c
    end
    return sum == 14
end

-- 掐张
function M.is_jiahu(indexMap, curIndex)
    if curIndex > 27 then
        return false
    end
    -- 2到8
    local from = ((curIndex-1)/9)*9+1
    local to = from + 8
    if curIndex == from or curIndex == to then
        return false
    end

    if not indexMap[curIndex-1] or not indexMap[curIndex+1] then
        return false
    end

    if indexMap[curIndex-1] == 0 or indexMap[curIndex+1] == 0 then
        return false
    end

    indexMap[curIndex] = indexMap[curIndex] - 1
    indexMap[curIndex-1] = indexMap[curIndex-1] - 1
    indexMap[curIndex+1] = indexMap[curIndex+1] - 1
    local hu = self:CanHuPai(indexMap)
    indexMap[curIndex] = indexMap[curIndex] + 1
    indexMap[curIndex-1] = indexMap[curIndex-1] + 1
    indexMap[curIndex+1] = indexMap[curIndex+1] + 1
    return hu
end

-- 检查单调(扣除一对当前牌，能组成顺子各刻子的组合，则为单调)
function M.is_dandiao(hand_cards, cur_card)
    if hand_cards[cur_card] < 2 then
        return false
    end

    local cards = {}
    for card, num in pairs(hand_cards) do
        cards[card] = num
    end

    cards[cur_card] = cards[cur_card] - 2

    -- 检查东南西北中发白
    local success, eye = splitlib.check_zi(cards)
    if not success then
        return false
    end

    -- 检查万
    success, eye = splitlib.check_color(cards, 1, eye)
    if not success then
        return false
    end
    
    -- 检查筒
    success, eye = splitlib.check_color(cards, 10, eye)
    if not success then
        return false
    end
    
    -- 检查条
    success = splitlib.check_color(cards, 19, eye)
    return success
end

-- 大吊车（只剩一张手牌，单钓这张牌胡）
function M.is_da_diao_che(cards)
    local sum = 0
    for _, c in pairs(cards) do
        if c % 2 ~= 0 then
            return false
        end
        sum = sum + c
    end
    return sum == 2
end

-- 是否是清一色
function M.is_qing_yi_se(logic)
    local colors = {0,0,0,0}
    for i,v in pairs(cards) do
        if v > 0 then
            local color = math.floor((i-1)/9) + 1
            colors[color] = 1
        end
    end
    
    for _, v in pairs(weaves) do
        local color = math.floor((logic.Card2Index(v.centerCard)-1)/9) + 1
        colors[color] = 1
    end

    return colors[1] + colors[2] + colors[3] + colors[4] == 1
end

-- 缺几门，是否清一色
function M.get_que_men_count(logic)
    local colors = {0,0,0,0}
    for i,v in pairs(cards) do
        if v > 0 then
            local color = math.floor((i-1)/9) + 1
            colors[color] = 1
        end
    end
    
    for _, v in pairs(weaves) do
        local color = math.floor((logic.Card2Index(v.centerCard)-1)/9) + 1
        colors[color] = 1
    end
    
    local count = colors[1] + colors[2] + colors[3] + colors[4]
    return 3 - (colors[1] + colors[2] + colors[3]), count == 1
end

return M