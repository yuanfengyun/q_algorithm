#include "hulib.h"
#include "stdio.h"
#include "time.h"

char cards_buf[2500000*21];
int cards_buf_len = 0;
void read_all(char* name)
{
	FILE *fp = fopen(name, "rb");

	memset(cards_buf, 0, sizeof(cards_buf));
	long long xiao;
	long long da;
	int tmp;
	cards_buf_len = 0;
	while(fscanf(fp, "%lld-%lld=%d\n", &da, &xiao, &tmp) != EOF)
	{
		char* cards = &cards_buf[cards_buf_len * 21];
		int i = 9;
		while (xiao > 0) {
			cards[i--] = (xiao % 10);
			xiao /= 10;
		}
		i = 19;
		while (da > 0) {
			cards[i--] = (da % 10);
			da /= 10;
		}
		cards[20] = tmp;
		++cards_buf_len;
	}
}

int test_all(char* name)
{
	read_all(name);

	for (int i = 0; i < cards_buf_len; ++i)
	{
		int huxi = get_huxi(&cards_buf[i * 21]);
		int table_huxi = cards_buf[i * 21 + 20];
		if (huxi != table_huxi) {
			printf("胡息判断不对, 表%s, 序号%d, 计算%d，表%d\n", name, i+1, huxi, table_huxi);
		}
	}
	printf("test_all测试%s完成\n", name);
	return cards_buf_len;
}

void test() {
	char cards[] = {
		0,2,1,1,0,0,1,0,0,1,
		0,1,2,2,0,0,2,0,0,2
		
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
	//test();
	//getchar();

	time_t begin = time(NULL);
	int count = test_all("table");

	count += test_all("eye_table");
	printf("时间: %dS\n", time(NULL) - begin);
	printf("次数: %d万次\n", count/10000);
	getchar();

	return 0;
}
