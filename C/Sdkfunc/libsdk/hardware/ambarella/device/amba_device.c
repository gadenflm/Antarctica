#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include "linux/rtc.h"
#include <termios.h>

#include "amba_device.h"
#include "amba_debug.h"

#ifndef AM_IOCTL
#define AM_IOCTL(_filp, _cmd, _arg)	\
        do {                        \
            if (ioctl(_filp, _cmd, _arg) < 0) { \
                perror(#_cmd);      \
                TIPC_PRINTF_ERR("%s err [%d]", #_cmd, errno); \
                return -1;      \
            }                       \
        } while (0)
#endif

#define AMBA_FILE_STR_MAX_SIZE 64
#define AMBA_RTC_DEV "/dev/rtc0"
#define AMBA_AMBAD_DEV "/dev/ambad"
#define YEAR_BASE   (1900)
#define MONTH_BASE  (1)

#define AMBA_PWM_MIN_PERIOD_NS 1000 //最小周期,单位ns

#define AMBA_UART_NUM 2


/**
 * @brief 串口配置信息
 */
typedef struct amba_dev_uart_s
{
    int              s32UartNum;        ///< 所在串口号
    int              s32Fd;             ///< 文件描述符
}AMBA_DEV_UART_S;

typedef struct amba_dev_cfg_s
{
    int              s32AmbadFd;
    pthread_mutex_t  s32AmbadMtx;
    int              s32RtcFd;
    pthread_mutex_t  s32RtcMtx;
    AMBA_DEV_UART_S  stUartInfo[AMBA_UART_NUM];
}AMBA_DEV_CFG_S;


AMBA_DEV_CFG_S g_stDevCfg = {0};


/**
 * @brief SysFs 写入
 * @return 0：成功；非0：失败
 */
static inline int amba_dev_writeFile(char* pszFileName, char* pszBuf)
{
    int s32Fd = -1;
    int s32Ret = 0;
    if(NULL == pszFileName || NULL == pszBuf)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    s32Fd = open(pszFileName, O_WRONLY);
    if (s32Fd < 0) 
    {
        TIPC_PRINTF_ERR("Can't open export sys file[%s]!\n", pszFileName);
        return -1;
    }

    s32Ret = write(s32Fd, pszBuf, strlen(pszBuf));
    if(s32Ret < 0)
    {
        TIPC_PRINTF_ERR("write [%s] to file[%s] failed[%d]\n", s32Ret);
    }
    close(s32Fd);

    return s32Ret;
}

/**
 * @brief 打开RTC
 * @return 0：成功；非0：失败
 */
static int amba_dev_openRtc()
{
    g_stDevCfg.s32RtcFd = open(AMBA_RTC_DEV, O_RDWR);
    if(g_stDevCfg.s32RtcFd <= 0)
    {
        TIPC_PRINTF_ERR("rtc device open failed [%d]", errno);
        return -1;
    }
    pthread_mutex_init(&g_stDevCfg.s32RtcMtx, NULL);
    return 0;
}

/**
 * @brief 关闭RTC
 * @return 0：成功；非0：失败
 */
static int amba_dev_closeRtc()
{
    if(g_stDevCfg.s32RtcFd > 0)
    {
        close(g_stDevCfg.s32RtcFd);
        g_stDevCfg.s32RtcFd = -1;
        pthread_mutex_destroy(&g_stDevCfg.s32RtcMtx);
    }
    return 0;
}

/**
 * @brief 关闭ambad设备
 * @return 0：成功；非0：失败
 */
static int amba_dev_closeAmbad()
{
    if(g_stDevCfg.s32AmbadFd > 0)
    {
        close(g_stDevCfg.s32AmbadFd);
        g_stDevCfg.s32AmbadFd = -1;
        pthread_mutex_destroy(&g_stDevCfg.s32AmbadMtx);
    }
    return 0;
}

/**
 * @brief 打开ambad设备
 * @return 0：成功；非0：失败
 */
static int amba_dev_openAmbad()
{
    int s32DebugFlag = 0;
    g_stDevCfg.s32AmbadFd = open(AMBA_AMBAD_DEV, O_RDWR, 0);
    if(g_stDevCfg.s32AmbadFd <= 0)
    {
        TIPC_PRINTF_ERR("ambad device open failed [%d]", errno);
        return -1;
    }
    pthread_mutex_init(&g_stDevCfg.s32AmbadMtx, NULL);
    if (ioctl(g_stDevCfg.s32AmbadFd, AMBA_DEBUG_IOC_GET_DEBUG_FLAG, &s32DebugFlag) < 0) 
    {
        perror("AMBA_DEBUG_IOC_GET_DEBUG_FLAG");
        TIPC_PRINTF_CHK("AMBA_DEBUG_IOC_GET_DEBUG_FLAG err [%d]\n", errno);
        amba_dev_closeAmbad();
        return -1;
    }
    TIPC_PRINTF_CHK("debug_flag 0x%08x\n", s32DebugFlag);
    return 0;
}

/**
 * @brief 关闭所有设备
 * @return 0：成功；非0：失败
 */
static int amba_dev_closeUart()
{
    int i = 0;
    for(i = 0; i < AMBA_UART_NUM; i++)
    {
        if(g_stDevCfg.stUartInfo[i].s32Fd > 0)
        {
            close(g_stDevCfg.stUartInfo[i].s32Fd);
            g_stDevCfg.stUartInfo[i].s32Fd = -1;
            g_stDevCfg.stUartInfo[i].s32UartNum = -1;
        }
    }
    return 0;
}

/**
 * @brief 芯片外设打开串口描述符
 * @return 0：成功；非0：失败
 */
static int amba_dev_openUartFd(int s32UartNum, int* ps32Fd)
{
    int s32Ret = 0;
    int s32Fd = -1;
    char szFileName[AMBA_FILE_STR_MAX_SIZE] = {0};
    sprintf(szFileName, "/dev/ttyS%d", s32UartNum);
    s32Fd = open(szFileName, O_RDWR);
    if(s32Fd < 0)
    {
        TIPC_PRINTF_ERR("cannot open uart dev[%d] [%d]\n", s32UartNum, s32Fd);
        return -1;
    }
    struct termios stTermios;
    s32Ret = tcgetattr(s32Fd, &stTermios);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tcgetattr error[%d]", s32Ret);
        close(s32Fd);
        return -1;
    }
    
    stTermios.c_oflag &= ~OPOST;
    s32Ret = tcsetattr(s32Fd, TCSANOW, &stTermios);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR( "set TCSANOW error [%d]", s32Ret);
        close(s32Fd);
        return -1;
    }

    *ps32Fd = s32Fd;
    return 0;
}

