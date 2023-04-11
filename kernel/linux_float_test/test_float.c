#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <asm/fpu/api.h>
#include <linux/delay.h>

static noinline double float_divide(double float1, double float2)
{
    return float1 / float2;
}

static int __init test_float_init(void)
{
  double result, float1 = 4.9, float2 = 0.49;

  kernel_fpu_begin();
  result = float_divide(float1, float2);
  kernel_fpu_end();
  printk("result = %d\n", (int)result);
  return 0;
}
static void __exit test_float_exit(void)
{
  ;
}
module_init(test_float_init);
module_exit(test_float_exit);
MODULE_LICENSE("GPL");
