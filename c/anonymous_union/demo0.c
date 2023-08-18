//1. 使用#把宏参数变为一个字符串,用##把两个宏参数贴合在一起.
//2. 当宏参数是另一个宏的时候，需要注意的是凡宏定义里有用’#’或’##’的地方宏参数是不会再展开.
//即， 只有当前宏生效, 参数里的宏！不！会！生！效 ！！！！
//3.解决这个问题的方法很简单. 加多一层中间转换宏. 
//加这层宏的用意是把所有宏的参数在这层里全部展开,
//那么在转换宏里的那一个宏(_STR)就能得到正确的宏参数.
//
#include<stdio.h>

#define A           2
#define _STR(s)     #s
#define STR(s)      _STR(s)          // 转换宏
#define _CONS(a,b)  (int)a##e##b
#define CONS(a,b)   _CONS(a,b)       // 转换宏

#define __stringify_1(x)	#x
#define __stringify(x)		__stringify_1(x)

#define PHY_STATE_STR(_state)			\
	case PHY_##_state:			\
		return __stringify(_state);	\

enum phy_state {
	PHY_DOWN = 0,
	PHY_READY,
	PHY_HALTED,
	PHY_UP,
	PHY_RUNNING,
	PHY_NOLINK,
};

static const char *phy_state_to_str(enum phy_state st)
{
	switch (st) {
	PHY_STATE_STR(DOWN)
	PHY_STATE_STR(READY)
	PHY_STATE_STR(UP)
	PHY_STATE_STR(RUNNING)
	PHY_STATE_STR(NOLINK)
	PHY_STATE_STR(HALTED)
	}

	return NULL;
}

int main(void)
{
	const char *str;
	enum phy_state state = PHY_RUNNING;


	str = phy_state_to_str(state);

	printf(__stringify(jojo\n));//把宏参数变为一个字符串
	printf("%d\n", CONS(2,3));  //把两个宏参数贴合在一起. 2e3 输出:2000
	printf("%d\n", CONS(A,A));  //把两个宏参数贴合在一起. 2e2 输出:200
	//printf("%d\n", _CONS(A,A));  //把两个宏参数贴合在一起. 2e2 输出:200
	printf("%s\n", str);


	return 0;
}
