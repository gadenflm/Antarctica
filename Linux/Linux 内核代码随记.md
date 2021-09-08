[TOC]
# Linux 内核代码相关随记

## 双向循环链表相关

### 已知结构体，由结构体成员地址得到结构体首地址 `list_entry`

__根据 链表 中某个 节点 查询其结构体的首地址，用以访问数据域__
宏定义如下：
```C 
#define list_entry(link, type, member) \
    ((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))
// 即
list_entry(link, type, member)
 ((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))
```

* link      为成员节点的地址 (一般都是链表中节点)
* type      为结构体类型(宏内用以转换类型)
* member    为结构体中该成员的名字(一般都是链表成员)

__获得结构体内member偏移量：__
```C
(unsigned long)(&((type *)0)->member))

/* 另有宏定义如下 */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
```

<font color="red" size="4">
拆解执行步骤如下：

1. `((type *)0)` 假定 内存0x0处有一个 type 类型的结构体, __并不访问内存位置__
2. `(&((type *)0)->member)` 根据 type 类型结构体的定义，查找成员member的的地址。__也不访问内存位置,直接根据地址0x0开始加上member成员所在的偏移量__
3. `(unsigned long)` 转换成 `size_t` 即 `sizeof()` 返回类型，为地址偏移量通用类型。

整个过程完全安全,这里面没有写任何内存位置,甚至没有访问任何内存位置。只是操作了指向这些位置的指针，而指针一般存储在机器寄存器或是通常的本地堆栈。

写成如下也是对的 `( (size_t) &( ((TYPE *)2021)->MEMBER ) - 2021 )` ,针对2021的虚拟地址取地址，获得地址2021开始的该结构体member地址，再减去2021，得到偏移量。 
</font>

由 结构体首地址 + 成员地址偏移量 == 成员地址,  得到 结构体首地址 == 成员地址 - 成员地址偏移量.

个人举例如下 :
```C
struct list temp_node = list_head->next;
while ( list_entry(temp_node, KV_STORE, list)->key != key ){
    temp_node=temp_node->next;
}
```

__相关宏定义,直接根据结构体 子成员链表 的关系 , 操作结构体, 链表操作正常维护即可:__

```C
/* 链表宏 定义, 基于 list_entry 宏 */

#define list_head(list, type, member)       \
    list_entry((list)->next, type, member)

#define list_tail(list, type, member)       \
    list_entry((list)->prev, type, member)

#define list_next(elm, member)                  \
    list_entry((elm)->member.next, typeof(*elm), member)

#define list_for_each_entry(pos, list, member)          \
    for (pos = list_head(list, typeof(*pos), member);   \
         &pos->member != (list);                \
         pos = list_next(pos, member)) 
```

内核源码中使用案例:

```C
/* 链表宏 定义, 基于 list_entry 宏 */
struct pwm_chip {
    /* 数据域 */
    struct device *dev;
    const struct pwm_ops *ops;
    int base;
    unsigned int npwm;

    struct pwm_device * (*of_xlate)(struct pwm_chip *pc,
                    const struct of_phandle_args *args);
    unsigned int of_pwm_n_cells;

    /* 逻辑关系 */
    /* only used internally by the PWM framework */
    struct list_head list;
    struct pwm_device *pwms;
};

/* 遍历整个链表，根据名字查找对应 pwm chip */
static struct pwm_chip *pwmchip_find_by_name(const char *name)
{
    struct pwm_chip *chip;
    list_for_each_entry(chip, &pwm_chips, list) {
        const char *chip_name = dev_name(chip->dev);
        if (chip_name && strcmp(chip_name, name) == 0) {
            mutex_unlock(&pwm_lock);
            return chip;
        }
    }
    return NULL;
}
```

---

## 基础宏定义

### 赋值/读写 `WRITE_ONCE` `READ_ONCE`
宏定义如下：
```C 
#define WRITE_ONCE(var, val) \
    (*((volatile typeof(val) *)(&(var))) = (val))

#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))
```

以 WRITE_ONCE 实现赋值为例
```C 
WRITE_ONCE(var, val)
    (*((volatile typeof(val) *)(&(var))) = (val))
```

* `volatile` 作用：
    - 保证调用该宏位置处的语句不会被编译器优化, 并发中保证安全
    - 确保每次 该变量都从内存中读取，而不使用寄存器中的暂存值。 并发中保证安全

* `*((typeof(val) *)(&(var)))` 强制赋值，避免类型差异导致的问题。
    - 第一步： `(&(var))` 获得变量的地址。
    - 第二步： `(typeof(val) *)` 根据右值的类型，将改地址理解为指向右值类型的指针变量。
    - 第三步： `*(···)=(val)` 解引用，将右值赋给该变量。

个人理解，这样做保证右值一定按内存空间大小写入左侧变量的地址中，而不触发编译器报错等问题，增强代码鲁棒性，但若出现左右类型长度不统一，会导致内存覆盖等问题。

内核中实际使用源码：
```C
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
}

static inline int list_empty(const struct list_head *head)
{
    return READ_ONCE(head->next) == head;
}
```

