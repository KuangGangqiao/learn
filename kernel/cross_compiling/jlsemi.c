// SPDX-License-Identifier: GPL-2.0+
/*
 * drivers/net/phy/jlsemi.c
 *
 * Driver for JLSemi PHYs
 *
 * Author: Gangqiao Kuang
 *
 * Copyright (c) 2020 JingLue Semiconductor, Inc.
 */
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


#define JLSEMI_PHY_ID	0x937c4032

#define MII_JLSEMI_PHY_PAGE     31
#define MII_JLSEMI_COPPER_CONTROL       0x00

MODULE_DESCRIPTION("JLSemi PHY driver");
MODULE_AUTHOR("Gangqiao Kuang");
MODULE_LICENSE("GPL");


static int jlsemi_soft_reset(struct phy_device *phydev)
{
	return 0;
}

static int jlsemi_config_aneg(struct phy_device *phydev)
{
	int err;
	err = genphy_config_aneg(phydev);
	
	return (err < 0) ? err : 0;
}

static int jlsemi_config_init(struct phy_device *phydev)
{
	return 0;
}

static int jlsemi_read_status(struct phy_device *phydev)
{
	printk("%x", phydev->phy_id);
	return genphy_read_status(phydev);
}

static int jlsemi_probe(struct phy_device *phydev)
{
	return 0;
}

static int jlsemi_aneg_done(struct phy_device *phydev)
{
	return 0;
}

static int jlsemi_set_loopback(struct phy_device *phydev, bool enable)
{
	return 0;
}

static struct phy_driver jlsemi_driver[] = {
{
        .phy_id         = JLSEMI_PHY_ID,
        .name           = "JLSemi JL2x11",
        .phy_id_mask    = 0xfffffff0,
        /* PHY_BASIC_FEATURES */
	.soft_reset	= jlsemi_soft_reset,
        .probe		= jlsemi_probe,
	.features	= PHY_GBIT_FEATURES | SUPPORTED_MII |
			  SUPPORTED_AUI | SUPPORTED_FIBRE |
			  SUPPORTED_BNC,
	.aneg_done	= jlsemi_aneg_done,
	.read_status	= jlsemi_read_status,
	.set_loopback	= jlsemi_set_loopback,
	.config_init    = jlsemi_config_init,
        .config_aneg    = jlsemi_config_aneg,
        .suspend        = &genphy_suspend,
        .resume         = &genphy_resume,
	.driver»»       = { .owner = THIS_MODULE,},
}};

module_phy_driver(jlsemi_driver);

static struct mdio_device_id __maybe_unused jlsemi_tbl[] = {
        { JLSEMI_PHY_ID, 0xfffffff0 },
        { }
};

MODULE_DEVICE_TABLE(mdio, jlsemi_tbl);

