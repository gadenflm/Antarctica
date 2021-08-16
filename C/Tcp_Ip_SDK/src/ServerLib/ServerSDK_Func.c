/**
 * Author: K_gadenflm
 */
#include "ServerSDK_Func.h"
#include "Socket_Func.h"
#include "CfgIO.h"

#include <stdlib.h>

int ServerInit (void) {
    int ret = 0;

    ret += CfgInit();

    if (ret) {
        WKL_PRINTE("ServerTest ERROR!");
        return WKL_FAIL;
    }
    return WKL_SUCC;
}

int ServerTest (void) {
    int ret = 0;

    ret += ServerSocketInit();
    ret += ServerSocketRelease();

    if (ret) {
        WKL_PRINTE("ServerTest ERROR!");
        return WKL_FAIL;
    }
    WKL_PRINTI("Server Test OK!");
    return WKL_SUCC;
}

