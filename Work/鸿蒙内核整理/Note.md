[TOC]
# 鸿蒙内核liteos-a的源码笔记

___巫昆仑___

---

## 基础数据结构:双向链表

鸿蒙内核中最重要的结构体：双向链表

### 概念

双向链表是指含有往前和往后两个方向的链表，即每个结点中除存放下一个节点指针外，还增加一个指向其前一个节点的指针。其头指针head 是唯一确定的。从双向链表中的任意一个结点开始，都可以很方便地访问它的前驱结点和后继结点，这种数据结构形式使得双向链表在查找时更加方便，特别是大量数据的遍历。由于双向链表具有对称性，能方便地完成各种插入、删除等操作，但需要注意前后方向的操作。

__在鸿蒙中LOS_DL_LIST 没有数据域！LOS_DL_LIST使用时寄生在内容结构体上的，谁用它谁就是它的数据.__

### 定义源码

`los_list.h` 中定义：

```C
typedef struct LOS_DL_LIST {//双向链表，内核最重要结构体
    struct LOS_DL_LIST *pstPrev; /**< Current node's pointer to the previous node *///前驱节点(左手)
    struct LOS_DL_LIST *pstNext; /**< Current node's pointer to the next node *///后继节点(右手)
} LOS_DL_LIST;
```

### 接口源码__链接关系

```C
 //将指定节点初始化为双向链表节点
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListInit(LOS_DL_LIST *list)
{
    list->pstNext = list;
    list->pstPrev = list;
}

 //将指定节点插入到双向链表头端
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListAdd(LOS_DL_LIST *list, LOS_DL_LIST *node)
{
    node->pstNext = list->pstNext;
    node->pstPrev = list;
    list->pstNext->pstPrev = node;
    list->pstNext = node;
}

//将指定节点插入到双向链表尾端
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListTailInsert(LOS_DL_LIST *list, LOS_DL_LIST *node)
{
    LOS_ListAdd(list->pstPrev, node);
}

//将指定节点从链表中删除
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListDelete(LOS_DL_LIST *node)
{
    node->pstNext->pstPrev = node->pstPrev;
    node->pstPrev->pstNext = node->pstNext;
    node->pstNext = NULL;
    node->pstPrev = NULL;
}

//将指定节点从链表中删除，并使用该节点初始化链表
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListDelInit(LOS_DL_LIST *list)
{
    list->pstNext->pstPrev = list->pstPrev;
    list->pstPrev->pstNext = list->pstNext;
    LOS_ListInit(list);
}

//判断链表是否为空
LITE_OS_SEC_ALW_INLINE STATIC INLINE BOOL LOS_ListEmpty(LOS_DL_LIST *list)
{
    return (BOOL)(list->pstNext == list);
}

```

总结： 

1. `LOS_DL_LIST *list` 为双向链表。特点是指针指向关系形成环。 e.g. __两个节点时，node_1的next和prev 都指向node_2;node_2的next和prev 也都指向node_1__ 
2. 阅读源码时注意前后方向的操作以及执行顺序!
3. 双向链表为空时,( head->next == head->prev ) && ( head->next == head )

---

## 进程

### 概念

* 进程是运行起来的程序，相对于操作系统，__进程是资源管理单元__。例如：使用或等待CPU、使用内存空间等系统资源，并独立于其它进程运行。资源是什么? 内存，线程，文件，信号量等等都是资源。
* __鸿蒙内核的进程采用抢占式调度机制__，支持时间片轮转调度方式和FIFO调度机制。进程一共有32个优先级(0-31)，用户进程可配置的优先级有22个(10-31)，最高优先级为10，最低优先级为31。__高优先级的进程可抢占低优先级进程，低优先级进程必须在高优先级进程阻塞或结束后才能得到调度。__

### 源码

* __`los_process_pri.h`__

`struct ProcessCB` 结构体定义 (CB: Control Block 控制块)

```C
typedef struct ProcessCB {
    ...
}
```

拆解各部分如下,

