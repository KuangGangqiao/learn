1.编译：gcc -o xxx xxx.c
2.生成可调试文件：gcc -o xxx -g xxx.c
3.调试程序：gdb xxx
4.执行程序：
start 从程序第一步开始执行
run 直接运行程序到结束或者断点处
5.设置断点：break line(行数）或函数名或者条件表达式
break 6 在第6行设置断点
break Swap 在Swap函数入口设置断点
break 6 if i == 10 在第6行设置断点，要求i == 10
6.删除断点：
clear 删除所有断点
clear 行号 : 删除这行的断点
clear 函数名 : 删除该函数的断点
7.info 查看断点
8.c 继续执行到下一个断点
9.print +变量 打印变量值
10.n 下一步
11.q 退出调试
