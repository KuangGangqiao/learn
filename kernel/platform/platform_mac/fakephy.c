#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/phy.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <linux/uaccess.h>

MODULE_DESCRIPTION("Fake PHY driver");
MODULE_AUTHOR("jojo");
MODULE_LICENSE("GPL");


static int fake_config_intr(struct phy_device *phydev)
{
	printk("%s\n", __func__);
	return 0;
}

static int fake_config_aneg(struct phy_device *phydev)
{
	printk("%s\n", __func__);
	return 0;
}

static int fake_config_init(struct phy_device *phydev)
{
	printk("%s\n", __func__);
	return 0;
}

static int fake_soft_reset(struct phy_device *phydev)
{
	printk("%s\n", __func__);
	return 0;
}

static int fake_phy_probe(struct phy_device *phydev)
{
	printk("%s\n", __func__);
	return 0;
}

static void fake_get_wol(struct phy_device *phydev,
			   struct ethtool_wolinfo *wol)
{
	printk("%s\n", __func__);
}

static int fake_set_wol(struct phy_device *phydev,
			  struct ethtool_wolinfo *wol)
{
	printk("%s\n", __func__);
	return 0;
}

static int fake_get_tunable(struct phy_device *phydev,
			      struct ethtool_tunable *tuna, void *data)
{
	printk("%s\n", __func__);
	return 0;
}

static int fake_set_tunable(struct phy_device *phydev,
			      struct ethtool_tunable *tuna, const void *data)
{
	printk("%s\n", __func__);
	return 0;
}

static void fake_get_stats(struct phy_device *phydev,
			    struct ethtool_stats *stats, u64 *data)
{
	printk("%s\n", __func__);
}

static void fake_get_strings(struct phy_device *phydev, u8 *data)
{
	printk("%s\n", __func__);
}

static struct phy_driver fake_driver[] = {
{
	.phy_id		= 0xbeefbeef,
	.name		= "Fake Phy",
	.phy_id_mask	= 0xfffffff0,
	/* PHY_BASIC_FEATURES */
	.probe		= fake_phy_probe,
	.soft_reset	= fake_soft_reset,
	.config_init	= fake_config_init,
	.config_aneg	= fake_config_aneg,
	.config_intr	= fake_config_intr,
	.get_wol	= fake_get_wol,
	.set_wol	= fake_set_wol,
	.get_tunable	= fake_get_tunable,
	.set_tunable	= fake_set_tunable,
	.get_stats	= fake_get_stats,
	.get_strings	= fake_get_strings,
},
 };

module_phy_driver(fake_driver);

static struct mdio_device_id __maybe_unused fakephy_tbl[] = {
	{ 0xbeefbeef, 0xfffffff0 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, fakephy_tbl);

