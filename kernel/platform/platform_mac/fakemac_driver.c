// 1. platform driver 和 device 匹配，先看有没有id_table
// 没有就看name能不能匹配
// 在/sys/bus/platform/device/也有duang
// 这些目录其实就是机构体层次
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/delay.h>

#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/skbuff.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/phy_fixed.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/atomic.h>
#include <linux/mtd/physmap.h>
#include <linux/of_mdio.h>


#define FAKEMAC_VERSION "1.0.0"
#define FAKEMAC_QUEUES	8

struct reg {
	u16 addr;
	u16 val;
};

struct fakemac_priv {
	spinlock_t lock;
	spinlock_t rx_lock;
	u32 phy_addr;
	u32 bus_id;
	struct fakemac_desc *rx_head;
	int ring_size;
	struct fakemac_desc *desc_ring;
	dma_addr_t dma_ring;
	void __iomem *regs;
	struct mii_bus *mii_bus;
	char phy_name[64];
	int oldlink, oldspeed, oldduplex;
	u32 msg_enable;
	struct net_device *dev;
	struct work_struct reset_work;
	struct platform_device *pdev;
	struct napi_struct napi;
	atomic_t reset_pending;
};

static void fakemac_phy_link_change(struct net_device *dev)
{
	printk("%s\n", __func__);
}

static int fakemac_phy_open(struct net_device *dev)
{
	struct fakemac_priv *priv = netdev_priv(dev);
	struct phy_device *phydev;
	int ret;

	printk("%s\n", __func__);
	
	if (!priv->mii_bus) {
		printk("mii bus is NULL\n");
		return -1;
	}

#if 0
	struct phy_c45_device_ids c45_ids;
	/* try force creat phy device  */
	phydev = phy_device_create(priv->mii_bus, 0, 0xbeefbeef, 0, &c45_ids);
	if (!phydev) {
		printk("phydev is NULL\n");
		return -2;
	}
#endif

	phydev = phy_find_first(priv->mii_bus);
	if (!phydev) {
		printk("phydev find first failed\n");
		return -2;
	} else {
		printk("phydev find first phy at addr: %d\n", phydev->mdio.addr);
		printk("phydev find first phy phydev id: 0x%x\n", phydev->phy_id);
	}
	ret = phy_connect_direct(dev, phydev,
				 fakemac_phy_link_change,
				 PHY_INTERFACE_MODE_GMII);
	if (ret) {
		printk("phy connect direct fail!\n");
		return ret;
	} else {
		printk("phy connect direct success!\n");
	}
	/* MAC doesn't support 1000T Half */
	phy_remove_link_mode(phydev, ETHTOOL_LINK_MODE_1000baseT_Half_BIT);
	/* support both flow controls */
	phy_support_asym_pause(phydev);
	phy_start(phydev);
	phy_start_aneg(phydev);
	phy_attached_info(phydev);
	return 0;
}

static void fakemac_phy_close(struct net_device *dev)
{
	struct phy_device *phydev = dev->phydev;
	//struct module *ndev_owner = NULL;
	struct mii_bus *bus;

	if (phy_is_started(phydev))
		phy_stop(phydev);

	if (phy_interrupt_is_valid(phydev))
		phy_free_interrupt(phydev);

	phydev->adjust_link = NULL;

	if (phydev->sysfs_links) {
		if (dev)
			sysfs_remove_link(&dev->dev.kobj, "phydev");
		// don't have attached_dev
		//sysfs_remove_link(&phydev->mdio.dev.kobj, "attached_dev");
	}

	if (!phydev->attached_dev)
		printk("have some thing\n");
		//sysfs_remove_file(&phydev->mdio.dev.kobj,
		//		  &dev_attr_phy_standalone.attr);

	phy_suspend(phydev);
	if (dev) {
		printk("debug0\n");
		phydev->attached_dev->phydev = NULL;
		phydev->attached_dev = NULL;
	}
	phydev->phylink = NULL;

	if (!phydev)
		printk("phydev is NULL\n");

	if (phydev->mdio.dev.driver) {
		printk("debug1\n");
		module_put(phydev->mdio.dev.driver->owner);
	}

	printk("toto\n");
	if (phy_driver_is_genphy(phydev) ||
	    phy_driver_is_genphy_10g(phydev)) {
		printk("debug2\n");
		device_release_driver(&phydev->mdio.dev);
	}

	phy_device_reset(phydev, 1);

	bus = phydev->mdio.bus;

	put_device(&phydev->mdio.dev);

#if 0
	if (dev)
		ndev_owner = dev->dev.parent->driver->owner;
	if (ndev_owner != bus->owner)
		module_put(bus->owner);
#endif

	phydev = NULL;
}

