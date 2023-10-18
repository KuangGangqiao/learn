#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int rc;
	char buf[32];
	int fd = open("/dev/test_chr_dev", O_RDWR);
	if (fd < 0) {
		printf("open file failed!\n");
		return -1;
	}
	read(fd, buf, 32);
	printf("%s", buf);
	write(fd, "write test\n", 32);
	close(fd);
	return 0;
}

