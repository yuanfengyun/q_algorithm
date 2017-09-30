local M = {}

function M.get_hu_info(hand_cards)
    local cards = {}
    for card, num in pairs(hand_cards) do
        cards[card] = num
    end

    -- 检查东南西北中发白
    local success, eye = M.check_zi(cards)
    if not success then
        return false
    end

    -- 检查万
    success, eye = M.check_color(cards, 1, eye)
    if not success then
        return false
    end
    
    -- 检查筒
    success, eye = M.check_color(cards, 10, eye)
    if not success then
        return false
    end
    
    -- 检查条
    success = M.check_color(cards, 19, eye)
    return success
end

function M.check_color(cards, from, eye)
    local sum = 0
    for i=from,from+8 do
        sum = sum + cards[i]
    end

    if sum == 0 then
        return true, eye
    end

    local yu = sum % 3
    -- 牌总数除3余1，不能胡（多出的一张，既不能做将，也不能做刻子）
    if yu == 1 then
        return false
    -- 余两张牌，只能做将
    elseif yu == 2 then
        -- 如果已经有将，不能出现两对将，不能胡了
        if eye then
            return false
        end
        eye = true
        -- 调用带将的拆分
        return M.split_eye(cards, from), eye
    end
    -- 调用不带将的拆分
    return M.split(cards, from, from+8), eye
end

function M.split_eye(cards, from)
    -- 找出本花色所有可能的将
    local eye_tbl = M.find_eye(cards, from, from+8)
    if not eye_tbl then
        return false
    end

    -- 遍历每种将，有一种将能满足，则花色满足胡牌条件
	local tmp_cards = {0,0,0,0,0,0,0,0,0}
    for i, _ in pairs(eye_tbl.eyes) do
        -- 扣除将
        cards[i] = cards[i] - 2
		local index = 0
		for j=eye_tbl.from, eye_tbl.to do
			index = index + 1
			tmp_cards[index] = cards[j]
		end
        -- 拆分剩余牌
        if M.split(tmp_cards, 1, index) then
            return true
        end
        -- 把扣除的将加回来
        cards[i] = cards[i] + 2
    end

    return false
end

function M.find_eye(cards, from, to)
    local sum = 0
    local t = {}
    local eye_tbl
    local begin
    for i=from,to do
        local c = cards[i]
        if c > 0 then
            sum = sum + c
            -- 没有将，牌张数大于等于2，才可能做将
            if not eye_tbl and c >= 2 then
                t[i] = true
            end
            if not begin then
                begin = i
            end
        end

        if c == 0 or i == to then
            local yu = sum%3
            -- 连续的牌除3余1，不能胡牌了
            if yu == 1 then
                return
            -- 连续的牌除3余2才能产生将
            elseif yu == 2 then
				if not next(t) then
					return
				end
				sum = 0
				eye_tbl = {eyes = t, from=begin, to = i}
				begin = nil
            else
                -- 连续的牌，能整除3，判断是否能进行拆分，如果不能，则不能胡
                if sum > 0 and not M.split(cards, begin, i) then
                    return
                end
                t = {}
                begin = nil
                sum = 0
            end
        end
    end
    return eye_tbl
end

function M.split(cards, from, to)
    for i=from,to do
        local n
        local c = cards[i]
        -- 1张牌或4张牌，拆1个顺子
        if c == 1 or c == 4 then
            n = 1
        -- 2张牌，拆两个顺子
        elseif c == 2 then
            n = 2
        end

        if n then
            if i + 2 > to then
                return false
            end
            local c1 = cards[i+1]
            if c1 < n then
                return false
            end
            local c2 = cards[i+2]
            if c2 < n then
                return false
            end
            cards[i+1] = c1-n
            cards[i+2] = c2-n
        end
    end

    return true
end

-- 检查字牌
function M.check_zi(cards)
    local eye = false
    for i=28,34 do
        local c = cards[i]
        if c == 1 or c == 4 then
            return false
        elseif c == 2 then
            if not eye then
                eye = true
            else
                return false
            end
        end
    end
    return true, eye
end

return M