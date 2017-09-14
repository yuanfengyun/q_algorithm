local mjlib = require "mjlib"

function test_fail()
    local cards = {
        1,1,1,0,0,0,2,0,0,
        0,0,0,0,2,0,0,0,0,
        0,0,0,0,1,2,2,1,0
    }

    local result = true
    if mjlib._check_normal(cards) == result then
        print("test_fail 测试成功")
    else
        print("test_fail 测试失败")
    end
end

function test_success()
    local cards = {
        1,1,1,0,0,0,2,0,0,
        0,0,0,0,3,0,0,0,0,
        0,0,0,0,1,2,2,1,0
    }

    local result = true
    if mjlib._check_normal(cards) == result then
        print("test_success 测试成功")
    else
        print("test_success 测试失败")
    end
end

function test_time()
    local count = 100*10000
    local cards = {
        1,1,1,0,0,0,2,0,0,
        0,0,0,0,3,0,0,0,0,
        0,0,0,0,1,2,2,1,0
    }

    local start = os.time()
    for i=1,count do
        mjlib._check_normal(cards)
    end
    print("测试",count,"次,耗时",os.time()-start,"秒")
end

test_fail()
test_success()
test_time()