__一、 进程间关系__
```C
    CHAR                 processName[OS_PCB_NAME_LEN]; /**< Process name */ //进程名称
    UINT32               processID;                    /**< process ID = leader thread ID */    //进程ID,由进程池分配,范围[0,64]
    UINT16               processStatus;                /**< [15:4] process Status; [3:0] The number of threads currently
                                                            running in the process *///这里设计很巧妙.用一个变量表示了两层逻辑 数量和状态,点赞!从这里也可以看出一个进程可以有多个正在运行的线程
    UINT16               priority;                     /**< process priority */ //进程优先级
    UINT16               consoleID;                    /**< The console id of task belongs  *///线程的控制台id归属
    UINT16               processMode;                  /**< Kernel Mode:0; User Mode:1; */  //模式指定为内核还是用户进程
    UINT16               readyTaskNum;                 /**< The number of ready tasks in the current process */
    UINT32               parentProcessID;              /**< Parent process ID */    //父进程ID
    UINT32               exitCode;                     /**< process exit status */  //进程退出状态码
    LOS_DL_LIST          pendList;                     /**< Block list to which the process belongs */ //进程所在的阻塞列表,进程因阻塞挂入相应的链表.
    LOS_DL_LIST          childrenList;                 /**< my children process list */ //孩子进程都挂到这里,形成双循环链表
    LOS_DL_LIST          exitChildList;                /**< my exit children process list */    //要退出的孩子进程链表.
    LOS_DL_LIST          siblingList;                  /**< linkage in my parent's children list */ //兄弟进程链表,来自同一个父进程.
    ProcessGroup         *group;                       /**< Process group to which a process belongs */ //所属进程组
    LOS_DL_LIST          subordinateGroupList;         /**< linkage in my group list */ //进程组员链表
    #if (LOSCFG_KERNEL_LITEIPC == YES)
    ProcIpcInfo         ipcInfo;       /**< memory pool for lite ipc */ //用于进程间通讯的虚拟设备文件系统,设备装载点为 /dev/lite_ipc
    #endif
```

鸿蒙内核进程采用家族性管理，内核态进程和用户态进程分别有自己的根祖先，祖先进程在内核初始化时就创建好了，分别是1号(用户进程祖先)和2号(内核
进程祖先)进程.这种方式使进程创建时根据家族性关系决定了权限不同。 终会形成树状结构，每个进程都能找到自己的位置。

规则：

1. __进程退出时会主动释放持有的进程资源__，但持有的pid资源需要父进程通过wait/waitpid或父进程退出时回收。__一个子进程的消亡要通知父进程，以便父进程在族谱上抹掉它的痕迹__，一些异常情况下的坏孩子进程消亡没有告知父进程的，系统也会有定时线程能检测到而回收其资源。
2. 进程创建后，只能操作自己进程空间的资源，无法操作其它进程的资源（共享资源除外）。
3. 进程间有多种通讯方式，事件，信号，消息队列，管道等等。
4. __高优先级的进程可抢占低优先级进程，低优先级进程必须在高优先级进程阻塞或结束后才能得到调度。__

<font color="red" size=4>
进程的5种状态：

1. Init: (初始化)该进程正在被创建。 --> Ready
2. Ready: (就绪)该进程在就绪列表中，等待CPU调度。 --> Pending \ Running
3. Running: (运行)该进程正在运行。 --> Ready \ Pending \ Zombies
4. Pending: (阻塞)该进程被阻塞挂起。进程内所有的线程均被阻塞时，进程被阻塞挂起。 --> Ready 
5. Zombies: (僵尸态)该进程运行结束，等待父进程回收其控制块资源。

</font>

__二、 与线程(thread)关系__

```C
    UINT32               threadGroupID;                /**< Which thread group , is the main thread ID of the process */ //哪个线程组是进程的主线程ID
    LOS_DL_LIST          threadSiblingList;            /**< List of threads under this process *///进程的线程(线程)列表
    volatile UINT32      threadNumber; /**< Number of threads alive under this process */   //此进程下的活动线程数
    UINT32               threadCount;  /**< Total number of threads created under this process */   //在此进程下创建的线程总数
    LOS_DL_LIST          waitList;     /**< The process holds the waitLits to support wait/waitpid *///父进程通过进程等待的方式，回收子进程资源，获取子进程退出信息
```

