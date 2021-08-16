[TOC]
#gcc g++ 编译选项

### 通用

##### 标准 `-std=`
`gcc -std=c99` 
`g++ -std=c++11`
指定不同的语言编译标准。

##### 自动优化(`-O$(n)`)
`-O0` `-O1` `-O2` `-O3`

依次对应编译时优化等级，-O0 为不优化。 -O3 优化等级最高(会修改执行顺序，对debug过程产生严重干扰！！)

##### 指定include目录 -I 

`-I$(PATH)`
后接目录，指定include目录，优先级高于系统自带,当前目录不用指定(可查看环境变量得到)

##### 指定编译时动态库目录 -L

`-L$(PATH)`
后接目录，指定编译时寻找动态库位置的目录，优先级高于系统自带(可查看环境变量得到)

##### 指定程序执行时寻找动态库目录 -Wl,rpath

```Makefile
-Wl,rpath=$(PATH)
-Wl,rpath=.
-Wl,rpath=lib
```

指定程序执行时寻找动态库位置的目录，优先级高于系统自带

##### 警告设置 -Wall

Warning all,对一切编译器觉得不合理的地方产生警告

##### 动态内存位置 -PIC ( 编译动态库时，绝大多数情况都要加)

position independent code，产生的代码中，没有绝对地址，全部使用相对地址，故而代码可以被加载器加载到内存的任意位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。

##### define -D$(parameter)

编译时预定义参数，在预编译阶段前定义完成，直接作用于编译时的`ifdef\ifndef` 等过程

### 编译动态库(.so \ .dll)

##### 指明为动态库 -shared

`gcc -fPIC -shared -o libsample.so libsample.c`
```Makefile

SHARE:= -fPIC -shared -o
$(TARGET):$(OBJ)
    $(CC) $(CFLAGS) $(SHARE) $@ $(OBJS) $(LDFLAGS)
    $(STRIP) $(TARGET) # 发布时加上，调试时取消
```

个人习惯写法如上

##### 压缩 strip

去除目标文件中的一些符号表、调试符号表信息，以减小静态库、动态库和程序的大小。  
__strip前的库用来调试， strip后的库用来实际发布__,一旦发布的strip后的库出了问题， 就可以找对应的未strip的库来定位。

### 编译中间过程文件(.o)

##### 只编译不链接 -c

只编译不链接：产生.o文件，就是obj文件，不产生执行文件(用于Makefile)