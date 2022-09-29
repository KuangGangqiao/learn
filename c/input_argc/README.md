argc,argv 用命令行编译程序时有用。
主函数main中变量(int argc,char *argv[ ])的含义
有些编译器允许将 main() 的返回类型声明为void，这已不再是合法的C++
main(int argc, char *argv[ ], char **env)才是UNIX和Linux中的标准写法。
argc: 整数,用来统计你运行程序时送给main函数的命令行参数的个数
* argv[ ]: 指针数组，用来存放指向你的字符串参数的指针，每一个元素指向一个参数
argv[0] 指向程序运行的全路径名
argv[1] 指向在DOS命令行中执行程序名后的第一个字符串
argv[2] 指向执行程序名后的第二个字符串
...
argv[argc]为NULL。
**env:字符串数组。env[ ]的每一个元素都包含ENVVAR=value形式的字符
串。其中ENVVAR为环境变量，value 为ENVVAR的对应值。
argc, argv, env是在main( )函数之前被赋值的(下面例子有体现)，
编译器生成的可执行文件，main()不是真正的入口点，
而是一个标准的函数,这个函数名与具体的操作系统有关。

假设将其编译为 args_test.o
在命令行下
./args_test.o  test hello
得到的输出结果为
3
hello
test
./args_test.o
main(int argc, char* argv[ ])，其中argc是指变量的个数，
本例中即指test和hello这两个变量和程序运行的全路径名或程序的名字，argc即为3。
argv是一个char *的数组，其中存放指向参数变量的指针
，此处argv[0]指向test.exe的全路径名或test.exe，argv[1]指向test，argv[2]指向hello。
