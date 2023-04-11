SSE 寄存器返回的报错信息为 “SSE disabled”。我们执行 make V=1 查看关键的编译信息
我们发现在 gcc 的参数中有 -mno-sse -mno-mmx -mno-sse2 选项，原来 gcc 默认的编译选项禁用了
sse、mmx、sse2 等浮点运算指令。
为了让内核支持浮点运算，我们在 Makefile 中添加支持 sse 等选项，源码中添加
kernel_fpu_begin/kernel_fpu_end 函数，

结合内核源码中 arch/x86/Makefile 中的 KBUILD_CFLAGS，可以看到编译内核及内核模块时，
gcc 选项继承 Linux 中的规则，指定了 -mno-sse -mno-mmx -mno-sse2，
也就是禁用了 FPU 。所以，要想内核模组支持浮点运算，编译选项需要显式的指定 -msse -msse2。

kernel_fpu_begin 和 kernel_fpu_end 也是必须的，因为 Linux 内核为了提高系统的运行速率，
在任务上下文切换时，只会保存/恢复普通寄存器的值，并不包括 FPU 浮点寄存器的值，
而调用 kernel_fpu_begin 主要作用是关掉系统抢占，浮点计算结束后调用 kernel_fpu_end 开启系统抢占，
这使得代码不会被中断，从而安全的进行浮点运算，并且要求这之间的代码不能有休眠或调度操作，
另外不得有嵌套的情况出现（将会覆盖原始保存的状态，然后执行 kernel_fpu_end() 最终将恢复错误的 
FPU 状态）。
