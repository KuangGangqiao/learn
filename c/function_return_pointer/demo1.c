#include <stdio.h>

//typedef 一个函数指针类型, 记忆一下
typedef int (*fun_t)(void);

struct dog {
	int age;
	int weight;
	void (*action)(void *a);
};

static void dog_voice(void)
{
	printf("%s\n", __func__);
}

static void action(void* a)
{
	printf("%s\n", __func__);
	//强行转换void *a 为fun_t 类型指针
	fun_t b = a;
	b();
}

int main(void)
{
	struct dog d;
	d.age = 10;
	d.weight = 100;
	d.action = action;

	d.action(dog_voice);

	return 0;
}
