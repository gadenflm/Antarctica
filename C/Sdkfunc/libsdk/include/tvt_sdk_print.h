#ifndef _TVT_SDK_PRINT_H_
#define _TVT_SDK_PRINT_H_

#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>

#ifdef SDK_DEBUG
#define TIPC_PRINTF(fmt,...)\
{ \
    printf("[TIPC_SDK]<<" #fmt ">> @%s,%d\n",##__VA_ARGS__,__FILE__,__LINE__);\
    fflush(stdout);  \
}

#define TIPC_PRINTF_INFO(fmt,...)\
{ \
    printf("[TIPC_SDK_INFO]<<" #fmt ">> @%s,%s,%d\n",##__VA_ARGS__,__FUNCTION__,__FILE__,__LINE__);\
    fflush(stdout); \
}
#else
#define TIPC_PRINTF(fmt,...)
#define TIPC_PRINTF_INFO(fmt,...)
//#define TIPC_PRINTF_ERR(fmt,...)
#endif
#define TIPC_PRINTF_ERR(fmt,...)\
{ \
    printf("[TIPC_SDK_ERR]<<" #fmt ">> @%s,%s,%d\n",##__VA_ARGS__,__FUNCTION__,__FILE__,__LINE__);\
    fflush(stdout);\
}

// release也打印但不是error的情况用以下打印，这样有error时搜TIPC_SDK_ERR能快速查看
#define TIPC_PRINTF_CHK(fmt,...)\
{ \
    printf("[TIPC_SDK_CHK]<<" #fmt ">> @%s,%s,%d\n",##__VA_ARGS__,__FUNCTION__,__FILE__,__LINE__);\
    fflush(stdout);\
}



#define TIPC_MUTEX_LOCK(_lock, _arg0, _arg1, _arg2) \
    do \
    {\
        if((int)_arg2){TIPC_PRINTF_CHK("================Lock %s seq %u======================", (char*)_arg0, *((unsigned int*)(_arg1)));} \
        pthread_mutex_lock((pthread_mutex_t *)(_lock)); \
        (void)_arg0; \
        (void)_arg1; \
    } while(0)
#define TIPC_MUTEX_UNLOCK(_lock, _arg0, _arg1, _arg2) \
    do \
    {\
        pthread_mutex_unlock((pthread_mutex_t *)(_lock)); \
        if((int)_arg2){TIPC_PRINTF_CHK("================UnLock %s seq %u======================", (char*)_arg0, *((unsigned int*)(_arg1)));*((unsigned int*)(_arg1))+=1;} \
        (void)_arg0; \
        (void)_arg1; \
    } while(0)

// wkl add:

#define WKL_DEBUG 0

#endif