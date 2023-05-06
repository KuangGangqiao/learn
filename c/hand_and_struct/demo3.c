#include <stdio.h>

struct port {
	int name;
	int num;
	int addr;
	int (*open)(int);
};

struct port *name(struct port *p, int name)
{
	p->name = name;

	return p;
}

struct port *num(struct port *p, int num)
{
	p->num = num;

	return p;
}

struct port *addr(struct port *p, int addr)
{
	p->addr = addr;

	return p;
}

typedef struct port P;

//typedef 函数指针类型
typedef P* (test_t)(struct port*, int);

int main(void)
{
	int i;
	struct port p;
	//数组指针: 数组的元素是 test_t 类型的指针
	test_t *test[] =
	{
		name,
		num,
		addr,
	};

	for (i = 0; i < 3; i++)
		test[i](&p, i);
	
	printf("name: %d\n", p.name);
	printf("num: %d\n", p.num);
	printf("addr: %d\n", p.addr);
	
	return 0;
}