int fakemac_mdio_unregister(struct net_device *ndev)
{
	struct fakemac_priv *priv = netdev_priv(ndev);

	printk("%s\n", __func__);
	if (!priv->mii_bus)
		return 0;

	mdiobus_unregister(priv->mii_bus);
	priv->mii_bus->priv = NULL;
	mdiobus_free(priv->mii_bus);
	priv->mii_bus = NULL;

	return 0;
}

static int fakemac_open(struct net_device *dev)
{
	int rc;

	printk("%s\n", __func__);
	rc = fakemac_phy_open(dev);
	if (rc) {
		netdev_err(dev, "failed to setup phy (%d)\n", rc);
		return -1;
	}

	return 0;
}

static int fakemac_stop(struct net_device *dev)
{
	printk("%s\n", __func__);
	fakemac_phy_close(dev);
	return 0;
}

static netdev_tx_t fakemac_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk("%s\n", __func__);
	return NETDEV_TX_OK;
}

static void fakemac_set_multicast_list(struct net_device *dev)
{
	
	printk("%s\n", __func__);
}

static int _phy_do_ioctl_running(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	printk("%s\n", __func__);
	return 0;
}

static int _eth_validate_addr(struct net_device *dev)
{
	printk("%s\n", __func__);
	return 0;
}

static int _eth_mac_addr(struct net_device *dev, void *p)
{
	printk("%s\n", __func__);
	return 0;
}
static const struct net_device_ops fakemac_netdev_ops = {
	.ndo_open		= fakemac_open,
	.ndo_stop		= fakemac_stop,
	.ndo_start_xmit		= fakemac_start_xmit,
	.ndo_set_rx_mode	= fakemac_set_multicast_list,
	.ndo_do_ioctl		= _phy_do_ioctl_running,
	.ndo_validate_addr	= _eth_validate_addr,
	.ndo_set_mac_address	= _eth_mac_addr,
};

static void fakemac_get_drvinfo(struct net_device *dev,
			      struct ethtool_drvinfo *info)
{
	printk("%s\n", __func__);
	strlcpy(info->driver, "fakemac", sizeof(info->driver));
	strlcpy(info->version, FAKEMAC_VERSION, sizeof(info->version));
	snprintf(info->bus_info, sizeof(info->bus_info), "%s", "fakemac");
}

static u32 fakemac_ethtool_op_get_link(struct net_device *dev)
{
	printk("%s\n", __func__);
	return 1;
}

static int fakemac_phy_ethtool_get_link_ksettings(struct net_device *ndev,
				   struct ethtool_link_ksettings *cmd)
{
	printk("%s\n", __func__);
	return 0;
}

static int fakemac_phy_ethtool_set_link_ksettings(struct net_device *ndev,
				   const struct ethtool_link_ksettings *cmd)
{
	printk("%s\n", __func__);
	return 0;
}
void fakemac_get_wol(struct net_device *ndev, struct ethtool_wolinfo *info)
{
	printk("%s\n", __func__);

}

int fakemac_set_wol(struct net_device *ndev, struct ethtool_wolinfo *info)
{
	printk("%s\n", __func__);
	return 0;
}

int fakemac_get_tunable(struct net_device *ndev,
			const struct ethtool_tunable *tun, void *v)
{
	printk("%s\n", __func__);
	return 0;
}

int fakemac_set_tunable(struct net_device *ndev,
			const struct ethtool_tunable *tun, const void *v)
{
	printk("%s\n", __func__);
	return 0;
}


static const struct ethtool_ops fakemac_ethtool_ops = {
	.get_drvinfo = fakemac_get_drvinfo,
	.get_link = fakemac_ethtool_op_get_link,
	.get_link_ksettings = fakemac_phy_ethtool_get_link_ksettings,
	.set_link_ksettings = fakemac_phy_ethtool_set_link_ksettings,
	.get_wol = fakemac_get_wol,
	.set_wol = fakemac_set_wol,
	.get_tunable = fakemac_get_tunable,
	.set_tunable = fakemac_set_tunable,
};

