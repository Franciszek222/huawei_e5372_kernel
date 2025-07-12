/*************************************************************************
*Copyright (C) 1987-2004, Shenzhen Huawei Technologies Co., Ltd.
*
*File name: huawei_info.c
*
*Author: Wu Zechun
*
*Description: Hardware version information
*
*Modification record: November 12, 2011 v1.00 Wu Zechun Created
*
*************************************************************************/
/*******************************************************************************************************************************************
 *Date Modifier Question order number Modify content
 ********************************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/string.h>
#include <mach/common/bsp_version.h>
#include <mach/common/bsp_memory.h>
#include "BSP.h"
#include <linux/slab.h>

#include "huawei_info.h"
#include "ptable_def.h"

/*--------------------------------------------------------------*
*Macro definition *
*--------------------------------------------------------------*/

PRODUCT_INFO_T g_huaweiProductInfo = { 0 };                        /*Global variables, save product information*/

const PRODUCT_INFO_T huaweiProduct[] =
{
	/*Product item description, variable meanings are as follows*/
	/*Hardware version number value (large version number 1 + large version number 2, distinguishing different products), sub-version number,
	 *Internal product name, internal product name PLUS, hardware version name, name used in the upgrade, external product name,
	 *The frequency bands supported by the product,
	 *Product supported diversity (not used yet),
	 *Reserved fields
	 */

	/*1. Test board*/
	{ HW_VER_PRODUCT_UDP,					     0, \
	    "UDP", "\0", "UDP", "MPWUDP", "UDP", \
	    { RF_BAND_ALL,						 RF_BAND_ALL, RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },
#if (defined(CHIP_BB_6920CS) && ((FEATURE_EPAD == FEATURE_ON) || (FEATURE_PHONE == FEATURE_ON)))
	/*It is temporary modification for EPAD, and subsequent modifications need to be made according to actual needs.
	 * ͬWhen the hardware version number needs to be defined in DrvInterface.h with macro*/

	{ HW_VER_PRODUCT_S10_TEMP,				     0, \
	    "S10-Temp", "\0", "SH2101UM", "H69DUSAVA", "S10", \
	    { RF_BAND_ALL,						 RF_BAND_ALL, RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_S10_101U,				     0, \
	    "S10-101U", "\0", "SH2101UM", "H69DUSAVA", "S10", \
	    { RF_WCDMA_2100 | RF_WCDMA_900 | RF_GSM_ALL, \
	    RF_BAND_ALL, \
	    RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_S10_102U,				     0, \
	    "S10-102U", "\0", "SH2101UM", "H69DUSAVA", "S10", \
	    { RF_WCDMA_2100 | RF_WCDMA_850 | RF_WCDMA_1900 | RF_GSM_ALL, \
	    RF_BAND_ALL, \
	    RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_S10_101L,				     0, \
	    "S10-101L", "\0", "SH2101UM", "H69DUSAVA", "S10", \
	    { RF_BAND_ALL,						 RF_BAND_ALL, RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_S10_MODEM,				     0, \
	    "S10-Modem", "\0", "SH2101UM", "H69DUSAVA", "S10", \
	    { RF_BAND_ALL,						 RF_BAND_ALL, RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	/*It is temporary modification for EPAD, and subsequent modifications need to be made according to actual needs.
	 * ͬWhen the hardware version number needs to be defined in DrvInterface.h with macro*/
#endif
#if defined(CHIP_BB_6920ES)
	/* 2. Data card */
	{ HW_VER_PRODUCT_E392S_U,				     0, \
	    "E392s", "\0", "CH1E392DM", "CH1E392DM", "E392", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1800 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B8 | RF_LTE_B20, \
		    RF_LTE_B38
	    },
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_E3276S_150,				     0, \
	    "E3276s-150", "\0", "CH1E3276SM", "CH1E3276SM", "E3276", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1900 | RF_WCDMA_1700 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B20, \
		    0
	    },
	    { 0 }, \
	    { 0 } },

	/* E3276s 200 to be implemented*/

	/* A. */
	{ HW_VER_PRODUCT_E5_SBM,				     0, \
	    "E5776s", "\0", "CL1E5776SF", "CL1E5776SF", "102HW", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_J1500, \
		    0, \
		    RF_LTE_B64 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_E5776_EM,				     0, \
	    "E5776s-71", "\0", "CL1E5776SS", "CL1E5776SS", "GL04P", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_J1700,    \
		    RF_LTE_B3, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
#else
	/* 2. Data card */
	{ HW_VER_PRODUCT_E392S_U,				     0, \
	    "E392s", "\0", "CH1E392DM", "B710S0", "E392", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1800 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B8 | RF_LTE_B20, \
		    RF_LTE_B38
	    },
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_E3276S_150,				     0, \
	    "E3276s-150", "\0", "CH1E3276SM", "B710S0", "E3276", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1900 | RF_WCDMA_1700 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B20, \
		    0
	    },
	    { 0 }, \
	    { 0 } },

	/* E3276s 200 to be implemented*/

	/* A. */
	{ HW_VER_PRODUCT_E5_SBM,				     0, \
	    "E5776s", "\0", "CL1E5776SF", "B710D0", "102HW", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_J1500, \
		    0, \
		    RF_LTE_B64 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_E5_CMCC,				     0, \
		/* modified for E5776s-860 begin */
	    "E5776s-860", "\0", "CL3E5776SM", "B710D0", "E5776", \
		/* modified for E5776s-860 end */
	    {
		    RF_WCDMA_2100 | RF_WCDMA_J1700,    \
		    RF_LTE_B3, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	/*BEGIN DST2012092001529 liangshukun 20121025 ADDED*/
	{ HW_VER_PRODUCT_E5_CMCC_CY,				     0, \
	    "E5776Bs-860", "\0", "CL3E5776BSM", "B710D0", "E5776B", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_J1700,    \
		    RF_LTE_B3, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	/*END DST2012092001529 liangshukun 20121025 ADDED*/
	{ HW_VER_PRODUCT_E5776_EM,				     0, \
	    "E5776s-71", "\0", "CL1E5776SS", "B710D0", "GL04P", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_J1700,    \
		    RF_LTE_B3, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	/* Added for E5371_DCM */
	/* E5371 product name changed from E5371S-7 to HW-02E */
	{ HW_VER_PRODUCT_E5371_DCM,				     0, \
	    "HW-02E", "\0", "CL1E5371SM", "B710D0", "HW-02E", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_800 | RF_WCDMA_850 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B19 | RF_LTE_B21, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_E5372_32,				     0, \
	    "E5372s-32", "\0", "CL1E5372SM", "B710D0", "E5372", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1900 | RF_WCDMA_900 | RF_WCDMA_850 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B8 | RF_LTE_B20, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_E5375,					     0, \
	    "E5375", "\0", "CL1E5375SM", "B710D0", "E5375", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1900 | RF_WCDMA_850 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B17 | RF_LTE_B38 | \
		    RF_LTE_B39 | RF_LTE_B40 | RF_LTE_B41, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_E5375_SEC,				     0, \
	    "E5375", "\0", "CL1E5375SS", "B710D0", "E5375", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1900 | RF_WCDMA_850 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B17 | RF_LTE_B38 | \
		    RF_LTE_B39 | RF_LTE_B40 | RF_LTE_B41, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_E5775S_925,				     0, \
	    "E5775s-925", "\0", "CL1E5775SM", "B710D0", "E5775", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B38 | \
		    RF_LTE_B40 | RF_LTE_B41, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_E5372_601,				     0, \
	    "E5372s-601", "\0", "CL1E5372SM02", "B710D0", "E5372", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B40, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_E5372TS_32,				     0, \
	    "E5372TS-32", "\0", "CL1E5372TSM", "B710D0", "E5372T", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_1900 | RF_WCDMA_900 | RF_WCDMA_850 | RF_GSM_ALL, \
		    RF_LTE_B1 | RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B8 | RF_LTE_B20, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_E5372S_22,				     0, \
	    "E5372s-22", "\0", "CL1E5372SM03", "B710D0", "E5372", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B20 | RF_LTE_B38, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
	{ HW_VER_PRODUCT_R215,					     0, \
	    "R215", "\0", "CL1E5372SM", "B710D0", "R215", \
	    {
		    RF_WCDMA_2100 | RF_WCDMA_900 | RF_GSM_ALL, \
		    RF_LTE_B3 | RF_LTE_B7 | RF_LTE_B40, \
		    0 \
	    }, \
	    { 0 }, \
	    { 0 } },
#endif

/* Other boards adapted */
	{ HW_VER_PRODUCT_PV500,					     0, \
	    "PV500", "\0", "PV500", "PV500", "PV500", \
	    { RF_BAND_ALL,						 RF_BAND_ALL, RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	{ HW_VER_PRODUCT_PORTING,				     0, \
	    "PORTING", "\0", "PORTING", "PORTING", "PORTING", \
	    { RF_BAND_ALL,						 RF_BAND_ALL, RF_BAND_ALL }, \
	    { 0 }, \
	    { 0 } },

	/*NULL product*/
	{ 0xFFFF,						     0, \
	    "\0", "\0", "\0", "\0", "\0", \
	    { 0 }, \
	    { 0 }, \
	    { 0 } }
};

/*****************************************************************************
*Function Name: BSP_Flashless_HwGetVerMain
*
*Function description: Get the single board version number in the Flashless product form (large version number 1 + large version number 2)
*
*Input parameters:
*Output parameters:
*
*Return Value: Return the version number of the board
*
*Other instructions:
*
*****************************************************************************/
BSP_U32 BSP_Flashless_HwGetVerMain()
{
	ST_PART_TBL_ST *ram_ptable = (ST_PART_TBL_ST *)PTABLE_RAM_TABLE_ADDR;
	BSP_U32 cnt = 0;

	for (; cnt < PTABLE_PARTITION_MAX; cnt++) {
		if (0 == strcmp(PTABLE_END_STR, ram_ptable->name)) {
			printk("HwVersion is %x, MEMORY_AXI_HW_ID_ADDR is %x\n", ram_ptable->offset, MEMORY_AXI_HW_ID_ADDR);
			return ram_ptable->offset;
		}
		//printk("Ptable name is %s, HwVersion is %x\n", ram_ptable->name, ram_ptable->offset);

		ram_ptable++;
	}

	printk("Can not get HwVersion\n");
	return 0;
}


/*****************************************************************************
*Function Name: BSP_ProductInfoInit
*
*Function description: Initialize product information according to hardware version number
*
*Input parameters:
*Output parameters:
*
*Return value:
*
*Other instructions:
*
*****************************************************************************/
BSP_S32 BSP_ProductInfoInit(BSP_VOID)
{
	BSP_U32 u32HwId;
	BSP_U16 u16IdMain;
	BSP_U8 u16IdSub;
	BSP_U32 i;

	/*Clear the memory of local structure variables, that is, initialize to all 0*/
	memset((void *)(&g_huaweiProductInfo), 0, sizeof(PRODUCT_INFO_T));

	/* Default configuration */
	//g_huaweiProductInfo.index = HW_VER_INVALID;
	//g_huaweiProductInfo.hwIdSub = 0xFF;

	g_huaweiProductInfo.name = "PRODUCT_NAME";
	g_huaweiProductInfo.namePlus = "PRODUCT_NAME_PLUS";
	g_huaweiProductInfo.hwVer = "HW_VER";
	g_huaweiProductInfo.dloadId = "DLOAD_ID";
	g_huaweiProductInfo.productId = "PRODUCT_ID";
	memset(&g_huaweiProductInfo.band, 0x0, sizeof(BAND_INFO_T));

	memset(&g_huaweiProductInfo.bandDiv, 0x0, sizeof(BAND_INFO_T));
#if defined (FEATURE_FLASH_LESS)
	u32HwId = BSP_Flashless_HwGetVerMain();
	u16IdMain = (u32HwId >> 16) & 0xFFFF;
	u16IdSub = u32HwId & 0xFFFF;
	if ((HW_VER_INVALID == u16IdMain) || (0x0000 == u16IdMain)) {
		printk("%s: Flashless get wrong hardware version, use hkadc hardware version.\n", __FUNCTION__);
		/* VxWorks is obtained directly from AXI Mem saved by BootLoader */
		u32HwId = *(BSP_U32 *)MEMORY_AXI_HW_ID_ADDR;
		u16IdMain = (u32HwId >> 16) & 0xFFFF;
		u16IdSub = u32HwId & 0xFFFF;
	} else {
		*(BSP_U32 *)MEMORY_AXI_HW_ID_ADDR = u32HwId;
		printk("%s: Flashless get right hardware version.\n", __FUNCTION__);
	}
#else
	/* VxWorks is obtained directly from AXI Mem saved by BootLoader */
	u32HwId = *(BSP_U32 *)MEMORY_AXI_HW_ID_ADDR;
	u16IdMain = (u32HwId >> 16) & 0xFFFF;
	u16IdSub = u32HwId & 0xFFFF;
#endif

	/* Check the table to initialize product information */
	for (i = 0; i < ARRAY_SIZE(huaweiProduct); i++) {
		if (huaweiProduct[i].index == u16IdMain) {
			g_huaweiProductInfo = huaweiProduct[i];
			g_huaweiProductInfo.hwIdSub = u16IdSub;
			break;
		}
	}

	/* Not found, return failed */
	if (i == ARRAY_SIZE(huaweiProduct)) {
		printk("%s: product info is not defined, pls check huaweiProduct!\n", __FUNCTION__);
		return BSP_ERROR;
	}

	printk("hw id: main,%#x, sub,%#x\n", u16IdMain, u16IdSub);

	return BSP_OK;
}


/*****************************************************************************
*Function Name: BSP_HwGetVerMain
*
*Function description: Obtain the single board version number (large version number 1 + large version number 2)
*
*Input parameters:
*Output parameters:
*
*Return value:
*
*Other instructions:
*
*****************************************************************************/
BSP_U16 BSP_HwGetVerMain(BSP_VOID)
{
	/* VxWorks is obtained directly from AXI Mem saved by BootLoader */
	return ((*(BSP_U32 *)MEMORY_AXI_HW_ID_ADDR) >> 16) & 0xFFFF;
}

/*****************************************************************************
*Function Name: BSP_HwGetVerSub
*
*Function description: Get the subversion number of the board, corresponding to GPIO4/5
*
*Input parameters:
*Output parameters:
*
*Return value:
*
*Other instructions:
*
*****************************************************************************/
BSP_U16 BSP_HwGetVerSub(BSP_VOID)
{
	/* VxWorks is obtained directly from AXI Mem saved by BootLoader */
	return (*(BSP_U32 *)MEMORY_AXI_HW_ID_ADDR) & 0xFFFF;
}

/*****************************************************************************
*Function Name: BSP_HwGetHwVersion
*
*Function Description: Get the hardware version name
*
*Input parameters: BSP_CHAR*pHwVersion, string pointer, guaranteed to be no less than 32 bytes
*Output parameters: None
*
*Return Value: None
*
*Modification record: 2011-3-29 wuzechun creat
*
*****************************************************************************/
BSP_S32 BSP_HwGetHwVersion(char *pFullHwVersion, BSP_U32 ulLength)
{
	unsigned char hwVerLit = 0;
	unsigned int len = 0;

	/*The board is hard core, soft core, or udp*/
	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pFullHwVersion))
		return BSP_ERROR;

	len = strlen(g_huaweiProductInfo.hwVer) + strlen(" Ver.X");

	if (len >= ulLength)
		return BSP_ERROR;

	hwVerLit = (BSP_S8)g_huaweiProductInfo.hwIdSub + 'A';

	(void)memset((void *)pFullHwVersion, 0, ulLength);
	strcat(strcat(pFullHwVersion, g_huaweiProductInfo.hwVer), " Ver.");
	*((pFullHwVersion + len) - 1) = (char)hwVerLit;
	*(pFullHwVersion + len) = 0;

	return BSP_OK;
}

/*****************************************************************************
*Function Name: BSP_HwGetPCBVersion
*
*Function Description: Get the PCB version name
*
*Input parameters: BSP_CHAR*pFullPcbVersion, string pointer, guaranteed to be no less than 32 bytes
*Output parameters: None
*
*Return Value: None
*
*Modification record: 2012-3-16 wzs creat
*
*****************************************************************************/
BSP_S32 BSP_HwGetPCBVersion(char *pFullPcbVersion, BSP_U32 ulLength)
{
	unsigned int len = 0;

	/*The board is hard core, soft core, or udp*/
	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pFullPcbVersion))
		return BSP_ERROR;

	len = strlen(g_huaweiProductInfo.hwVer);

	if (len >= ulLength)
		return BSP_ERROR;

	(void)memset((void *)pFullPcbVersion, 0, ulLength);
	(void)memcpy((void *)pFullPcbVersion, g_huaweiProductInfo.hwVer, len);

	return BSP_OK;
}


/*****************************************************************************
*Function Name: BSP_GetProductName
*
*Function Description: Get the product name
*
*Input parameters: char*pProductName, string pointer, guaranteed to be no less than 32 bytes
*BSP_U32 ulLength, buffer length
*Output parameters: None
*
*Return Value: 0: Correct, non-0: Failed
*
*Modification record: 2011-3-30 wuzechun creat
*
*****************************************************************************/
BSP_S32 BSP_GetProductName(char *pProductName, unsigned int ulLength)
{
	unsigned int len;

	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pProductName))
		return BSP_ERROR;

	len = strlen(g_huaweiProductInfo.productId);

	if (len >= ulLength)
		return BSP_ERROR;

	(void)memset((void *)pProductName, 0, ulLength);
	strcat(pProductName, g_huaweiProductInfo.productId);
	*(pProductName + strlen(g_huaweiProductInfo.productId)) = 0;


	return BSP_OK;
}

/*****************************************************************************
*Function Name: BSP_GetProductDloadName
*
*Function description: Get the product download version name, and use one-click upgrade to determine whether the upgrade is allowed.
*
*Input parameters: None
*Output parameters: None
*
*Return Value: Product Download Version Name String Pointer
*
*Modification record: 2011-3-30 wuzechun creat
*
*****************************************************************************/
BSP_S32 BSP_GetProductDloadName(char *pProductDloadName, unsigned int ulLength)
{
	unsigned int len;

	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pProductDloadName))
		return BSP_ERROR;

	len = strlen(g_huaweiProductInfo.dloadId);

	if (len >= ulLength)
		return BSP_ERROR;

	(void)memset((void *)pProductDloadName, 0, ulLength);
	strcat(pProductDloadName, g_huaweiProductInfo.dloadId);
	*(pProductDloadName + strlen(g_huaweiProductInfo.dloadId)) = 0;

	return BSP_OK;
}

/*****************************************************************************
*Function Name: BSP_GetProductInnerName
*
*Function Description: Get the internal product name
*
*Input parameters: char*pProductIdInter, string pointer
*BSP_U32 ulLength, buffer length
*Output parameters: None
*
*Return Value: 0: Correct, non-0: Failed
*
*Modification record: 2011-3-29 wuzechun creat
*
*****************************************************************************/
BSP_S32 BSP_GetProductInnerName(char *pProductIdInter, unsigned int ulLength)
{
	unsigned int len;

	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pProductIdInter))
		return BSP_ERROR;

	len = strlen(g_huaweiProductInfo.name) + strlen(g_huaweiProductInfo.namePlus);

	if (len >= ulLength)
		return BSP_ERROR;

	(void)memset((void *)pProductIdInter, 0, ulLength);
	strcat(strcat(pProductIdInter, g_huaweiProductInfo.name), g_huaweiProductInfo.namePlus);
	*(pProductIdInter + strlen(g_huaweiProductInfo.name) + strlen(g_huaweiProductInfo.namePlus)) = 0;

	return BSP_OK;
}

/*****************************************************************************
*Function Name: BSP_GetSupportBands
*
*Function Description: Get supported frequency bands
*
*Input parameters: BAND_INFO_T *pBandInfo, frequency band information structure pointer
*Output parameters:
*
*Return value:
*
*Other instructions:
*
*****************************************************************************/
BSP_S32 BSP_GetSupportBands(BAND_INFO_T *pBandInfo)
{
	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pBandInfo))
		return BSP_ERROR;

	*pBandInfo = g_huaweiProductInfo.band;

	return BSP_OK;
}
/*****************************************************************************
*Function Name: BSP_GetSupportBandsForGu
*
*Function description: Obtain the frequency band supported by GU mode
*
*Input parameters: pusWBands -WCDMA frequency band value
*pusGBands -GSM's band value
*
*Output parameters: pusWBands -WCDMA frequency band value
*pusGBands -GSM's band value
*
*Return Value: 0: Correct, non-0: Failed
*Bit bits to indicate which bands are supported.
*
*****************************************************************************/
int BSP_GetSupportBandsForGu(unsigned short *pusWBands, unsigned short *pusGBands)
{
	if ((HW_VER_INVALID == g_huaweiProductInfo.index) || (NULL == pusWBands) || (NULL == pusGBands))
		return BSP_ERROR;

	*pusWBands = g_huaweiProductInfo.band.bandGU & 0xFFFF;

	*pusGBands = (g_huaweiProductInfo.band.bandGU >> 16) & 0xFFFF;

	return BSP_OK;
}

