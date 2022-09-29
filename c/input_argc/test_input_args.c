#include <stdio.h>

int main(int argc, char* argv[])
{
	printf("%d\n",argc);
	while(argc)
		printf("%s\n",argv[--argc]);

	printf("========================\n");
	int i, n = 0;
	for(;n<=(26+i);n++)
		printf("argv_%d=[%s]\n",n,argv[n]);
	return 0;
}
