#-*-coding:ascii-*-
import split

def main():
	cards = [1,1,1,0,0,0,1,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,0,0,0,0,2]
	print("test begin")
	if split.get_hu_info(cards,34,33):
		print("hu le")
	else:
		print("can't hu")
		
if __name__ == "__main__":
	main()
	