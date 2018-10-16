local hulib = require "hulib"

hulib:init()

local cards = {
	1,2,2,1,1,0,1,0,0,1,
	1,1,2,2,2,1,0,1,1,1
	

}

cards = {
	1,1,1,0,0,1,1,2,0,0,
	1,2,1,0,0,0,1,2,0,1
}

local begin = os.time()
for i = 1,10*10000 do
	local huxi = hulib:get_hu_info(cards)
end

print("huxi",huxi,"time =",os.time()-begin,"seconds")