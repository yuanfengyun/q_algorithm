package.path = "../../lualib/?.lua;"..package.path

local auto_tbl = require "auto_table"
local utils = require "utils"
local mjlib = require "mjlib"

local tested_tbl = {}

local function test_hu_tbl(tbl)
    for _=1,4 do
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

    local num = 0
    for i=1,9 do
        num = num * 10 + tbl[i]
    end

    if tested_tbl[num] then
        return true
    end

    local ret = mjlib.get_hu_info(tbl)
    tested_tbl[num] = ret
    return ret
end

local function gen_auto_table()
    local base_tbl = {}
    for _=1,33 do
        table.insert(base_tbl,0)
    end

    table.insert(base_tbl,2)

    local tbl = {}
    for _=1,34 do
        table.insert(tbl,0)
    end

    local start = os.time()
    math.randomseed(os.time())
    local count = 10000*10000
    local fail_count = 0
    local percent = 0
    for i=1,count do
        for i=1,34 do
            tbl[i] = base_tbl[i]
        end

        if not test_hu_tbl(tbl) then
            fail_count = fail_count + 1
        end
        if i % (100*10000) == 0 then
            percent = percent + 1
            print("完成"..percent.."%")
        end
    end
    print("测试",count/10000,"万次,耗时",os.time() - start,"秒")
    print("失败次数:", fail_count)
    dump_auto_tbl()
    utils.dump_table_2_file(auto_tbl, "./auto_table.lua")
end

local function gen_auto_table_with_eye()
    local auto_table_with_eye = {}

    local function add(item)
        local num = 0
        for i,v in ipairs(item) do
            num = num * 10 + v
        end
        auto_table_with_eye[num] = true
    end

    local function get(num)
        local t = {}
        while(num > 0) do
            local yushu = num%10
            num = math.floor(num/10)
            table.insert(t,1,yushu)
        end
        return t
    end

    for num,_ in pairs(auto_tbl) do
        local t = get(num)
        -- 在能加将的地方加一对牌
        if #t < 9 then
            local tmp = utils.copy_array(t)
            utils.print_array(tmp)
            table.insert(tmp,1,2)
            add(tmp)

            tmp = utils.copy_array(t)
            table.insert(tmp,2)
            add(tmp)
        end

        for i,c in ipairs(t) do
            if c <= 2 then
                local tmp = utils.copy_array(t)
                tmp[i] = c + 2
                add(tmp)
            end
        end
    end

    --utils.print_array(auto_table_with_eye)
    utils.dump_table_2_file(auto_table_with_eye, "./auto_table_with_eye.lua")
end

local function main()
    gen_auto_table_with_eye()
end

main()
