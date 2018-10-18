local utils = require "utils"

local tt = {
	{1,2,3,huxi=3},
	{1,5,10,huxi=3},
	{2,7,10,huxi=3},
	{11,12,13,huxi=6},
	{12,17,20,huxi=6},
	{11,15,20,huxi=6}
}

local need_tbl = {1,2,3,5,7,10,11,12,13,15,17,20}

function copy(cards,huxi)
	local cc = {}
	for _,i in ipairs(need_tbl) do
		if cards[i] > 0 then
			cc[i] = cards[i]
		end
	end
	cc.huxi = huxi
	return cc
end

local cache = {}
local result = {}
function add_table(cards,huxi)
	local key = table.concat(cards,"")
	if cache[key] then
		return false
	end

	cache[key] = true
	table.insert(result,copy(cards,huxi))
	return true
end

function addone(cards,level,huxi)
	for k,v in pairs(tt) do
		if cards[v[1]] < 2 and cards[v[2]] < 2 and cards[v[3]] < 2 then
			cards[v[1]] = cards[v[1]] + 1
			cards[v[2]] = cards[v[2]] + 1
			cards[v[3]] = cards[v[3]] + 1
			if add_table(cards,huxi+v.huxi) and level < 6 then
				addone(cards,level+1,huxi+v.huxi)
			end
			cards[v[1]] = cards[v[1]] - 1
			cards[v[2]] = cards[v[2]] - 1
			cards[v[3]] = cards[v[3]] - 1
		end
	end
end

local cards = {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
}
addone(cards,1,0)

table.sort(result,function(a,b)
	return a.huxi > b.huxi
end)

local data = utils.table_2_str(result)

local f = io.open("tbl_huxi.lua","w+")
f:write(data)
f:close()