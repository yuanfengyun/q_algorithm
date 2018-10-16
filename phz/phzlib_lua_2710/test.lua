local hulib = require "hulib"

local cards = {
	0,0,1,1,1,2,2,2,2,2,
	0,0,0,0,0,1,0,0,0,1,
}

local begin = os.clock()
--for i = 1,20 do
	huxi = hulib.get_huinfo(cards,-2)
--end

print("huxi",huxi,"time =",os.clock()-begin,"seconds")