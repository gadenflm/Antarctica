#Make 与 Makefile

######***巫昆仑 2021.7***

&emsp;
### 1. 程序的编译过程 

C\C++\Pascal 等语言的编译规范：
**第一步，把源文件编译成中间代码文件**,在windows下是.obj ； Linux\UNIX 下是.o ，统称Object File，**这个动作叫编译（compile）**。

**第二步，大量的Object File合成执行文件，这个动作叫链接（link）。**

+  **编译时，编译器需要的是语法的正确，函数与变量的声明的正确。**对于后者，通常是你需要告诉编译器头文件的所在位置（头文件中应该只是声明，而定义应该放在C/C++文件中），只要所有的语法正确，编译器就可以编译出中间目标文件。一般来说，每个源文件都应该对应于一个中间目标文件（O文件或是OBJ文件）。 
+   **链接时，主要是链接函数和全局变量，用中间目标文件（O文件或是OBJ文件）来链接应用程序。**链接器并不管函数所在的源文件，只管函数的中间目标文件（Object File），在大多数时候，由于源文件太多，编译生成的中间目标文件太多，而在链接时需要明显地指出中间目标文件名，这对于编译很不方便，所以，我们要给**中间目标文件打个包，在Windows下这种包叫“库文件”(Library File)，也就是 .lib 文件，在UNIX\Linux下，是Archive File，也就是 .a 文件**

编译时，编译器只检测程序语法，和函数、变量是否被声明。如果函数未被声明，编译器会给出一个警告，但可以生成Object File。而在链接程序时，链接器会在所有的Object File中找寻函数的实现，如果找不到，那到就会报链接错误码（Linker Error）

e.g.在VC下，这种错误一般是：Link 2001错误，意思说是说，链接器未能找到函数的实现。你需要指定函数的ObjectFile.

&emsp;
### 2. make 与 Makefile 的规则

GNU的make使用手册，给出的示例规则：

1. 如果这个工程没有编译过，那么我们的所有C文件都要编译并被链接。
2. 如果这个工程的某几个C文件被修改，那么我们只编译被修改的C文件，并链接目标程序。
3. 如果这个工程的头文件被改变了，那么我们需要编译引用了这几个头文件的C文件，并链接目标程序。

Makefile核心结构：

```
[target]:[prerequisites]
    [command]
    ...
```

+ **target** 是一个目标文件，可以是Object File，也可以是执行文件。还可以是一个标签（Label）
+ **prerequisites**生成target所需要的文件或是目标.
+ **command**是make需要执行的命令,**以Tab(\t)开头**。可以用;分号分隔，\为换行符（任意的Shell命令）

这个结构建立出**make下的依赖关系**，target这一个或多个的目标文件依赖于prerequisites中的文件，其生成规则定义在command中。

**即prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行。**

make 工作机制：
1. 当前目录下找“Makefile”或“makefile”
2. 找文件中的第一个目标文件（target）并把这个文件作为最终的目标文件。
3. 如果target不存在，或是target所依赖(.o 文件)的文件修改时间要比target文件新，那么，他就会执行后面所定义的命令来生成target.
4. 如果所依赖的.o文件也存在，那么make会在当前文件中找目标为.o文件的依赖性，如果找到则再根据那一个规则生成.o文件。（这有点像一个堆栈的过程）
总结：**make不管命令是怎么工作的，只管执行所定义的命令。make会比较targets文件和prerequisites文件的修改日期，如果prerequisites文件的日期要比targets文件的日期要新，或者target不存在的话，那么，make就会执行后续定义的命令。并且make会一层又一层地去找文件的依赖关系**

**prerequisites** 可以为空，比如常用的clean。即该目标没有依赖

如果当前环境中定义了环境变量MAKEFILES，make会把这个变量中的值做一个类似于include的动作。建议不要使用，出错可以反插。

&emsp;
###3. Makefile 惯例：

每个Makefile中都应该写一个清空目标文件（.o和执行文件）的规则
```
clean:
    rm -fr [target] $(objects)
```
或**更安全**的写法：
```
.PHONY : clean
clean :
    -rm -f [target] $(objects)
```

+ **.PHONY意思表示clean是一个“伪目标”**伪目标”并不是一个文件，只是一个标签，由于“伪目标”不是文件，所以make无法生成它的依赖关系和决定它是否要执行。通过显示地指明这个“目标”才能让其生效。
+ rm命令前面加了一个小减号是，也许某些文件出现问题，但不要管，继续做后面的事。
+ clean的规则不要放在文件的开头，不然，这就会变成make的默认目标。不成文的规矩是——“clean从来都是放在文件的最后”。


**以下三个变量：`$@，$^，$<`代表的意义分别是：**

+ $@ &nbsp;&nbsp;&nbsp;&nbsp;目标文件，目前规则中所有的目标的集合
+ $^ &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;所有的依赖文件，
+ $< &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;第一个依赖文件。

&emsp;
###4. 其他：

##### <font color=red>命令执行</font>

```
exec:

    cd /home/hchen; pwd
```

当规则的目标需要被更新时，make会一条一条的执行其后的命令。需要注意的是，如果要让上一条命令的结果应用在下一条命令时，应该使用分号分隔这两条命令。比如第一条命令是cd命令，你希望第二条命令得在cd之后的基础上运行，那么就不能把这两条命令写在两行上，而应该把这两条命令写在一行上，用分号分隔。

##### make的一些关键参数

