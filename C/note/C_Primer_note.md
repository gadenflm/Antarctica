[TOC]

#C-Primer 书本整理

###C-Primer Chapter.1-8

`#include`
把目标文件的所有内容输入到该行所在位置

`#define`
编译时替换，(参照 Charpter 16, 不全是 直接替换)

__把浮点数转为整数，会直接丢弃小数部分__

中字符串为字符数组，末尾字符为'\0', 即空字符null,
__末尾不是'\0'也是字符数组，但不是字符串__

scanf 会返回成功读取到的项数，失败则返回0，检测到“文件结尾”（ or 其他等价情况），返回EOF
stdio.h 中 #define EOF -1

__goto 应用场景：出现问题时从一组嵌套循环中跳出(break 只影响一层循环)__


###C-Primer Chapter.9 函数

__如果返回值的类型与申明的返回类型不匹配，返回值将被转换成函数申明的返回类型__


###C-Primer Chapter.10 数组与指针
数组是派生类型，即建立在其他类型基础上的类型！！！！

__const定义的数组为只读类型，只能从数组中检索值，不能把新值写入数组，一般使用时必须初始化值。__

__配合函数申明：__
```C
int sum(const int arr[], int n)
// or
int sum(const int* arr, int n)
```
__这种申明形参方式，使得函数处理该变量时将其视为常量，使得该函数处理过程中不能改变其中指针指向变量的值。__

```C
const int * pOne;    //指向整形常量的指针，它指向的值不能修改,但可以指向其他整形
int * const pTwo;    //指向整形的常量指针，它不能在指向别的变量，但指向（变量）的值可以修改。
```

数组初始化使用` {31, 28, 31, 30}`，__当初始化列表中的值少于数组元素个数时，剩余元素自动补0.__

C不允许把数组作为一个单元赋给其他数组！
除了初始化以外也不允许使用花括号列表的形式赋值。

__二维数组是按顺序存储的__

！！！！！
int * pax[2]
[] 优先级最高，先与pax结合，pax被理解成含有两个元素的数组， 接着\*, pax 被理解成含有两个指针的数组。
最后 int, pax 最后变为含有两个指向int指针的数组。

int (* pax)[2]
`*`先与pax结合，pax 为一个指针，接着[]表明该指针指向一个含有两个元素的数组，最后int表明数组类型是int。
即pax指向一个数组的指针，存储数组首元素地址，用于多维数组！！！！


###C-Primer Chapter.11 字符串

__字符串常量（字面量） 为静态存储(staic storage),
如果在函数内使用字符串常量，则只会被存储一次，在整个程序生命周期内存在。__

即用双引号括起来的内容被视为指向该字符串存储位置的指针。
e.g. `*"Hello World!" `, `*`为解引用运算符，返回指针中存放的元素，
指针指向"Hello World!"首元素，所以返回'H'.

__`char* str = "Hello World!"` 与 `char str[] = "Hello World!"` 的区别：__

先分析数组情况(`char str[]`), 程序载入内存时，先将字面量"Hello World"存储在静态存储区, 程序开始运行时，为数组分配内存，在内存中开辟对应大小空间并依次存储字符。 "Hello World"有两个副本。__且此时str 为指针常量，及数组首元素地址，可以执行str + 1 但不能执行str++__(`++`为单目运算符，只能作用于变量。)

而指针情况(`char* str`)，程序载入内存时，同样也将字面量"Hello World"存储在静态存储区, 但程序开始运行时只为指针变量 str开辟一个存储空间并将字面量的首地址存储在指针变量中。"Hello World"只有一个副本。__但是，编译器会用内存中同一个副本表示所有完全相同的字符串字面量，导致对字符指针内容的更改会影响到程序内其他字面量的值！！__

__所以，绝大多数情况下，指针型字符申明使用`const char* str`的方式__，程序过程中不更改其中的值。需要更改时，可申明字符数组`char []`。