线程是CPU的调度单元。进程和线程的关系是1:N 的关系，进程可以有多个线程但一个线程不能同属于多个进程。线程是作为一种资源被进程管理的，进程为线程提供内存支持，提供文件支持，提供设备。

规则：

1. 一般情况下,main函数就是入口函数。进程加载时，__main函数创建第一个线程为主线程__。
2. 执行过程中根据代码创建新的线程，其本质和main函数创建的线程没有区别，只是入口函数变了，统一参与调度.
3. __进程的主线程或所有线程运行结束后，进程转为僵尸态，一般只能由所有线程结束后，进程才能自然消亡.__
4. __进程创建后进入就绪态，发生进程切换时，就绪列表中最高优先级的进程被执行，从而进入运行态。__
 
    若此时该进程中已无其它线程处于就绪态，则该进程从就绪列表删除，只处于运行态；

    __若此时该进程中还有其它线程处于就绪态，则该进程依旧在就绪队列，此时进程的就绪态和运行态共存。进程可以允许多种状态并存!__  相关: [位图管理]()。

5. 进程内所有的线程均处于阻塞态时，__进程在最后一个线程转为阻塞态时，同步进入阻塞态__，然后发生进程切换。
6. __阻塞进程内的任意线程恢复就绪态时，进程被加入到就绪队列，同步转为就绪态__，若此时发生进程切换，则进程状态由就绪态转为运行态

__三、与内存关系__
```C
#ifdef LOSCFG_KERNEL_VM
    LosVmSpace          *vmSpace;       /**< VMM space for processes */ //虚拟空间,描述进程虚拟内存的数据结构，linux称为内存描述符
#endif
```

__每一个用户态进程均拥有自己独立的进程空间，相互之间不可见，实现进程间隔离，独立进程空间意味着每个进程都要将自己的虚拟内存和物理内存进行映射.并将映射区保存在自己的进程空间.另外进程的代码区，数据区，堆栈区，映射区都存放在自己的空间中，但内核态进程的空间是共用的，只需一次映射.__

`LosVmSpace` 为封装后的虚拟内存空间，详见 [内存]()

__四、与文件关系__
```C 
#ifdef LOSCFG_FS_VFS
    struct files_struct *files;        /**< Files held by the process */ //进程所持有的所有文件，注者称之为进程的文件管理器
#endif  //每个进程都有属于自己的文件管理器,记录对文件的操作. 注意:一个文件可以被多个进程操作

// 定义见fd_table.h
struct files_struct {//进程文件表结构体
    int count;              //持有的文件数量
    struct fd_table_s *fdt; //持有的文件表
    unsigned int file_lock; //文件互斥锁
    unsigned int next_fd;   //下一个fd
#ifdef VFS_USING_WORKDIR
    spinlock_t workdir_lock;    //工作区目录自旋锁
    char workdir[PATH_MAX];     //工作区路径,最大 256个字符
#endif
};

```

files_struct，可理解为进程的文件管理器。详见 [文件系统]()

规则：

1. 一个真实的物理文件( inode )，可以同时被多个进程打开，并有进程独立的文件描述符， 进程文件描述符( ProcessFD )后边映射的是系统文件描述符( SystemFD ).
2. 系统文件描述符( 0-stdin，1-stdout，2-stderr )默认被内核占用，任何进程的文件描述符前三个都是( stdin，stdout，stderr )
3. 文件映射跟内存映射一样，每个进程都需要单独对同一个文件进行映射，page_mapping记录了映射关系，而页高速缓存( page cache )提供了文件实际内存存放位置。
4. __内存<->文件的置换以页为单位(4K)，进程并不能对硬盘文件直接操作，必须通过页高速缓存( page cache )完成.__其中会涉及到许多经典的概念比如COW (写时拷贝)技术。

__五、其他__

```
    timer_t             timerID;       /**< iTimer */

#ifdef LOSCFG_SECURITY_CAPABILITY   //安全能力
    User                *user;      //进程的拥有者
    UINT32              capability; //安全能力范围 对应 CAP_SETGID
#endif
#ifdef LOSCFG_SECURITY_VID
    TimerIdMap          timerIdMap;
#endif
#ifdef LOSCFG_DRIVERS_TZDRIVER
    struct file         *execFile;     /**< Exec bin of the process */
#endif
    mode_t umask;//umask(user file-creatiopn mode mask)为用户文件创建掩码，是创建文件或文件夹时默认权限的基础。
#ifdef LOSCFG_KERNEL_CPUP
    OsCpupBase           processCpup; /**< Process cpu usage */
#endif
```

