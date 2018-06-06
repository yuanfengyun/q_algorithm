local gen_jiao = require "gen_jiao"
local result = gen_jiao.gen_jiao()

local n = 0
for k,v in pairs(result) do
	n = n + 1
end

print(n)

--[[
local hulib = require "hulib"

hulib:init()

local cards = {
	1,2,1,0,3,0,1,0,0,1,
	1,2,1,0,2,0,1,0,0,1
}

local begin = os.time()
local huxi = hulib:get_hu_info(cards)

print("huxi",huxi,"time =",os.time()-begin,"seconds")]]--