严重错误：(未初始化的指针)
```C
char* name;
scanf ("%s", name);
```
name 初试化时指向一个随机地址，读入时可能会擦除程序中其他的数据或者代码！

从终端读入一行字符串使用`fgets(char* buffer, int MaxLength, stdin)`, 会存储末尾换行符。

`scanf("n%s", char* )`

读取非空白字符间的字符串，n用来指定字段宽度，超出时自动舍弃，防止读入时内存溢出(申明的数组长度小于输入长度)！

##### 字符串函数

* `strlen (char* )` 返回字符串长度, 读到`\0`返回
* `strcat (char* , char*)` 拼接字符串，存储在参数1串上。__参数1空间不够时会导致内存溢出__
* `strncat(char* , char* , int)` 第三个参数指定在参数1上添加的最大字符数，超过自动停止。
* `strcmp(char* , char*)` 比较两字符串，相同返回0。 __不使用指针比较，两指针输入后会比较指针存储地址是否相同，地址不同则返回非0__
* `strncmp(char* , char* , int)` 同理，仅比较前n个字符。
* __`strcpy(char*, char*)`__ 相当于字符串赋值，先开辟空间，把参数2依次给参数1。直接赋值为传递地址。返回为第一个参数的地址。
* `strncpy(char*, char*, int)` 同理，防止内存溢出
* __`sprintf(char*, ...)`__ 同printf，但组合的字符串存于参数1中。__(常用)__
* `strchr(char*, char)` 查找字符在字符串中出现的位置。
* `strstr(char*, char*)` 查找字符串在字符串中出现的位置。

-- -

###C-Primer Chapter.12 存储类别、链接、内存管理

局部变量的作用域在内可以访问，作用域外不行。例如
```
for (int i = 0; i < n; i++) s += i;
```

`i`定义在for 循环中，虽没有大括号注明的块，但C编译器自动识别for 循环体为块内容，循环体之外均不能访问变量i。

	__auto在C和C++中用法完全不同且不相关！！C / C++兼容程序不使用auto__

	寄存器变量申明：
	`register int quick; `
该变量存储在CPU寄存器中，访问速度远快于普通变量(存储在内存中), 但数量有限，超过上限申明时自动转为普通变量，且寄存器变量没有地址，无法使用指针和取地址运算！！

__static__ 静态存储:

* 作用于块内(局部变量)时，程序在运行周期内保留其空间，但只有在执行其块内代码时可以访问。如果局部变量不使用static，那么块语句执行结束后则立马释放空间。
* 作用于文件(单文件全局变量)区域时，只有本文件内的代码可以访问该变量，不能作为多文件共享使用。

__extern__ 申明外部变量：

多文件中，除了定义式申明外，其他申明都要使用extern关键字。达到多文件全局变量的效果。

函数申明的存储类别：

* extern 申明定义在其他文件中的函数。
* static 申明的函数属于特定模块私有，往往仅能被本文件访问。避免多文件函数重名问题。

##### 内存分配：

`void*` 通用指针，可以指向任何类型。

`malloc( Size )` 返回`void*`类型，在运行时开辟相应大小空间，然后使该指针指向该空间。失败则返回NULL。
e.g. `double* p = (double *) malloc (30 * sizeof(double)); `

__程序运行中malloc开辟的内存占内存池空间后，无法被其他过程访问，所以使用完必须用`free(); 即 free(PointVarName); `释放，否则可能造成内存泄漏(重大错误)__

<font color = "red" size = 4>
静态存储类别(static)，所开辟的空间大小在编译时已经确定。程序开始执行时即被创建，程序执行过程中，相关代码块可以访问该部分内存。程序结束后释放。
__所以C在初始化静态变量和全局变量时，右值大小必须是常量！__

自动存储类别变量在程序进入变量定义所在块时开辟内存空间，程序离开块时内存释放，随着程序进行，使用的内存数量随之变化！ __这部分的内存通常作为栈来处理，按顺序开辟内存后销毁!__

