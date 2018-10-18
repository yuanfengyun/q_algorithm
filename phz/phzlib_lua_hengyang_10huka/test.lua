local hulib = require "hulib"

local cards = {
    --1,2,3,4,5,6,7,8,9,0
	  0,0,0,1,1,1,0,0,0,0,
	  1,1,1,0,0,0,0,0,0,0,
}

local begin = os.clock()
--for i = 1,20 do
--	huxi = hulib.get_huinfo(cards,3,2)
--end

print(hulib.is_piao(cards,10,4))

print("huxi",huxi,"time =",os.clock()-begin,"seconds")