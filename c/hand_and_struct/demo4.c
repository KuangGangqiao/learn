#include<stdio.h>

//x86默认4个字节对齐, 相邻的两个元素字节数相加小于4,
//那么gcc会把他们两个挤起来,后面不足的再填充其他的
typedef struct{
	char age;
	char x;
	int num;
	char name[200];
}A;

typedef struct {
	char age;
	int num;
	char x;
	char name[200];
}B;

typedef struct {
	char age;
	char x;
	int num;
	char name[200];
}__attribute__((packed))a;

typedef struct {
	char age;
	char x;
	int num;
	char name[200];
}__attribute__((packed))b;

void test(char *x)
{
	printf("x:%d\n", x[0]);
	printf("addr_x:%p\n", x);
}

int main()
{
	A test0;
	B test1;
	a test2;
	b test3;
	char *h;

	test3.age = 1;
	test3.x = 2;
	test3.num = 10;
	test3.name[0] = 'w';
	printf("size: %ld\n", sizeof(test0));
	printf("size: %ld\n", sizeof(test1));
	printf("size: %ld\n", sizeof(test2));
	printf("size: %ld\n", sizeof(test3));
	
	printf("age_addr: %p\n", &test3.age);
	printf("num_addr: %p\n", &test3.num);
	printf("arr_addr: %p\n", &test3.name[0]);
	h = (char*)&test3;
	printf("addr: %p\n", &h[6]);
	printf("h[0]: %d\n", h[6]);
	test(h);
}
