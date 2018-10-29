----------------- 操作提、偎、胡、跑、碰、吃，按优先级排序 -----------------
-- 提：前提在发牌后进行，有提必须提
-- 偎：有偎必须偎
-- 胡：只能胡别人摸出的牌或自己摸出的牌
-- 跑：能跑别人打出的牌、别人摸出的牌、自己摸出的牌
-- 碰: 能碰别人摸出的牌、别人打出的牌
-- 吃：能吃上家打出的牌、自己摸出的牌

local phz = require "phz"

-- 跑胡子
local M = {}

M.OP_TYPE = {
	HU  = 0x0001
	TI  = 0x0002,
	PAO = 0x0004,
	WEI = 0x0008,
	PENG= 0x0010,
	OUT_CARD = 0x0020,
	PASS = 0x0040,
	CHI_A_AB = 0x0100,
	CHI_A_BB = 0x0200,
	CHI_2710 = 0x0400,
	CHI_LEFT = 0x0800,
	CHI_RIGHT = 0x1000,
	CHI_CENTER = 0x2000,
}

-- 洗牌
-- 1-10  一二三四五六七八九十
-- 11-20 壹贰仨肆伍陆柒捌玖拾
function M.shuffle()
	local cards = {
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		 1,  2,  3,  4,  5,  6,  7,  8,  9, 10,

		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20
	}

	return math.random_shuffle(cards)
end

-- 判断是否可以进行吃操作
function M.has_chi(ops)
	return ops & 0xff00 ~= 0
end

-- 计算胡息
function M.get_huxi(cards, waves)
	local huxi = phz.get_huxi(cards)
	if huxi < 0 then
		return
	end

	for _, wave in ipairs(waves) do
		huxi = huxi + wave.huxi
	end

	-- 如果小于最小胡息
	if huxi < 15 then
		return
	end

	return huxi
end

-- 获取桌面下牌胡息
function M.get_wave_huxi(op, card)
	if op == M.OP_TYPE.TI then          -- 提
		return card > 10 and 12 or 9
	elseif  op == M.OP_TYPE.PAO then    -- 跑
		return card > 10 and 9 or 6
	elseif  op == M.OP_TYPE.WEI then    -- 偎
		return card > 10 and 6 or 3
	elseif  op == M.OP_TYPE.PENG then   -- 碰
		return card > 10 and 3 or 1
	elseif (op == M.OP_TYPE.CHI_LEFT or op == M.OP_TYPE.CHI_CENTER or op == M.OP_TYPE.CHI_RIGHT) and (card % 10) == 2 then    -- 吃 
		return card > 10 and 6 or 3
	elseif  op == M.OP_TYPE.CHI_2710 then    -- 吃 
		return card > 10 and 6 or 3
	end

	return 0
end

-- 获取所有能提的牌
function M.get_all_ti(cards)
	local ret = {}
	for i,n in ipairs(cards) do
		if n == 4 then
			table.insert(t, i)
		end
	end
end

local CHI_OPS = {
	M.OP_TYPE.CHI_A_AB,
	M.OP_TYPE.CHI_A_BB,
	M.OP_TYPE.CHI_2710,
	M.OP_TYPE.CHI_LEFT,
	M.OP_TYPE.CHI_RIGHT,
	M.OP_TYPE.CHI_CENTER
}

function M.clone_cards(cards)
	local t = {}
	for i=1,20 do
		table.insert(t, cards[i])
	end
	return t
end

function M.is_valid_chi_op(op)
	for _,v in ipairs(CHI_OPS) do
		if v == op then
			return true
		end
	end
	return false
end

-- 检查客户端的吃法对不对
function M.check_chi(cards, card, tips, ops)
	if not ops or #ops ~= cards[card] + 1 then
		return
	end

	if ops[1] & tips == 0 then
		return false
	end

	local num_tbl = {}
	for _,op in ipairs(ops) do
		if not M.is_valid_chi_op(op) then
			return false
		end
		local card1,card2 = M.get_chi_cards(op, card)
		if not card1 then
			return false
		end
		num_tbl[card1] = (num_tbl[card1] or 0) + 1
		num_tbl[card2] = (num_tbl[card2] or 0) + 1
	end

	for c,n in pairs(num_tbl) do
		if cards[c] >= 3 then
			return false
		end
		if cards[c] < n then
			return false
		end
	end
	return true
end

-- 吃牌
function M.chi(cards, waves, chi_waves, card, ops)
	cards[card] = 0
	for _,op in ipairs(ops) do
		local card1,card2,center_card = M.get_chi_cards(op, card)
		cards[card1] = cards[card1] - 1
		cards[card2] = cards[card2] - 1
		local wave = {
			op = op,
			card = center_card,
			huxi = M.get_wave_huxi(op, center_card)
		}
		table.insert(waves, wave)
		table.insert(chi_waves, wave)
	end
end

function M.get_chi_ops(cards, card)
	local ops = 0
	for _, op in ipairs(CHI_OPS) do
		local tmp_cards = M.clone_cards(cards)
		if M.sub_chi(op, tmp_cards, card, false) and 
		   M.check_bi(tmp_cards, card) then
			ops = ops | op
		end
	end
	return ops
end

function M.sub_chi(op, cards, card, self)
	local card1, card2 = M.get_chi_indexs(op, card)
	if not card1 then
		return false
	end

	if cards[card1] ~= 1 and cards[card1] ~= 2 then
		return false
	end

	if cards[card2] ~= 1 and cards[card2] ~= 2 then
		return false
	end

	if card1 == card2 and cards[card1] ~= 2 then
		return false
	else

	-- 扣除
	if self then
		cards[card] = cards[card] - 1
	end
	cards[card1] = cards[card1] - 1
	cards[card2] = cards[card2] - 1

	return true
end

function M.check_bi(cards, card)
	if cards[card] == 0 then
		return true
	end
	for _, op in ipairs(CHI_OPS) do
		repeat
			if not M.sub_chi(op, tmp_cards, card, true) then 
				break
			end
		until cards[card] == 0

		if cards[card] == 0 then
			return true
		end
	end

	return false
end

function M.get_chi_cards(op, card)
	if op == CHI_LEFT then
		if (card <= 10 and card + 2 > 10) or (card > 10 and card + 2 > 20) then
			return
		end
		return card+1, card+2, card+1
	elseif op == CHI_CENTER then
		if card == 1 or card == 10 or card == 11 or card == 20 then
			return
		end
		return card-1, card+1, card
	elseif op == CHI_RIGHT then
		if (card <= 10 and card - 2 <= 0) or (card > 10 and card - 2 <= 10) then
			return
		end
		return card-2, card-1, card-1
	elseif op == CHI_2710 then
		local t = {2,7,10}
		local base = card < 10 and 0 or 10
		for i,v in ipairs(t) do
			if card == i or card == i + 10 then
				table.remove(t,i)
				return base + t[1], base + t[2], base + 7
			end
		end
	elseif op == CHI_A_AB then
		if card < 10 then
			return card, card+10, card
		else
			return card, card-10
		end
	elseif op == CHI_A_BB then
		if card < 10 then
			return card+10, card+10, card
		else
			return card-10, card-10, card
		end
	end
end

return M


