package.path = "../../lualib/?.lua;"..package.path

local utils = require "utils"

local mjlib = require "mjlib"

local function test_wave()
    math.randomseed(os.time())

    for i=1,100 do
        local tbl = {1,1,1,1,1,1}
        local pt = {}
        for j=1, 6 do
            tbl[j] = math.random(1,4)
            pt[j] = tbl[j]
        end

        local total = pt[1] + pt[2] + pt[3] + pt[4] + pt[5] + pt[6]

        if total == 6 or total == 9 or total == 12 then
            if mjlib.check_wave(tbl) then
                print(pt[1],pt[2],pt[3],pt[4],pt[5],pt[6])
                print("测试成功")
            else
                print(pt[1],pt[2],pt[3],pt[4],pt[5],pt[6])
                print("测试失败")
            end
        end
    end
end

local function test_wave_1()
    mjlib.check_wave(tbl)
end

local function add_ke(tbl)
    while(true) do
        local index = math.random(1,34)
        if tbl[index] <= 1 then
            tbl[index] = tbl[index] + 3
            return
        end
    end
end

local function add_shun(tbl)
    local t = {{1,7}, {10, 16}, {19, 25}}
    local i = math.random(1, #t)
    local min = t[i][1]
    local max = t[i][2]
    while(true) do
        local index = math.random(min, max)
        if tbl[index] < 4 and tbl[index + 1] < 4 and tbl[index + 2] < 4 then
            tbl[index] = tbl[index] + 1
            tbl[index + 1] = tbl[index + 1] + 1
            tbl[index + 2] = tbl[index + 2] + 1
            return
        end
    end
end

local function add_dui(tbl)
    while(true) do
        local index = math.random(1,34)
        if tbl[index] <= 2 then
            tbl[index] = tbl[index] + 2
            return
        end
    end
end

local function test_one()
    -- 6万6万6万4筒4筒4筒4条4条5条5条6条6条发发
    local t = {
        0,0,0,   0,0,3,   0,0,0,
        0,0,0,   3,0,0,   0,0,0,
        0,0,0,   2,2,2,   0,0,0,
        0,0,0,0, 0,2,0}
    if not mjlib.get_hu_info(t) then
        print("测试失败")
    end
end

local function test_hu_tbl()
    local tbl = {}
    for i=1,34 do
        table.insert(tbl,0)
    end

    for i=1,4 do
        if math.random(1,5) <= 1 then
            while(true) do
                local index = math.random(1,9)
                if tbl[index] <= 1 then
                    tbl[index] = tbl[index] + 3
                    break
                end
            end
        else
            while(true) do
                local index = math.random(1, 7)
                if tbl[index] < 4 and tbl[index + 1] < 4 and tbl[index + 2] < 4 then
                    tbl[index] = tbl[index] + 1
                    tbl[index + 1] = tbl[index + 1] + 1
                    tbl[index + 2] = tbl[index + 2] + 1
                    break
                end
            end
        end
    end

    while(true) do
        local index = math.random(1,34)
        if tbl[index] <= 2 then
            tbl[index] = tbl[index] + 2
            break
        end
    end
    if not mjlib.get_hu_info(tbl) then
        utils.print_array(tbl)
        print("测试失败")
        assert(false)
    end
end

local function test_hu()
    local t = {}
    for i=1,34 do
        table.insert(t,0)
    end

    for i=1,4 do
        if math.random(1,5) <= 1 then
            add_ke(t)
        else
            add_shun(t)
        end
    end

    add_dui(t)

    mjlib.print(t)
    if not mjlib.get_hu_info(t) then
        print("测试失败")
    end
end

local function main()
    local start = os.time()
    math.randomseed(os.time())
    local count = 10000000
    for i=1,count do
        test_hu_tbl()
    end
    print("测试",count,"次,耗时",os.time() - start)
end

main()
