#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mtd/physmap.h>

// 对static const char * const part_probes[]的理解
// 静态常量型指针变量　　static const char*
// 常量型变量　　const part_probes[]
// 这样修饰的作用为
// 指针 part_probes 不能做修改、做运算, 即 part_probes++，part_probes-- 非法　该指针为 only-read
// 变量 part_probes 中的内容不能修改, 该变量为 only-read
static const char * const part_probes[] = { "fakemac_part", NULL };

static struct physmap_flash_data bcma_pflash_data = {
	.part_probe_types	= part_probes,
};

static struct resource fakemac_resource = {
	.name	= "fakemac_mem",
	.flags  = IORESOURCE_MEM,
};

static void fakemac_device_release(struct device *dev)
{
	printk("%s\n", __func__);
	return;
}

//这个对象其实也可以用device-tree来自动生成，
//device_tree -> device_node -> platform_device struct,然后
//platform_driver 根据匹配规则进行probe match
//自己手动写不常见了
static struct platform_device fakemac_device =
{
	.name = "fakemac",
	.id = -1,
	.num_resources	= 1,
	.resource	= &fakemac_resource,
	.dev		= {
		.platform_data  = &bcma_pflash_data,
		.release = fakemac_device_release,
	},
};

static int fakemac_device_init(void)
{
	printk("%s\n", __func__);
	return platform_device_register(&fakemac_device);

}

static void fakemac_device_exit(void)
{
	printk("%s\n", __func__);
	platform_device_unregister(&fakemac_device);
	return;
}

MODULE_LICENSE("GPL");
module_init(fakemac_device_init);
module_exit(fakemac_device_exit);