/**
 * @brief 芯片外设关闭串口描述符
 * @return 0：成功；非0：失败
 */
// static int chip_dev_closeUartFd(int s32UartNum)
// {
//     int i = 0;
//     for(i = 0; i < AMBA_UART_NUM; i++)
//     {
//         if((s32UartNum == g_stDevCfg.stUartInfo[i].s32UartNum) &&
//             (g_stDevCfg.stUartInfo[i].s32Fd > 0))
//         {
//             close(g_stDevCfg.stUartInfo[i].s32Fd);
//             g_stDevCfg.stUartInfo[i].s32Fd = -1;
//         }
//     }
//     return 0;
// }

/**
 * @brief 芯片外设获取串口描述符
 * @return 0：成功；非0：失败
 */
static int amba_dev_getUartFd(int s32UartNum, int* ps32Fd)
{
    int i = 0;
    int s32Ret = 0;
    int s32Fd = -1;
    for(i = 0; i < AMBA_UART_NUM; i++)
    {
        if(s32UartNum == g_stDevCfg.stUartInfo[i].s32UartNum)
        {
            if(g_stDevCfg.stUartInfo[i].s32Fd <= 0)
            {
                s32Ret = amba_dev_openUartFd(s32UartNum, &s32Fd);
                if(s32Ret != 0)
                {
                    return -1;
                }
                g_stDevCfg.stUartInfo[i].s32Fd = s32Fd;
                g_stDevCfg.stUartInfo[i].s32UartNum = s32UartNum;
                *ps32Fd = s32Fd;
                return 0;
            }
            else
            {
                *ps32Fd = g_stDevCfg.stUartInfo[i].s32Fd;
            }
            break;
        }
    }
    if(i >= AMBA_UART_NUM)
    {
        int j = 0;
        // 都没找到对应的描述符, 找空闲位置新建
        for(j = 0; j < AMBA_UART_NUM; j++)
        {
            if(g_stDevCfg.stUartInfo[j].s32Fd <= 0)
            {
                s32Ret = amba_dev_openUartFd(s32UartNum, &s32Fd);
                if(s32Ret != 0)
                {
                    return -1;
                }
                g_stDevCfg.stUartInfo[j].s32Fd = s32Fd;
                g_stDevCfg.stUartInfo[j].s32UartNum = s32UartNum;
                *ps32Fd = s32Fd;
                return 0;
            }
        }
        if(j >= AMBA_UART_NUM)
        {
            // 已占满
            TIPC_PRINTF_ERR("reach max open uart num\n");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief 芯片外设模块获取RTC时间
 * @return 0：成功；非0：失败
 */
int chip_dev_getRtcTime(CHIP_DEV_RTC_CFG_S* pstRtcInfo)
{
    int s32Ret = 0;
    if(NULL == pstRtcInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    pstRtcInfo->u32Year = 1970;
    pstRtcInfo->u32Month = 1;
    pstRtcInfo->u32Date = 1;
    pstRtcInfo->u32Hour = 0;
    pstRtcInfo->u32Minute = 0;
    pstRtcInfo->u32Second = 0;
    pstRtcInfo->u32Weekday = 1;
    if(g_stDevCfg.s32RtcFd <= 0)
    {
        s32Ret = amba_dev_openRtc();
        if(s32Ret != 0)
        {
            return -1;
        }
    }
    pthread_mutex_lock(&g_stDevCfg.s32RtcMtx);
    struct rtc_time stRtcTime = {0};
    s32Ret = ioctl(g_stDevCfg.s32RtcFd, RTC_RD_TIME, &stRtcTime);
    if(s32Ret < 0)
    {
        perror("RTC_RD_TIME");
        TIPC_PRINTF_ERR("get RTC time failed [%d]", errno);
        pthread_mutex_unlock(&g_stDevCfg.s32RtcMtx);
        return -1;
    }
    /// TODO: 需要验证下是否从1900算起的
    pstRtcInfo->u32Year = stRtcTime.tm_year + YEAR_BASE;
    pstRtcInfo->u32Month = stRtcTime.tm_mon + MONTH_BASE;
    pstRtcInfo->u32Date = stRtcTime.tm_mday;
    pstRtcInfo->u32Hour = stRtcTime.tm_hour;
    pstRtcInfo->u32Minute = stRtcTime.tm_min;
    pstRtcInfo->u32Second = stRtcTime.tm_sec;
    pstRtcInfo->u32Weekday = stRtcTime.tm_wday;
    pthread_mutex_unlock(&g_stDevCfg.s32RtcMtx);
    return 0;
}

/**
 * @brief 芯片外设模块获取RTC时间
 * @return 0：成功；非0：失败
 */
int chip_dev_setRtcTime(CHIP_DEV_RTC_CFG_S* pstRtcInfo)
{
    int s32Ret = 0;
    if(NULL == pstRtcInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if(g_stDevCfg.s32RtcFd <= 0)
    {
        s32Ret = amba_dev_openRtc();
        if(s32Ret != 0)
        {
            return -1;
        }
    }
    pthread_mutex_lock(&g_stDevCfg.s32RtcMtx);
    struct rtc_time stRtcTime = {0};
    stRtcTime.tm_year = pstRtcInfo->u32Year - YEAR_BASE;
    stRtcTime.tm_mon = pstRtcInfo->u32Month - MONTH_BASE;
    stRtcTime.tm_mday = pstRtcInfo->u32Date;
    stRtcTime.tm_hour = pstRtcInfo->u32Hour;
    stRtcTime.tm_min = pstRtcInfo->u32Minute;
    stRtcTime.tm_sec = pstRtcInfo->u32Second;
    stRtcTime.tm_wday = pstRtcInfo->u32Weekday;
    s32Ret = ioctl(g_stDevCfg.s32RtcFd, RTC_SET_TIME, &stRtcTime);
    if(s32Ret < 0)
    {
        perror("RTC_SET_TIME");
        TIPC_PRINTF_ERR("set RTC time failed [%d]", errno);
        pthread_mutex_unlock(&g_stDevCfg.s32RtcMtx);
        return -1;
    }
    pthread_mutex_unlock(&g_stDevCfg.s32RtcMtx);
    return 0;
}

/**
 * @brief 芯片外设GPIO输入输出配置
 * @return 0：成功；非0：失败
 */
int chip_dev_setGpioDir(int s32GpioGrp, int s32GpioNum, int s32Dir, int s32DefVal)
{
    int s32Ret = 0;
    if(g_stDevCfg.s32AmbadFd <= 0)
    {
        s32Ret = amba_dev_openAmbad();
        if(s32Ret != 0)
        {
            return -1;
        }
    }
    // s32Dir: 0: input 1: ouput
    struct amba_vin_test_gpio stGpio = {0};
    stGpio.id = s32GpioNum;
    stGpio.data = (s32DefVal > 0)?1:0;
    if(0 == s32Dir)
    {
        s32Ret = ioctl(g_stDevCfg.s32AmbadFd, AMBA_DEBUG_IOC_GET_GPIO, &stGpio);
    }
    else
    {
        s32Ret = ioctl(g_stDevCfg.s32AmbadFd, AMBA_DEBUG_IOC_SET_GPIO, &stGpio);
    }
    if(s32Ret < 0)
    {
        perror("GPIO CTRL");
        TIPC_PRINTF_ERR("GPIO CTRL failed [%d]", errno);
        return -1;
    }
    return 0;
}

/**
 * @brief 芯片外设获取GPIO值
 * @return 0：成功；非0：失败
 */
int chip_dev_getGpioVal(int s32GpioGrp, int s32GpioNum, int* ps32Val)
{
    int s32Ret = 0;
    if(NULL == ps32Val)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
    }
    if(g_stDevCfg.s32AmbadFd <= 0)
    {
        s32Ret = amba_dev_openAmbad();
        if(s32Ret != 0)
        {
            return -1;
        }
    }
    struct amba_vin_test_gpio stGpio = {0};
    stGpio.id = s32GpioNum;
    s32Ret = ioctl(g_stDevCfg.s32AmbadFd, AMBA_DEBUG_IOC_GET_GPIO, &stGpio);
    if(s32Ret < 0)
    {
        perror("AMBA_DEBUG_IOC_GET_GPIO");
        TIPC_PRINTF_ERR("AMBA_DEBUG_IOC_GET_GPIO failed [%d]", errno);
        return -1;
    }
    *ps32Val = stGpio.data;
    return 0;
}

/**
 * @brief 芯片外设设置GPIO值
 * @return 0：成功；非0：失败
 */
int chip_dev_setGpioVal(int s32GpioGrp, int s32GpioNum, int s32Val)
{
    int s32Ret = 0;

    if(g_stDevCfg.s32AmbadFd <= 0)
    {
        s32Ret = amba_dev_openAmbad();
        if(s32Ret != 0)
        {
            return -1;
        }
    }
    struct amba_vin_test_gpio stGpio = {0};
    stGpio.id = s32GpioNum;
    stGpio.data = (s32Val > 0)?1:0;
    s32Ret = ioctl(g_stDevCfg.s32AmbadFd, AMBA_DEBUG_IOC_SET_GPIO, &stGpio);
    if(s32Ret < 0)
    {
        perror("AMBA_DEBUG_IOC_SET_GPIO");
        TIPC_PRINTF_ERR("AMBA_DEBUG_IOC_SET_GPIO failed [%d]", errno);
        return -1;
    }
    return 0;
}


/**
 * @brief 芯片外设设置PWM周期占空比
 * @param [in] s32PwmNum:    pwm设备号
 * @param [in] s32PwmPeriod: 设置周期
 * @param [in] s32PwmDuty:   设置占空比
 * @return 0:成功;非0:失败
 * @return 0：成功；非0：失败
 */
int chip_dev_setPwmInfo(int s32PwmNum, int s32PwmPeriod, int s32PwmDuty)
{
    int s32Ret = -1;
    char szCmd[64] = {0};
    char szVal[16] = {0};

    // 检查参数
    if(s32PwmPeriod < AMBA_PWM_MIN_PERIOD_NS)
    {
        TIPC_PRINTF_ERR("input Period [%d] should not less than min value [%d]", s32PwmPeriod, AMBA_PWM_MIN_PERIOD_NS);
        return -1;
    }

    if(s32PwmPeriod < s32PwmDuty)
    {
        TIPC_PRINTF_ERR("input duty[%d] must less than period[%d]", s32PwmDuty, s32PwmPeriod);
        return s32Ret;
    }

    // 先关闭PWM
    memset(szCmd, 0, sizeof(szCmd));
    sprintf(szCmd, "/sys/class/pwm/pwmchip0/pwm%d/enable", s32PwmNum);
    s32Ret = amba_dev_writeFile(szCmd, "0");
    if(s32Ret < 0)
    {
        return s32Ret;
    }

    // 修改占空比为0, 防止新周期小于之前占空比导致写入失效
    memset(szCmd, 0, sizeof(szCmd));
    sprintf(szCmd, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", s32PwmNum);
    s32Ret = amba_dev_writeFile(szCmd, "0");
    if(s32Ret < 0)
    {
        return s32Ret;
    }

    // 修改周期
    memset(szCmd, 0, sizeof(szCmd));
    sprintf(szCmd, "/sys/class/pwm/pwmchip0/pwm%d/period", s32PwmNum);
    memset(szVal, 0, sizeof(szVal));
    sprintf(szVal, "%d", s32PwmPeriod);
    s32Ret = amba_dev_writeFile(szCmd, szVal);
    if(s32Ret < 0)
    {
        return s32Ret;
    }

    // 修改占空比
    memset(szCmd, 0, sizeof(szCmd));
    sprintf(szCmd, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", s32PwmNum);
    memset(szVal, 0, sizeof(szVal));
    sprintf(szVal, "%d", s32PwmDuty);
    s32Ret = amba_dev_writeFile(szCmd, szVal);
    if(s32Ret < 0)
    {
        return s32Ret;
    }

    // 重新使能PWM
    memset(szCmd, 0, sizeof(szCmd));
    sprintf(szCmd, "/sys/class/pwm/pwmchip0/pwm%d/enable", s32PwmNum);
    s32Ret = amba_dev_writeFile(szCmd, "1");
    if(s32Ret < 0)
    {
        return s32Ret;
    }

    return 0;
}


/**
 * @brief 芯片外设初始化Uart速率
 * @return 0：成功；非0：失败
 */
int chip_dev_setUartSpeed(int s32UartNum, int s32SpeedIDX)
{
    int s32Ret = 0;
    int s32Fd = -1;
    s32Ret = amba_dev_getUartFd(s32UartNum, &s32Fd);
    if(s32Ret != 0 || s32Fd < 0)
    {
        return -1;
    }

    struct termios stTermiosAttr = {0};
    speed_t s32Speed;
    const int s32UartSpeedTable[] = 
    {
        B110, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200,
    };
    int s32ArraySize = sizeof(s32UartSpeedTable)/sizeof(int);
    if(s32SpeedIDX >= s32ArraySize)
    {
        TIPC_PRINTF_ERR("err uart rate[%d]", s32SpeedIDX);
        return -1;
    }
    s32Speed = s32UartSpeedTable[s32SpeedIDX];

    s32Ret = tcgetattr(s32Fd, &stTermiosAttr);
    if(0 != s32Ret)
    {
        TIPC_PRINTF("tcgetattr failed[%d]", s32Ret);
        return -1;
    }
    cfmakeraw(&stTermiosAttr);
    s32Ret = cfsetispeed(&stTermiosAttr, s32Speed);
    if(0 != s32Ret)
    {
        TIPC_PRINTF("cfsetispeed failed[%d] [%x]", s32Ret, s32Speed);
        return -1;
    }

    s32Ret = cfsetospeed(&stTermiosAttr, s32Speed);
    if(0 != s32Ret)
    {
        TIPC_PRINTF("cfsetospeed failed[%d] [%x]", s32Ret, s32Speed);
        return -1;
    }

    stTermiosAttr.c_cflag |= (CLOCAL|CREAD);
    stTermiosAttr.c_cflag &= ~CRTSCTS;
    stTermiosAttr.c_cflag &= ~CSIZE;
    stTermiosAttr.c_cflag |= CS8;
    stTermiosAttr.c_lflag &= ~(ICANON|ECHO|ECHOE|ISIG);
    stTermiosAttr.c_iflag &= ~(INLCR|ICRNL);
    stTermiosAttr.c_iflag &= ~(IXON);
    stTermiosAttr.c_oflag &= ~OPOST;
    stTermiosAttr.c_cc[VTIME] = 0;
    stTermiosAttr.c_cc[VMIN] = 0;
    stTermiosAttr.c_cflag &= ~PARENB;
    stTermiosAttr.c_cflag &= ~CSTOPB;
    
    tcflush(s32Fd, TCIFLUSH);

    s32Ret = tcsetattr(s32Fd, TCSANOW, &stTermiosAttr);
    if(0 != s32Ret)
    {
        TIPC_PRINTF("tcsetattr failed[%d]", s32Ret);
        return -1;
    }

    return s32Ret;
}


/**
 * @brief 芯片外设获取(读取)Uart数据
 * @return 0：成功；非0：失败
 */
int chip_dev_getUartData(int s32UartNum, char* pszData, int s32DataLens)
{
    int s32Fd = -1;
    int s32Ret = 0;

    if(NULL == pszData)
    {
        TIPC_PRINTF("NULL Pointer");
        return -1;
    }

    s32Ret = amba_dev_getUartFd(s32UartNum, &s32Fd);
    if(s32Ret != 0 || s32Fd < 0)
    {
        return -1;
    }
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(s32Fd, &readfd);
    
    s32Ret = select(s32Fd + 1, &readfd, NULL, NULL, &tv);
    if (s32Ret == -1)
    {
        TIPC_PRINTF_ERR("select error, fd %d\n", s32Fd);
        return -1;
    }
    else if (s32Ret && FD_ISSET(s32Fd, &readfd))
    {
        return read(s32Fd, pszData, s32DataLens);
    }
    else
    {
        return 0;
    }
   
    return 0;
}


/**
 * @brief 芯片外设设置(写入)Uart数据
 * @return 0：成功；非0：失败
 */
int chip_dev_setUartData(int s32UartNum, const char* pszData, int s32DataLens)
{
    int s32Fd = -1;
    int s32Ret = 0;

    if(NULL == pszData)
    {
        TIPC_PRINTF("NULL Pointer");
        return -1;
    }

    s32Ret = amba_dev_getUartFd(s32UartNum, &s32Fd);
    if(s32Ret != 0 || s32Fd < 0)
    {
        return -1;
    }

    int s32WriteLens = write(s32Fd, pszData, s32DataLens);
    if(s32WriteLens != s32DataLens)
    {
        TIPC_PRINTF_ERR("write uart data warning, len %d, buf len %d\n", s32WriteLens, s32DataLens);
        return -1;
    }

    return 0;
}


/**
 * @brief 芯片外设初始化wiegand
 * @return 0：成功；非0：失败
 */
int chip_dev_initWiegand()
{
    /// TODO: 
    return 0;
}


/**
 * @brief 芯片外设设置wiegand输入/输出模式
 * @return 0：成功；非0：失败
 */
int chip_dev_setWiegandMode(int s32Mode)
{
    /// TODO: 
    return 0;
}

/**
 * @brief 芯片外设设置wiegand位数
 * @return 0：成功；非0：失败
 */
int chip_dev_setWiegandBitNum(int s32BitNum)
{
    /// TODO: 
    return 0;
}

/**
 * @brief 芯片外设获取wiegand数据
 * @return 0：成功；非0：失败
 */
int chip_dev_getWiegandData(CHIP_DEV_WIEGAND_DATA_S* pstWiegand)
{
    /// TODO: 
    return 0;
}

/**
 * @brief 芯片外设设置wiegand数据
 * @return 0：成功；非0：失败
 */
int chip_dev_setWiegandData(const CHIP_DEV_WIEGAND_DATA_S* pstWiegand)
{
    /// TODO: 
    return 0;
}

/**
 * @brief 芯片外设去初始化wiegand
 * @return 0：成功；非0：失败
 */
int chip_dev_exitWiegand()
{
    /// TODO: 
    return 0;
}

/**
 * @brief 芯片外设检查SD卡是否接入
 * @return 0：成功；非0：失败
 */
int chip_dev_getTFCardStatus(const char* pszName, int* ps32Status)
{
    int s32Status = 0;
    if(NULL == pszName || NULL == ps32Status)
    {
        TIPC_PRINTF("NULL Pointer");
        return -1;
    }
    FILE* pFile = fopen("/proc/partitions", "r");
    if(NULL == pFile)
    {
        return -1;
    }
    char szBuf[128] = {0};
    while(0 != fgets(szBuf, sizeof(szBuf), pFile))
    {
        if(strstr(szBuf, pszName))
        {
            fclose(pFile);
            s32Status = 1;
        }
    }
    fclose(pFile);
    *ps32Status = s32Status;
    return 0;
}

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_device_init()
{
    return 0;
}

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_device_exit()
{
    amba_dev_closeAmbad();
    amba_dev_closeRtc();
    amba_dev_closeUart();

    return 0;
}