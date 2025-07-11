#ifndef _BSP_OM_H_
#define _BSP_OM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <mach/common/bsp_version.h>
#include "BSP.h"

/* 单板名宏定义 */
#define BOARD_MAINNAME_LEN  32
#define BOARD_SUBNAME_LEN   16

#define BOARD_TYPE_REG_ADDR (0x5016c000)

#define VERSION_INFO_NAME_LEN 16

extern const BSP_CHAR BoardMainName[8][BOARD_MAINNAME_LEN];
extern const BSP_CHAR BoardSubName[4][BOARD_SUBNAME_LEN];


typedef enum {
	BOOT_FAST	= 0x04,
	BOOT_SLOW	= 0x02
}ENBOOTFLAG;


typedef enum tagUE_SW_BUILD_VER_PRODUCT_ENUM {
	UE_SW_BUILD_VER_PRODUCT_FPGA	= 0,
	UE_SW_BUILD_VER_PRODUCT_ASIC	= 1,
	UE_SW_BUILD_VER_PRODUCT_PORTING,
	UE_SW_BUILD_VER_PRODUCT_CPE,
	// TODO ...
	UE_SW_BUILD_VER_PRODUCT_UNKNOWN = 0xFFFFFFFF
} UE_SW_BUILD_VER_PRODUCT_ENUM;

typedef enum tagBSP_MEMORY_FLASH_OWNER_E {
	BSP_MEMORY_FLASH_OWNER_BOOTLOAD = 0,
#ifdef BSP_IMAGE_DBLVXWORKS
	BSP_MEMORY_FLASH_OWNER_VXWORKS_A,
	BSP_MEMORY_FLASH_OWNER_VXWORKS_B,
#else
	BSP_MEMORY_FLASH_OWNER_NV_LTE,
	BSP_MEMORY_FLASH_OWNER_NV_GU,
	BSP_MEMORY_FLASH_OWNER_BOOTROM_A,
	BSP_MEMORY_FLASH_OWNER_BOOTROM_B,
	BSP_MEMORY_FLASH_OWNER_VXWORKS_MCORE,
	BSP_MEMORY_FLASH_OWNER_VXWORKS_ACORE,
#endif
	BSP_MEMORY_FLASH_OWNER_YAFFS_MCORE,
	BSP_MEMORY_FLASH_OWNER_YAFFS_ACORE,
	BSP_MEMORY_FLASH_OWNER_MAX
}BSP_MEMORY_FLASH_OWNER_E;

/*2010-08-26 wangxuesong added for Hi6910 begin*/
typedef enum tagCTRLCLOCK_TYPE_E {
	CTRLCLOCK_AXIMEMORY = 0,
	CTRLCLOCK_CEVADATA,
	CTRLCLOCK_CEVAPROGRAM,
	CTRLCLOCK_AHBSLAVE,
	CTRLCLOCK_APBSLAVE,
	CTRLCLOCK_MEMORY,
	CTRLCLOCK_ALL,
	CTRLCLOCK_TYPE_MAX
}CTRLCLOCK_TYPE_E;

typedef enum tagCTRLCLOCK_STATUS_E {
	CTRLCLOCK_NOAUTOOFF	= 0,
	CTRLCLOCK_AUTOOFF	= 1,
	CTRLCLOCK_STATUS_MAX
}CTRLCLOCK_STATUS_E;
/*2010-08-26 wangxuesong added for Hi6910 end*/


typedef struct tagOM_VERSION_INFO_S {
	BSP_CHAR	stAsic[VERSION_INFO_NAME_LEN];
	BSP_CHAR	stPlat[VERSION_INFO_NAME_LEN];
	BSP_CHAR	stProduct[VERSION_INFO_NAME_LEN];
	BSP_CHAR	stBoard[VERSION_INFO_NAME_LEN];
	BSP_CHAR	stCore[VERSION_INFO_NAME_LEN];
	BSP_CHAR	stCompile[VERSION_INFO_NAME_LEN];
	BSP_CHAR	stImageType[VERSION_INFO_NAME_LEN];
}OM_VERSION_INFO_S;

extern BSP_VOID BSP_OM_ShutDown(BSP_VOID);  //clean warning
extern BSP_S32 BSP_OM_SetBootFlag(ENBOOTFLAG enBootFlag);

BSP_VOID BSP_InitBootVersion(VOID);
BSP_S32 BSP_InitBspVersion(VOID);
const UE_SW_BUILD_VER_INFO_STRU *BSP_GetBootBuildVersion(VOID);
const UE_SW_BUILD_VER_INFO_STRU *BSP_GetBuildVersion(VOID);

/*****************************************************************************
* 函 数 名  : BSP_OM_GetFlashDistribution
*
* 功能描述  : 获取系统FLASH空间划分
*
* 输入参数  : BSP_MEMORY_FLASH_OWNER_E eFlashOwner  : FLASH所有者
*
* 输出参数  : BSP_U32 *pu32StartAddr                : 起始地址
*             BSP_U32 *pu32EndAddr                  : 结束地址
*
* 返 回 值  : BSP_ERROR:获取失败
*             BSP_OK:获取成功
*
* 其它说明  : 无
*
*****************************************************************************/
BSP_S32 BSP_OM_GetFlashDistribution(BSP_MEMORY_FLASH_OWNER_E eFlashOwner, BSP_U32 *pu32StartAddr, BSP_U32 *pu32EndAddr);

/*****************************************************************************
* 函 数 名  : BSP_CtrlClock_AutoClose
*
* 功能描述  : 设置时钟门控是否自动打开开关
*
* 输入参数  : enClockType    时钟类型
*             enClockStatus  时钟门控开关状态
*
* 输出参数  :
*
* 返 回 值  : OK
*             ERROR
*
* 其它说明  : 无
*
*****************************************************************************/
BSP_S32 BSP_CtrlClock_AutoClose(CTRLCLOCK_TYPE_E enClockType, CTRLCLOCK_STATUS_E enClockStatus);

#ifdef __cplusplus
}
#endif



#endif //end of "#ifndef _BSP_OM_H_"
