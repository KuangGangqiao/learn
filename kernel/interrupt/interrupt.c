#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>

// 中断号
static int irq;

// 驱动程序名称
static char * devname;

// 用来接收加载驱动模块时传入的参数
module_param(irq, int, 0644);
module_param(devname, charp, 0644);

// 定义驱动程序的 ID，在中断处理函数中用来判断是否需要处理
#define MY_DEV_ID           1211

// 驱动程序数据结构
struct myirq
{
    int devid;
};

// 保存驱动程序的所有信息
struct myirq mydev  ={ MY_DEV_ID };

// 键盘相关的 IO 端口
#define KBD_DATA_REG        0x60
#define KBD_STATUS_REG      0x64
#define KBD_SCANCODE_MASK   0x7f
#define KBD_STATUS_MASK     0x80

// 中断处理函数
static irqreturn_t myirq_handler(int irq, void *dev)
{
    struct myirq mydev;
    unsigned char key_code;
    mydev = *(struct myirq*)dev;

    // 检查设备 id，只有当相等的时候才需要处理
    if (MY_DEV_ID == mydev.devid)
    {
        // 读取键盘扫描码
        key_code = inb(KBD_DATA_REG);

        /* 这里如果放开，每次按键都会打印出很多信息
        printk("key_code: %x %s\n",
                key_code & KBD_SCANCODE_MASK,
                key_code & KBD_STATUS_MASK ? "released" : "pressed");
        */

        // 判断：是否为 ESC 键
        if (key_code == 0x01)
        {
            printk("EXC key is pressed! \n");
        }
    }

    return IRQ_HANDLED;
}

// 驱动模块初始化函数
static int __init myirq_init(void)
{
    printk("myirq_init is called. \n");

    // 注册中断处理函数
    if(request_irq(irq, myirq_handler, IRQF_SHARED, devname, &mydev)!=0)
    {
        printk("register irq[%d] handler failed. \n", irq);
        return -1;
    }

    printk("register irq[%d] handler success. \n", irq);
    return 0;
}

static void __exit myirq_exit(void)
{
    printk("myirq_exit is called. \n");

    free_irq(irq, &mydev);
}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);

