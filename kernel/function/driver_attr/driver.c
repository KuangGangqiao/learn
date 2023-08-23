// 1. platform driver 和 device 匹配，先看有没有id_table
// 没有就看name能不能匹配
// 在/sys/bus/platform/device/也有duang
// 这些目录其实就是机构体层次
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>


static int hello_probe(struct platform_device *pdev);
static 	int hello_remove(struct platform_device *pdev);

static ssize_t peng_show(struct device_driver *drv, char *buf)
{
	printk("%s\n", __func__);
	return 0;
}

static ssize_t peng_store(struct device_driver *dev,
			  const char *buf,size_t count)
{
	printk("%s\n", __func__);
	printk("buf: %s count: %ld", buf, count);
	return count;
}

static DRIVER_ATTR_RW(peng);//在/sys/bus/platform/drivers/duang/目录生成peng


static struct platform_driver hello_driver =
{
	.probe = hello_probe,
	.driver.name = "duang",// platform根据name匹配，也可以根据id_table进行匹配
	.remove = hello_remove,
};

struct resource *res;
static int hello_probe(struct platform_device *pdev)
{
	int ret;
	printk("match ok \n");

	ret = driver_create_file(&(hello_driver.driver), &driver_attr_peng);
	// 在/sys/bus/platform/drivers/生成duang
	if (ret < 0){
		dev_err(&pdev->dev, "could not create sysfs files\n");
		ret = -ENOENT;
	}


	return 0;
}

static 	int hello_remove(struct platform_device *pdev)
{
	printk("hello_remove \n");
	return 0;
}

static int hello_init(void)
{
	printk("hello_init \n");
	return platform_driver_register(&hello_driver);
}
static void hello_exit(void)
{
	printk("hello_exit \n");
	platform_driver_unregister(&hello_driver);
	return;
}
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
