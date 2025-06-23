#!/bin/bash

export pwd="$(pwd)"
export INCLUDE_FLAG="-I${pwd}/include/linux"
export DRV_INCLUDE_FLAG=-I${pwd}/arch/arm/mach-balong/include 
export ASM_INCLUDE_FLAG="-I${pwd}/arch/arm/include/asm"

export ARCH=arm
export SUBARCH=arm
export CROSS_COMPILE=${pwd}/android_toolchains/arm-2010q1/bin/arm-none-linux-gnueabi-

if ! [ -e android_toolchains ]; then git clone https://github.com/milaq/android_toolchains.git ; fi

export PATH=$CROSS_COMPILE:$PATH
export FEATURE_WIFI=FEATURE_ON


make hisi_balong_hi6920cs_e5776s_defconfig
make  -C ./ -j $(nproc) BOARD_TYPE=BOARD_ASIC VERSION_TYPE=CHIP_BB_6920CS PRODUCT_CFG_BUILD_TYPE= 2>log.txt

if [ 0 -ne $? ]
then
	echo "############################# "
	echo "Make KERNEL Failed!"
	echo "Check log.txt for details"
	echo "#############################"
	exit 1
else
	echo "############################# "
	echo "Make KERNEL Success!"
	echo "#############################"
fi 