/*****************************************************************************
*Function Name: BSP_HwIsSupportWifi
*
*Function Description: Print hardware version information
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: None
*
*Other Instructions: None
*
*****************************************************************************/
BSP_BOOL BSP_HwIsSupportWifi(BSP_VOID)
{
	/* E5 supports wifi, UDP also supports wifi by default. It is added to support wifi boards in the later stage.*/
#if (FEATURE_E5 == FEATURE_ON) || #defined(FEATURE_BOARD_STUB_BJ_UDP)
	return BSP_TRUE;
#else
	return BSP_FALSE;
#endif
}

/*****************************************************************************
*Function Name: BSP_PrintHwVersion
*
*Function Description: Print hardware version information
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: None
*
*Other Instructions: None
*
*****************************************************************************/
BSP_S32 BSP_PrintHwVersion(BSP_VOID)
{
#define MAX_HARD_VERSION_NUM 32

	BSP_S32 s32Ret = BSP_OK;
	BAND_INFO_T bandInfo;

	BSP_CHAR hwVer[MAX_HARD_VERSION_NUM];

	printk("main ID:                 %#x\n", BSP_HwGetVerMain());
	printk("sub ID:                  %#x\n", BSP_HwGetVerSub());

	s32Ret = BSP_GetSupportBands(&bandInfo);

	printk("support bands:\n");
	printk("   GU:%#x\n", bandInfo.bandGU);
	printk("   LTELow:%#x\n", bandInfo.bamdLTELow);
	printk("   LTEHigh:%#x\n", bandInfo.bamdLTEHigh);

	(BSP_VOID)BSP_GetProductName(hwVer, MAX_HARD_VERSION_NUM);
	printk("product name:            %s\n", hwVer);

	(BSP_VOID)(BSP_VOID)BSP_GetProductInnerName(hwVer, MAX_HARD_VERSION_NUM);
	printk("product inner name:      %s\n", hwVer);

	(BSP_VOID)BSP_GetProductDloadName(hwVer, MAX_HARD_VERSION_NUM);
	printk("product download name:   %s\n", hwVer);

	(BSP_VOID)BSP_HwGetHwVersion(hwVer, MAX_HARD_VERSION_NUM);
	printk("hardware version:        %s\n", hwVer);
	(BSP_VOID)BSP_HwGetPCBVersion(hwVer, MAX_HARD_VERSION_NUM);
	printk("pcb version:        %s\n", hwVer);

	return s32Ret;
}

EXPORT_SYMBOL(BSP_HwGetVerMain);
EXPORT_SYMBOL(BSP_HwGetVerSub);
EXPORT_SYMBOL(BSP_HwGetHwVersion);
EXPORT_SYMBOL(BSP_GetProductName);
EXPORT_SYMBOL(BSP_GetProductDloadName);
EXPORT_SYMBOL(BSP_GetProductInnerName);
EXPORT_SYMBOL(BSP_GetSupportBands);
EXPORT_SYMBOL(BSP_HwIsSupportWifi);
