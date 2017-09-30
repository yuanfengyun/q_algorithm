local M = {}

function M.get_hu_info(hand_cards)
    local cards = {}
    for card, num in pairs(hand_cards) do
        cards[card] = num
    end

    local success, eye = M.check_zi(cards)
    if not success then
        return false
    end

    success, eye = M.check_color(cards, 1, eye)
    if not success then
        return false
    end
    success, eye = M.check_color(cards, 10, eye)
    if not success then
        return false
    end
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
    if yu == 1 then
        return false
    elseif yu == 2 then
        if eye then
            return false
        end
        eye = true
        return M.split_eye(cards, from), eye
    end
    return M.split(cards, from), eye
end

function M.split_eye(cards, from)
    local eye_tbl = M.find_eye(cards, from)
    if not eye_tbl then
        return false
    end

    for _, eye in pairs(eye_tbl.eyes) do
        cards[eye] = cards[eye] - 2
        if M.split(cards, eye_tbl.from, eye_tbl.to) then
            return true
        end
        cards[eye] = cards[eye] + 2
    end

    return false
end

function M.find_eye(cards, from)
    local sum = 0
    local t = {}
    local eye_tbl
    local begin
    for i=from,to do
        local c = cards[i]
        if c > 0 then
            sum = sum + c
            if not eye_tbl and c >= 2 then
                t[i] = true
            end
            if not begin then
                begin = i
            end
        end

        if c == 0 or i == to then
            local yu = sum%3 
            if yu == 1 then
                return
            elseif yu == 2 then
                eye_tbl = {eyes = t, from=begin, to = i}
            else
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
        if c == 1 or c == 4 then
            n = 1
        elseif c == 2 then
            n = 2
        end

        if n then
            if c + 2 > to then
                return false
            end
            local c1 = cards[i+1]
            if c1 == 0 then
                return false
            end
            local c2 = cards[i+2]
            if c2 == 0 then
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
