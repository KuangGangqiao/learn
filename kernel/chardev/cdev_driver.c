#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kgq");

static dev_t devno;

#define KMAX_LEN 32

static int demo_open(struct inode *ind, struct file *fp)
{
	printk("demo open\n");
	return 0;
}

static int demo_release(struct inode *ind, struct file *fp)
{
	printk("demo release\n");
	return 0;
}

static ssize_t demo_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
	int rc;
	char kbuf[KMAX_LEN] = "read test\n"; 
	if (size > KMAX_LEN)
		size = KMAX_LEN;
	
	rc = copy_to_user(buf, kbuf, size);
	if(rc < 0) {
		return -EFAULT;
		pr_err("copy_to_user failed!");
	}
	return size;
}

static ssize_t demo_write(struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
	int rc;
	char kbuf[KMAX_LEN];
	if (size > KMAX_LEN)
		size = KMAX_LEN;

	rc = copy_from_user(kbuf, buf, size);
	if(rc < 0) {
		return -EFAULT;
		pr_err("copy_from_user failed!");
	}
	printk("%s", kbuf);
	return size;
}

static struct file_operations fops = {
	.open = demo_open,
	.release = demo_release,
	.read = demo_read,
	.write = demo_write,
};

static struct cdev cd;

static int demo_init(void)
{
	int rc;
	rc = alloc_chrdev_region(&devno, 0, 1, "test");
	if(rc < 0) {
		pr_err("alloc_chrdev_region failed!");
		return rc;
	}
	printk("MAJOR is %d\n", MAJOR(devno));
	printk("MINOR is %d\n", MINOR(devno));

	cdev_init(&cd, &fops);
	rc = cdev_add(&cd, devno, 1);
	if (rc < 0) {
		pr_err("cdev_add failed!");
		return rc;
	}
	return 0;
}

static void demo_exit(void)
{
	cdev_del(&cd);
	unregister_chrdev_region(devno, 1);
	return;
}

module_init(demo_init);
module_exit(demo_exit);

