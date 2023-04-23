// 结构体指针还是什么指针不能直接赋值，需要间接引用赋值！！！
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10

typedef int (*cmd_func_t)(int, char *[]);
struct cmd_t {
	const char *name;
	const cmd_func_t function;
};

static int do_set(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static int do_get(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static int do_adj(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static int do_freq(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static int do_cmp(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static int do_caps(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static int do_wait(int cmdc, char *cmdv[])
{
	printf("%s\n", __func__);
	return 0;
}

static const struct cmd_t all_commands[] = {
	{ "set", &do_set },
	{ "get", &do_get },
	{ "adj", &do_adj },
	{ "freq", &do_freq },
	{ "cmp", &do_cmp },
	{ "caps", &do_caps },
	{ "wait", &do_wait },
	{ 0, 0 }
};

static cmd_func_t get_command_function(const char *name)
{
	int i;
	cmd_func_t cmd = NULL;

	for (i = 0; all_commands[i].name != NULL; i++) {
		if (!strncmp(name,
			     all_commands[i].name,
			     strlen(all_commands[i].name)))
			cmd = all_commands[i].function;
	}

	return cmd;
}

int main(void)
{
	cmd_func_t get;
	char *c[] = {"xxx"};;
	get = get_command_function("get");
	
	get(1, c);

	return 0;
}
