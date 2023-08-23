#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
static void hello_release(struct device *dev)
{
     return;
}

static struct platform_device hello_device =
{
	.name = "duang",
	.id = -1,
	.dev.release = hello_release,
};

static int hello_init(void)
{
	printk("hello_init \n");
	return platform_device_register(&hello_device);

}

static void hello_exit(void)
{
	printk("hello_exit \n");
	platform_device_unregister(&hello_device);
	return;
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
