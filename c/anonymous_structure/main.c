//使用匿名结构体，结构体对象 jim 可以通过 jim.area_code 
//直接访问匿名结构体成员变量 area_code，代码相对比较简洁
//反之则必须通过 jim.office.area_code 来访问结构体成员变量 
//匿名联合体也一样的道理，他们共用一片内存,这里有两个匿名结构体
#include <stdio.h>

struct person
{
	char    *name;
	char     gender;
	int      age;
	int      weight;
	union {
		struct {
			int  area_code;
			long phone_number;
		};
		struct {
			int  _area_code;
			long _phone_number;
			int test;
		};
	};
};

int main(void)
{
	//这种方式赋值是CPU自动在栈空间赋值的，栈空间是连续的
	//堆空间是不连续的，且由手动分配
	struct person jim = {"jim", 'F', 28, 65, {21, 58545566}};
	jim.test = 20;
	printf("jim.area_code: %d\n", jim.area_code);
	printf("jim,phone_number: %ld\n", jim.phone_number);
	printf("jim,_area_code: %d\n", jim._area_code);
	printf("jim,_phone_number: %d\n", *(&(jim.area_code) + 2));
	printf("jim,test: %d\n", *(&(jim.area_code) + 4));
}