`malloc() calloc() free()` 属于程序员自己管理的内存块，即动态分配的内存，这部分内存的创建和销毁由程序员自己分配。__结果是内存使用支离破碎，未使用的内存分散在已使用的内存块之间。所以访问动态内存往往比栈内存要慢!__
</font >

头文件定义全局常量:
`static const PI = 3.141592653689793`

##### volatile

`volatile int time; `
__volatile 申明的变量告诉计算机，除本程序之外的其他过程也能访问该变量！__

用处：
1. 硬件地址
2. 多程序共享数据
3. 多线程通信

__个人理解：__
高速缓存(caching)举例:
```C
var1 = x;
... //大段代码,不涉及x！
var2 = x;
```
程序编译时优化: 执行完`var1 = x`的赋值后，临时把x的值存在寄存器内，而不是内存中！(var1 = x时CPU已经读取内存, 访问了该值)，var2使用x时，从寄存器(不是原始内存位置)上读取x的值，节约时间。

理论上高速缓存是个非常有效的优化方案，但当这时其他代理(本程序之外的其他过程)改变了内存中该值，则程序错误。因此需要有一个申明告诉程序，该值不能使用高速缓存，即volatile。若申明变量无volatile，则程序在编译时会尝试优化。

##### restrict

`int* restrict p_int`

restrict 只能用于申明指针，__表明该指针是访问此数据的唯一且初始的方式，此申明仅用于编译器优化使用，同时提醒开发者使用时满足这一要求。 编译过程不会检查是否遵循这一限制，无视它编译器优化后的程序出错后果自负！__

##### `_Atomic`

C11 新标准，只能通过宏定义访问，原子类用于多线程并发处理。当一个线程对一个原子类型执行原子操作时，其他线程不能访问该内存。

---

### C-Primer Chapter.13 文件I/O

C一律把数据看成是字节构成的序列，即字节流，对文件的存取也是以字节为单位的，所以说C语言文件为流式文件。 C11前有 两种常规文件格式:

* 文本文件, 将数值型数据的每一位作为一个字符以其ASCII码的形式存储，因此，文本文件中的每一位数字都单独占用一个字节的存储空间
* 二进制文件则是把整个数字作为一个二进制数存储的，并非数值的每一位数字都占用单独的存储空间

例如 `12345` 通过两种方式分别存取

* 文本文件
'1' '2' '3' '4' '5' 分别转成二进制码存入
`00110001 00110010 00110011 00110100 00110101`

* 二进制文件
假设数据类型为 双字节(`uint16_t`), 16位存放对应二进制码
`00110000 001110001`

#### 标准流 stdin stdout stderr

三个标准文件本质都是`FILE * ` 类型指针，三个文件在底层分别对应如下

* `stdin` 键盘
* `stdout` 显示器
* `stderr` 显示器

#### 标准库接口整理(`stdio.h`)

##### `fopen()`

```C
FILE * fopen(const char *filename, const char *mode);
// example:
FILE * fp = fopen ("sample.cfg", "w+")
```

返回文件类型指针，__封装了与文件有关的信息，如文件句柄、位置指针及缓冲区等, 缓冲文件系统为每个被使用的文件在内存中开辟一个缓冲区__

`const char *mode`:

* `r` 以只读的方式打开文件（该文件必须已经存在，若文件不存在，则会出错）
* `w` 以只写的方式打开文件，__若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件__
* `a` 以只写的方式打开文本文件，位置指针移到文件末尾，向文件尾部添加数据，原文件数据保留，__若文件不存在则会出错__
* ` +` 与上面的字符串组合，表示以读写的方式打开文本文件，既可向文件中写入数据，也可从文件中读出数据
* `b` 与上面的字符串组合，表示打开二进制文件
* `t` (无需添加，为默认属性)与上面的字符串组合，表示打开文本文件

##### `fclose()`

`　int　fclose(FILE* )`

成功返回0，失败返回EOF.

##### `getc()` `putc()`

