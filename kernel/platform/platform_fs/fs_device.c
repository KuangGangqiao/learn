#include<linux/kernel.h>
#include<linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_device.h>



#define MV643XX_ETH_NAME	"eth_test0"

struct platform_device *ppdev;
struct device_node pnp;
struct resource res;

static int __init fs_init(void)
{
	int dev_num = 0;
	int ret;

	memset(&pnp, 0, sizeof(pnp));
	ppdev = platform_device_alloc(MV643XX_ETH_NAME, dev_num);
	if (!ppdev)
		return -ENOMEM;

	ppdev->dev.of_node = &pnp;

	ret = platform_device_add_resources(ppdev, &res, 1);
	if (ret)
		goto port_err;

	ret = platform_device_add(ppdev);
	if (ret)
		goto port_err;

	return 0;

port_err:
	platform_device_put(ppdev);
	return ret;

	
	return 0;
}

static void __exit fs_exit(void)
{
	printk("%s\n", __func__);
	platform_device_put(ppdev);
}

MODULE_LICENSE("GPL");
module_init(fs_init);
module_exit(fs_exit);
