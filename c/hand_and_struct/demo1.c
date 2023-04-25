// 结构体指针还是什么指针不能直接赋值，需要间接引用赋值！！！
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10

struct pid {
	int kp;
	int ki;
	int kd;
	int (*pid_adj)(int);
};

struct phy_priv {
	struct pid pid;
};

struct mdio {
	char *name;
	int addr;
};

struct child {
	char *name;
	int age;
};

struct phy_device {
	char *name;
	int phy_addr;
	void *priv;
	struct mdio *mdio;
	struct child ch;
};

static int adj(int freq)
{
	printf("%s: %d\n", __func__, freq);
	return 0;
}

static void phy_init(struct phy_device *phydev)
{
#if 1
	//(指针类型)不能像pid_init一样整体赋值
	//但是取地址可以看child3_init
	phydev->name = "jlsemi";
	phydev->phy_addr = 1;
#else
	//结构体phy 在phy_init完后变量内存就释放了
	//这种方式是错误的
	struct phy_device phy = {
		.name = "phy",
		.phy_addr = 1,
	};
	phydev = phy;
#endif
}

static void child0_init(struct phy_device *phy)
{
	phy->ch.name = "child0";
	phy->ch.age = 18;
#if 0
	struct child cfg = {
		.name ="child0";
		.age = 16;
	};
	phy->ch = cfg;
#endif
}

static void child1_init(struct child c)
{
	//函数值传递通过return，或者传入指针
	//传指针需要小心
	struct child cfg = {
		.name ="child1",
		.age = 17,
	};
	c = cfg;
}

static void child2_init(struct child *c)
{
	c->name = "child2";
	c->age = 16;
}

static void child3_init(struct child *c)
{
	struct child cfg = {
		.name ="child3",
		.age = 15,
	};
	//*c是c指针指向地址的堆空间,按child格式解析
	*c = cfg;
}

struct mdio *mdio_init(void)
{
	struct mdio *mdio = malloc(MAXSIZE * sizeof(mdio));

	static struct mdio m = {
		.name = "phy",
		.addr = 1,
	};
	*mdio = m;
	return mdio;
}

void pid_init(struct phy_device *phydev)
{
#if 0
	struct phy_priv *priv = phydev->priv;
	struct pid  pid_cfg =  {
		.kp = 1,
		.ki = 3,
		.kd = 4,
		.pid_adj = adj,
	};
	priv->pid = pid_cfg;
#else
	struct phy_priv *priv = phydev->priv;
	struct pid *p = &priv->pid;
	//注意指针这里的赋值
	p->kp = 1;
	p->ki = 3;
	p->kd = 4;
	p->pid_adj = adj;
#endif
}

int main(void)
{
	struct phy_device *phy = malloc(MAXSIZE * sizeof(phy));
	//这里必须要分配内存才能使用, 不然是野地址
	struct phy_priv *priv = malloc(MAXSIZE * sizeof(priv));
	struct pid *p;

	phy->priv = priv;

	//1.phy_init
	phy_init(phy);
	printf("phy_name: %s\n", phy->name);

	//2.mdio_init这样赋值只适用于子结构体
	phy->mdio = mdio_init();
	printf("mdio_name: %s\n", phy->mdio->name);

	//3.pid_init
	pid_init(phy);
	p = &priv->pid;
	p->pid_adj(1);
	printf("kp: %d\n", p->kp);

	//4. child1 init
	child1_init(phy->ch);
	printf("child1 name: %s\n", phy->ch.name);
	printf("child1 age: %d\n", phy->ch.age);

	//5. child0 init
	child0_init(phy);
	printf("child0 name: %s\n", phy->ch.name);
	printf("child0 age: %d\n", phy->ch.age);

	//5. child2 init
	child2_init(&phy->ch);
	printf("child2 name: %s\n", phy->ch.name);
	printf("child2 age: %d\n", phy->ch.age);

	//6. child3 init
	child3_init(&phy->ch);
	printf("child3 name: %s\n", phy->ch.name);
	printf("child3 age: %d\n", phy->ch.age);
	
	free(phy->mdio);
	free(priv);
	free(phy);

}
