#-*-coding:ascii-*-
def get_hu_info(hand_cards, cur_card, gui_index):
	cards = hand_cards[:]
	if cur_card != 34:
		cards[cur_card] = cards[cur_card] + 1
	gui_num = 0
	if gui_index != 34:
		gui_num = cards[gui_index]
		cards[gui_index] = 0
	eyes = []
	empty = -1
	for i in range(0,34):
		n = cards[i]
		if n == 0:
			empty = i
		elif n == 1 and gui_num >= 1:
			eyes.append(i)
		else:
			eyes.append(i)

	if gui_num > 2:
		eyes.append(empty)
		
	hu = False
	cache = [0,0,0,0]
	for i in eyes:
		if i == empty:
			hu = foreach_eye(cards, gui_num - 2, gui_num, 1000, cache)
		else:
			n = cards[i]
			color = i/9
			if n == 1:
				cards[i] = 0
				hu = foreach_eye(cards, gui_num - 1, gui_num, color, cache)
			else:
				cards[i] = cards[i] - 2
				hu = foreach_eye(cards, gui_num,gui_num,color,cache)
			cards[i] = n
		if hu:
			break
			
	if gui_num > 0:
		cards[gui_index] = gui_num
			
	return hu
	
def foreach_eye(cards, gui_num, max_gui, eye_color, cache):
	left_gui = gui_num
	for i in range(0, 3):
		cache_index = -1
		if eye_color != i:
		    cache_index = i
 
		need_gui = check_normal(cards, i*9, max_gui, cache_index, cache)
		if cache_index >= 0:
			cache[cache_index] = need_gui+1
		left_gui = left_gui - need_gui
		if left_gui < 0:
			return False

	cache_index = -1
	if eye_color != 3: 
	   cache_index = 3
	need_gui = check_zi(cards, max_gui, cache_index, cache)
	if cache_index> 1:
		cache[3] = need_gui + 1
	return left_gui >= need_gui

def check_zi(cards,  max_gui, cache_index, cache):
	if cache_index >= 1:
		n = cache[cache_index]
		if n > 0:
		   return n - 1

	need_gui = 0
    
	for i in range(27, 34):
		c = cards[i]
		if c == 1 or c == 4:
			need_gui = need_gui + 2
		elif c == 2:
			need_gui = need_gui + 1
		if need_gui > max_gui:
		   return need_gui
	return need_gui

def check_normal(cards, begin, max_gui, cache_index, cache):
	if cache_index >= 1:
		n = cache[cache_index]
		if n > 0:
		   return n - 1

	n = 0
	for i in range(begin, begin+9):
		n = n * 10 + cards[i]
	
	if n == 0:
	   return 0
	return next_split(n, 0, max_gui)


def next_split(n, need_gui, max_gui):
	c = 0
	while True:
		if n == 0:
		   return need_gui
		
		while (n > 0):
			c = n % 10
			n = n / 10
			if c != 0:
			   break
		if c == 1 or c == 4:
			return one(n, need_gui, max_gui)
		elif c == 2:
			return two(n, need_gui, max_gui)
	return need_gui

def one(n, need_gui, max_gui):
	c1 = n % 10
	c2 = (n % 100) / 10

	if c1 == 0:
	    need_gui = need_gui + 1
	else:
		n = n - 1

	if c2 == 0:
	   need_gui = need_gui + 1
	else:
	   n = n - 10

	if n == 0:
	   return need_gui

	if need_gui > max_gui :
	   return need_gui

	return next_split(n, need_gui, max_gui)

def two(n, need_gui, max_gui):
	c1 = n % 10
	c2 = (n % 100) / 10
	c3 = (n % 1000) / 100
	c4 = (n % 10000) / 1000
    
	choose_ke = True
	if c1 == 0:
		pass
	elif c1 == 1:
		if c2 == 0 or c2 == 1:
			pass
		elif c2 == 2:
			if c3 == 2:
				if c4 == 2:
					choose_ke = false
			elif c3 == 3:
				if c4 != 2:
					choose_ke = false
			else:
				choose_ke = false
		elif c2 == 3:
			if c3 == 0 or c3 == 2 or c3 == 1 or c3 == 4:
				choose_ke = false
		elif c2 == 4:
			if c3 == 2:
				if c4 == 2 or c4 == 3 or c4 == 4:
					choose_ke = false
			elif c3 == 3:
				choose_ke = false
	elif c1 == 2:
		choose_ke = false
	elif c1 == 3:
		if c2 == 2:
			if c3 == 1 or c3 == 4:
				choose_ke = false
			elif c3 == 2:
				if c4 != 2:
					choose_ke = false
		if c2 == 3:
			choose_ke = false
		elif c2 == 4:
			if c3 == 2:
				choose_ke = false
	elif c1 == 4:
		if c2 == 2 and c3 != 2:
			choose_ke = false
		elif c2 == 3:
			if c3 == 0 or c3 == 1 or c3 == 2:
				choose_ke = false
		elif c2 == 4:
			if c3 == 2:
				choose_ke = false


	if choose_ke:
		need_gui = need_gui + 1
	else:
		if c1 < 2:
			need_gui = need_gui + (2 - c1)
			n = n - c1
		else: 
			n = n - 2

		if c2 < 2:
			need_gui = need_gui + (2 - c2)
			n = n - c2
		else:
			n = n - 20

	if n == 0:
	   return need_gui

	if need_gui > max_gui:
	   return need_gui

	return next_split(n, need_gui, max_gui)
