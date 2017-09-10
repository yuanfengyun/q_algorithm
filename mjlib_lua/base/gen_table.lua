local table_mgr = require "table_mgr"
local tconcat = table.concat

local tested_tbl = {}

local function dump_t(t)
	return tconcat(t, "")
end

local function add_to_table(t)
	local num = tonumber(dump_t(t))

    if tested_tbl[num] then
        return
    end

    tested_tbl[num] = true

	string.gsub(num..'', '([1-9]+)', function(key)
		table_mgr:add(key)
	end)
end

local do_add
local gen_table_sub

do_add = function(t, num)
	add_to_table(t)
	if num < 4 then
		gen_table_sub(t, num + 1)
	end
end

gen_table_sub = function(t, num)
    for j=1,16 do
        repeat
            local index
            if j <= 9 then
                if t[j] > 1 then
                    break
                end
                t[j] = t[j] + 3

				do_add(t, num)

                t[j] = t[j] - 3
            elseif j<= 16 then
                index = j - 9
                if t[index] >= 4 or t[index+1] >= 4 or t[index+2] >= 4 then
                    break
                end

				t[index] = t[index] + 1
				t[index + 1] = t[index + 1] + 1
				t[index + 2] = t[index + 2] + 1

				do_add(t, num)

				t[index] = t[index] - 1
				t[index + 1] = t[index + 1] - 1
				t[index + 2] = t[index + 2] - 1
            end
        until(true)
    end
end

local function gen_table()
    local t = {0,0,0,0,0,0,0,0,0}
    gen_table_sub(t, 1)
end

local function gen_eye_table()
    local t = {0,0,0,0,0,0,0,0,0}

    for i=1,18 do
        t[i] = 2
        print("将",i)
        add_to_table(t)
        gen_table_sub(t, 1)
        t[i] = 0
    end
end

local function main()
    gen_table()
	gen_eye_table()
	table_mgr:dump()
end

main()
