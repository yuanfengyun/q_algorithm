package.path = "../../lualib/?.lua;"..package.path

local auto_table = require "auto_table"
local auto_table_with_eye = require "auto_table_with_eye"
local utils = require "utils"
local mjlib = require "mjlib"
local hulib = require "hulib"

local tested_tbl = {}

local function check_hu(t)
    for i=1,34 do
        if t[i] > 4 then
            return
        end
    end

    local num = 0
    for i=1,9 do
        num = num * 10 + t[i]
    end

    if tested_tbl[num] then
        return
    end

    tested_tbl[num] = true

    if not hulib.get_hu_info(t) then
        --print("测试失败")
        --utils.print_array(t)
    end
end

local function test_hu_sub(t, num)
    for j=1,32 do
        local index
        if j <= 18 then
            t[j] = t[j] + 3
        elseif j<= 25 then
            index = j - 18
        else
            index = j - 16
        end
        if index then
            t[index] = t[index] + 1
            t[index + 1] = t[index + 1] + 1
            t[index + 2] = t[index + 2] + 1
        end

        if num == 4 then
            check_hu(t)
        else
            test_hu_sub(t, num + 1)
        end

        if j<= 18 then
            t[j] = t[j] - 3
        else
            t[index] = t[index] - 1
            t[index + 1] = t[index + 1] - 1
            t[index + 2] = t[index + 2] - 1
        end
    end
end

local function gen_auto_table()
    local t = {}
    for i=1,34 do
        table.insert(t,0)
    end

    for i=1,18 do
        t[i] = 2
        print("将",i)
        test_hu_sub(t, 1)
        t[i] = 0
    end

    utils.dump_table_2_file(auto_table, "./auto_table.lua")
    utils.dump_table_2_file(auto_table_with_eye, "./auto_table_with_eye.lua")
end

local function main()
    auto_table.collect = true
    auto_table_with_eye.collect = true
    gen_auto_table()
end

main()
