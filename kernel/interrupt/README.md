
https://cloud.tencent.com/developer/article/1917595

介绍：这个linux中断程序实现了按Esc键的共享中断

**Test**
Step1:
``
cat /dev/char_interrutp

``
Step2:
``
sudo insmod interrupt.ko irq=1 devname=myirq

``