包括一些安全性，统计性的能力。

### 通讯(待完成)

---

## 线程

### 源码
* __`los_task_pri.h`__

`struct LosTaskCB` 结构体定义 

```C 
typedef struct {
    ...
} LosTaskCB;
```

拆解各部分如下,

__一、 线程调度与线程间关系__
```C
    UINT16          taskStatus;         /**< Task status */         //各种状态标签，可以拥有多种标签，按位标识
    UINT16          priority;           /**< Task priority */       //线程优先级[0:31],默认是31级
    UINT16          policy;             //线程的调度方式(三种 .. LOS_SCHED_RR )
    UINT64          startTime;          /**< The start time of each phase of task *///线程开始时间
    UINT64          irqStartTime;       /**< Interrupt start time *///线程中断开始时间
    UINT32          irqUsedTime;        /**< Interrupt consumption time *///线程中断恢复时间
    UINT32          initTimeSlice;      /**< Task init time slice *///线程初始的时间片
    INT32           timeSlice;          /**< Task remaining time slice *///线程剩余时间片
    UINT32          waitTimes;          /**< Task delay time, tick number *///设置线程调度延期时间
    SortLinkList    sortList;           /**< Task sortlink node */  //线程排序链表节点
    UINT32          taskID;             /**< Task ID */             //线程ID，线程池本质是一个大数组，ID就是数组的索引，默认 < 128
    TSK_ENTRY_FUNC  taskEntry;          /**< Task entrance function */  //线程执行入口函数
    VOID            *joinRetval;        /**< pthread adaption */    //用来存储join线程的返回值
    UINTPTR         args[4];            /**< Parameter, of which the maximum number is 4 */ //入口函数的参数 例如 main (int argc,char *argv[])
    CHAR            taskName[OS_TCB_NAME_LEN]; /**< Task name */    //线程的名称
    LOS_DL_LIST     pendList;           /**< Task pend node */      //如果线程阻塞时就通过它挂到各种阻塞情况的链表上,比如OsTaskWait时
    LOS_DL_LIST     threadList;         /**< thread list */         //挂到所属进程的线程链表上
    UINT32          processID;          /**< Which belong process *///所属进程ID
    UINT32          priBitMap;          /**< BitMap for recording the change of task priority,  //线程在执行过程中优先级会经常变化，这个变量用来记录所有曾经变化
                                             the priority can not be greater than 31 */         //过的优先级，例如 ..01001011 曾经有过 0,1,3,6 优先级
    UINTPTR         waitID;             /**< Wait for the PID or GID of the child process */
    UINT16          waitFlag;           /**< The type of child process that is waiting, belonging to a group or parent,
                                             a specific child process, or any child process */  //以什么样的方式等待子进程结束(OS_TASK_WAIT_PROCESS | OS_TASK_WAIT_GID | ..)

```

鸿蒙内核用的是抢占式调度(policy)，可以插队，比优先级(priority)大小。
同时也支持先来后到(FIFO)。

__线程有时间限制timeSlice，叫时间片，默认20ms，用完了会重置，发起重新调度，找出优先级高的执行__，阻塞的线程(比如没拿到锁的，等信号量同步的，等读写消息队列的)都挂到pendList上，方便管理.

__二、多核CPU相关__

```C
#if (LOSCFG_KERNEL_SMP == YES)
    UINT16          currCpu;            /**< CPU core number of this task is running on */  //正在运行此线程的CPU内核号
    UINT16          lastCpu;            /**< CPU core number of this task is running on last time */ //上次运行此线程的CPU内核号
    UINT16          cpuAffiMask;        /**< CPU affinity mask, support up to 16 cores */   //CPU亲和力掩码，最多支持16核，亲和力很重要，多核情况下尽量一个线程在一个CPU核上运行，提高效率
#if (LOSCFG_KERNEL_SMP_TASK_SYNC == YES)
    UINT32          syncSignal;         /**< Synchronization for signal handling */ //用于CPU之间 同步信号
#endif
#if (LOSCFG_KERNEL_SMP_LOCKDEP == YES)  //死锁检测开关
    LockDep         lockDep;
#endif
#endif
#ifdef LOSCFG_SCHED_DEBUG
    SchedStat       schedStat;          /**< Schedule statistics */
#endif
```

