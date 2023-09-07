// linux/mocos用户态下的C语言应用程序
// 作用: 查看函数指针的名字
#include <stdio.h>
#include <execinfo.h>
int test1(int a){
	printf("%s\n", __func__);
	printf("%d\n", a);
	return 0;
}

int test2(int b){
	return 1;
}

int main() {
	int (*p)(int a);
	p = test1 ;
	p(10);
	backtrace_symbols_fd((void *const)&p, 1, 1); 
}
