// 追溯源头大法
#include <stdio.h>
#define container_of(ptr, type, member) \
(type *)( (char *)ptr - ((size_t)&((type  *)0)->member ));

struct test {
	int age;
	char *name;
	int data;
};

void debug(int *d) {
	struct test *t = container_of(d, struct test, data )
	printf("name: %s\n", t->name);
}

int main(void)
{
	struct test t = {.age=10, .name = "Tom", .data = 100};

	debug(&t.data);
	return 0;
}

