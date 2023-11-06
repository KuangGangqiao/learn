#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/io.h>
#include <linux/slab.h>

struct test {
	int age;
	char *name;
};

struct test *t;

static int __init io_init(void)
{
	int ret;

	printk("%s\n", __func__);

	t = kmalloc(sizeof(*t), GFP_KERNEL);
	t->age = 100;
	t->name = "test";
	printk("addr:%p\n", t);
	ret  = readl((volatile void __iomem *)t);
	printk("before ret:%d\n", ret);
	printk("ret set:%d\n", 10);
	writel(10, (volatile void __iomem *)t);
	ret  = readl((volatile void __iomem *)t);
	printk("after ret:%d\n", ret);
	
	return 0;
}

static void __exit io_exit(void)
{
	printk("%s\n", __func__);
	kfree(t);
}

MODULE_LICENSE("GPL");
module_init(io_init);
module_exit(io_exit);
