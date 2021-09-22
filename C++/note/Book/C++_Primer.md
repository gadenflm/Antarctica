[TOC]
# C++ Primer 笔记

## Chapter 1

### 面向对象 和 面向过程

将计算机程序理解成 算法 + 数据。

C 面向过程编程 可以理解成更强调程序算法(过程)方面，__使用结构化编程，思考方式自顶向下：将大型程序需要解决的任务分解成小型的，易于编写的模块。__

C++ 面向对象编程 可以理解成更强调程序的数据，__理念上追求设计出与任务本质特性相对应的数据格式，即对象。 面向对象的思考出发点首先是设计对象(class), 让类可以准确表示程序要处理的东西。__ 最后逐步设计一个使用这些类的程序。  __思考方式为自下向上。__

### 泛型编程

面向对象编程强调的是数据方面，__泛型编程则是想独立于特定数据类型，使得一个泛型函数可以用于各种数据类型，C++ 模板(template)提供这一机制__

## Chapter 2 ~ 3 基础

### 随记

```CPP
cout << "Hello World!"
	 << endl;
```
`cout` 为 `iostream` 头文件中定义的`ostream`类(class)的对象. 其重载运算符 `<<`, 从而将 `<<` 右侧信息插入到输出流中. 
`endl` 为 `iostream` 头文件中定义的控制符(manipulator), 位于名称空间(namespaces) `std` 中.

---

```CPP
int a,b,c;
a = b = c = 726;
```
同C, 赋值从右向左进行 并 可以连续赋值。

---

```CPP 
int var{7};   		// set var to 7
int array[26] = {};	// set all elements of array to 0
int array[26]{};
```
C++ 使用大括号给变量赋予初始化值时可以省略等号, 同时可以省略大括号里的内容(此时初始化为0).

---

```CPP
std::cout << std::hex;
cout << "Hex Num: 0x" << 0x42 << endl;
```
对cout对象的输入控制符`std::hex`, 并不会输出任何内容, 而是修改`cout`显示整数的方式

同理有 `std:dec` `std:oct` `std:hex`. 因此`using namespace std;` 后, 不能使用变量名`hex` `oct` `dec`.

---

```CPP 	
cout << A;	  		// show 65 in console;
cout.put('A');		// show A in console;
```
cout 默认输出char类型会输出其整数值即ASCII码. 单独输出字符可使用 ostream 类的成员函数 `put()`

---

__bool 类型的字节大小为 1, 任何数值类型的量都可以被隐式转换为 bool 类型.__

---



### C++ 常量(字面量)类型确定

* 整形常量默认类型为int, 仅当太大或者有特殊后缀时会理解为其他类型。
* 后缀:
	- `l L` 表示该整数为 long 类型常量
	- `u U` 表示该整数为 unsigned 类型常量

* 浮点常量默认类型为 double, 改为 float 或 long double 则需要使用后缀 `f F` `l L`

举例 :
```CPP 
//  unsigned long long int:
uint64_t var = 19970726ULL;
```

### 通用字符名(多字节字符)

针对独立于键盘的其他字符,C++引入特殊字符机制。对应ISO 10646 码点, 其通过转义符 `\u` 代表2字节字符 `\U` 代表4字节字符。 

``` CPP 
int k\u00F6rper; 							// C++存储变量名为 uǎrper
cout << "Let's eat g\u00E2teau\n";			// will show "Let's eat güteau"
```
如注释, `\u`后的四个16进制数会被解析成对应字符, 并用于c++ 存储或输出中

__ISO10646 正常情况下对应于 Unicode编码__

__C++11新增变量类型 `char16_t` 对应 `\u` `\U` 的双字节和四字节字符, 举例如下__
```CPP
char16_t ch1 = u'q';
char32_t ch2 = U'\U0000222A';
```
### 隐式类型转换相关

1. C++ 允许将一种类型的值赋给另一种类型的变量, 值将被转为接收的类型

* 长浮点类型(double) 转为 浮点(float), 若超出范围会导致结果不确定
* 浮点(float) 转为 整型, 若超出范围会导致结果不确定
* 长整型(long) 转为 整型(int), 只复制右边的字节 

__使用大括号`{}`进行变量初始化, 会保证左值右值的字节数一样. 可用于避免许多隐式类型转换__

```CPP
char ch = {65};
```
2. C++ 计算表达式中 存在自动类型转换.

* __int 为计算机默认计算最自然的类型, 所有小于 int 字节数的 整型 字符类型等等都会被转换为 int/ unsigned int 参与运算, 这一过程称为 整型提升(integral promotion)__
* C++ 11 的校验表
	- (1) 双目运算 存在类型 long double, 另一个数 自动转为 long double
	- 否则(2) 双目运算 存在类型 double, 另一个数 自动转为 double
	- 否则(3) 双目运算 存在类型 float, 另一个数 自动转为 float
	- 否则(4) 双目运算 两侧全为 整型, 整型提升
	- ...(有无符号位类型转换)

3. 传递参数时, __类型转换通常由 c++ 函数原型控制.__ 若取消原型对参数的控制, 则 C++ 会引入整型提升, 同时将 float 转为 double 传入.

### 强制类型转换