从数据流中读取一个字符，文件结尾返回特殊标识符EOF.

```C
	ch = getc (fpin);
	putc (ch, fpout);

	while ( (ch = getc(fp)) != EOF) {};
```

##### `fprintf()` `fscanf()`

将自己制定的`FILE*`类型指针作为第一个参数。其余与`printf` `scanf` 一致.


##### `fgets()` `fputs()`
`fgets()` 读取文件流直到第一个换行符或EOF。 去除最后的换行符后添加 `\0` 使得返回字符串。

`fputs` 输出一行，结尾不会自动添加`\n`

##### `fseek()` `ftell()` `fgetpos()` `fsetpos()`

用来定义数据流的偏移量。 在此仅简单举例
```C
fseek (fp, 10L, SEEK_SET) //文件开头10字节处
fseek (fp, 2L, SEEK_CUR) //文件当前位置移动2个字节
fseek (fp, -10L, SEEK_END) //文件结尾处回退10个字节
```

第二参数偏移量(offset)值必须是long类型，常加上L表示其值。

文件大小超过20亿以上后(例如UNIX框架下的磁盘型文件), 给出`fpos_t`类型 定位位置，配套函数如下:

```
int fgetpos (FILE * restrict stream, fpos_t * restrict pos)
int fsetpos (FILE * restrict stream, const fpos_t * pos)
```

##### `fread()` ` fwrite()`

```C
size_t fwrite (const void* restrict ptr, size_t size, size_t nmemb, FILE* fp);
size_t fread (const void* restrict ptr, size_t size, size_t nmemb, FILE* fp);
```

用来操作二进制数据流。__`ptr`为待写入数据块的地址__, `size` 为数据块大小，以字节为单位, `nmemb` 数据块数量。

##### `feof()` `ferror()`

`feof()` 用来检验一个文件的数据流是否到结尾
`ferror()` 用来返回一个文件的读写中是否出错

#### 个人其余常用接口

* ` int access(const char *filepath, int mode)` 获取文件状态信息，用来判断文件是否存在，是否可写等!
* `sprintf()` `sscanf()` 也为标准库中定义的IO流函数，输入输出源为特定字符串，常用!
* `ungetc(int c, FILE *fp)` 把c指定的字符放回输入流中！(可以放入`stdin`)
* `fflush (FILE *fp)` 刷新缓冲区，会将输出缓冲区一次性发送到fp文件

-- -

### C-Primer Chapter.14 结构和其他数据形式

#### 结构体

结构体申明可以在函数外部，也可以在函数体内部，在函数体内部时，该结构体仅限于函数内使用。

结构体定义时花括号右边可以加变量名同时定义变量，例如:
```C
struct sample_st {
	...
} var_st_1;

```

对于相同类型的结构, C 允许把结构赋值给另一个结构(数组不行！)

__在结构体中申明字符指针要非常注意(字符数组相对安全)！！ 字符指针仅代表`sizt_t`大小的存储空间，具体内容赋值时指向内存中其他空间，其值并不在该结构体连续内存中，如果使用初始化的字符指针值直接赋值，可能会改变内存中其他位置的值！__

##### 内存分配

结构体根据从上到下的成员，依次开辟连续内存空间。 结构数组为连续结构体空间。

__针对结构数组，若结构数组为自动存储类别的局部变量，即在栈内存中开辟对应空间，普通编译默认栈大小可能不够，此时可以设置编译器中栈大小(`Linux 下为 ulimit 指令`)，或者将变量申明成静态空间。__

结构体文件存储或读写常使用二进制格式, 这样非常方便，但不同系统二进制表示法不同，文件移植性变差。
```C
fread (&var_st_1, sizeof(struct sample_st), 1, fp);
fwrite (&var_st_1, sizeof(struct sample_st), 1, fp);
```
##### 初始化

