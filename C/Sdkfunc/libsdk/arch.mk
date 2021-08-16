#makefile依赖的变量库

COMPILE_FLAG	:= -DSYS_USED_64ADDRESS -D${CHIP_PLATFORM} -D${PRODUCT_NAME}
# TOOLCHAIN   	:= 
#DEBUG          := 

#公共头文件
# export PUBINC 	:= ./pub-inc 
#公共库文件
# export PUBLIB 	:= ./pub-lib 

#输出位置
export EXP_OUTPUT_DIR 	:= ./output/
export EXP_OBJ_DIR 	    := ./obj/

#软件版本信息
# export SDK_VERSION = V1.0.0.1_build_${CURRENT_TIME}
SDK_PLATFORM_VERSION=3
SDK_PATCH_VERSION=3
SDK_COMMFUNC_VERSION=1
SDK_BUILD_VERSION=45
SDK_VERSION_FLAG = -DSDK_PLATFORM_VERSION=$(SDK_PLATFORM_VERSION) -DSDK_COMMFUNC_VERSION=$(SDK_COMMFUNC_VERSION) -DSDK_PATCH_VERSION=$(SDK_PATCH_VERSION) -DSDK_BUILD_VERSION=$(SDK_BUILD_VERSION)
CURRTIME_FLAG = -DBUILD_TIME=\"${CURRENT_TIME}\"

#编译参数
# DEFINE 从环境变量获取
export CFLAGS	= -shared -fPIC -Wall -O3 -Wno-unknown-pragmas -Wno-format -fno-strict-aliasing -Wunused-function -std=c99 \
                  -fgnu89-inline -D_GNU_SOURCE ${SDK_VERSION_FLAG} ${COMPILE_FLAG} ${CURRTIME_FLAG}
export CXXFLAGS	= -shared -fPIC -Wall -O3 -Wno-unknown-pragmas -Wno-format -fno-strict-aliasing -Wunused-function -std=c++11 \
                  -Wno-reorder -Wno-sign-compare -Wno-write-strings ${SDK_VERSION_FLAG} ${COMPILE_FLAG} ${CURRTIME_FLAG}

#编译器工具配置
export CC		= $(TOOLCHAIN)gcc
export CXX		= $(TOOLCHAIN)g++
export AR		= $(TOOLCHAIN)ar
export AS		= $(TOOLCHAIN)as
export LD		= $(TOOLCHAIN)ld
export TP		= $(TOOLCHAIN)strip

#shell操作命令配置
export CP		= cp
export MV		= mv
export RM		= rm -vf
export INSTALL	= cp -rf
export LN_S		= ln -s
