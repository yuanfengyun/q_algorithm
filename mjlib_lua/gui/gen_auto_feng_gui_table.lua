package.path = "../../lualib/?.lua;"..package.path

local utils = require "utils"
local hulib = require "hulib"

local no_gui_table = {}
local one_gui_table = {}
local two_gui_table = {}
local three_gui_table = {}
local four_gui_table = {}

local no_gui_eye_table = {}
local one_gui_eye_table = {}
local two_gui_eye_table = {}
local three_gui_eye_table = {}
local four_gui_eye_table = {}

local function add_2_table(t, tbl)
    local n = 0
    for i=10,16 do
        n = n*10 + t[i]
    end

    tbl[n] = true
end

local function parse_table_sub(t, num, eye)
    local tbl
    local eye_tbl
    if num == 1 then
        tbl = one_gui_table
        eye_tbl = one_gui_eye_table
    elseif num == 2 then
        tbl = two_gui_table
        eye_tbl = two_gui_eye_table
    elseif num == 3 then
        tbl = three_gui_table
        eye_tbl = three_gui_eye_table
    elseif num == 4 then
        tbl = four_gui_table
        eye_tbl = four_gui_eye_table
    end
    for i=10,16 do
        local n = t[i]
        if n > 0 then
            t[i] = n - 1
            if not eye then
                add_2_table(t, tbl)
            else
                add_2_table(t, eye_tbl)
            end
            if num < 4 then
                parse_table_sub(t, num+1, eye)
            end
            t[i] = n
        end
    end
end

local function parse_table(t)
    local count = 0
    for i=10,16 do
        count = count + t[i]
    end

    local eye = false
    if count % 3 ~= 0 then
        eye = true
    end

    if not eye then
        add_2_table(t, no_gui_table)
        parse_table_sub(t, 1, false)
    else
        add_2_table(t, no_gui_eye_table)
        parse_table_sub(t, 1, true)
    end

end

local total_count=0
local hu_count=0
local fail_count=0
local tested_tbl = {}

local function check_hu(t)
    total_count = total_count + 1
    
    for i=1,34 do
        if t[i] > 4 then
            fail_count = fail_count + 1
            return
        end
    end

    local num = 0
    for i=1,16 do
        num = num * 10 + t[i]
    end

    if tested_tbl[num] then
        return
    end

    tested_tbl[num] = true

    if not hulib.get_hu_info(t, nil, 19) then
        --print("测试失败")
        --utils.print_array(t)
        fail_count = fail_count + 1
    else
        parse_table(t)
        hu_count = hu_count + 1
    end
end

local function test_hu_sub(t, num)
    -- 9 + 7 + 7
    for j=1,23 do
        if j<= 16 then
            t[j] = t[j] + 3
        else
            local index = j - 16
            t[index] = t[index] + 1
            t[index + 1] = t[index + 1] + 1
            t[index + 2] = t[index + 2] + 1
        end

        if num == 4 then
            check_hu(t)
        else
            test_hu_sub(t, num + 1)
        end

        if j<= 16 then
            t[j] = t[j] - 3
        else
            local index = j - 16
            t[index] = t[index] - 1
            t[index + 1] = t[index + 1] - 1
            t[index + 2] = t[index + 2] - 1
        end
    end
end

local function gen_auto_gui_table()
    local t = {}
    for _=1,34 do
        table.insert(t,0)
    end

    for i=1,16 do
        print("将",i)
        t[i] = 2
        test_hu_sub(t, 1)
        t[i] = 0
    end
    print("总数", total_count)
    print("胡", hu_count)
    print("失败", fail_count)
 
    utils.dump_table_2_file(no_gui_table, "./no_gui_feng_table.lua")
    utils.dump_table_2_file(no_gui_eye_table, "./no_gui_feng_eye_table.lua")
    local no_gui_count = 0
    for _,_ in pairs(no_gui_table) do
        no_gui_count = no_gui_count + 1
    end

    local no_gui_eye_count = 0
    for _,_ in pairs(no_gui_eye_table) do
        no_gui_eye_count = no_gui_eye_count + 1
    end

    print("没有鬼表大小", no_gui_count)
    print("没有鬼将表大小", no_gui_eye_count)

    utils.dump_table_2_file(one_gui_table, "./one_gui_feng_table.lua")
    utils.dump_table_2_file(one_gui_eye_table, "./one_gui_feng_eye_table.lua")
    local one_gui_count = 0
    for _,_ in pairs(one_gui_table) do
        one_gui_count = one_gui_count + 1
    end

    local one_gui_eye_count = 0
    for _,_ in pairs(one_gui_eye_table) do
        one_gui_eye_count = one_gui_eye_count + 1
    end

    print("1鬼表大小", one_gui_count)
    print("1鬼将表大小", one_gui_eye_count)

    utils.dump_table_2_file(two_gui_table, "./two_gui_feng_table.lua")
    utils.dump_table_2_file(two_gui_eye_table, "./two_gui_feng_eye_table.lua")
    local two_gui_count = 0
    for _,_ in pairs(two_gui_table) do
        two_gui_count = two_gui_count + 1
    end

    local two_gui_eye_count = 0
    for _,_ in pairs(two_gui_eye_table) do
        two_gui_eye_count = two_gui_eye_count + 1
    end

    print("2鬼表大小", two_gui_count)
    print("2鬼将表大小", two_gui_eye_count)

    utils.dump_table_2_file(three_gui_table, "./three_gui_feng_table.lua")
    utils.dump_table_2_file(three_gui_eye_table, "./three_gui_feng_eye_table.lua")
    local three_gui_count = 0
    for _,_ in pairs(three_gui_table) do
        three_gui_count = three_gui_count + 1
    end

    local three_gui_eye_count = 0
    for _,_ in pairs(three_gui_eye_table) do
        three_gui_eye_count = three_gui_eye_count + 1
    end

    print("3鬼表大小", three_gui_count)
    print("3鬼将表大小", three_gui_eye_count)

    utils.dump_table_2_file(four_gui_table, "./four_gui_feng_table.lua")
    utils.dump_table_2_file(four_gui_eye_table, "./four_gui_feng_eye_table.lua")

    local four_gui_count = 0
    for _,_ in pairs(four_gui_table) do
        four_gui_count = four_gui_count + 1
    end

    local four_gui_eye_count = 0
    for _,_ in pairs(four_gui_eye_table) do
        four_gui_eye_count = four_gui_eye_count + 1
    end

    print("4鬼表大小", four_gui_count)
    print("4鬼将表大小", four_gui_eye_count)
end

local function main()
    gen_auto_gui_table()
end

main()
