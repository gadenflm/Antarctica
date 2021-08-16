[TOC]
# 标准库 `<signal.h>`

<!-- TODO -->

# Linux 下线程库 libpthread `pthread.h`

## 函数接口

#### 线程创建 `pthread_create()`

```C
pthread_create(pthread_t *thread, const pthread_attr_t *attr, void * (start_routine)(void*), void *arg);

// 测试例子：
#include <pthread.h>
#include <unistd.h>     // 提供 sleep 等标准函数

struct  thread_ctrl_st
{
    uint8_t     run_flag;
    pthread_t   th_id;
};

static struct thread_ctrl_st th_ctrl = {0};

void* sample_thread(void* args)
{
//  ...
}

int main()
{
    th_ctrl.run_flag = 1;
    int err = pthread_create(&th_ctrl.th_id, NULL, sample_thread, (void*) NULL);
    if (err) {
        printf("Error,[%s]\n", strerror(err));
    }
}
```

* 参数一`pthread_t *` 为 `unsigned long int *`,申明线程id。__(注意为指针类型，申明`pthread_t`后用`&`获得)__
* 参数二`const pthread_attr_t *` 用来指定创建的该线程的属性，提前设定完毕后传入。具体参数如下。 __(无需配置时，传入`NULL`即可)__
```C 
typedef struct __pthread_attr_s
{
    int __detachstate;    //设置可取消属性 

    int __schedpolicy;    //调度策略

    struct __sched_param __schedparam;

    int __inheritsched;

    int __scope;

    size_t __guardsize;
    int __stackaddr_set;

    void *__stackaddr;
    size_t __stacksize;   //表示堆栈的大小。

}pthread_attr_t; 

```
* 参数三`void * (start_routine)(void*)` 为线程入口函数, __注意返回类型为`void*`,参数也为`void*`!(申明线程入口函数时检查函数返回类型)__
* 参数四` void *arg` 为传入入口函数的参数，提前设定完毕后传入。


## 常用例
#### 线程命名 
```C
include <sys/prctl.h>
void thread_func(void* args){
    prctl(PR_SET_NAME,"SampleThreadName");
    //......;
}

int main(int argc, char** argv){
    int ret=pthread_create(id,NULL,thread_func,arg);
    //......;
}
```
