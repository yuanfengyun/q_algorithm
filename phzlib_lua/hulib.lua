local table_mgr = require "table_mgr"
local M = {}

function M:get_hu_info(cards)
    local big_key = 0
    local small_key = 0

    for i=1,10 do
        big_key = big_key*10 + cards[i]
        small_key = small_key*10 + cards[10+i]
    end

    return table_mgr:check(big_key, small_key)
end

return M