鸿蒙内核支持多CPU，多CPU效率高。但线程往往很多，而CPU数量非常有限，所以经常
A线程被调度算法打断让CPU去做B线程.A线程被打断回来接着做的还会是原来那个CPU吗?

答案是:不一定. __变量`cpuAffiMask`叫CPU亲和力，它的作用是可以指定A线程始终是同一个CPU来完成，也可以交给调度算法__，最多支持16核，亲和力很重要，多核情况下尽量一个线程在一个CPU核上运行，提高效率。

__三、栈空间相关__

```C
    VOID            *stackPointer;      /**< Task stack pointer */  //内核栈指针位置(SP)
    UINT32          stackSize;          /**< Task stack size */     //内核态栈大小,内存来自内核空间
    UINTPTR         topOfStack;         /**< Task stack top */      //内核态栈顶 bottom = top + size
    UINTPTR         userArea;          //用户空间的堆区开始位置
    UINTPTR         userMapBase;        //用户空间的栈顶位置,内存来自用户空间,和topOfStack有本质的区别.
    UINT32          userMapSize;        /**< user thread stack size ,real size : userMapSize + USER_STACK_MIN_SIZE *///用户栈大小
```

__进程分内核态进程和用户态进程，这个区别表现在线程层面上就是内核态进程下创建的线程只有内核态的栈空间。用户态进程下创建的线程除了有内核态的栈空间外，还有用户态栈空间。__

__四、资源竞争/同步相关__

```C
    VOID            *taskMux;           /**< Task-held mutex */     //task在等哪把锁
    VOID            *taskEvent;         /**< Task-held event */     //task在等哪个事件
    UINT32          signal;             /**< Task signal */ //线程信号类型,(SIGNAL_NONE,SIGNAL_KILL,SIGNAL_SUSPEND,SIGNAL_AFFI)
    sig_cb          sig;                //信号控制块，用于异步通信,类似于 linux singal模块
    FutexNode       futex;              //实现快锁功能
    LOS_DL_LIST     joinList;           /**< join list */ //联结链表,允许线程之间相互释放彼此
    LOS_DL_LIST     lockList;           /**< Hold the lock list */  //该链表上挂的都是已持有的锁
    UINT32          eventMask;          /**< Event mask */          //线程对哪些事件进行屏蔽
    UINT32          eventMode;          /**< Event mode */          //事件三种模式(LOS_WAITMODE_AND,LOS_WAITMODE_OR,LOS_WAITMODE_CLR)
```

__互斥锁(taskMux，futex)目的是解决解决线程间资源紧张的竞争性问题__.办事前先拿锁，拿到了锁的获得资格，没有拿到的需要在lockList上排队。

__信号量(sig_cb)用于线程的同步。__线程和线程之间是会有关联的,CPU在执行B线程时发现前置条件是需要A线程完成到某一步才能进行，这时B就需要主动让出CPU先办完A的事.这就是信号量的原理，解决的是线程间的同步问题.

__五、IPC线程间通讯__

```C
#if (LOSCFG_KERNEL_LITEIPC == YES)
    UINT32          ipcStatus;          //IPC状态
    LOS_DL_LIST     msgListHead;        //消息队列头结点,上面挂的都是线程要读的消息
    BOOL            accessMap[LOSCFG_BASE_CORE_TSK_LIMIT];//访问图,指的是task之间是否能访问的标识,LOSCFG_BASE_CORE_TSK_LIMIT 为线程池总数
#endif
```
ipc解决线程间通讯问题
__liteipc是鸿蒙专有的通讯消息队列实现.简单说是基于文件的，而传统的ipc消息队列是基于内存的__.

__六、辅助工具__

```C
#ifdef LOSCFG_KERNEL_CPUP
    OsCpupBase      taskCpup;           /**< task cpu usage */
#endif
    INT32           errorNo;            /**< Error Num */
```

