#! /bin/sh
set -e

#检查arch.mk文件存在
if [ ! -e ./arch.mk ];then ERR "\"./arch.mk\" is not exist!";exit 1;fi
#获取当前时间
CURRENT_TIME=`date +"%Y-%m-%d_%H:%M:%S"`

echo ' __________________________________________________________'
echo '|                      Compile Start                       |'
echo '|__________________________________________________________|'
echo time:$CURRENT_TIME
export CURRENT_TIME=$CURRENT_TIME
echo '-------------------------Check-----------------------------|'

#检查设置工具链

#检查平台
CHIP=""
if [ "ambarellax" == "${PLATFORM_NAME}x" ];then
    CHIP=Amba
    export CHIP_PLATFORM=CHIP_AMBA
fi

# wkl add
# echo ${PLATFORM_NAME}

#检查设置线程数
echo ">>>>>>>check cpu core number"
CPU_CORE_NUMBER=$(grep -c ^processor /proc/cpuinfo 2>/dev/null)
echo "cpu core numbre = ${CPU_CORE_NUMBER}"

echo '-------------------------Ready-----------------------------|'

#obj存在则清空内容 不存在则创建
echo ">>>>>>>ready obj"
if [ -d ./obj ];then rm -rf ./obj/*;else mkdir ./obj;fi

#lib存在则清空内容 不存在则创建
echo ">>>>>>>ready output"
if [ -d ./output ];then rm -rf ./output/*;else mkdir ./output;fi

echo '---------------------Make libsdk--------------------------|'
echo ">>>>>>>make hardware"
cd hardware; make clean -f Makefile.${CHIP}; make -j${CPU_CORE_NUMBER} -f Makefile.${CHIP};cd ..;
echo ">>>>>>>make service"
cd service;make clean;make -j${CPU_CORE_NUMBER};cd ..;
echo ">>>>>>>make main"
make clean -f Makefile.${CHIP}; make -j${CPU_CORE_NUMBER} -f Makefile.${CHIP};

# echo ">>>>>>>make demo"
# cd demo; make clean; make -j${CPU_CORE_NUMBER}; cd ..;

echo ' __________________________________________________________'
echo '|                      Compile End                         |'
echo '|__________________________________________________________|'