+ -C 指定读取makefile的目录。
+ -e 指明环境变量的值覆盖makefile中定义的变量的值。
+ -i 在执行时忽略所有的错误。
+ **-j 指同时运行命令的个数。如果没有这个参数，make运行命令时能运行多少就运行多少。** 常用 `-j4` or `-j8`
+ -k 出错也不停止运行。如果生成一个目标失败了，那么依赖于其上的目标就不会被执行了。

##### 静态模式

```
[targets...]: [target-pattern]: [prereq-patterns ...]
    [commands]
    ...
```

+ targets定义了一系列的目标文件，可以有通配符。是目标的一个集合。
+ target-parrtern是指明了targets的模式，也就是的目标集模式。
+ prereq-parrterns是目标的依赖模式

例子

```
objects = foo.o bar.o
all: $(objects)

$(objects): %.o: %.c
    $(CC) -c $(CFLAGS) $< -o $@
```
目标从$object中获取

“%.o”表明要所有以“.o”结尾的目标，也就是“foo.o ; bar.o”，也就是变量`$object`集合的模式。

而依赖模式“%.c”则取模式“%.o”的“%”，也就是“foo ; bar”
并为其加下“.c”的后缀，于是，我们的依赖目标就是“foo.c ; bar.c”。

`“$<”`表示所有的依赖目标集(foo.c bar.c)

`“$@”`表示目标集(foo.o bar.o)。

---

&emsp;
## Makefile 语法：
**过程前加-号，表示无论过程出现什么错误，都继续执行**

> `# 注释内容...`

**注释是用 # 字符**

> `include [filename]`

引用其他 Makefile 格式的文件，中间用空格隔开，支持当前操作系统Shell的文件模式（可以保含路径和通配符）

> `@echo Hello World！`

echo用来显示参数

通常，make会把其要执行的命令行在命令执行前输出到屏幕上。当我们用“@”字符在命令行前，这个命令将不被make显示出来。

```
define run_set
[command 1]
[command 2]
[...]
endef
```

如果Makefile中出现一些相同命令序列，那么我们可以为这些相同的命令序列定义一个变量。定义这种命令序列的语法以“define”开始，以“endef”结束.

调用时用`$(run_set)`

#### 条件 if endif

```
<conditional-directive>
<text-if-true>
else
<text-if-false>
endif
```

其中`<conditional-directive>`表示条件关键字，如“ifeq”。这个关键字有四个。

+ **`ifeq (<arg1>, <arg2>)` 比较参数“arg1”和“arg2”的值是否相同。 arg可以取空。**
+ **`ifneq (<arg1>, <arg2>)` 比较参数“arg1”和“arg2”的值是否不同。**
+ **`ifdef <variable-name>`如果变量`<variable-name>`值非空，那到表达式为真。否则，表达式为假。**
+ **`ifndef <variable-name>`,同理** 

特别注意的是，make是在读取Makefile时就计算条件表达式的值，并根据条件表达式的值来选择语句，最好不要把自动化变量（如“$@”等）放入条件表达式中，因为自动化变量是在运行时才有的。

#### <font color=red>变量</font>

Makefile中的定义的变量，就像是C/C++语言中的宏一样，他代表了一个文本字串，**在Makefile中执行的时候其会自动原模原样地展开在所使用的地方。**变量是大小写敏感的。

**变量在声明时需要给予初值，而在使用时，需要给在变量名前 加上“$”符号，且最好用小括号“（）”或是大括号“{}”把变量给包括起来**

#### 三种赋值方式

> `:=`

右侧值会被现场展开，**即右值只能使用前面定义好的变量，否则为空。**

> `=`

在“=”左侧是变量，右侧是变量的值，**右侧变量的值可以定义在文件的任何一处，也就是说，右侧中的变量不一定非要是已定义好**

make最后统一展开，但遇到递归定义时，展开会死循环。
e.g.`A=$(B);B=$(A)`

> `?=`

如果左边变量没有被定义过，那么变量值将等于右值，如果FOO先前被定义过，那么将什么也不做。

#### 追加变量值

```
objects = main.o foo.o bar.o utils.o
objects += another.o
```

如果变量之前没有定义过，那么，“+=”会自动变成“=”，如果前面有变量定义，那么“+=”会继承于前次操作的赋值符(三种)。

#### 把变量的值再当成变量

```
x = y
y = z
a := $($(x))
```

    $(x)的值是“y”，所以$($(x))就是$(y)，于是$(a)的值就是“z”。

```
ifdef do_sort
func := sort
else
func := strip
endif

arr := a d b g q c

foo := $($(func) $(arr))
```

**这种技术和“函数”与“条件语句”可一同使用**

#### 局部变量

Makefile中默认定义的变量都是“全局变量”

> `<target ...> : <variable-assignment>`

可以为某个目标设置局部变量，可以和“全局变量”同名，因为它的作用范围只在这条规则以及连带规则中，所以其值也只在作用范围内有效。而不会影响规则链以外的全局变量的值。
e.g.

```
prog : CFLAGS = -g
prog : prog.o foo.o bar.o
$(CC) $(CFLAGS) prog.o foo.o bar.o

prog.o : prog.c
$(CC) $(CFLAGS) prog.c
```

在这个示例中，不管全局的`$(CFLAGS)`的值是什么，在prog目标，以及其所引发的所有规则中（prog.o foo.o bar.o），`$(CFLAGS)`的值都是“-g”

> `<pattern ...> : <variable-assignment>`

`%.o : CFLAGS = -O`

所有以[.o]结尾的目标定义局部变量

    学习源：https://blog.csdn.net/weixin_38391755/article/details/80380786，到函数为止，个人理解为基础部分