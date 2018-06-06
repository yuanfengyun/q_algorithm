local M = {}

local function add_jiao(tt, result, cached)
	local t = {tt[1],tt[2],tt[3],tt[4]}
	table.sort(t)
	local key = t[4] + t[3]<<5 + t[2] << 10 + t[1] << 15
	if cached[key] then
		return false
	end
	
	cached[key] = true
	
	result[key] = true
	
	return true
end

local function gen_jiao_sub(t,level,result,cached)
	for i=1,20 do
		t[level] = i
		
		local same
		for j=1,level-1 do
			if t[j] == i or t[j] == i+10 or t[j] == i-10 then
				same = true
				break
			end
		end

		if not same then
			add_jiao(t, result, cached)
			if level < 4 then
				gen_jiao_sub(t,level+1,result,cached)
			end
		end
		
		t[level] = 0
	end
end

function M.gen_jiao()
	local t = {0,0,0,0}
	local result = {}
	local cached = {}
	gen_jiao_sub(t,1,result,cached)
	return result
end

return M