```C
// 初始化方法一
struct sample_st var_st_2 = {1, "Hello World", {1, 2, 3}};
// 初试化方法二(初始化器)
struct sample_st var_st_2 = { .int_var = 1,
	       .str_var = "Hello World",
	        .arr_var = {1, 2, 3}
};

//将该特性与类型转换运算结合，可以起到直接赋值的效果
struct sample_st var_st_3 = {0};
var_st_3 = (sturct sample_st) {1, "Hello World", {1, 2, 3}};
```

##### 结构体指针

传递指针通常比传递结构体有效率。

__结构体的变量名并不是该结构体的首地址，取地址时需要使用运算符`&`，对结构体数组同理！__

```C
struct sample_st arr_st[2] = {0};
// 取该结构体数组的首地址时如下：
struct sample_st* p_st = &arr_st[0];

//此时为真：
p_st->int_var == (*p_st).int_var;
```

#### 联合`union`

联合可以再同一个内存空间中存储不同类型的数据。（非同时） __系统申明联合`union`类型的变量时，会自动分碰联合内最大类型所占的内存空间。__

	使用联合类型的数组可以达到数组每个元素大小相等且连续，但具体存储类型不同。

```C
union sample_union {
	char            c_var;
	unsigned int    uint_var;
	double          llf_var;
};

union sample_union union_arr[10];
union_arr[0].c_var = '\0';
union_arr[0].uint_var = 0;
union_arr[0].llf_var = 0.0;
```

##### 匿名结构与匿名联合(常结合使用)

```C
struct car_data {
	char                        name[15];
	uint64_t                    id;
	uint8_t                     status;  // 0 --> human's car;  1 --> company's car
	union {
		sturct human_data       owner;
		struct company_data     company;
	}
} sample;
```

上例中联合类型没有具体名称，访问时直接使用`sample.owner.[...]` 或 `sample.company.[...]` 访问即可，且同时仅能存在一个。

#### 枚举`enum`

enum 默认存储为 int类型。 其定义的符号常量被称为 枚举符(enumerator), 在编译过程中替换(与define 不同，由编译器解释，而不是预处理器)!!

__对于特定量，定义成枚举类型可以显著性提高程序可读性，节省define等空间。__

```C
// C 会自动按顺序分配，即MON == 0 ; TUE == 1 ...
enum week = {MON, TUE, WED, THR, FRI, SAT, SUN};

// C 同时会自动顺延, FEB == 2; MAR == 3 ...
enum month = {
	JAN = 1,
	FEB,
	MAR,
	...
}
```

<font color = "red">
__C中enum 可以使用++运算，自动按顺序枚举，但C++ 中取消了这一特性。对应变量需要转成int类型后才能做 `++` `--`运算__
</font >

#### typedef

为特定类型自定义名称，__用于提升代码可读性，压缩代码空间，常用于`struct` 等类型申明__

`typedef` 由编译器解释，而不是预处理器。 其创建的符号名之受限于类型，不能用于值。

__可以定义在函数等局部范围内，仅有局部作用域。__

#### 指向函数的指针!!!!

```C
// 指向函数的指针申明：
              void (*pfunc) (char*);
```

__`pfunc` 是个指针变量, 指向一个 参数列表为`(char*)`, 返回类型为`void` 的函数。__
__赋值时，函数名作为该函数的地址可以直接赋值。__

使用举例:

```C
static void ToLower(char* str);
static void ToUpper(char* str);

int main() {
	char sample_string[] = "helloworld";
	void (*pf) (char*);
	if (islower)
		pf = ToUpper;
	else
		pf = ToLower;

	(*pf)(sample_string);
}
```

```C
// 常用情形，作为函数参数
void show(void (*pf)(char*), char* str)
{
	(*fp)(str);
	return;
}

// 常用情形
// 第一个参数对应函数参数的起始地址，第二参数对应参数内存中字节长度
int (*pf)(void*, size_t);

```

__申明指向函数的指针变量时, 需要用括号的本质原因为调整运算优先级。如下：__

##### `* [] ()` 的优先级问题

