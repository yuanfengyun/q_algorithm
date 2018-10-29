local utils = require "utils"

local tt = {
	-- 小将
	{{[1]=2},eye=1,huxi=0},
	-- 大将
	{{[11]=2},eye=1,huxi=0},
	-- 小偎
	{{[1]=3},huxi=6,eye=0},
	-- 大偎
	{{[11]=3},huxi=9,eye=0},
	-- 一二三
	{{[1]=1,[2]=1,[3]=1},eye=0,huxi=0,b1=1},
	{{[11]=1,[12]=1,[13]=1},eye=0,huxi=0,b11=1},
	-- 二七十
	{{[1]=1,[6]=1,[9]=1},eye=0,huxi=3,b2=1},
	{{[11]=1,[16]=1,[19]=1},eye=0,huxi=6,b12=1},
	-- 绞
	{{[1]=2,[11]=1},eye=0,huxi=0},
	{{[1]=1,[11]=2},eye=0,huxi=0},
}

function copy(cards,huxi,b1,b11,b2,b12)
	local cc = {[1]=0,[11]=0}
	for k,v in ipairs(cards) do
		if v > 0 then
			cc[k] = v
		end
	end
	cc.x = huxi
	return cc
end

local cache = {}
local result = {}
function add_table(cards,huxi,eye,b1,b11,b2,b12)
	if b1 >= 3 or b11 >= 3 or b2 >= 3 or b12 >= 3 then
		return
	end
	if b1 + b11 >= 4 or b11 + b12 >= 4 then
		return
	end
	local old1 = cards[1]
	local old11 = cards[11]
	if old1 > 2 then
		cards[1] = 2
		cards[9] = old1 - 2
	end
	if old11 > 2 then
		cards[11] = 2
		cards[9] = cards[9] + old11 - 2
	end
	local key = table.concat(cards,"")
	local old = cache[key]

	cards[1] = old1
	cards[11] = old11
	cards[9] = 0
	if old and old > huxi then
		return false
	end

	cache[key] = huxi
	table.insert(result,copy(cards,huxi,b1,b11,b2,b12))
	return true
end

function check(cards,adds,eye)
	if eye > 1 then
		return false
	end
	local sum = 0
	for card,n in pairs(adds) do
		if cards[card]+n > 6 then
			return false
		end
		sum = sum + n
	end

	for i=1,20 do
		local n = cards[i]
		if adds[i] then
			n = n + adds[i]
		end
		if n > 2 then
			sum = sum + n - 2
		end
	end
	return sum <= 4
end

function addone(cards,level,huxi,eye,b1,b11,b2,b12)
	for k,v in ipairs(tt) do
		if check(cards,v[1],eye+v.eye) then
			for card,n in pairs(v[1]) do
				cards[card] = cards[card]+n
			end
			if add_table(cards,huxi+v.huxi,eye+v.eye,b1 +(v.b1 or 0),b11+(v.b11 or 0),b2+(v.b2 or 0),b12+(v.b12 or 0)) and level < 7 then
				addone(cards,level+1,huxi+v.huxi,eye+v.eye,b1 +(v.b1 or 0),b11+(v.b11 or 0),b2+(v.b2 or 0),b12+(v.b12 or 0))
			end
			for card,n in pairs(v[1]) do
				cards[card] = cards[card]-n
			end
		end
	end
end

collectgarbage("stop")
local cards = {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
}
addone(cards,1,0,0,0,0,0,0)
collectgarbage("collect")
table.sort(result,function(a,b)
	return a.x > b.x
end)

local f = io.open("tbl_split1.lua","w+")

f:write("return {\n")

for _,v in ipairs(result) do
	local data = utils.table_2_str(v)
	f:write(data)
	f:write(",\n")
end
f:write("}\n")

f:close()