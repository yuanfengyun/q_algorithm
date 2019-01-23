--[[

两幅扑克牌  
同花色的 三张以上的叫 顺子   一样的值的三张叫坎
起手20个牌全部为顺子和坎的（至少有一个3张的同花顺）这二十张里面最多有八张癞子

 思路
 组合出所有的两种同花顺情况
 扣除同花顺以后，其它的牌没有花色的要求，合并为一种花色进行操作
 从头到尾尝试进行拆解，能拆解完则表示能拆尽，表示成功
]]--

-- 3, 4, 5
-- 6, 7, 8, 9, 10
-- 14 13 12 11 10

-- 所有的拆解方式
local t = {
    { 1,1,1,0,0},
    { 1,1,1,1,0},
    { 1,1,1,1,1},

    { 2,2,2,2,2},

    { 3,0,0,0,0},
    { 3,3,0,0,0},

    { 3,3,3,0,0},
    { 3,3,3,1,0},
    { 3,3,3,2,0},
    { 3,3,3,3,0},
    { 3,3,3,1,1},
    { 3,3,3,2,1},
    { 3,3,3,2,2},
    { 3,3,3,3,1},
    { 3,3,3,3,2},

    { 4,1,1,0,0},
    { 4,1,1,1,0},
    { 4,1,1,1,1},

    { 4,4,4,0,0},
    { 4,4,4,1,0},
    { 4,4,4,2,0},
    { 4,4,4,1,1},

    { 5,2,2,0,0},
    { 5,2,2,1,0},
    { 5,2,2,2,0},
    { 5,2,2,1,1},
    { 5,2,2,2,1},
    { 5,2,2,2,2},

    { 6,0,0,0,0},

    { 7,1,1,0,0},
    { 7,1,1,1,0},
    { 7,1,1,1,1},

    { 8,2,2,0,0},
    { 8,2,2,1,0},
    { 8,2,2,1,1},
    { 8,2,2,2,0},

    { 9,0,0,0,0},

 -- {10,1,1,0,0}, 9 1 1 1
 -- {10,1,1,1,0},
 -- {10,1,1,1,1},

 -- {12,0,0,0,0}, 12 = 9 + 3
}

local num_t = {}

local d = {
    {0x11,0x1d},
    {0x21,0x2d},
    {0x31,0x3d},
    {0x41,0x4d}
}

local two_t = {}

local M = {}

function M.init_num_t()
    for n=3,14 do
        local array = {}
        for _,v in ipairs(t) do
            local sum = v[1]+v[2]+v[3]+v[4]+v[5]
            if sum <= n then
                table.insert(array,v)
            end
        end
        if next(array) then
            num_t[n] = array
        end
    end
end

local function sort_two(array)
    table.sort(array)
    local key = table.concat(array,"-")

    local cards = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,  -- 黑 A-K
        0,0,0,0,0,0,0,0,0,0,0,0,0,  -- 红
        0,0,0,0,0,0,0,0,0,0,0,0,0,  -- 梅
        0,0,0,0,0,0,0,0,0,0,0,0,0   -- 方
    }

    for _,c in ipairs(array) do
        local i = 13*(((c & 0xf0)>>4) -1) + (c & 0x0f)
        cards[i] = cards[i] + 1
    end

    return key,cards
end

function M.init_two_t()
    local array = {}
    for _,v in ipairs(d) do
        for i=v[1],v[2] do
            if i+3<=v[2]+1 then
                table.insert(array,{i,i+1,i+2})
            end
            if i+4<=v[2]+1 then
                table.insert(array,{i,i+1,i+2,i+3})
            end
            if i+5<=v[2]+1 then
                table.insert(array,{i,i+1,i+2,i+3,i+4})
            end
        end
    end

    for _,v1 in ipairs(array) do
        for _,v2 in ipairs(array) do
            local a = {table.unpack(v1)}
            for _,c in ipairs(v2) do
                table.insert(a,c)
            end
            local k,v = sort_two(a)
            two_t[k] = v
        end
    end
end

