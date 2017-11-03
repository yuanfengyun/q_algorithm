-- 方块(Diamond): 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 方块A-10 J Q K
-- 梅花(Club)   : 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1a 0x1b 0x1c 0x1d 梅花A-10 J Q K
-- 红桃(Heart)  : 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2a 0x2b 0x2c 0x2d 红桃A-10 J Q K
-- 黑桃(Spade)  : 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x3a 0x3b 0x3c 0x3d 黑桃A-10 J Q K
-- 王(Joker)    : 0x4E 0x4F 大小王
-- 牌型定义

local card_pool = {
    -- 16张玩法(只有一张黑桃2，去除黑桃A外的三张A，无大小王)
	[45] = {
		0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
		0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
		0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d
    },
    [48] = {
		0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
		0x11, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
		0x21, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d
    }
}

local M = {}

M.type = {
    t_1  = "t_1",       -- 单张
    t_1n = "t_1n",      -- 顺子
    t_2  = "t_2",       -- 对
    t_2n = "t_2n",      -- 连对
    t_3  = "t_3",       -- 三个
    t_3n = "t_3n",      -- 多组三个
    t_31 = "t_31",      -- 三带1
    t_31n= "t_31n",     -- 三带一飞机
    t_32 = "t_32",      -- 三带2
    t_32n= "t_32n",     -- 三带二飞机
    t_4  = "t_4",       -- 炸弹
    t_41 = "t_41",      -- 四带一
    t_42 = "t_42",      -- 四带二
    t_43 = "t_43",      -- 四带三
    t_king = "t_king",  -- 王炸
}

function M.shuffle(max)
    return math.random_shuffle(card_pool[max])
end

function M.get_card_index(card)
	local c = card & 0x0f
	if c == 0x01 then 
		return 14
	elseif c == 0x02 then
		return 16
	elseif c == 0x0e then
		return 17
	elseif c == 0x0f then
		return 18
	end

	return c
end

-- 获取牌型
function M.get_type(out_cards)
    local tmp_cards = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}    --a-k  大小王  15张序列
    for _,c in ipairs(out_cards) do
		local index = M.get_card_index(c)
        tmp_cards[index] = tmp_cards[index] + 1
    end

    local counts = {0,0,0,0}      --1,2,3,4组合的数目
    local cards = {{},{},{},{}}   --数目分别为1,2,3,4的牌的序列
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
    if counts[4] > 1 then    --两个炸弹不能一起出？
        return
    end
	
	-- 飞机
	if counts[3] > 1 then
		if not M.is_continue(cards[3]) then   --修改
			return
		end
		
		if counts[3] == 4 then       --炸弹做单牌
			if counts[2] + counts[1] ~= 0 then
				return
			end
			return {t = M.type.t_31n, card = cards[3][1], n = counts[3]}     --card 开始的牌是多少
		elseif counts[3] == 2 + counts[2] and counts[1] == 0 then    --炸弹做对子
			return {t = M.type.t_32n, card = cards[3][1], n = counts[3]}	
		end
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
        return {t = M.type.t_43, card = card}  --四带三
    elseif counts[2] == 1 then
        return {t = M.type.t_42, card = card}  --四带二
    elseif counts[1] == 1 then
        return {t = M.type.t_41, card = card}  --四带一
    end
end

function M.get_type3(counts, cards)
    local card = cards[3][#cards[3]]    --结尾的牌是多少     好像就是飞机是最大开始
    local count3 = counts[3]
    local count2 = counts[2]
    local count1 = counts[1]
	local sum = count1 + count2 + count3

	-- 三带一，或三带二，三个不带
    if count3 == 1 then
		if count2 == 1 and count1 == 0 then
			return {t = M.type.t_32, card = card}
		elseif count2 == 0 and count1 == 1 then
			return {t = M.type.t_31, card = card}
		elseif count2 == 0 and count1 == 0 then
			return {t = M.type.t_3, card = card}
		end

		return
    end
  
    if M.is_continue(cards[3]) then
		if sum % 4 == 0 then
			return {t = M.type.t_31n, card = card, n = count3}
		elseif sum % 5 == 0 and count1 == 0 then
			return {t = M.type.t_32n, card = card, n = count3}
		elseif count2 == 0 and count1 == 0 then
			return {t = M.type.t_3n, card = card, n = count3}    --修改
		end
		return
	end

    local count   
	count, card = M.get_max_continue(cards[3])     --如 444555666999   三带一飞机
	if count * 4 == sum then
		return {t = M.type.t_31n, card = card, n = count}
	end
end

function M.get_type2(counts, cards)
    if counts[1] > 0 then
        return
    end

    local card = cards[2][1]
    if counts[2] == 1 then
        return {t = M.type.t_2, card = card}
    end

    if not M.is_continue(cards[2]) then
		return false
	end

    return  {t = M.type.t_2n, card = card, n = counts[2]}
end

function M.get_type1(counts, cards)
    local count = counts[1]
    if count < 5 and count ~= 2 then
        return
    end

    if not M.is_continue(cards[1]) then
		return false
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

function M.is_continue(cards)
	local last
    for _,c in ipairs(cards) do
        if last and last+1 ~= c then
            return
        end

        last = c 
    end
	return true
end

function M.get_max_continue(cards)
	local t = {}
	local last
	local tmp
    for _,c in ipairs(cards) do
        if last and last+1 ~= c then
            table.insert(t, tmp)
			tmp = nil
        end

		tmp = tmp or {}
		table.insert(tmp, c)
        last = c 
    end

	if tmp then
		table.insert(t, tmp)
	end
	
	local m
	for _,v in ipairs(t) do
		if not m then
			m = v
		elseif #v >= #m then
			m = v
		end
	end
	
	return #m, table.remove(m)
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
