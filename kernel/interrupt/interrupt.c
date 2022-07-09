#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/fs.h>

#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/io.h>
#include <asm/irq.h>

#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/sched.h>


#define IMX_GPIO_NR(bank, nr)               (((bank) - 1) * 32 + (nr))
#define CYNO_GPIO_BEEP_NUM                  IMX_GPIO_NR(6,10)

int char_interrupt_major = 0;
dev_t char_devno;

struct char_interrupt_dev {
    struct cdev cdev;
};

static struct pin_desc {
    int irq;
    unsigned char *name;
    unsigned int pin;
};

static struct pin_desc beep_desc = {
    0,
    "beep_num",
    CYNO_GPIO_BEEP_NUM
};

struct char_interrupt_dev *char_interrupt_devp;
struct class *char_interrupt_class;
DECLARE_WAIT_QUEUE_HEAD(wq);  
static int condition = 0; 



static irqreturn_t beep_interrupt_handler(int irq, void *dev_id)
{
    printk("%s\n", __func__);
    condition = 1;
    wake_up_interruptible(&wq);
    return IRQ_HANDLED;
}


static int char_interrupt_open (struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "%s\n", __func__);
    return 0;
}

static ssize_t char_interrupt_read (struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    char *data = "button falling\n\n\n";
    condition = 0;
    //printk(KERN_INFO "%s\n", __func__);
    wait_event_interruptible(wq,condition);
    //printk(KERN_INFO "%s : %s\n", __func__, data);
    copy_to_user(buf,data,strlen(data));
    return strlen(data);
}

static ssize_t char_interrupt_write (struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    char data[30];
    //printk(KERN_INFO "%s\n", __func__);
    memset(data, 0, 30);
    copy_from_user(data, buf, count);
    printk(KERN_INFO "%s : data is %s\n", __func__, data);
    return strlen(data);
}

static struct file_operations char_interrupt_fops = {
    .owner = THIS_MODULE,
    .open  = char_interrupt_open,
    .read  = char_interrupt_read,
    .write = char_interrupt_write,

};

static int char_interrupt_init(void)
{
    int err, ret = -ENODEV;
    struct device *dev_temp;


    printk(KERN_INFO "%s\n", __func__);


    ret = alloc_chrdev_region(&char_devno, 0, 1, "char_interrupt");
    char_interrupt_major = MAJOR(char_devno);

    if(ret){
        printk(KERN_ERR "%s : chrdev_region fail\n", __func__);
        goto chrdev_region_fail;
    }

    char_interrupt_devp = kmalloc(sizeof(struct char_interrupt_dev), GFP_KERNEL);
    if(char_interrupt_devp == NULL){
        printk(KERN_ERR "%s : kmalloc is fail\n", __func__);
        goto kmalloc_fail;
    }

    memset(char_interrupt_devp, 0, sizeof(struct char_interrupt_dev));

    cdev_init(&char_interrupt_devp->cdev, &char_interrupt_fops);
    char_interrupt_devp->cdev.owner = THIS_MODULE;
    char_interrupt_devp->cdev.ops = &char_interrupt_fops;
    err = cdev_add(&char_interrupt_devp->cdev, char_devno, 1);
    if(err){
        printk(KERN_ERR "%s : cdev_add fail\n", __func__);
        goto cdev_add_fail;
    }

    //start create class point
    char_interrupt_class = class_create(THIS_MODULE, "char_interrupt");
    if(IS_ERR(char_interrupt_class)){
        printk(KERN_ERR "%s : class_create fail\n", __func__);
        goto class_create_fail;
    }

    dev_temp = device_create(char_interrupt_class, NULL, char_devno, NULL, "char_interrupt");
    if(IS_ERR(dev_temp)){
        printk(KERN_ERR "%s : device_create fail\n", __func__);
        goto device_create_fail;
    }

    //interrupt init
    if(gpio_request(beep_desc.pin ,beep_desc.name)){
        printk(KERN_ERR "%s : request gpio %d error\n", __func__, beep_desc.pin);
        goto err_gpio_request;
    }
    gpio_direction_input(beep_desc.pin);
    beep_desc.irq = gpio_to_irq(beep_desc.pin);
    printk(KERN_INFO "%s : the irq num is %d\n", __func__, beep_desc.irq);
    ret = request_threaded_irq(beep_desc.irq, NULL, beep_interrupt_handler , IRQF_ONESHOT | IRQF_TRIGGER_FALLING, beep_desc.name , &beep_desc);
    if(ret){
        printk(KERN_ERR "%s : request_irq is error\n", __func__);
        goto err_request_irq;
    }

    printk(KERN_INFO "%s : init end\n", __func__);

    return 0;

err_request_irq:
    free_irq(beep_desc.irq, &beep_desc);

err_gpio_request:
    gpio_free(beep_desc.pin);

device_create_fail:
    class_destroy(char_interrupt_class);

class_create_fail:
    cdev_del(&char_interrupt_devp->cdev);

cdev_add_fail:
    kfree(char_interrupt_devp);

kmalloc_fail:

chrdev_region_fail:
    unregister_chrdev_region(char_devno,1);
    return -1;

}

static void char_interrupt_exit(void)
{
    printk(KERN_INFO "%s\n", __func__);
    free_irq(beep_desc.irq, &beep_desc);
    gpio_free(beep_desc.pin);
    device_destroy(char_interrupt_class, char_devno);
    class_destroy(char_interrupt_class);
    cdev_del(&char_interrupt_devp->cdev);
    kfree(char_interrupt_devp);
    unregister_chrdev_region(char_devno,1);
}

module_init(char_interrupt_init);
module_exit(char_interrupt_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("linux char driver base");
MODULE_AUTHOR("xiaolei");

