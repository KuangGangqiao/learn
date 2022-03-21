#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char num[8];
	char name[8];
	int score;
} ElemType;

typedef struct LNode {
	ElemType data;
	struct LNode *next;
} Lnode, *LinkList;

LinkList creat_node(LinkList l)
{
	l = (LinkList)malloc(sizeof(Lnode));
	l->next = NULL;
	return l;
}

LinkList init_link_list(int length)
{
	LinkList head;
	LinkList temp;
	LinkList l;
	LinkList a;
	int i;

	head = creat_node(head);	//头节点
	a = creat_node(a);		//首元节点
	head->next = a;
	head->data.score = length;
	temp = a;			//指针用于遍历初始化

	for (i = 0; i < length; i++) {
		l = creat_node(l);
		temp->data.score = i;
		temp->next = l;
		temp = temp->next;
	}
	return head;
}

void info_link_list(LinkList l) {
	int score;
	int len;

	while (1) {
		score = l->data.score;
		l = l->next;
		printf("score: %d\n", score);
		if (l->next == NULL)
			break;
	}
}

int example ()
{
	int var_runoob = 10;
	int *p;
	p = &var_runoob;
	printf("var_runoob_addr： %p\n", p);
	return 0;
}


void main(void)
{
	LinkList l;

	l = init_link_list(15);

	info_link_list(l);

	printf("Hello world!\n");
}
