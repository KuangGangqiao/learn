// 结构体指针还是什么指针不能直接赋值，需要间接引用赋值！！！
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10

static void dog_voice(int num)
{
	int i;

	for (i = 0; i < num; i++)
		printf("wang wang wang!!!\n");
}

static void cat_voice(int num)
{
	int i;

	for (i = 0; i < num; i++)
		printf("miao miao miao!!!\n");
}

struct dog {
	char *name;
	int age;
	int wigth;
	void (*voice)(int);// int 后面的形参数可以省略
};

struct cat {
	char *name;	//8byte,对于64位机器，指针都是8byte,无论什么类型
	int age;	//4byte
	int wigth;	//4byte
	void (*voice)(int num);	//8byte
};

struct pet {
	struct dog *my_dog;
	struct cat my_cat;
};

void pet_init(struct pet *p)
{
	//分配堆空间
	p->my_dog = malloc(sizeof(p->my_dog));

	//init dog
	struct dog dog_cfg = {
		.name = "TaiDi",
		.age = 2,
		.wigth = 8,
		.voice = dog_voice,
	};
	//因为是指针，所以间接引用赋值
	p->my_dog = &dog_cfg;
	//init cat
	//因为是常结构体，所以可以直接赋值
	//p->my_dog->name = "Tom"; 这样赋值会段错误
	p->my_cat.name = "Tom";
	p->my_cat.age = 3;
	p->my_cat.wigth = 10;
	p->my_cat.voice = cat_voice;
}

static void exce_voice(struct dog *d)
{
	d->voice(4);
}

int main(void)
{
	struct pet *my_pet = malloc(MAXSIZE * sizeof(my_pet));

	pet_init(my_pet);

	printf("pet size: %ld\n", sizeof(my_pet));
	printf("dog size: %ld\n", sizeof(my_pet->my_dog));
	printf("cat size: %ld\n", sizeof(my_pet->my_cat));

	printf("dog_name: %s, addr: %p\n", my_pet->my_dog->name, my_pet->my_dog->name);
	printf("dog_age: %d, addr: %p\n", my_pet->my_dog->age, &my_pet->my_dog->age);
	printf("dog_wigth: %d, addr: %p\n", my_pet->my_dog->wigth, &my_pet->my_dog->wigth);
	printf("dog_voice(): addr: %p\n", &my_pet->my_dog->voice);
	//exce_voice(my_pet->my_dog);
	my_pet->my_dog->voice(4);
	printf("dog_name: %s, addr: %p\n", my_pet->my_dog->name, my_pet->my_dog->name);
	printf("dog_age: %d, addr: %p\n", my_pet->my_dog->age, &my_pet->my_dog->age);
	//这里有问题
	printf("dog_wigth: %d, addr: %p\n", my_pet->my_dog->wigth, &my_pet->my_dog->wigth);
	printf("dog_voice(): addr: %p\n", &my_pet->my_dog->voice);
	//exce_voice(my_pet->my_dog);
	my_pet->my_dog->voice(4);
	printf("cat_name: %s\n", my_pet->my_cat.name);
	printf("cat_age: %d\n", my_pet->my_cat.age);
	printf("cat_wigth: %d\n", my_pet->my_cat.wigth);
	printf("cat_voice(): %p\n", &my_pet->my_cat.voice);
	my_pet->my_cat.voice(3);
	printf("cat_name: %s\n", my_pet->my_cat.name);
	printf("cat_age: %d\n", my_pet->my_cat.age);
	printf("cat_wigth: %d\n", my_pet->my_cat.wigth);
	printf("cat_voice(): %p\n", &my_pet->my_cat.voice);
	my_pet->my_cat.voice(3);

	//my_pet->my_dog->wigth =  my_pet->my_cat.age - my_pet->my_dog->age;
	printf("test: %d\n", my_pet->my_dog->wigth);

	//free(my_pet->my_dog);
	free(my_pet);

}
