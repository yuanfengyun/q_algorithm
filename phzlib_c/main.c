#include "hulib.h"
#include "stdio.h"
#include "time.h"

void test() {
	char cards[] = {
		1,2,2,1,2,1,2,1,0,1,
		1,2,2,1,0,0,0,2,0,0
	};
	printf("胡息: %d\n", get_huxi(cards));
}

void test_eye() {
	char cards_eye[] = {
		1,1,2,1,2,1,1,0,0,0,
		1,2,2,1,0,0,2,1,1,1
	};
	printf("胡息: %d\n", get_huxi(cards_eye));
}

int main()
{
	char cards[] = {
		1,2,2,1,2,1,2,1,0,1,
		1,2,2,1,0,0,0,2,0,0
	};

	char cards_eye[] = {
		1,1,2,1,2,1,1,0,0,0,
		1,2,2,1,0,0,2,1,1,1
	};

	time_t begin = time(NULL);
	long long sum = 0;
	int k = 0;
	int count = 0;
	for (int i = 0; i < 10000*10000; ++i) {
		if (k) {
			sum += get_huxi(cards);
			count = count + 1;
			k = 0;
		}
		else {
			sum += get_huxi(cards_eye);
			count = count + 1;
			k = 1;
		}
		
	}
	printf("胡息: %d\n", sum);
	printf("时间: %d\n", time(NULL) - begin);
	printf("次数: %d万次\n", count/10000);
	getchar();

	return 0;
}
