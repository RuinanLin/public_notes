# 练习9：数组和字符串　习题解答

## 整理人：林睿楠

* 将一些字符赋给`numbers`的元素，之后用`printf`一次打印一个字符，你会得到什么编译器警告？

	对ex9.c进行如下改动：

	```diff
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

2. 对`names`执行上述的相反操作，把`names`当成`int`数组，并一次打印一个`int`，`Valgrind`会提示什么？
