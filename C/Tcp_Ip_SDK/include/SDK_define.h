/**
 * Author: K_gadenflm
 * Description: header for pre define in sdk.
 */
#ifndef _SDK_DEFINE_H_
#define _SDK_DEFINE_H_

#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>

#define WKL_SUCC 0
#define WKL_FAIL 1

#define WKL_PRINTI(fmt,...)\
{ \
    printf("[WKL_SDK INFO] " #fmt " \t@%s,%d\n",##__VA_ARGS__,__FILE__,__LINE__);\
    fflush(stdout);  \
}

#define WKL_PRINTE(fmt,...)\
{ \
    printf("[WKL_SDK ERROR] " #fmt " \t@%s,%d\n",##__VA_ARGS__,__FILE__,__LINE__);\
    fflush(stdout);  \
}

#define WKL_PRINTD(fmt,...)\
{ \
    printf("[WKL_SDK DEBUG] " #fmt " \t@%s,%d\n",##__VA_ARGS__,__FILE__,__LINE__);\
    fflush(stdout);  \
}

#endif