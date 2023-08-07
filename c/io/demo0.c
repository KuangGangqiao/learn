//编写程序，将字符串"hello"写入a.txt                 writefile.c
#include <stdio.h>
#include<string.h>

#include <sys/types.h>  //调用open()函数所需的头文件
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>     //调用wirte()函数所需的头文件

int main()
{	
	int fd;
	char *buf="hello";  //定义一个20字节大小的数组（数据缓冲区），用于存放要写入的数据

	//打开文件
	fd = open("./a.txt",O_RDWR);  //读写方式打开，返回一个文件描述符，后面根据这个文件描述符对文件进行操作
	if(fd == -1)
	{
		printf("open a.txt faild\n");
	}
	else
	{
		
		printf("open a.txt ok\n");
	}

	//写入数据
	int ret = write(fd, buf, strlen(buf));  //将数据从缓冲区中写入fd指向的文件
	if(ret == -1)
	{
		
		printf("write a.txt faild\n");
	}
	else
	{	
		printf("write a.txt ok\n");
	}

	return 0;

}

