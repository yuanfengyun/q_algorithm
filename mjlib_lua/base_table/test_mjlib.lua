local table_mgr = require "table_mgr"
local hulib = require "hulib"

local function test_one()
    local t = {
        0,0,3,   3,2,3,   0,0,0,
        0,0,0,   0,1,1,   0,0,0,
        0,0,0,   1,0,0,   0,0,0,
        0,0,0,0, 0,0,0}
    if hulib.get_hu_info(t) then
        print("胡")
    else
        print("不能胡")
    end
end

local function main()
    table_mgr:load()
    test_one()
end

main()