1. 数组后面的`[]` 和函数后面的`()` 优先级相同, 大于 `*`(解引用)
2. `[]` 和 `()` 优先级相同，从左至右结合
3. `int` `doulbe` 等最后运算

```C
int * a[10];
int (* b)[10];
int * c[3][4];
int (*d[4])(double );
```

<font color = "red">

  * a: __先运算`[10]`, 变量a为10个元素的数组, 接着运算`*`, 即 解引用`a[10]`, 得到该数组的每个元素都为 指向 int的指针。__
  * b: __先运算`*`, 解引用 b, 即b是一个指针，接着运算`[10]`, 该变量的指针为指向 `int [10]` 类型，指向一个内含10个int 元素的数组__
  * c: __先运算`[3]`, 变量c为内含3个元素的数组, 接着运算`[4]`, 每个数组的元素都是内含 4个元素的数组。 运算`*` 表示这些元素都是指针, 最后 `int` 表示这些指针指向的都是int类型的变量__
  * d: __首先`*d[4]` , 变量d为 内含四个元素的数组，每个元素都是指针, 接着`double` 每个指针都是指向函数, 函数参数为一个double, 最后int表示指针指向的函数的返回类型为int。__
</font >

---

### C-Primer Chapter.15 位运算

#### 补码

__以1字节二进制 带符号整数 为例: __

* 符号位 1 表示 负数, 符号位 0 表示 正数.
  	+ 符号整数(signed) `10000001`: ` -1`
  	+ 符号整数(signed) `00000001`: ` +1`

  此时有两个0 `10000000` 和 `00000000`, 为 ` -0` ` +0`, 产生编码浪费。即一个字节只能表示 ` -127 ~ 127`.

  引入补码机制, 对于正数和0 即 `00000000 ~ 01111111` 表示 `0 ~ 127`.
  对于负数 ` -x`, 通过9位`100000000`即无符号256 减去 对应无符号`x` 得到 ` -x` 的补码

  `10000000` 为 ` -128`, `10000001` 为 ` -127` , `11111111` 为 ` -1`.

  __此时, 相反数互相转换为 取反加一.__

#### 逻辑运算符

##### `~` `&` `|` `^`

逻辑运算符直接操作 对应类型字节长度的补码。
- `~` 取反
- `&` 与
- ` | ` 或
- `^` 异或

##### 常用案例 (掩码(MASK))

* 设置位为1，其余位不变
```C
// flags 中，MASK 为 1 的位，全部置1. 其余位不变
              MASK = 0B000100001;
maps |= MASK;
```

* 清空位为0，其余位不变
```C
// flags 中 MASK 位 1 的位， 全部置0. 其余位不变
MASK = 0B000100001;
maps &= ~MASK;
```

* __切换位!__
```C
// flags 中 MASK 位 1 的位, 全部切换, 即 0-->1 , 1-->0 . 其余位不变
MASK = 0B000100001;
maps ^= MASK;
```

* __检查位!__
```C
// 检查flags中, MASK 为1的位是否为1.
MASK = 0B000100001;
if ((flag & MASK) == MASK)
	putchar('Y');
```

#### 移位运算符

##### `<<` `>>`

` << ` 左移, 右侧补0.
` >> ` 右移，左侧补0.

  双目运算符，产生返回值，但并不改变 变量自身的值。

##### 常用案例

  * __快速对2求幂, 效率高于直接乘除。特别适用于 ` * 2` ` / 2` __

  ```C
// 左移等价于 乘以 2 的n 次幂
  a = number << n;
a = number * pow(2, n);

// 右移等价于 除以 2的n 次幂 (整除)
a = number >> 1;
a = a / 2;
```

#### 对齐特性

不同类型(字节长度) 的变量在内存存储的对其方式不一样， 假设`double`的对齐类型为8,  则其变量的地址可以被8整除。
__默认操作对齐的C保留字为 `_Alignas` `_Alignof`, 引入`stdalign.h` 后可以对应 `alignas` `alignof`, 从而与 C++ 对应__

