-- 牌型定义
local M = {}

M.type = {
    t_1  = "t_1",       -- 单张
    t_1n = "t_1n",      -- 顺子
    t_2  = "t_2",       -- 对
    t_2n = "t_2n",      -- 连对
    t_3  = "t_3",       -- 三个
    t_3n = "t_3n",      -- 多组三个
    t_31 = "t_31",      -- 三带1
    t_31n= "t_31n",     -- 飞机
    t_32 = "t_32",      -- 三带2
    t_32n= "t_32n",     -- 飞机
    t_4  = "t_4",       -- 炸弹
    t_41 = "t_41",      -- 四带一
    t_42 = "t_42",      -- 四带二
    t_43 = "t_43",      -- 四带三
    t_king = "t_king",  -- 王炸
}

function M.shuffle()
    
end

-- 获取牌型
function M.get_type(out_cards)
    local tmp_cards = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    for _,c in ipairs(out_cards) do
        tmp_cards[c] = tmp_cards[c] + 1
    end

    local counts = {0,0,0,0}
    local cards = {{},{},{},{}}
    for i,_ in ipairs(tmp_cards) do
        local c = tmp_cards[i]
        if c and c ~= 0 then
            counts[c] = counts[c] + 1
            table.insert(cards[c], i)
        end
    end

    if counts[4] ~= 0 then
        return M.get_type4(counts, cards)
    elseif counts[3] ~= 0 then
        return M.get_type3(counts, cards)
    elseif count[2] ~= 0 then
        return M.get_type2(counts, cards)
    elseif count[1] ~= 0 then
        return M.get_type1(counts, cards)
    end
end

function M.get_type4(counts, cards)
    if counts[4] > 1 then
        return
    end

    local sum = counts[3] + counts[2] + counts[1]
    if sum > 1 then
        return
    end

    local card = cards[4][1]
    if sum == 0 then
        return {t = M.type.t_4, card = card}
    end

    if counts[3] == 1 then
        return {t = M.type.t_43, card = card}
    elseif counts[2] == 1 then
        return {t = M.type.t_42, card = card}
    elseif counts[1] == 1 then
        return {t = M.type.t_41, card = card}
    end
end

function M.get_type3(counts, cards)
    local card = cards[3][1]
    local count3 = counts[3]
    local count2 = counts[2]
    local count1 = counts[1]

    if count2 > 0 and count1 > 0 then
        return
    end

    if count3 == 1 then
         if count2 > 0 then
            return {t = M.type.t_32, card = card}
         elseif count1 > 0 then
            return {t = M.type.t_31, card = card}
         end

         return {t = M.type.t_3, card = card}
    end

    local last
    for _,c in ipairs(cards[3]) do
        if last and last+1 ~= c then
            return
        end

        last = c 
    end
    -- 飞机
    if count2 > 0 then
        return {t = M.type.t_32n, card = card, n = count3}
    elseif count1 > 0 then
        return {t = M.type.t_31n, card = card, n = count3}
    end

    return {t = M.type.t_3n, card = card, n = count3}
end

function M.get_type2(counts, cards)
    if counts[1] > 0 then
        return
    end

    local card = cards[3][1]
    if counts[2] == 1 then
        return {t = M.type.t_2, card = card}
    end

    local last
    for _,c in ipairs(cards[2]) do
        if last and last+1 ~= c then
            return
        end

        last = c 
    end

    return  {t = M.type.t_2n, card = card, n = counts[2]}
end

function M.get_type1(counts, cards)
    local count = counts[1]
    if count < 5 and count ~= 2 then
        return
    end

    local last
    for _,c in ipairs(cards[1]) do
        if last and last+1 ~= c then
            return
        end

        last = c 
    end

    local card = cards[1][1]
    -- 判断王炸
    if count == 2 then
        
        if card ~= "小王" then
            return
        end

        return {t=M.type.t_king}
    end

    return {t=M.type.t_1n, card = card, n = count}
end

-- 牌型2是否管得起牌型1
function M.is_big(info1, info2)
    if info2.t == M.type.t_king then
        return true
    end

    if info2.t == M.type.t_4 then
        if info1.t ~= M.type.t_4 then
            return true
        end

        return info1.card < info2.card
    end

    if info1.t ~= info2.t or info1.n ~= info2.n then
        return
    end

    return info1.card < info2.card
end

return M
