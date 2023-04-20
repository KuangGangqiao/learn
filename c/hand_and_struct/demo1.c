// 结构体指针还是什么指针不能直接赋值，需要间接引用赋值！！！
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10

struct pid {
	int kp;
	int ki;
	int kd;
};

struct phy_priv {
	struct pid pid;
};

struct phy_device {
	char *name;
	int phy_addr;
	void *priv;
};

void phy_init(struct phy_device *phydev)
{
	//指针类型不能像pid_init一样整体赋值
	phydev->name = "jlsemi";
	phydev->phy_addr = 1;
}

void pid_init(struct phy_device *phydev)
{
#if 0
	struct phy_priv *priv = phydev->priv;
	struct pid  pid_cfg =  {
		.kp = 1,
		.ki = 3,
		.kd = 4,
	};
	priv->pid = pid_cfg;
#else
	struct phy_priv *priv = phydev->priv;
	struct pid *p = &priv->pid;
	//注意指针这里的赋值
	p->kp = 1;
	p->ki = 3;
	p->kd = 4;
#endif
}

int main(void)
{
	struct phy_device *phy = malloc(MAXSIZE * sizeof(phy));
	//这里必须要分配内存才能使用, 不然是野地址
	struct phy_priv *priv = malloc(MAXSIZE * sizeof(priv));
	struct pid *p;
	phy->priv = priv;
	phy_init(phy);
	pid_init(phy);
	printf("name: %s\n", phy->name);
	printf("kp: %d\n", priv->pid.kp);
	p = &priv->pid;
	printf("kp: %d\n", p->kp);

	free(priv);
	free(phy);

}