```C
// 得到一个类型的对齐字节数
size_t d_align = _Alignof (float);

// 申明变量时, 指定其按某字节数对齐
_Alignas (double) char c1;
_Alignas (8) char c2;
```
---

### C-Primer Chapter.16 预处理和C库

#### 编译过程相关

<font color="red" size="4">

* 预处理前,翻译处理:
	- 第一步, 把源代码中的字符 映射到 源字符集。 即处理多字节字符等, 让 C 更加国际化
	- 第二步, 定位反斜杠链接换行符的情况, 把物理行 替换成 逻辑行
	- 第三步,__用空格替换空白字符序列(空格、制表符)和注释序列,不替换换行符。 因为替换注释,所以如下代码编译合法`int/*此处替换成空格*/var;`__
	
* 预处理: __查找一行中以#开始的预处理指令__
</font>

#### `#define`

```C 
#define [宏] [替换体]
```

宏命名 遵循 C变量命名规则且首字符不能是数字, __预处理过程中的定位到宏后,将其转换为替换体(多数情况为直接替换),这一过程称为宏展开.__

__C 中不允许把 const定义的变量作为常量表达式, C++ 中则允许。__

* __宏只能定义成一行, 多行时应该用`\` 将逻辑行转为多个物理行__
* 个人总结技巧一:__防止宏替换对上下文影响, 常引入 `do \ { \  ... \ } \ while(0);` 形成代码块隔绝影响 __
* 个人总结技巧二:__防止宏中未使用参数的影响, 对可能未使用的参数使用 `(void)var;` 处理__

##### `#define 中引入参数`

```C 
#define SQUARE(X) ((X)*(X))
```

宏展开时, 将把对应的宏参数记号带入替换体进行替换,__与函数参数有本质性区别,宏参数为直接替换,替换时并不考虑运算优先级,同时不会检查类型和单双目运算符等等__

__避免在宏参数中使用`++` `--` 等单目运算符,同时应该用括号严格控制优先级__

__C99后支持变参宏, 即`...` `__VA_ARGS__`, `...` 在宏参数中表示可变的参数, `__VA_ARGS__` 则在替换体中对应 `...` __

---

##### `#define` 中的`#`运算符

__使用`#`可得到字符串形式的宏参数, 如下宏展开时`#x` 被自动展开成字符串 __
```C 
#define PSQR(x) printf("The square of " #x " is %d\n", (x)*(x) );
```

<font color="red">
	
__粘合剂`##`运算符:__

将左右记号(symbol)组合成一个记号。

```C
#define X(n) x ## n 
#define PRINT_X(n) printf("x" #n " = %d\n", x ## n);

int main()
{
	// 宏展开为
	// int x1 = 1;
	// int x2 = 2;
	int X(1) = 1;
	int X(2) = 2;

	//宏展开为
	// printf("x1 = %d\n", x1);
	PRINT_X(1);
	PRINT_X(2);
	...
}
```
</font>

#### 常用预处理指令

* `#include` 把被包含文件的全部内容输出到源文件`include`所在位置
* `#ifdef/ifndef #else #endif` 条件编译, 注意使用`else` `endif` 标记指令块
* `#if #elif #else #endif` 同上
* `#error` __让预处理器发送错误指令, 正常情况下, 此时编译会中断__

#### 预定义宏

__如下为部分C标准预定义的宏, 常用于日志输出(log)__

* `__DATE__` 预处理的日期, 字符串常量（字面量）
* `__TIME__` 预处理的时间, 字符串常量（字面量）
* `__FILE__` 当前文件的文件名, 字符串常量（字面量）
* `__LINE__` 当前文件预编译的行号, 整形常量

#### 部分库函数整理

##### `stdlib.h`

* `atexit()` 该函数接受一个函数指针作为参数,  程序调用 `exit()`时会触发对应函数.
* `exit()` 显示调用`exit()`退出程序. __程序结束时会自动调用`exit()`.__