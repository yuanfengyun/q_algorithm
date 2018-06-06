local MIN_HUXI = 15
local table_mgr = require "table_mgr"

local cache_table = {}

function add_to_table(cards, shun)
    local big_key=0
    local key=0
	local num = 0
    for i=10,1,-1 do
        key = key*4 + cards[i]
		num = num + cards[i]
    end

    local t = cache_table[key]
    if t and t >= shun then
		return true
    end

    table_mgr:add(key, (shun<<8) + num)
    cache_table[key] = shun
    return false
end

function add_menzi(cards, level, total_shun)
    -- 1-8的顺子
	-- 2-7-10
	local tmp = {0,0,0}
    for i=1,9 do
        repeat
            local shun = 0
            if i == 1 or i == 9 then
				shun = 1
            end

            tmp[1]=0
            tmp[2]=0
            tmp[3]=0
            if i<=8 then
                tmp[1]=i
                tmp[2]=i+1
                tmp[3]=i+2
			elseif i == 9 then
                tmp[1]=2
                tmp[2]=7
                tmp[3]=10
            end
            local add = true
            for _,index in ipairs(tmp) do
                cards[index] = cards[index]+1
				if add and cards[index] >= 3 then
					add = false
				end
            end

			if add then
				if not add_to_table(cards, total_shun + shun) then
					if level < 5 then
						add_menzi(cards, level+1, total_shun + shun)
					end
				end
			end
 
            for _,index in ipairs(tmp) do
                cards[index] = cards[index]-1
            end
        until(true)
    end
end

function main()
    local cards = {
        0,0,0,0,0,0,0,0,0,0,
    }
    local begin = os.time()
    print("generate start")
    add_menzi(cards, 1, 0)
    table_mgr:dump_tbl()
    print("generate end, use",os.time()-begin,"S")
end

main()