local split
split = function (cards,sum,g,cur)
    local c = cards[cur]
    if cur == 13 then
        local yu = c % 3
        if yu > 0 then
            return g >= 3 - yu
        end
        return true
    end
   
    if c == 0 then
        return split(cards,sum,g,cur+1)
    end

    local list = num_t[sum]
    if not list then
        return false
    end

    for _,v in ipairs(list) do
        -- 比较
        if c <= v[1] and c + g >= v[1] then
            local cur_sum = 0
            local g1 = {0,0,0,0,0}
            for i,n in ipairs(v) do
                local cur_n = cards[cur + i - 1]
                if cur_n < n then
                    g1[i] = n - cur_n
                end
                cur_sum = n + cur_sum
            end

            local g_sum = g1[1]+g1[2]+g1[3]+g1[4]+g1[5]
            if g_sum <= g then
                -- 扣
                for i,n in ipairs(v) do
                    cards[cur+i-1] = cards[cur+i-1] - (n-g1[i]) 
                end
                local ret = split(cards,sum-cur_sum,g-g_sum,cur+1)
                -- 加回来
                for i,n in ipairs(v) do
                    cards[cur+i-1] = cards[cur+i-1] + (n-g1[i])
                end
                if ret then
                    return ret
                end
            end
        end
    end
    return false
end

function M.is(cards,gui)
    -- 遍历所有两组同花情况
    for _,v in pairs(two_t) do
        -- 判断是否够扣除
        local sum = 0
        local gui_sum = 0
        local need_gui = {}
        for i,c in ipairs(cards) do
            -- 数量不够，需要赖子补
            local need = v[i]
            sum = sum + need
            if c < need then
                need_gui[i] = need - c
                gui_sum = gui_sum + need - c
            end
        end

        -- 赖子够弥补
        if gui_sum <= gui then
            -- 转化为忽略花色的数组
            local no_color_cards = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
            for i,c in ipairs(cards) do
                local need = v[i]
                local index = ((i-1)%13) + 1
                if c > need then
                    no_color_cards[index] = c - need
                end
            end
            -- 判断是否为顺子和坎的组合
            local ret = split(no_color_cards,20-sum,gui - gui_sum,1)

            if ret then
                return true
            end
        end
    end
    return false
end

function M.test_true()
    -- 手牌(不包含赖子牌)
    local cards = {
        1,1,1,0,0,0,0,0,0,0,0,0,0,  -- 黑 A-K
        0,0,1,1,1,0,0,0,0,0,0,0,0,  -- 红
        0,0,0,4,1,1,0,0,0,0,0,0,0,  -- 梅
        0,0,0,0,0,0,0,0,0,0,0,0,0   -- 方
    }

    -- 赖子牌数量
    local gui = 8
    local ret = M.is(cards,gui)
    print(ret)
end

function M.test_false()
    -- 手牌(不包含赖子牌)
    local cards = {
        1,1,1,0,0,0,0,0,0,0,0,0,0,  -- 黑 A-K
        0,0,1,1,1,0,0,0,0,0,0,0,0,  -- 红
        0,0,0,4,1,1,0,0,0,0,0,0,0,  -- 梅
        0,0,0,0,0,0,0,0,0,1,0,0,6   -- 方
    }

    -- 赖子牌数量
    local gui = 1
    local ret = M.is(cards,gui)
    print(ret)
end


function M.test_time()
    -- 手牌(不包含赖子牌)
    local cards = {
        1,1,1,0,0,0,0,0,0,0,0,0,0,  -- 黑 A-K
        0,0,1,1,1,0,0,0,0,0,0,0,0,  -- 红
        0,0,0,4,1,1,0,0,0,0,0,0,0,  -- 梅
        0,0,0,0,0,0,0,0,0,1,0,0,6   -- 方
    }

    -- 赖子牌数量
    local gui = 1
    local ret
    local n = 1000
    local begin = os.time()
    for i=1,n do
        ret = M.is(cards,gui)
    end
    print("use seconds:",os.time()-begin,ret)
end

M.init_num_t()
M.init_two_t()
M.test_true()
M.test_false()
M.test_time()
