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
		temp = temp->next; }

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

#define MAXSIZE	8
struct stack {
	int *top;	//引用类型不占空间
	int *bottom;	//引用类型不占空间
	int depth;	//4 bytes
	/* 4 bytes, 但是如果你要访问整个数组
	 * 的话需要多分配一些空间
	 * ，不然就会出现跨界访问内存的问题 */
	int number[10];
};

struct stack *creat_stack(struct stack *s)
{
	/*(struct stack *) 表示强制转化为指针类型*/
	s = (struct stack *)malloc(sizeof(s) * MAXSIZE);

	return s;
}

static void free_stack(struct stack *s)
{
	if (!s)
		free(s);
}

static void insert_operation(struct stack *s, int n)
{
	/* 通过指针修改数组的值 */
	*(s->top) = n;
	s->top ++;
	s->depth = s->top - s->bottom;
}

static int stack_by_array(void)
{
	struct stack *s;
	int i;

	s = creat_stack(s);
	s->top = s->bottom = &s->number[0];

	for (i = 0; i< 11; i++) {
		insert_operation(s, i);
		/* --s->top 的原因是它先 ++ 了*/
		printf("addr:%p, val: %d\n", s->top, *(s->top - 1));
		if (s->depth > 10)
			break;
	}

	printf("stack_size: %ld\n", sizeof(s));
	printf("length: %ld\n", s->top - s->bottom);

	for (i = 0; i < 10; i++) {
		printf("number%d: %d\n", i, s->number[i]);
	}

	free_stack(s);

	return 0;
}

void main(void)
{
#if 0
	LinkList l;
	LinkList n;

	l = init_link_list(15);

	n = creat_node(n);
	n->data.score = 99;

	insert_node(l, n, 14);

	delate_node(l, 1);

	free_link_list(l);

	info_link_list(l);
#else
	stack_by_array();

#endif

	printf("Hello world!\n");
}
