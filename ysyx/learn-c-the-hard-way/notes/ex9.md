# 练习9：数组和字符串　习题解答

## 整理人：林睿楠

* 将一些字符赋给`numbers`的元素，之后用`printf`一次打印一个字符，你会得到什么编译器警告？

	对ex9.c进行如下改动：

	```
	19,22c19,22
	< 	numbers[0] = 1;
	< 	numbers[1] = 2;
	< 	numbers[2] = 3;
	< 	numbers[3] = 4;
	---
	> 	numbers[0] = 'a';
	> 	numbers[1] = 'b';
	> 	numbers[2] = 'c';
	> 	numbers[3] = 'd';
	31c31
	< 	printf("numbers: %d %d %d %d\n",
	---
	> 	printf("numbers: %c %c %c %c\n",
	```

	然后进行编译，发现并未产生任何警告．

	在进行了相关文档的阅读之后，可以解释这两处改动都是符合规范的．

	首先，为什么将一个字符常量赋值给一个int型没有报错．在C17标准[ISO/IEC 9899:202x][1]第66页第10条提到：

	> An integer character constant has type **int**.

	这说明`'a'`, `'b'`, `'c'`, `'d'`天然就是int类型的．

	[1]: <https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2347.pdf> "ISO/IEC 9899:202x"

	其次，为什么使用`printf`将int型以`%c`的格式打印出来也是没有问题的．在`man 3 printf`文档对`%c`的介绍中是这么说的：

	> If  no  l modifier is present, the int argument is converted to an unsigned char, and the resulting character is written.

	这表明`%c`本来接收的也就是int型．

	至于为什么C语言这样设计，就要追溯C语言的发展历史了．<br><br>

* 对`names`执行上述的相反操作，把`names`当成`int`数组，并一次打印一个`int`，`Valgrind`会提示什么？

	首先对`ex9.c`进行如下的修改：

	```
	25,28c25,28
	< 	name[0] = 'Z';
	< 	name[1] = 'e';
	< 	name[2] = 'd';
	< 	name[3] = '\0';
	---
	> 	name[0] = 1;
	> 	name[1] = 2;
	> 	name[2] = 3;
	> 	name[3] = 4;
	35c35
	< 	printf("name each: %c %c %c %c\n",
	---
	> 	printf("name each: %d %d %d %d\n",
	38,40d37
	< 
	< 	// print the name like a string
	< 	printf("name: %s\n", name);
	```

	编译之后，编译器仍然没有警告，同时Valgrind也没有任何的警告或者报错．这是因为这些操作仍然是类型匹配的，更没有涉及到运行时的内存访问错误．<br><br>

* 有多少种其它的方式可以用来打印它？

	这个问题问得太模糊了，我只能按照我的不同的理解来解答．

	首先，可以使用`for`等循环结构，对每个元素进行遍历，来打印；其次，对于字符打印，可以使用`putchar`，对于字符串打印，可以使用`puts`；还可以选择其他的占位符，例如`%x`，用其他的方式打印．<br><br>

* 如果一个字符数组占四个字节，一个整数也占４个字节，你可以像整数一样使用整个`name`吗？你如何用黑魔法实现它？

	可以通过指针的类型转换来实现．

	```c
	#include <stdio.h>

	int main(int argc, char *argv[]) {
		char name[4] = {'a', 'b', 'c', 'd'};
		int* p = (int *)name;

		// read
		int read = *p;
		printf("Read out as an int: %d\n", read);
		printf("Turn it into hex: 0x%x\n", read);

		// write
		*p = 0x30313233;
		printf("Read out the written stuff: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

		return 0;
	}
	```

	运行结果如下所示：

	```
	Read out as an int: 1684234849
	Turn it into hex: 0x64636261
	Read out the written stuff: 3 2 1 0
	```

* 拿出一张纸，将每个数组画成一排方框，之后在纸上画出代码中的操作，看看是否正确．

	略．<br><br>

* 将`name`转换成`another`的形式，看看代码是否能正常工作．

	当然能，改动如下：

	```
	5c5
	< 	char name[4] = {'a'};
	---
	> 	char *name;
	12,17d11
	< 	printf("name each: %c %c %c %c\n",
	< 					name[0], name[1],
	< 					name[2], name[3]);
	< 
	< 	printf("name: %s\n", name);
	< 
	25,28c19
	< 	name[0] = 'Z';
	< 	name[1] = 'e';
	< 	name[2] = 'd';
	< 	name[3] = '\0';
	---
	> 	name = "Zed";
	```