static int fakemac_mdio_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	printk("%s addr: %d, phyreg: %d\n", __func__, phyaddr, phyreg);
	if (phyaddr == 31)
		if (phyreg == 2 || phyreg == 3)
			return 0xbeef;
		
	// must return 0xffff
	return 0xffff;
}

static int fakemac_mdio_write(struct mii_bus *bus, int phyaddr,
			      int phyreg, u16 phydata)
{
	printk("%s\n", __func__);
	return 0;
}

int fakemac_mdio_reset(struct mii_bus *bus)
{
	printk("%s\n", __func__);
	// must return 0
	return 0;
}


int fakemac_mdio_register(struct net_device *ndev)
{
	struct fakemac_priv *priv = netdev_priv(ndev);
	struct mii_bus *new_bus;
	int ret;

	printk("%s\n", __func__);
	new_bus = mdiobus_alloc();
	if (!new_bus)
		return -ENOMEM;

	new_bus->name = "fakemac";
	new_bus->read = &fakemac_mdio_read;
	new_bus->write = &fakemac_mdio_write;
	new_bus->reset = &fakemac_mdio_reset;
	snprintf(new_bus->id, 61, "%s-%x",
			new_bus->name, priv->bus_id);
	new_bus->priv = ndev;
	// phy addr mask 0~31: 0xfffffff0表示0，1，2，3.phyaddr有效
	new_bus->phy_mask = 0;

	priv->mii_bus = new_bus;

	/* register mdiobus */
	ret = mdiobus_register(priv->mii_bus);
	printk("mdiobus->state: %d\n", priv->mii_bus->state);
	if (ret < 0) {
		printk("mdiobus_register failed!\n");
		return ret;
	}
	return 0;
}

static int fakemac_driver_probe(struct platform_device *pdev)
{
	struct net_device *dev = NULL;
	struct plat_fakemac_data *pdata = NULL;
	struct physmap_flash_data *test;
	struct fakemac_priv *priv;
	int rc;

	printk("%s\n", __func__);
	pdata = dev_get_platdata(&pdev->dev);

	// for test
	test = (struct physmap_flash_data *)pdata;
	printk("test-name: %s\n", test->part_probe_types[0]);

	if (!pdata) {
		printk("Get platform data faild!\n");
		return -1;
	}

	dev = alloc_etherdev_mq(sizeof(*priv), FAKEMAC_QUEUES);
	if (!dev)
		return -ENOMEM;

	// 这两句必须要加 给driver unregitster提供对象
	SET_NETDEV_DEV(dev, &pdev->dev);
	platform_set_drvdata(pdev, dev);

	priv = netdev_priv(dev);
	priv->dev = dev;
	priv->pdev = pdev;

	dev->netdev_ops = &fakemac_netdev_ops;
	dev->ethtool_ops = &fakemac_ethtool_ops;

	/* carrier off reporting is important to ethtool even BEFORE open */
	netif_carrier_off(dev);

	rc = fakemac_mdio_register(dev);
	if (rc < 0) {
		printk("MDIO bus registration failed\n");
		return -1;
	}

	rc = register_netdev(dev);
	if (rc) {
		dev_err(&pdev->dev, "Could not register net device\n");
		return -1;
	}


	return 0;
}

static 	int fakemac_driver_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);

	fakemac_mdio_unregister(dev);
	printk("%s\n", __func__);
	unregister_netdev(dev);
	free_netdev(dev);

	return 0;
}

static struct platform_driver fakemac_driver =
{
	.driver = {
		.owner = THIS_MODULE,
		.name = "fakemac",
	},
	.probe = fakemac_driver_probe,
	.remove = fakemac_driver_remove,
};


static int fakemac_driver_init(void)
{
	printk("%s\n", __func__);
	return platform_driver_register(&fakemac_driver);
}
static void fakemac_driver_exit(void)
{
	printk("%s\n", __func__);
	platform_driver_unregister(&fakemac_driver);
	return;
}

MODULE_LICENSE("GPL");
module_init(fakemac_driver_init);
module_exit(fakemac_driver_exit);
