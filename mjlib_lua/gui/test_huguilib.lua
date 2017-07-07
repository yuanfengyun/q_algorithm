package.path = "../../lualib/?.lua;"..package.path

local utils = require "utils"
local mjlib = require "mjlib"
local hulib = require "huguilib"

local function test_one()
    -- 6万6万6万4筒4筒4筒4条4条5条5条6条6条发发
    local t = {
        2,2,0,   1,0,0,   0,0,0,
        1,1,2,   1,1,1,   0,1,1,
        0,0,0,   0,0,0,   0,0,0,
        0,0,0,0, 0,0,0}
    if not hulib.get_hu_info(t, nil, 4) then
        print("测试失败")
    else
        print("测试成功")
    end
end

local tested = {}
local function test_gui(t,min,max)
    local num = 0
    for i=min,max do
        if t[i] > 4 then
            return
        end
        num = num * 10 + t[i]
    end

    if tested[num] then
        return
    end

    tested[num] = true

    for i=min,max do
        if not hulib.get_hu_info(t, nil, i) then
            print("测试失败", i)
            utils.print_array(t)
            assert(false)
        end
    end
end

local function test_one_color_sub(t, level)
    for i=1,9+7 do
        if i <= 9 then
            t[i] = t[i] + 3
        else
            local index = i - 9
            t[index] = t[index] + 1
            t[index+1] = t[index+1] + 1
            t[index+2] = t[index+2] + 1
        end

        if level == 4 then
            test_gui(t, 1, 9)
        else
            test_one_color_sub(t, level + 1)
        end

        if i <= 9 then
            t[i] = t[i] - 3
        else
            local index = i - 9
            t[index] = t[index] - 1
            t[index+1] = t[index+1] - 1
            t[index+2] = t[index+2] - 1
        end
    end
end

local function test_one_color()
    local t = {}
    for i=1,34 do
        table.insert(t,0)
    end

    for eye = 1,9 do
        t[eye] = 2
        test_one_color_sub(t, 1)
        t[eye] = 0
    end
end

local function test_two_color_sub(t, level)
    for j=1,32 do
        local index
        if j<= 18 then
            t[j] = t[j] + 3
        elseif j <= 25 then
            index = j - 18
        else
            index = j - 16
        end

        if index then
            t[index] = t[index] + 1
            t[index + 1] = t[index + 1] + 1
            t[index + 2] = t[index + 2] + 1
        end

        if level == 4 then
            test_gui(t, 1,18)
        else
            test_two_color_sub(t, level + 1)
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

local function test_two_color()
    local t = {}
    for i=1,34 do
        table.insert(t,0)
    end

    for i=1,18 do
        print("将",i)
        t[i] = 2
        test_two_color_sub(t, 1)
        t[i] = 0
    end
end

local function main()
    test_one()
end

main()
