#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
    int fd, ret;
    char data[20];

    fd = open("/dev/char_interrupt", O_RDWR);
    if (fd < 0)
    {
        printf("can't open!\n");
    }


    write(fd, "xiaolei_write", strlen("xiaolei_write"));

    ret = read(fd, data, 1);
    printf("ret = %d\n", ret);
    printf("read data is %s", data);

    close(fd);
    return 0;
}

