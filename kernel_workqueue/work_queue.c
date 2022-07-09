/*
 * Deferred Work
 *
 * Exercise #1, #2: simple timer
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_DESCRIPTION("Simple kernel timer");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

static struct delayed_work ms_queue_work;

void work_func(struct work_struct *work)
{
	printk("hello work queue\n");
	/* If you want to call the handler all
	 * the time, put it in the handler */
	schedule_delayed_work(&ms_queue_work, 250);
}

static int __init work_queue_init(void)
{
	printk("work queue init!\n");
	INIT_DELAYED_WORK(&ms_queue_work, work_func);
	schedule_delayed_work(&ms_queue_work, 250);

	return 0;
}

static void __exit work_queue_exit(void)
{
	printk("work queue exit!\n");
	cancel_delayed_work_sync(&ms_queue_work);
}

module_init(work_queue_init);
module_exit(work_queue_exit);

