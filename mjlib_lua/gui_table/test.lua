local mjlib = require "mjlib"
local hulib = require "hulib"
local table_mgr = require "table_mgr"

local function test_one()
    -- 6万6万6万4筒4筒4筒4条4条5条5条6条6条发发
    local t = {
        0,0,0,   0,0,3,   0,0,0,
        0,0,0,   3,0,0,   0,0,0,
        0,0,0,   2,2,2,   0,0,0,
        0,0,0,0, 0,2,0}
    if not hulib.get_hu_info(t, 0) then
        print("测试失败")
    end
end

local function main()
    table_mgr:init()
    table_mgr:load()

    local start = os.time()
    math.randomseed(os.time())
    local count = 1
    for i=1,count do
        test_one()
    end
    print("测试",count,"次,耗时",os.time() - start)
end

main()
