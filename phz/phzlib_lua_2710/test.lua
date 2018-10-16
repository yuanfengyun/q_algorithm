local hulib = require "hulib"

local cards = {
	1,0,0,0,0,0,0,1,2,0,
	2,1,0,0,0,0,1,2,1,1,
}

local begin = os.clock()
--for i = 1,20 do
	huxi = hulib.get_huinfo(cards,6)
--end

print("huxi",huxi,"time =",os.clock()-begin,"seconds")