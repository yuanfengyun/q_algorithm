public class main {
	public static void test_success() {
		int[] cards = {
			1,1,0,0,1,0,1,0,0,
			0,0,0,2,4,2,0,0,0,
			0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,2
		};
		
		if(split.get_hu_info(cards, 34, 33)){
			System.out.println("test success"); 
		}
	}
	
	public static void test_fail() {
		int[] cards = {
			1,1,0,0,1,0,1,0,0,
			0,0,0,2,4,2,1,0,0,
			0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1
		};
		
		if(!split.get_hu_info(cards, 34, 33)){
			System.out.println("test success"); 
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		test_success();
		test_fail();
	}

}