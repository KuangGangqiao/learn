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
	head->data.score = length;	//头节点储存链表长度
	temp = a;			//指针用于遍历初始化

	for (i = 0; i < length; i++) {
		l = creat_node(l);
		temp->data.score = i;
		temp->next = l;
		temp = temp->next;
	}
	return head;
}

LinkList insert_node(LinkList head, LinkList new, int index)
{
	LinkList temp;
	int i;

	if (index > head->data.score - 1 || index < 0) {
		printf("Insert node to linklist failed! Check index!\n");
		/* 返回原链表 */
		return head;
	}

	temp = head;

	for (i = 0; i < head->data.score; i++) {
		temp = temp->next;
		if (i == index) {
			new->next = temp->next;	//保护现场
			temp->next = new;
			temp = new;
		}
	}

	return head;
}

LinkList delate_node(LinkList head, int index)
{
	LinkList temp;
	LinkList n;
	int i;

	temp = head;
	for (i = 0; i < head->data.score; i++) {
		if (i == index) {
			n = temp->next;
			temp->next = n->next;
			free(n);
		}
		temp = temp->next;
	}

	return head;
}

void free_link_list(LinkList head)
{
	LinkList temp;
	LinkList n;
	int i;

	temp = head;

	for (i = 0; i < head->data.score; i++) {
		n = temp->next;
		temp->next = n->next;
		free(n);
	}

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

int example(void)
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
	LinkList n;

	l = init_link_list(15);

	n = creat_node(n);
	n->data.score = 99;

	l = insert_node(l, n, 14);

	delate_node(l, 1);

	free_link_list(l);

	info_link_list(l);

	printf("Hello world!\n");
}
