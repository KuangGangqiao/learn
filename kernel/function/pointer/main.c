// 我们可以只输出指针地址，然后去系统目录下的： /proc/kallsyms 文件中
// 根据指针值查相应的函数名，这个文件也是 printk  %pS的实现原理。
#include<linux/module.h>

struct human {
	char *name;
	int (*run)(struct human *, int);
	int (*eat)(struct human *, char *);
	int (*clothes)(struct human *, char *);
	int (*house)(struct human *, char *);
	int (*love)(struct human *);
};

int run0(struct human *h, int len)
{
	printk("%s run %dm\n", h->name, len);
	return 0;
}

int run1(struct human *h, int len)
{
	printk("%s run %dm\n", h->name, len);
	return 1;
}

struct human x;

static int hello_init(void)
{
	x.name = "Tom";
	x.run = run0;
	x.run(&x, 10);
	//打印函数指针具体调用函数名字
	printk("func: %pS at address: %px\n", x.run, x.run);
	printk("%s is both!\n", x.name);
	return 0;
}

static void hello_exit(void)
{
	printk("%s is go die!\n", x.name);
	return;
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);

