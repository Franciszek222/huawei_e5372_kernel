/*************************************************************************
*Copyright (C) 1987-2011, Shenzhen Huawei Technologies Co., Ltd.
*
*File name: DrvInterface.h
*
*Author: yangzhi
*
*Description: This file is named "DrvInterface.h", which gives the API interface statistics between V7R1 bottom soft and protocol stack
*
*Modify record: January 18, 2011 v1.00 yangzhi created
*************************************************************************/

#ifndef __DRV_INTERFACE_H__
#define __DRV_INTERFACE_H__

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM) || defined(__VXWORKS__)

#include "FeatureConfigDRV.h"
#include "FeatureConfig.h"

#ifndef _lint
#include "MemoryMap.h"
#endif

#else

#ifndef _lint
#include <asm/io.h>
#include <mach/hardware.h>
#include <generated/FeatureConfig.h>
#include <mach/drvLogDef.h>
#include <linux/scatterlist.h>
#include <generated/MemoryMap.h>
#endif
#endif


/*This file is 4 bytes aligned by default, close the following two lint alarms*/
/*lint -e958 -e959*/
#pragma pack(4)

/*************************GLOBAL BEGIN*****************************/
/* Basic data type definition */
#ifndef _WIN32_COMPILE
typedef signed long long    BSP_S64;
#else
typedef double              BSP_S64;
#endif
typedef signed int          BSP_S32;
typedef signed short        BSP_S16;
typedef signed char         BSP_S8;
typedef char                BSP_CHAR;

#ifndef _WIN32_COMPILE
typedef unsigned long long  BSP_U64;
#else
typedef double              BSP_U64;
#endif
typedef unsigned int        BSP_U32;
typedef unsigned short      BSP_U16;
typedef unsigned char       BSP_U8;

typedef int                 BSP_BOOL;
typedef void                BSP_VOID;
typedef int                 BSP_STATUS;

#ifndef _WIN32_COMPILE
typedef signed long long*   BSP_PS64;
#else
typedef double*             BSP_PS64;
#endif
typedef signed int*         BSP_PS32;
typedef signed short*       BSP_PS16;
typedef signed char*        BSP_PS8;

#ifndef _WIN32_COMPILE
typedef unsigned long long* BSP_PU64;
#else
typedef double*             BSP_PU64;
#endif
typedef unsigned int*       BSP_PU32;
typedef unsigned short*     BSP_PU16;
typedef unsigned char*      BSP_PU8;

typedef unsigned char       UINT8;
typedef unsigned int        UINT32;
typedef int*                BSP_PBOOL;
typedef void*               BSP_PVOID;
typedef int*                BSP_PSTATUS;

typedef void                VOID;
typedef BSP_S32             STATUS;
typedef BSP_S32             UDI_HANDLE;

#ifndef BSP_CONST
#define BSP_CONST           const
#endif

#ifndef OK
#define OK                  (0)
#endif

#ifndef ERROR
#define ERROR               (-1)
#endif

#ifndef TRUE
#define TRUE                (1)
#endif

#ifndef FALSE
#define FALSE               (0)
#endif

#ifndef BSP_OK
#define BSP_OK              (0)
#endif

#ifndef BSP_ERROR
#define BSP_ERROR           (-1)
#endif

#ifndef BSP_TRUE
#define BSP_TRUE            (1)
#endif

#ifndef BSP_FALSE
#define BSP_FALSE           (0)
#endif

#ifndef BSP_NULL
#define BSP_NULL            (void*)0
#endif

#ifndef _LINUX_KERNEL_STAT_H  /* FUNCPTR defined in kernel_stat.h */
#ifndef FUNCPTR_IS_DEFINED
typedef BSP_S32 (*FUNCPTR)(void);
#define FUNCPTR_IS_DEFINED  1
#endif
#endif

typedef BSP_S32 (*FUNCPTR_1)(int);
typedef int (*PWRCTRLFUNCPTRVOID)(void);

#ifndef PWRCTRLFUNCPTR_D
typedef unsigned int (*PWRCTRLFUNCPTR)(unsigned int arg);     /* ptr to function returning int */
#define PWRCTRLFUNCPTR_D
#endif


#ifndef INLINE
#define INLINE              __inline__
#endif

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
typedef BSP_VOID (*VOIDFUNCPTR)();
#else
typedef BSP_VOID (*VOIDFUNCPTR)(BSP_U32);
#define SEM_FULL            (1)
#define SEM_EMPTY           (0)
#define LOCAL                   static
#define IVEC_TO_INUM(intVec)    ((int)(intVec))
#endif


/* Error code macro definition */
#define BSP_ERR_MODULE_OFFSET (0x1000)    /* Prevent overlapping with system error codes */
#define BSP_DEF_ERR( mod, errid) \
    ((((BSP_U32) mod + BSP_ERR_MODULE_OFFSET) << 16) | (errid))

#define BSP_REG(base, reg) (*(volatile BSP_U32 *)((BSP_U32)base + (reg)))

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM) || defined(__VXWORKS__)
#define BSP_REG_READ(base, reg, result) \
    ((result) = BSP_REG(base, reg))

#define BSP_REG_WRITE(base, reg, data) \
    (BSP_REG(base, reg) = (data))

#else
#define BSP_REG_READ(base, reg, resule) \
    (resule = readl(base + reg))

#define BSP_REG_WRITE(base, reg, data) \
    (writel(data, (base + reg)))
#endif

/*****************************************************************************
  1 GU side driver external interface, GUPorting project is added.
*****************************************************************************/

/*****************************************************************************
  2 Macro definition
*****************************************************************************/

#define DRV_OK                          (0)
#define DRV_ERROR                       (-1)
#define DRV_INTERFACE_RSLT_OK           (0)
#define BSP_BBP_OK   0x1
#define BSP_BBP_ERROR    0xffff

/* Basic data type definition */
typedef void (*PVOIDFUNC)(void);

typedef int  (*pFUNCPTR)(void);

typedef unsigned long (*pFUNCPTR2)( unsigned long ulPara1, unsigned long ulPara2);

typedef unsigned int tagUDI_DEVICE_ID_UINT32;

/*Need to move to another header file to start*/
/*Upgrade flash information*/
typedef struct
{
    BSP_U32 ulblockCount;    /*Block number*/
    BSP_U32 ulpageSize;    /*Page page size*/
    BSP_U32 ulpgCntPerBlk;    /*Number of pages in a block*/
}DLOAD_FLASH_STRU;
/*Need to move to another header file end*/

/* Uplink package function pointer */
typedef BSP_VOID (*UpLinkRxFunc)(BSP_U8 *buf, BSP_U32 len);

/* Package package release function pointer */
typedef BSP_VOID (*FreePktEncap)(BSP_VOID *PktEncap);

/* Gmac caller enumeration */
typedef enum tagGMAC_OWNER_E
{
    GMAC_OWNER_VXWORKS = 0, /* Vxworks network protocol stack */
    GMAC_OWNER_PS,          /* Lte network protocol stack */
    GMAC_OWNER_MSP,         /* Msp */
    GMAC_OWNER_MAX          /* Boundary value */
}GMAC_OWNER_E;

typedef enum tagWDT_TIMEOUT_E
{
    WDT_TIMEOUT_1   = 0,        /*0xFFFF000/WDT_CLK_FREQ,  about 3657ms*/     /*WDT_CLK_FREQ = ARM_FREQ/6 = 70M*/
    WDT_TIMEOUT_2,              /*0x1FFFE000/WDT_CLK_FREQ, about 7314ms*/
    WDT_TIMEOUT_3,              /*0x3FFFC000/WDT_CLK_FREQ, about 14628ms*/
    WDT_TIMEOUT_4,              /*0x7FFF8000/WDT_CLK_FREQ, about 29257ms*/
    WDT_TIMEOUT_BUTT
}WDT_TIMEOUT_E;

BSP_U8* BSP_GetMacAddr();
extern int ddmPhaseScoreBoot(const char * phaseName, unsigned int param);

/*************************GLOBAL END****************************/

/*************************VERSION START*****************************/
/* Hardware product information adaptation */
#define HW_VER_INVALID              (BSP_U16)0xFFFF

/* Porting board and p500 board compatible */
#define HW_VER_PRODUCT_PORTING      (BSP_U16)0xFFFE
#define HW_VER_PRODUCT_PV500        (BSP_U16)0xFFFD

/* Test board */
#define HW_VER_PRODUCT_UDP          (BSP_U16)0xFFFC

/* Stick */
#define HW_VER_PRODUCT_E392S_U      (BSP_U16)HW_VER_INVALID
#define HW_VER_PRODUCT_E3276S_150   (BSP_U16)0x0201

/* brother */
#define HW_VER_PRODUCT_E5_SBM       (BSP_U16)0x0001

/*Lame*/
#define HW_VER_PRODUCT_E5776_EM     (BSP_U16)0x0000

#define HW_VER_PRODUCT_E5_CMCC   	(BSP_U16)0x0003

/*E5371 dcm*/
#define HW_VER_PRODUCT_E5371_DCM     (BSP_U16)0x0100

/*I am*/
#define HW_VER_PRODUCT_E5372_32      (BSP_U16)0x0103

/*Pumping*/
#define HW_VER_PRODUCT_E5375          (BSP_U16)0x0104

/*E5375 Second Edition*/
#define HW_VER_PRODUCT_E5375_SEC      (BSP_U16)0x0106

#define HW_VER_PRODUCT_E5775S_925     (BSP_U16)0x0107
#define HW_VER_PRODUCT_E5372_601      (BSP_U16)0x0109
#define HW_VER_PRODUCT_E5372TS_32     (BSP_U16)0x0903

#define HW_VER_PRODUCT_E5372S_22     (BSP_U16)0x0900
#define HW_VER_PRODUCT_R215      (BSP_U16)0x0901

/*BEGIN DST2012092001529 liangshukun 20121025 ADDED*/
#define HW_VER_PRODUCT_E5_CMCC_CY   (BSP_U16)0x0007
/*END DST2012092001529 liangshukun 20121025 ADDED*/

/*BEGIN DST2012092001529 liangshukun 20121025 ADDED*/
/* LNA-free board, PCB logo 3~5 is displayed as A~C */
#define HW_CMCC_NON_LNA_PCB_VER_START_VALUE     3

/* CMCC The hardware logo of the wrong BOM is posted*/
#define HW_VER_PRODUCT_E5_CMCC_BOM_ERR  (BSP_U16)0x0603
/*BEGIN DST2012092001529 liangshukun 20121025 ADDED*/

/*Epad, S10 101u */
#define HW_VER_PRODUCT_S10_101U     (BSP_U16)0x0401
#define HW_VER_PRODUCT_S10_101U_1   HW_VER_PRODUCT_S10_101U
#define HW_VER_PRODUCT_S10_101U_2   (BSP_U16)0x0402
#define HW_VER_PRODUCT_S10_101U_3   (BSP_U16)0x0403
#define HW_VER_PRODUCT_S10_101U_4   (BSP_U16)0x0404

/*Epad, S10 102u */
#define HW_VER_PRODUCT_S10_102U      (BSP_U16)0x0406
#define HW_VER_PRODUCT_S10_102U_1    HW_VER_PRODUCT_S10_102U
#define HW_VER_PRODUCT_S10_102U_2    (BSP_U16)0x0407
#define HW_VER_PRODUCT_S10_102U_3    (BSP_U16)0x0408
#define HW_VER_PRODUCT_S10_102U_4    (BSP_U16)0x0409

/*Epad, S10 LTE */
#define HW_VER_PRODUCT_S10_101L     (BSP_U16)0x0501
#define HW_VER_PRODUCT_S10_101L_1   HW_VER_PRODUCT_S10_101L
#define HW_VER_PRODUCT_S10_101L_2   (BSP_U16)0x0502
#define HW_VER_PRODUCT_S10_101L_3   (BSP_U16)0x0503
#define HW_VER_PRODUCT_S10_101L_4   (BSP_U16)0x0504

/*Epad, S10 Reserved HW ID*/
#define HW_VER_PRODUCT_S10_RESERVED_1   (BSP_U16)0x0506
#define HW_VER_PRODUCT_S10_RESERVED_2   (BSP_U16)0x0507
#define HW_VER_PRODUCT_S10_RESERVED_3   (BSP_U16)0x0508
#define HW_VER_PRODUCT_S10_RESERVED_4   (BSP_U16)0x0509

/*Epad, S10 Modem */
#define HW_VER_PRODUCT_S10_MODEM     (BSP_U16)0x0000
#define HW_VER_PRODUCT_S10_MODEM_1   (BSP_U16)0x0005
#define HW_VER_PRODUCT_S10_MODEM_2   HW_VER_PRODUCT_S10_MODEM
#define HW_VER_PRODUCT_S10_MODEM_3   (BSP_U16)0x0500

/*Epad, S10 Temp */
#define HW_VER_PRODUCT_S10_TEMP     (BSP_U16)0x0005

/*****************************************************************************
*Function Name: BSP_HwGetVerMain
*Function Description: Get the product version number
*Input parameters: None
*Output parameters: Product version number
*Return Value: 0: Correct, non-0: Failed
*Modification record: 2011-3-30 wuzechun creat
*****************************************************************************/
BSP_U16 BSP_HwGetVerMain( BSP_VOID );

/*****************************************************************************
*Function Name: BSP_GetProductName
*Function Description: Get the product name
*Input parameters: char*pProductName, string pointer, guaranteed to be no less than 32 bytes
*BSP_U32 ulLength, buffer length
*Output parameters: None
*Return Value: 0: Correct, non-0: Failed
*Modification record: 2011-3-30 wuzechun creat
*****************************************************************************/
BSP_S32 BSP_GetProductName (char * pProductName, unsigned int ulLength);

/*************************VERSION END  *****************************/


/*************************MMI START*****************************/

/***************************************************************
*Function name: BSP_MMI_TestResultSet
*Function function: This interface writes the MMI test flag to NV
                     If ulFlag == 1, write "ST P"
                     If ulFlag == 0, write "ST F"
*Input parameters: BSP_U32 ulFlag
*Output parameters: None
*Return value: Success (0) Failed (-1)
***************************************************************/
int BSP_MMI_TestResultSet(unsigned int ulFlag);
#define DVR_MMI_TEST_RESULT_SET(ulFlag) BSP_MMI_TestResultSet(ulFlag)

/***************************************************************
*Function name: BSP_MMI_TestResultGet
*Function function: This interface compares whether the MMI NV is "ST P"
                                  Yes Return 1 Not Return 0
*Input parameters: None
*Output parameters: None
*Return value: Success (1) Failure (0)
***************************************************************/
unsigned int BSP_MMI_TestResultGet(void);
#define DVR_MMI_TEST_RESULT_GET() BSP_MMI_TestResultGet()

/*************************MMI END*****************************/

/*************************OLED START**************************/

/*****************************************************************************
* Function:  DRV_OLED_CLEAR_WHOLE_SCREEN
* Description: oled clear *
* Called By:AP
* Table Accessed:
* Table Updated:
* Input:
*        N/A
* Output:
*        N/A
* Return:
*        N/A
*****************************************************************************/
extern BSP_VOID DRV_OLED_CLEAR_WHOLE_SCREEN(BSP_VOID);

/*****************************************************************************
* Function:  DRV_OLED_UPDATE_STATE_DISPLAY
* Description: oled display right or not right  *
* Called By:AP
* Table Accessed:
* Table Updated:
* Input:
*        N/A
* Output:
*        N/A
* Return:
*        N/A
*****************************************************************************/
extern  void DRV_OLED_UPDATE_STATE_DISPLAY(int UpdateStatus);

/*****************************************************************************
 Function Name: DRV_OLED_UPDATE_DISPLAY
 Function description: SD card burning version upgrade, OLED display
 Input parameters: BOOL UpdateStatus
 Output parameters: None.
 Return value: None
 Others: After the upgrade is successful, call this function and check it after the NV backup is completed.
        After the upgrade failed, call the function and make a cross
*****************************************************************************/
extern void DRV_OLED_UPDATE_DISPLAY(int UpdateStatus);

/*****************************************************************************
 Function Name: DRV_OLED_STRING_DISPLAY
 Function Description: Oled string display function
 Input parameters:
 *To display the starting coordinates of the string
 *String to display
 Output parameters: none
 Return Value: void
*****************************************************************************/
extern BSP_VOID DRV_OLED_STRING_DISPLAY(UINT8 ucX, UINT8 ucY, UINT8 *pucStr);

/*****************************************************************************
 Function Name: DRV_OLED_POWER_OFF
 Function description: oled power down function
 Input parameters:
 Output parameters: None
 Return Value: void
*****************************************************************************/
extern BSP_VOID DRV_OLED_POWER_OFF(BSP_VOID);

/*************************OLED END******************************/

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)

#else
/*************************LCD START*****************************/

typedef enum
{
    LCD_LIGHT = 3,
    LCD_SLEEP,
    LCD_DIM,
    LCD_RESET,
    LCD_UPDATE,
    LCD_CONTRAST,
    LCD_BRIGHTNESS,
    LCD_POWER_ON,
    LCD_POWER_OFF,
    LCD_DEL_TIMER,/*delete the power on animation timer*/
    LCD_SYS_POWER_DOWN,
    LCD_INIT_IMAGE,
    LCD_POWER_OFF_CHARGING_START,/*restart power off charging animation*/
    LCD_POWER_OFF_CHARGING_STOP,/*stop power off charging animation*/
    LCD_GET_FRAME_BUFFER,
    LCD_WRITE_IMAGE,
    LCD_ALL_WHITE,
    LCD_ALL_BLACK,
    LCD_CMD_MAX
}LCD_IOCTL_CMD;


/*****************************************************************************
 Function Name: lcdRefresh
 Function description: lcd flushing function at the specified position
 Input parameters:
 Output parameters:
*UINT32 ulXStart, UINT32 ulYStart---Start coordinates
*UINT32 ulXOffset, UINT32 ulYOffset---horizontal vertical offset
*UINT8 *pucBuffer－Display data
 Return Value: void
*****************************************************************************/
extern int lcdRefresh(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char * test_buf);

/*****************************************************************************
 Function name: balong_lcd_ioctl
 Function description: lcd ioctl
 Input parameters:
*int cmd --command ID
*arg--para
 Output parameters: none
 Return Value: BSP_S32
*****************************************************************************/
extern BSP_S32 balong_lcd_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

extern BSP_VOID lcdClearWholeScreen(BSP_VOID);
extern BSP_VOID lcdStringDisplay(UINT8 ucX, UINT8 ucY, UINT8 *pucStr);

/*************************LCD END*******************************/
#endif



/*************************SCI BEGIN*****************************/

/*  SIM  state*/
typedef enum tagSCI_CARD_STATE_E
{
    SCI_CARD_STATE_READY = 0,           	/* Ready */
    SCI_CARD_STATE_NOCARD,                	/* No card */
    SCI_CARD_STATE_BUSY,               		/* In initialization*/
	SCI_CARD_STATE_BUTT,               		/* Butt*/
} SCI_CARD_STATE_E;

/*****************************************************************************
*Function Name: BSP_SCI_Reset
*
*Function description: This interface is used to reset SCI (Smart Card Interface) driver and USIM (Universal)
*Subscriber Identity Module) card
*
*Input parameters: BSP_VOID
*Output parameters: None
*
*Return Value: OK Reset successfully
*
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_Reset(BSP_VOID);
#define DRV_USIMMSCI_RST()    BSP_SCI_Reset()

/********************************************************************************************************************
 Function Name: BSP_GUSCI_GetCardStatus
 Function description: This interface is used to obtain the current status of the card. Currently, the interface parameter types provided by LTE are inconsistent.
 Input parameters: None.
 Output parameters:
 Return Value: 0: The card is in Ready;
             -1: The card is not in Ready.
 Note: After effectively calling the BSP_SCI_Reset() function, call this function again.
********************************************************************************************************************/
extern int BSP_GUSCI_GetCardStatus(BSP_VOID);
#define DRV_USIMMSCI_GET_CARD_STAU()    BSP_GUSCI_GetCardStatus()

/*****************************************************************************
*Function Name: DRV_USIMMSCI_SND_DATA
*
*Function description: This interface is used to send a piece of data to the USIM card
*
*Input parameters: BSP_U32 u32DataLength The effective length of the data sent. Value range is 1 to 256, units are bytes
*BSP_U8 *pu8DataBuffer The first address of the memory where the data is sent, if it is dynamically allocated, the interface is called
*Cannot be released immediately afterwards, and the data is sent by hardware
*Output parameters: None
*
*Return Value: OK
*BSP_ERR_SCI_NOTINIT
*BSP_ERR_SCI_INVALIDPARA
*BSP_ERR_SCI_DISABLED
*BSP_ERR_SCI_NOCARD
*BSP_ERR_SCI_NODATA
*
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_S32 DRV_USIMMSCI_SND_DATA(BSP_U32 u32DataLength, BSP_U8 *pu8DataBuffer);

/*****************************************************************************
*Function Name: DRV_USIMMSCI_RCV
*
*Function description: This interface is used by USIM Manager to read card return data from SCI Driver
*This interface is a blocking interface, and it will return only after SCI receives sufficient data;
*The timeout threshold of this interface is 1s
*
*Input parameters: BSP_U32 u32DataLength The length of data that USIM Manager wants to read from SCI Driver.
*Output parameters: BSP_U8 *pu8DataBuffer The Buffer specified by USIM Manager, SCI Driver copies the data to this Buffer.
*Return Value: OK
*BSP_ERR_SCI_NOTINIT
*BSP_ERR_SCI_INVALIDPARA
*
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_S32 DRV_USIMMSCI_RCV(BSP_U32 u32Length,BSP_U8 *pu8Data);

/*****************************************************************************
*Function Name: DRV_USIMMSCI_GET_ATR
*
*Function description: This interface is used to return the ATR data and data cached by the Driver layer to the USIM Manager layer
*
*Input parameters: BSP_VOID
*Output parameters: unsigned long *u8DataLength The ATR data length read by the Driver, returned to the USIM Manager.
*Value range is 0 to 32, unit is bytes
*BSP_U8 *pu8ATR The Buffer specified by USIM Manager, SCI Driver will ATR
*Copy the data to this Buffer. Generally dynamic allocation of operating system functions
*or statically allocated address
*
*
*Return Value: OK
*BSP_ERR_SCI_NOTINIT
*BSP_ERR_SCI_INVALIDPARA
*
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_S32 DRV_USIMMSCI_GET_ATR (unsigned long *u32DataLength, BSP_U8 *pu8ATR);

/*****************************************************************************
*Function Name: DRV_USIMMSCI_CLASS_SWITCH
*
*Function description: This interface is used to support PS to switch the voltage type of the card, from 1.8V to 3V
*
*Input parameters: BSP_VOID
*Output parameters: None
*
*Return Value: OK The current voltage is not the highest, perform voltage switching operation
*BSP_ERR_SCI_CURRENT_STATE_ERR Switching failed Current SCI driver state is ready/rx/tx
*BSP_ERR_SCI_VLTG_HIGHEST The current voltage is already the highest voltage, and no voltage switching is performed
*BSP_ERR_SCI_NOTINIT
*BSP_ERR_SCI_CURRENT_VLTG_ERR The current voltage value is abnormal (not class B or C)
*
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_S32 DRV_USIMMSCI_CLASS_SWITCH(BSP_VOID);

/*****************************************************************************
*Function Name: DRV_USIMMSCI_TM_STOP
*
*Function description: This interface is used to support PS to turn off the SIM card clock
*
*Input parameters:
*SCI_CLK_STOP_TYPE_E enTimeStopCfg Clock Stop Mode
*
*Output parameters: None
*
*Return Value: OK -successful completion
*ERROR -failed
*BSP_ERR_SCI_NOTINIT
*BSP_ERR_SCI_INVALIDPARA -invalid mode specified
*BSP_ERR_SCI_UNSUPPORTED -not support such a operation
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_VOID DRV_USIMMSCI_TM_STOP(BSP_U32 ulStopType);

/*****************************************************************************
*Function Name: BSP_SCI_Deactive
*
*Function description: This interface is used to deactivate the SIM card
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: OK
*BSP_ERR_SCI_NOTINIT
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_Deactive(BSP_VOID);
#define DRV_USIMMSCI_DEACT()    BSP_SCI_Deactive()

/*****************************************************************************
*Function Name: BSP_SCI_GetClkStatus
*
*Function description: This interface is used to obtain the current SIM card clock status
*
*Input parameters: BSP_U32 *pu32SciClkStatus variable pointer, used to return the SIM card clock status:
*0: The clock is on;
*1: The clock stops
*Output parameters: None
*
*Return Value: OK Operation was successful
*BSP_ERR_SCI_INVALIDPARA
*Modified record: June 29, 2009 liumengcun creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetClkStatus(BSP_U32 *pu32SciClkStatus);
#define DRV_USIMMSCI_GET_CLK_STAU(pulSciClkStatus)    BSP_SCI_GetClkStatus(pulSciClkStatus)

/*****************************************************************************
*Function Name: BSP_SCI_GetClkFreq
*
*Function description: This interface is used to obtain the current SIM card clock frequency
*
*Input parameters: None
*
*Output parameters: unsigned long *pLen The length of clock frequency data
*BSP_U8 *pBuf Clock Frequency Data
*Return Value: OK Operation was successful
*BSP_ERR_SCI_INVALIDPARA
*Modified record: August 24, 2010 zhouluojun creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetClkFreq(unsigned long *pLen, BSP_U8 *pBuf);
#define DRV_PCSC_GET_CLK_FREQ(pDataLen, Buffer)    BSP_SCI_GetClkFreq(pDataLen, Buffer)

/*****************************************************************************
*Function Name: BSP_SCI_GetBaudRate
*
*Function description: This interface is used to obtain the current SIM card clock frequency
*
*Input parameters: None
*
*Output parameters: unsigned long *pLen The length of the baud rate data
*BSP_U8 *pBuf baud rate data
*
*Return Value: OK Operation was successful
*BSP_ERR_SCI_INVALIDPARA
*Modified record: August 24, 2010 zhouluojun creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetBaudRate(unsigned long *pLen, BSP_U8 *pBuf);
#define DRV_PCSC_GET_BAUD_RATE(pDataLen, Buffer)    BSP_SCI_GetBaudRate(pDataLen, Buffer)

/*****************************************************************************
*Function Name: BSP_SCI_GetPCSCParameter
*
*Function description: This interface is used to obtain PCSC-related parameters of the current SIM card
*
*Input parameters: None
*
*Output parameters: BSP_U8 *pBuf PCSC related parameters
*
*Return Value: OK Operation was successful
*BSP_ERR_SCI_INVALIDPARA
*Modified record: August 24, 2010 zhouluojun creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetPCSCParameter(BSP_U8 *pBuf);
#define DRV_PCSC_GET_SCI_PARA(Para)    BSP_SCI_GetPCSCParameter(Para)

/*****************************************************************************
 Function Name: sciRecordDataSave
 Function Description: This interface is used to obtain the current SIM card interactive data. Belongs to the exception handling module, V7 first drives piles
 Input parameters: None.
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed, the pointer parameter is empty.
 Notes:
*****************************************************************************/
/*extern int sciRecordDataSave(void);*/
extern int BSP_USIMMSCI_STUB(void);
#define DRV_USIMMSCI_RECORD_DATA_SAVE()        BSP_USIMMSCI_STUB()

typedef enum
{
    SIM_CARD_OUT = 0,
    SIM_CARD_IN  = 1
} SCI_DETECT_STATE;

typedef void (*OMSCIFUNCPTR)(unsigned int ulVal);

/*****************************************************************************
*Function Name: BSP_SCIFuncRegister
*
*Function description: This interface is used to register OAM callback functions
*
*Input parameters: omSciFuncPtr
*Output parameters: None
*
*Return Value: NA
*
*
*****************************************************************************/
void BSP_SCIFuncRegister(OMSCIFUNCPTR omSciFuncPtr);
#define DRV_USIMMSCI_FUNC_REGISTER(omSciFuncPtr)    BSP_SCIFuncRegister(omSciFuncPtr)

/*************************SCI END*************************************/

/*************************PMU BEGIN***********************************/

/*****************************************************************************
 Function Name: DRV_GET_PMU_STATE
 Function description: Obtain the PMU module power-on mode, charging status, battery power, and battery in position.
 Input parameters: None.
 Output parameters: Pmu_State: Power-on mode, charging status, battery power, and battery in position.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: This interface is only used in PS PC projects and is not currently used and is reserved for the time being.
*****************************************************************************/
extern unsigned int DRV_GET_PMU_STATE(void*  Pmu_State);

/*****************************************************************************
 Function Name: BSP_PMU_UsbEndRegActionFunc
 Function Description: This interface is a USB plug-in/unplug callback registration function.
 Input parameters: srcFunc: The callback function pointer when the registered USB insertion or unplugging action occurs.
             actionIndex: Action Indication.
                          0: The parameter srcFunc is a registered callback function for USB insertion action;
                          1: The parameter srcFunc is a registered callback function for USB pull-out action.

 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: Call this interface and pass in the function pointer to be executed, that is, call the function to be executed when USB is inserted or unplugged.
*****************************************************************************/
extern int BSP_PMU_UsbEndRegActionFunc(pFUNCPTR srcFunc, unsigned char actionIndex);
#define DRV_USB_ENDREGACTION_FUNC(srcFunc,actionIndex) \
                BSP_PMU_UsbEndRegActionFunc(srcFunc,actionIndex)


/*****************************************************************************
Function name: BSP_USB_PortTypeValidCheck
Function description: Provided to the upper layer query device port form configuration legality interface
           1. The port is supported, 2. It contains PCUI port, 3. There are no duplicate ports, 4. The number of endpoints does not exceed 16.
           5. The first device is not a MASS class
Input parameters: pucPortType port form configuration
           ulPortNum Number of port patterns
Return value: 0: Port form is legal
           Others: Illegal port form
*****************************************************************************/
unsigned int BSP_USB_PortTypeValidCheck(unsigned char *pucPortType, unsigned long ulPortNum);
#define DRV_USB_PORT_TYPE_VALID_CHECK(pucPortType, ulPortNum)  \
                    BSP_USB_PortTypeValidCheck(pucPortType, ulPortNum)

/*****************************************************************************
Function name: BSP_USB_GetAvailabePortType
Function description: Provided to the upper layer to query the current device to support port form list interface
Input parameters: ulPortMax protocol stack supports the maximum number of port patterns
Output parameters: pucPortType Supported port patterns list
           pulPortNum supports the number of port patterns
Return value: 0: Acquisition of port form list successfully
           Others: Failed to get the port pattern list
*****************************************************************************/
unsigned int BSP_USB_GetAvailabePortType(unsigned char *pucPortType,
                            unsigned long *pulPortNum, unsigned long ulPortMax);
#define DRV_USB_GET_AVAILABLE_PORT_TYPE(pucPortType, pulPortNum, ulPortMax)  \
                BSP_USB_GetAvailabePortType(pucPortType, pulPortNum, ulPortMax)

/*************************PMU End *************************************/

/*************************INT BEGIN***********************************/

/*****************************************************************************
*Function Name: BSP_INT_Enable
*
*Function Description: Enable an interrupt
*
*Input parameters: INT32 ulLvl Interrupt number to be enabled, the value range is 0 to 40
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: March 5, 2009 zhanghailun creat
*****************************************************************************/
BSP_S32 BSP_INT_Enable ( BSP_S32 s32Lvl);
#define DRV_VICINT_ENABLE(ulLvl)    BSP_INT_Enable(ulLvl)

/*****************************************************************************
 *Function Name: BSP_INT_Disable
 *
 *Function Description: Deactivate an interrupt
 *
 *Input parameters: INT32 ulLvl Interrupt number to be enabled, the value range is 0 to 40
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modified record: March 5, 2009 zhanghailun creat
 *****************************************************************************/
BSP_S32 BSP_INT_Disable ( BSP_S32 s32Lvl);
#define  DRV_VICINT_DISABLE(ulLvl)    BSP_INT_Disable(ulLvl)

/*****************************************************************************
 *Function Name: BSP_INT_Connect
 *
 *Function Description: Register an interrupt
 *
 *Input parameters: VOIDFUNCPTR *vector interrupt vector number, value range 0~40
 *VOIDFUNCPTR routine interrupt service program
 *INT32 parameter interrupt service program parameters
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modified record: March 5, 2009 zhanghailun creat
 *****************************************************************************/
BSP_S32 BSP_INT_Connect  (VOIDFUNCPTR * vector,VOIDFUNCPTR routine, BSP_S32 parameter);
#define DRV_VICINT_CONNECT(vector,routine,parameter)    BSP_INT_Connect(vector,routine,parameter)

/*************************INT END*************************************/

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)

/*************************WDT starts ***********************************

/*****************************************************************************
*Function Name: BSP_WDT_Init
*
*Function description: Initialize the custom watchdog, set the timeout time, and attach the interrupt service function
*
*Input parameters: wdtId Watchdog ID, the chip provides 1 watchdog, the ID can be 0
*wdtTimeOuts Watchdog timeout time, unit seconds, maximum time cannot exceed 51 seconds
*
*Output parameters: None
*
*Return Value: OK& ERROR
*
*Modify record: March 5, 2009 v1.00 wangxuesong created
*****************************************************************************/
BSP_S32 BSP_WDT_Init(BSP_U8 wdtId , BSP_U32 wdtTimeOuts);
#define DRV_WDT_INIT(wdtId,wdtTimeOutms)      BSP_WDT_Init(wdtId,wdtTimeOutms)

/*****************************************************************************
*Function Name: BSP_WDT_EnableWdt
*
*Function Description: Turn on the watchdog
*
*Input parameters: wdtId Watchdog ID
*
*Output parameters: None
*
*Return Value: OK& ERROR
*
*Modify record: March 5, 2009 v1.00 wangxuesong created
*****************************************************************************/
BSP_S32 BSP_WDT_Enable(BSP_U8 wdtId);
#define DRV_WDT_ENABLE(wdtId)    BSP_WDT_Enable(wdtId)

/*****************************************************************************
*Function Name: BSP_WDT_Disable
*
*Function Description: Close the watchdog
*
*Input parameters: wdtId Watchdog ID
*
*Output parameters: None
*
*Return Value: OK& ERROR
*
*Modify record: March 5, 2009 v1.00 wangxuesong created
*****************************************************************************/
BSP_S32 BSP_WDT_Disable(BSP_U8 wdtId);
#define DRV_WDT_DISABLE(wdtId)    BSP_WDT_Disable(wdtId)
#endif

/*****************************************************************************
*Function Name: BSP_WDT_HardwareFeed
*
*Function Description: Reset the watchdog count register (feed the dog)
*
*Input parameters: wdtId Watchdog ID
*
*Output parameters: None
*
*Return Value: OK& ERROR
*
*Modify record: March 5, 2009 v1.00 wangxuesong created
*****************************************************************************/
BSP_S32 BSP_WDT_HardwareFeed(BSP_U8 wdtId);
#define DRV_WDT_FEED(wdtId)    BSP_WDT_HardwareFeed(wdtId)

/*************************WDT END*************************************/

/*************************SD BEGIN************************************/
#define WRFlAG     1
#define RDFlAG     0

typedef enum
{
    TFUP_FAIL = 0,  /*Try a fork*/
    TFUP_OK         /*Tick*/
}TFUP_DISPLAY_TYPE;

enum SD_MMC_OPRT_ENUM
{
    SD_MMC_OPRT_FORMAT = 0,
    SD_MMC_OPRT_ERASE_ALL,
    SD_MMC_OPRT_ERASE_SPEC_ADDR,
    SD_MMC_OPRT_WRITE,
    SD_MMC_OPRT_READ,
    SD_MMC_OPRT_BUTT
};

typedef unsigned int SD_MMC_OPRT_ENUM_UINT32;


/*Sd card device type*/
typedef struct _sd_dev_type_str
{
    unsigned long   devNameLen;
    char            devName[16];
}SD_DEV_TYPE_STR;

/*****************************************************************************
 Function Name: BSP_SDMMC_ATProcess
 Function description: at^sd, SD card operation, write, erase, format operation
 Input parameters: Operation type ulOp:
            0 Format SD card
            1 Erase the entire SD card content;
            2 Used to specify the erase operation of the content of the address, and the specified erase content length is 512 bytes. Write all 1 in the erased address
            3 Write data to the specified address of the SD card, and the second and third parameters need to be included.
            4 Read the 512 bytes of the address specified by ulAddr (ulAddr*512) into the pucBuffer

            ulAddr < address > address, in 512BYTE as a unit, represented by the number n

            ulData
             < data > data content, representing the content of 512BYTE, and the content of each byte is the same.
             0 byte content is 0x00
             1 byte content is 0x55
             2 bytes content is 0xAA
             3 bytes of content is 0xFF

 Output parameters: pulErr
 Return Value: 0: OK Non 0: Error

            The specific error value is filled in *pulErr
            0 means that the SD card is not in place
            1 indicates that the SD card initialization failed
            2 means that the <opr> parameter is illegal and the corresponding operation is not supported (this error is used by AT and does not require soft use)
            3 means that the <address> address is illegal and exceeds the capacity of the SD card itself.
            4 Other unknown errors
*****************************************************************************/
extern  unsigned long  BSP_SDMMC_ATProcess(SD_MMC_OPRT_ENUM_UINT32 ulOp,
                unsigned long ulAddr,  unsigned long ulData,unsigned char *pucBuffer,unsigned long *pulErr);
#define DRV_SDMMC_AT_PROCESS(ulOp,ulAddr,ulData,pucBuffer,pulErr)   \
                     BSP_SDMMC_ATProcess (ulOp,ulAddr,ulData,pucBuffer,pulErr)

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
/*****************************************************************************
*Function name: sd_mmc_blk_w
*
*Function description: Card writing interface
*
*Input parameters: u32StartBlk Start block
				  pu8DataBuf write data cache
				  u32Len The write data size must be an integer multiple of 512 bytes.
*Output parameters: NA
*
*Return Value: Return 0 successfully
*
*Other Instructions: NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_blk_w(BSP_U32 u32StartBlk,BSP_U8 *pu8DataBuf,BSP_U32 u32Len);
/*****************************************************************************
*Function name: sd_mmc_blk_r
*
*Function description: Card reading interface
*
*Input parameters: u32StartBlk Start block
				  pu8DataBuf read data cache
				  u32Len The read data size must be an integer multiple of 512 bytes.
*Output parameters: NA
*
*Return Value: Return 0 successfully
*
*Other Instructions: NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_blk_r(BSP_U32 u32StartBlk,BSP_U8 *pu8DataBuf,BSP_U32 u32Len);
/*****************************************************************************
*Function name: sd_mmc_blk_erase
*
*Function description: Block erase interface
*
*Input parameters: u32StartBlk Start block
				  u32EndBlk Terminate Block
*Output parameters: NA
*
*Return Value: Return 0 successfully
*
*Other Instructions: NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_blk_erase(BSP_U32 u32StartBlk,BSP_U32 u32EndBlk);
/*****************************************************************************
*Function name: sd_mmc_get_status
*
*Function Description: Card in-location query
*
*Input parameters: void
*Output parameters: NA
*
*Return Value: 0: In position; -1: Not in position
*
*Other Instructions: NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_get_status(BSP_VOID);
#endif

/*****************************************************************************
 Function Name: BSP_SDMMC_GetOprtStatus
 Function description: at^sd, the current operating status of the SD card
 Input parameters: None
 Output parameters: None
 Return Value:
            0: No operation or operation has been completed;
            1: In operation
            2: The last operation failed
*****************************************************************************/
extern unsigned long BSP_SDMMC_GetOprtStatus(void);
#define DRV_SDMMC_GET_OPRT_STATUS()    BSP_SDMMC_GetOprtStatus()

/*****************************************************************************
 Function Name: BSP_SDMMC_AddHook
 Function description: TF module hook function registration.
 Input parameters: hookType: callback function type,
                       0: The callback function called after obtaining the upgrade file information;
                       1: Insert the callback function of the notification OM in the TF card;
                       2: Unplug the callback function of the TF card to notify OM;
                       3; The callback function of the U disk notifies the TF card after initialization.
             p: callback function pointer.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: For callback functions of the same type, repeated registration will override the previous settings.
*****************************************************************************/
//#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)

extern int sdmmcAddHook (int hookType , void * p);/*y00186965 for sd_update*/
#define DRV_SDMMC_ADD_HOOK(hookType,p)    sdmmcAddHook(hookType,p)
//#else
//extern int BSP_SDMMC_AddHook(int hookType, void *p);
//#define DRV_SDMMC_ADD_HOOK(hookType,p)    BSP_SDMMC_AddHook(hookType,p)
//#endif


/*****************************************************************************
 Function Name: BSP_SDMMC_UsbGetStatus
 Function Description: Return to SD card status
 Input parameters: None.
 Output parameters: None.
 Return value: 0 is available
           Non-0 Not available

*****************************************************************************/
extern unsigned int BSP_SDMMC_UsbGetStatus(void);
#define DRV_SDMMC_USB_STATUS()    BSP_SDMMC_UsbGetStatus();

/*****************************************************************************
*Function name: sdmmc_ClearWholeScreen
*Function description: Display after SD card upgrade NV recovery, clear the screen
*Input parameters: None
*Output parameters: None
*Return Value: None
*Other Instructions: None
*****************************************************************************/
extern void sdmmc_ClearWholeScreen(void);
#define DRV_SDMMC_CLEAR_WHOLE_SCREEN()    sdmmc_ClearWholeScreen()

/*****************************************************************************
*Function name: sdmmc_UpdateDisplay
*Function description: Display after SD card upgrade NV recovery, display
*Input parameters: BOOL UpdateStatus
*Output parameters: None
*Return Value: None
*Other instructions: After the upgrade is successful, call this function and check it after the NV backup is completed.
              After the upgrade failed, call the function and make a cross
*****************************************************************************/
extern void sdmmc_UpdateDisplay(int UpdateStatus);
#define DRV_SDMMC_UPDATE_DISPLAY(state)    sdmmc_UpdateDisplay(state)

/*****************************************************************************
*Function Name: DRV_SD_GET_STATUS
*
*Function Description: Card in-location query
*
*Input parameters: void
*Output parameters: NA
*
*Return Value: 0: In position; -1: Not in position
*
*Other Instructions: NA
*
*****************************************************************************/
extern int DRV_SD_GET_STATUS(void);

/*****************************************************************************
*Function Name: DRV_SD_GET_CAPACITY
*
*Function description: Card capacity query
*
*Input parameters: void
*Output parameters: NA
*
*Return Value: 0: Failed; >0: Card Capacity
*
*Other Instructions: NA
*
*****************************************************************************/
extern int DRV_SD_GET_CAPACITY(void);

/*****************************************************************************
*Function Name: DRV_SD_TRANSFER
*
*Function description: Data transmission
*
*Input parameters: struct scatterlist *sg Pointer for data structure to be transferred
                            unsigned dev_addr SD block address to be written
                            unsigned blocks The number of blocks to be written
                            unsigned blksz The size of each block, unit bytes
                            int wrflags read and write flag bit, write: WRFlAG; read: RDFlAG
*Output parameters: NA
*
*Return Value: 0: Success; Other: Failed
*
*Other Instructions: NA
*
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
extern int DRV_SD_TRANSFER(struct scatterlist *sg, unsigned dev_addr,unsigned blocks, unsigned blksz, int wrflags);
#endif
/*****************************************************************************
*Function Name: DRV_SD_SG_INIT_TABLE
*
*Function description: SD multi-block data transmission sg list initialization
*
*Input parameters: const void *buf The buffer address to be operated
				  unsigned int buflen The buffer size to be operated is less than 32K, an integer multiple of 512B
				  					Greater than 32K, size is an integer multiple of 32KB, maximum buffer is 128K
*Output parameters: NA
*
*Return Value: 0: Success; Other: Failed
*Other Instructions: NA
*
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
extern int DRV_SD_SG_INIT_TABLE(const void *buf,unsigned int buflen);
#endif
/*****************************************************************************
*Function Name: DRV_SD_MULTI_TRANSFER
*
*Function description: SD multi-block data transmission
*
*Input parameters: unsigned dev_addr SD block address to be written
				  unsigned blocks The number of blocks to be written
				  unsigned blksz The size of each block, unit bytes
				  int write read and write flag bit, write: 1; read: 0
*Output parameters: NA
*
*Return Value: 0: Success; Other: Failed
*Other Instructions: NA
*
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
extern int DRV_SD_MULTI_TRANSFER(unsigned dev_addr,unsigned blocks,unsigned blksz,int write);
#endif
/*************************SD END**************************************/


/*************************Dload beginning *************************************
/*--------------------------------------------------------------*
 *Macro definition *
 *--------------------------------------------------------------*/
/* Error code */
#define DLOAD_OK                    BSP_OK
#define DLOAD_ERROR                 BSP_ERROR
#define DLOAD_ERR_NOT_INITED        BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_MODULE_NOT_INITED)
#define DLOAD_ERR_NULL_PTR          BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_NULL_PTR)
#define DLOAD_ERR_INVALID_PARA      BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_INVALID_PARA)
#define DLOAD_ERR_ALLOC_FAILED      BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_BUF_ALLOC_FAILED)
#define DLOAD_ERR_FREE_FAILED       BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_BUF_FREE_FAILED)
#define DLOAD_ERR_RETRY_TIMEOUT     BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_RETRY_TIMEOUT)

#define DLOAD_ERR_NO_BACKUP         BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 1)  /* No backup version */
#define DLOAD_ERR_OPEN_FAILED       BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 2)  /* Virtual serial port failed to open */
#define DLOAD_ERR_SEM_CREAT         BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 3)  /* Semaphore creation failed */
#define DLOAD_ERR_ABORT             BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 4)  /* Upgrade termination */
#define DLOAD_ERR_MODE_MISMATCH     BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 5)  /* Working mode mismatch */
#define DLOAD_ERR_INVALID_ATSTRING  BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 6)  /* At string illegal */


#define MODE_GSM      0x01
#define MODE_CDMA     0x02
#define MODE_WCDMA    0x04


/*--------------------------------------------------------------*
 *Enumeration definition *
 *--------------------------------------------------------------*/
enum UPDATE_STATUS_I
{
	DL_UPDATE = 0,
	DL_NOT_UPDATE = 1
};

typedef enum
{
	NORMAL_DLOAD = 0,	/*Normal upgrade mode*/
	FORCE_DLOAD = 1    /*Forced upgrade mode*/
}DLOAD_TYPE;

/* Download mode enumeration */
typedef enum tagDLOAD_MODE_E
{
    DLOAD_MODE_DOWNLOAD = 0,
    DLOAD_MODE_NORMAL,
    DLOAD_MODE_DATA,
    DLOAD_MODE_MAX
}DLOAD_MODE_E;

/*--------------------------------------------------------------*
 *Function pointer type definition *
 *--------------------------------------------------------------*/
typedef BSP_U32 (*BSP_DLOAD_AtCallBack)( BSP_VOID* pDataIn, BSP_U32 ulLen);
typedef BSP_U32 (*BSP_DLOAD_NVGetInfoCb)( BSP_VOID* pDataOut, BSP_U32 ulLen);
typedef BSP_U32 (*BSP_DLOAD_NVSetInfoCb)( BSP_VOID* pDataIn, BSP_U32 ulLen);

/*****************************************************************************
*Function Name: BSP_DLOAD_GetProductId
*
*Function Description: Get the product name string
*
*Input parameters: BSP_S8 *str: String buffer
*BSP_S32 len: string length
*Output parameters: BSP_S8 *str: String buffer
*
*Return Value: DLOAD_ERR_INVALID_PARA: Input parameter is illegal
*DLOAD_ERROR : Failed
*Others: Return the string length
*
*Other instructions: AT module call
*Normal mode support
*Download mode support
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetProductId(BSP_CHAR *str, BSP_U32 len);
/*****************************************************************************
*Function Name: BSP_DLOAD_GetDloadType
*
*Function Description: Get the download type
*
*Input parameters: BSP_VOID
*Output parameters: None
*
*Return Value: 0: Normal mode
*1: Forced mode
*
*Other instructions: AT module call
*Normal mode support
*Download mode support
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetDloadType(BSP_VOID);
/*****************************************************************************
*Function Name: BSP_DLOAD_GetCurMode
*
*Function Description: Set the current working mode
*
*Input parameters: BSP_VOID
*Output parameters: DLOAD_MODE_NORMAL:Normal mode
*DLOAD_MODE_DATA : Data mode
*DLOAD_MODE_DOWNLOAD :Download mode
*
*Return Value: None
*
*Other instructions: AT module call
*Normal mode supports:
*BSP_DLOAD_GetCurMode(DLOAD_MODE_DOWNLOAD)
*The download mode supports:
*BSP_DLOAD_GetCurMode(DLOAD_MODE_DATA)
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_SetCurMode(DLOAD_MODE_E eDloadMode);
/*****************************************************************************
*Function Name: BSP_DLOAD_GetCurMode
*
*Function Description: Get the current working mode
*
*Input parameters: BSP_VOID
*Output parameters: None
*
*Return Value: DLOAD_MODE_NORMAL:Normal Mode
*DLOAD_MODE_DATA : Data mode
*DLOAD_MODE_DOWNLOAD :Download mode
*
*Other Instructions: None
*
*****************************************************************************/
DLOAD_MODE_E BSP_DLOAD_GetCurMode(BSP_VOID);
/*****************************************************************************
*Function Name: BSP_DLOAD_SetSoftLoad
*
*Function Description: Set the startup flag
*
*Input parameters: BSP_BOOL bSoftLoad:
*BSP_FALSE: Started from vxWorks
*BSP_TRUE: Start from bootrom
*Output parameters: None
*
*Return Value: None
*
*Other Instructions: None
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_SetSoftLoad (BSP_BOOL bSoftLoad);


/*****************************************************************************
*Function Name: BSP_DLOAD_GetSoftLoad
*
*Function Description: Get the Startup Flag
*
*Input parameters: BSP_VOID
*Output parameters: None
*
*Return Value: BSP_TRUE: Started from bootrom
*BSP_FALSE: Started from vxWorks
*
*Other Instructions: None
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetSoftLoad (BSP_VOID);

/*****************************************************************************
*Function Name: BSP_DLOAD_SendData
*
*Function description: Send data to the PC via the virtual serial port
*
*Input parameters: pBuf: Data buffer
*u32Len: Data buffer length
*Output parameters: pBuf: Data buffer
*
*Return Value: None
*
*Other instructions: pBuf must ensure that cache line (32 bytes) is aligned
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_SendData(BSP_CHAR *pBuf, BSP_U32 u32Len);
/*****************************************************************************
*Function Name: BSP_DLOAD_AtProcReg
*
*Function description: Register AT processing function
*
*Input parameters: pFun
*Output parameters: None
*
*Return Value: DLOAD_OK: Success
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_AtProcReg (BSP_DLOAD_AtCallBack pFun);

/*****************************************************************************
*Function Name: BSP_DLOAD_GetSoftwareVer
*
*Function Description: Get the software version number
*
*Input parameters: BSP_S8 *str: String buffer
*BSP_S32 len: string length
*Output parameters: BSP_S8 *str: String buffer
*
*Return Value: DLOAD_ERROR: Input parameter is illegal
*Others: Return the string length
*
*Other instructions: AT module call
*Normal mode support
*Download mode support
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetSoftwareVer(BSP_CHAR *str, BSP_U32 len);


/*****************************************************************************
*Function Name: BSP_DLOAD_GetDloadVer
*
*Function Description: Query the version number of the download protocol. This information BSP is fixedly written to 2.0.
*
*Input parameters: BSP_S8 *str: String buffer
*BSP_S32 len: string length
*Output parameters: BSP_S8 *str: String buffer
*
*Return Value: DLOAD_ERROR: Input parameter is illegal
*Others: Return the string length
*
*Other instructions: AT module call
*Normal mode support
*Download mode support
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetDloadVer(BSP_CHAR *str, BSP_U32 len);

/*****************************************************************************
*Function Name: BSP_DLOAD_GetISOVer
*
*Function description: Get the background version number string
*
*Input parameters: BSP_S8 *str: String buffer
*BSP_S32 len: string length
*Output parameters: BSP_S8 *str: String buffer
*
*Return Value: DLOAD_ERROR: Input parameter is illegal
*Others: Return the string length
*
*Other instructions: AT module call
*Normal mode support
*Download mode support
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetISOVer(BSP_CHAR *str, BSP_U32 len);
#define DRV_GET_CDROM_VERSION(pVersionInfo,ulLength)    BSP_DLOAD_GetISOVer(pVersionInfo, ulLength)

/*****************************************************************************
 Function Name: DRV_SET_UPDATA_FLAG
 Function Description: Set the upgrade loading flag.
 Input parameters: flag: Upgrade loading flag,
                   0: After startup, enter bootrom and upgrade and load.
                   1: After startup, do not enter bootrom and start normally.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int DRV_SET_UPDATA_FLAG(int flag);


/*****************************************************************************
 Function Name: DRV_USB_DISCONNECT
 Function Description: Disconnect the USB connection during upgrade
 Input parameters: None
                  
 Output parameters: None
 Return Value: None
*****************************************************************************/
BSP_VOID DRV_USB_DISCONNECT();
	
/*****************************************************************************
*Function Name: BSP_DLOAD_SetCdromMarker
*
*Function description: Set the device form mark
*
*Input parameters: BSP_BOOL bCdromMarker:
*BSP_TRUE: Set the normal device form
*BSP_FALSE: Set the bootrom device form
*Output parameters: None
*
*Return Value: None
*
*Other instructions: When upgrading the background file, set to report the normal device form and do not restore NV;
*When upgrading the front desk file, set to report bootrom device form and restore NV;
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_SetCdromMarker(BSP_BOOL bCdromMarker);
#define DRV_SET_CDROM_FLAG(flag)    BSP_DLOAD_SetCdromMarker(flag)

/*****************************************************************************
*Function Name: BSP_DLOAD_GetCdromMarker
*
*Function description: Obtain the device form mark
*
*Input parameters: BSP_VOID
*Output parameters: None
*
*Return value: BSP_TRUE: Report normal device form
*BSP_FALSE: Report bootrom device form
*
*Other instructions: When upgrading the background file, set to report the normal device form and do not restore NV;
*When upgrading the front desk file, set to report bootrom device form and restore NV;
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetCdromMarker (BSP_VOID);
#define DRV_GET_CDROM_FLAG()    BSP_DLOAD_GetCdromMarker()

/********************************************************************************************************
 Function Name: BSP_DLOAD_NVBackupRead
 Function description: Read data from the NV item backup area in Flash to realize the NV item recovery function.
 Input parameters: len: Starting from the start of the NV item backup area, the length (number of bytes) that needs to be read shall not exceed 1Block.
 Output parameters: pRamAddr: Destination RAM address, used to store read NV item data.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int BSP_DLOAD_NVBackupRead(unsigned char *pRamAddr, unsigned int len);
#define DRV_NVBACKUP_READ(pRamAddr,len)    BSP_DLOAD_NVBackupRead(pRamAddr, len)

/********************************************************************************************************
 Function Name: BSP_DLOAD_NVBackupWrite
 Function description: Write specific data to the NV item backup area in Flash to realize the NV item backup function.
 Input parameters: pRamAddr: source RAM address, used to store NV item data to be written.
             len: Starting from the start of the NV item backup area, the length (number of bytes) that needs to be written shall not exceed 1Block.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int BSP_DLOAD_NVBackupWrite(unsigned char *pRamAddr, unsigned int len);
#define DRV_NVBACKUP_WRITE(pRamAddr, len)   BSP_DLOAD_NVBackupWrite(pRamAddr, len)


/********************************************************************************************************
 Function Name: NVBackupFlashDataWrite
 Function Description: Implements the function of reading NV backup and recovery area from the specified location in FLASH.
 Input parameters: pRamAddr: Source RAM address
*offset: Offset starting from address 0 of NV backup area
*len: length required to write
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int NVBackupFlashDataWrite(unsigned char* pRamAddr, unsigned int offset,unsigned int len);
#define DRV_NV_FLASH_WRITE(pRamAddr, offset,len) NVBackupFlashDataWrite(pRamAddr, offset,len)


/*****************************************************************************
 Function Name: BSP_DLOAD_GetTFUpdateFlag
 Function Description: Determine whether it is a TF card upgrade
 Input parameters: None
 Output parameters: None
 Return Value: Return 1 is TF upgrade, NV recovery is successful, and blue light needs to be lit
             Returning 0 is not a TF upgrade, NV recovery is successful, no need to light a blue light

*****************************************************************************/
extern int BSP_DLOAD_GetTFUpdateFlag(void);
#define DRV_GET_TFUPDATE_FLAG()    BSP_DLOAD_GetTFUpdateFlag()

/*****************************************************************************
 Function Name: DRV_GET_DLOAD_VERSION
 Function description: Get dload version
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern BSP_S32 DRV_GET_DLOAD_VERSION(BSP_U8 *str, int len);


/*****************************************************************************
 Function Name: DRV_GET_DLOAD_INFO
 Function description: Get dload information
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/

extern BSP_S32 DRV_GET_DLOAD_INFO(unsigned char atCmdBuf[], unsigned int dloadType);

/*****************************************************************************
 Function Name: DRV_GET_AUTHORITY_VERSION
 Function description: Get Authority version
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern BSP_S32 DRV_GET_AUTHORITY_VERSION(BSP_U8 *str, BSP_S32 len);

/*****************************************************************************
 Function Name: DRV_GET_AUTHORITY_ID
 Function Description: Obtain the download authentication protocol Id
 Input parameters: unsigned char *buf
                          int len
 Output parameters: None
 Return Value: 0: The operation was successful;
                      -1: The operation failed.
*****************************************************************************/
extern BSP_S32 DRV_GET_AUTHORITY_ID(unsigned char *buf, BSP_S32 len);

/*****************************************************************************
 Function Name: BSP_DLOAD_GetDloadNetMode
 Function description: Get net mode
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern BSP_S32  BSP_DLOAD_GetDloadNetMode(BSP_U32 *netMode);
#define DRV_GET_DLOAD_NETMODE(netMode)    BSP_DLOAD_GetDloadNetMode (netMode)

/*****************************************************************************
 Function Name: BSP_DLOAD_GetDloadFlashInfo
 Function description: Get dload flash information
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern BSP_S32 BSP_DLOAD_GetDloadFlashInfo(DLOAD_FLASH_STRU* pFlashInfo);
#define DRV_GET_DLOAD_FLASHINFO(pFlashInfo)    BSP_DLOAD_GetDloadFlashInfo(pFlashInfo)


/*****************************************************************************
 Function Name: BSP_DLOAD_GetWebUIVersion
 Function Description: Obtain WEBUI version information
 Input parameters: pVersionInfo: The memory address that stores the returned version information
                           ulLength: The memory length of the returned version information, currently fixed
                           128
 Output parameter: pVersionInfo: The memory address of the returned version information
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern BSP_S32 BSP_DLOAD_GetWebUIVersion(BSP_CHAR *pVersionInfo, BSP_U32 u32Length);
#define DRV_GET_WEBUI_VERSION(pVersionInfo, u32Length) BSP_DLOAD_GetWebUIVersion(pVersionInfo,u32Length)

/*****************************************************************************
 Function Name: BSP_TFUP_CompleteDeal
 Function description: After the TF card is upgraded, processed
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void BSP_TFUP_CompleteDeal(void);
#define DRV_TFUP_COMPLETEDEAL() BSP_TFUP_CompleteDeal()

/*****************************************************************************
 Function Name: BSP_TFUP_CompleteDeal
 Function description: After the online upgrade is completed
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
void BSP_ONUP_CompleteDeal(void);
#define DRV_ONUP_COMPLETEDEAL() BSP_ONUP_CompleteDeal()

/*****************************************************************************
*Function Name: BSP_DLOAD_GetNVBackupFlag
*
*Function description: Get whether to perform NV backup flag before upgrading
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: BSP_TRUE: Backup NV
*BSP_FALSE: NV not backed up
*
*Other instructions: This interface is only valid for non-one-click upgrade mode (SD upgrade/online upgrade). One-click upgrade will issue AT commands to set
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetNVBackupFlag(BSP_VOID);
#define DRV_DLOAD_GETNVBACKUPFLAG() BSP_DLOAD_GetNVBackupFlag()

/*************************DLOAD END***********************************/

/*************************GPIO START**********************************/

typedef unsigned int GPIO_OPRT_ENUM_UINT32;

/*****************************************************************************
 Function Name: BSP_GPIO_Oprt
 Function description: at^GPIOPL, set and query the level of GPIO
 Input parameters: Operation type ulOp:
             0 Set the PL value of each GPIO
             1 Query the PL value of the current GPIO

             pucPL corresponds to an array of 20Bytes, each BYTE represents a value of 8 pins

             When setting the operation, pucPL is set to 20 Byte
             During query operation, pucPL is the hexadecimal data composed of the actual value of the currently obtained PL.
             For example, if the user sets at^GPIOPL = ABCDEF, the array value corresponding to pucPL is {A,B,C,D,E,F,0,0,...}

 Output parameters: None
 Return Value: 0: OK Non 0: Error

*****************************************************************************/
extern unsigned long BSP_GPIO_Oprt(unsigned long ulOp, unsigned char *pucPL);
#define DRV_GPIO_OPRT(ulOp,pucPL)    BSP_GPIO_Oprt(ulOp,pucPL)

/*************************GPIO END************************************/

/*************************IPCM Beginning **********************************/

/* Processor Type*/
typedef enum tagIPC_INT_CORE_E
{
    IPC_CORE_ARM11 = 0x0,   /* IPC on P500 */
    IPC_CORE_A9,
    IPC_CORE_CEVA,
    IPC_CORE_TENS0,
    IPC_CORE_TENS1,
    IPC_CORE_DSP,
    IPC_CORE_APPARM = 0x0,  /* ipc on V7 r1/v3 r2 */
    IPC_CORE_COMARM,
    IPC_CORE_LTEDSP,
    IPC_CORE_VDSP,
    IPC_CORE_ZSP,
	IPC_CORE_TDSDSP,
    IPC_CORE_DSP_GU,
    IPC_CORE_BUTTOM
}IPC_INT_CORE_E;

typedef enum tagIPC_INT_LEV_E
{
    IPC_INT_DSP_MODEM = 0,
    IPC_INT_DSP_APP = 1,
    IPC_INT_DSP_MSP = 0,
    IPC_INT_DSP_PS = 1,
    IPC_INT_MODEM_APP = 2,
    IPC_INT_DRX_APP = 3,
    IPC_INT_GUDSP_MODEM = 4,
    IPC_INT_MEDDSP_MODEM = 5,
    IPC_INT_DICC_USRDATA = 6,
    IPC_INT_DICC_RELDATA = 7,
    IPC_INT_DSP_HALT =8,  /*Dsp Notify arm to sleep*/
    IPC_INT_DSP_RESUME,   /*Dsp notifies the arm to complete the recovery operation after wake-up*/
    IPC_INT_DSP_WAKE,     /*Arm wake up dsp*/
    IPC_INT_ARM_SLEEP = 8,
    IPC_INT_WAKE_GU =11,     /*Master mode wakes up slave mode interrupt*/
    IPC_INT_SLEEP_GU,     /*Sleep interrupted from model*/
    IPC_INT_CDRX_DSP_HALT,	/*Dsp notify msp to enter the cdrx process*/
	IPC_INT_TDSDSP_HALT =14,
	IPC_INT_TDSDSP_IDLE =15,
	IPC_INT_TDSDSP_RESUME =16,
	IPC_INT_WAKE_TDS = 17,  /*Wake up tds interrupt from mode*/
	IPC_INT_WAKE_LTE = 18,  /*Wake up lte interrupt*/
	IPC_INT_SLEEP_TDS = 19, /*Tds from model sleep interruption*/
	IPC_INT_TDSDSP_MSP = 20, /*Used to send interrupts to MSP by TDS DSP*/
	IPC_INT_HIFI_FULL = 27,
    IPC_INT_HIFI_ON,
    IPC_INT_HIFI_OFF,
    IPC_INT_ICC_MODEM = 30,
    IPC_INT_ICC_APP,
    IPC_INT_BUTTOM
}IPC_INT_LEV_E;

typedef enum tagIPC_SEM_ID_E
{
    IPC_SEM_ICC,
    IPC_SEM_NAND,
    IPC_SEM_MEM,
    IPC_SEM_DICC,
    IPC_SEM_RFILE_LOG,          /*Used for flashless surveillance*/
    IPC_SEM_SYNC = 25,
    IPC_SEM_SYSCTRL = 30,
    IPC_SEM_ZSP_HALT = 31,
    IPC_SEM_BUTTOM
}IPC_SEM_ID_E;


#define INTSRC_NUM                     32

/*****************************************************************************
*Function Name: BSP_DRV_IPCIntInit
*
*Function description: IPC module initialization
*
*Input parameters: None
*Output parameters: None
*
*Return Value: None
*
*Modified record: March 5, 2009 wangjing creat
*****************************************************************************/
BSP_S32 BSP_DRV_IPCIntInit(void);


/*****************************************************************************
*Function Name: DRV_IPC_SEMCREATE
*
*Function Description: Semaphore creation function
*
*Input parameters: None
*Output parameters: None
*
*Return Value: None
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_SemCreate(BSP_U32 u32SignalNum);
extern BSP_S32 DRV_IPC_SEMCREATE(BSP_U32 u32SignalNum);

/*****************************************************************************
*Function Name: DRV_IPC_SEMDELETE
*
*Function Description: Delete semaphore
*
*Input parameters: BSP_U32 u32SignalNum semaphore number to be deleted

*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_SemDelete(BSP_U32 u32SignalNum);
extern BSP_S32 DRV_IPC_SEMDELETE(BSP_U32 u32SignalNum);

/*****************************************************************************
*Function Name: BSP_IPC_IntEnable
*
*Function Description: Enable an interrupt
*
*Input parameters:
                BSP_U32 ulLvl Interrupt number to be enabled, value range 0 to 31
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntEnable (IPC_INT_LEV_E ulLvl);
extern BSP_S32 DRV_IPC_INTENABLE(IPC_INT_LEV_E ulLvl);

/*****************************************************************************
*Function Name: DRV_IPC_INTDISABLE
*
*Function Description: Deactivate an interrupt
*
*Input parameters:
            BSP_U32 ulLvl Interrupt number to be enabled, value range 0 to 31
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntDisable (IPC_INT_LEV_E ulLvl);
extern BSP_S32 DRV_IPC_INTDISABLE(IPC_INT_LEV_E ulLvl);

/*****************************************************************************
*Function Name: BSP_IPC_IntConnect
*
*Function Description: Register an interrupt
*
*Input parameters:
           BSP_U32 ulLvl Interrupt number to be enabled, value range 0 to 31
           VOIDFUNCPTR routine interrupt service program
*BSP_U32 parameter interrupt service program parameters
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntConnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);
extern BSP_S32 DRV_IPC_INTCONNECT(IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
*Function Name: BSP_IPC_IntDisonnect
*
*Function Description: Unregister an interrupt
*
*Input parameters:
*BSP_U32 ulLvl Interrupt number to be enabled, value range 0 to 31
*VOIDFUNCPTR routine interrupt service program
*BSP_U32 parameter interrupt service program parameters
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntDisonnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
*Function Name: DRV_IPC_INTSEND
*
*Function Description: Send Interrupt
*
*Input parameters:
                IPC_INT_CORE_E enDstore core to receive interrupts
                BSP_U32 ulLvl Interrupt number to be sent, value range 0 to 31
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntSend(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl);
extern BSP_S32 DRV_IPC_INTSEND(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl);

/*****************************************************************************
*Function Name: DRV_IPC_SEMTAKE
*
*Function Description: Get semaphore
*
*Input parameters: None
*Output parameters: None
*
*Return Value: None
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_SemTake(BSP_U32 u32SignalNum,BSP_S32 s32timeout);
extern BSP_S32 DRV_IPC_SEMTAKE(BSP_U32 u32SignalNum, BSP_S32 s32timeout);

/*****************************************************************************
*Function Name: DRV_IPC_SEMGIVE
*
*Function description: Release semaphore
*
*Input parameters: None
*Output parameters: None
*
*Return Value: None
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_VOID BSP_IPC_SemGive(BSP_U32 u32SignalNum);
extern BSP_VOID DRV_IPC_SEMGIVE(BSP_U32 u32SignalNum);

/*****************************************************************************
*Function Name: BSP_IPC_SpinLock
*
*Function Description: Get semaphore
*
*Input parameters: None
*Output parameters: None
*
*Return Value: None
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_VOID BSP_IPC_SpinLock (BSP_U32 u32SignalNum);
extern BSP_VOID DRV_SPIN_LOCK (BSP_U32 u32SignalNum);

/*****************************************************************************
*Function Name: DRV_SPIN_UNLOCK
*
*Function description: Release semaphore
*
*Input parameters: None
*Output parameters: None
*
*Return Value: None
*
*Modified record: April 11, 2011 wangjing creat
*****************************************************************************/
BSP_VOID BSP_IPC_SpinUnLock (BSP_U32 u32SignalNum);
extern BSP_VOID DRV_SPIN_UNLOCK(BSP_U32 u32SignalNum);


BSP_S32 BSP_SGI_Connect(BSP_U32 ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);
BSP_S32 BSP_SGI_IntSend(BSP_U32 ulLvl);

/*************************IPCM END ************************************/


/*************************UDI START***********************************/

/**************************************************************************
  Macro definition
**************************************************************************/
#define UDI_INVALID_HANDLE     (-1)

#define UDI_CAPA_BLOCK_READ    ((BSP_U32)(1<<0)) /* The read interface of the device is a blocking interface */
#define UDI_CAPA_BLOCK_WRITE   ((BSP_U32)(1<<1)) /* The write interface of the device is a blocking interface */
#define UDI_CAPA_READ_CB       ((BSP_U32)(1<<2)) /* This device supports read callback function */
#define UDI_CAPA_WRITE_CB      ((BSP_U32)(1<<3)) /* This device supports write callback function */
#define UDI_CAPA_BUFFER_LIST   ((BSP_U32)(1<<4)) /* The read and write buffer of this device is a memory linked list structure (default is normal memory block) */
#define UDI_CAPA_CTRL_OPT      ((BSP_U32)(1<<5)) /* This device supports control operation */

#define MIN_UDI_HANDLE     0x5a0000
#define MAX_UDI_HANDLE     0x5a003f

/**************************************************************************
  Structural definition
**************************************************************************/
#define UDI_BUILD_DEV_ID(dev, type) (((BSP_U32)(dev) << 8) | ((BSP_U32)(type) & 0x00ff))
#define UDI_BUILD_CMD_ID(dev, cmd) (((BSP_U32)(dev) << 8) | ((BSP_U32)(cmd) & 0xffff))


/* Master device definition */
typedef enum tagUDI_DEVICE_MAIN_ID
{
    UDI_DEV_USB_ACM = 0,
    UDI_DEV_USB_NCM,
    UDI_DEV_ICC,
    UDI_DEV_UART,

    UDI_DEV_MAX                 /* Must be used at the end, for boundary values */
}UDI_DEVICE_MAIN_ID;

/* Definition of each device type (to be unified with usb multi-device form) */
/* Acm */
typedef enum tagUDI_ACM_DEV_TYPE
{
    UDI_USB_ACM_CTRL,
    UDI_USB_ACM_AT,
    UDI_USB_ACM_SHELL,
    UDI_USB_ACM_LTE_DIAG,
    UDI_USB_ACM_OM,
    UDI_USB_ACM_MODEM,
    UDI_USB_ACM_GPS,      /*Hiso*/
    UDI_USB_ACM_3G_GPS,   /*Ashell*/
    UDI_USB_ACM_3G_PCVOICE, /*Reservation*/
    UDI_USB_ACM_PCVOICE,    /*Reservation*/
    /*UDI_USB_ACM_3G_AT,    //3G AT*/
    /*UDI_USB_ACM_3G_MODEM, //3G PPP*/
    UDI_USB_HSIC_ACM0,
    UDI_USB_HSIC_ACM1,
    UDI_USB_HSIC_ACM2,
    UDI_USB_HSIC_ACM3,
    UDI_USB_HSIC_ACM4,
    UDI_USB_HSIC_ACM5,
    UDI_USB_HSIC_ACM6,
    UDI_USB_HSIC_ACM7,
    UDI_USB_HSIC_ACM8,
    UDI_USB_HSIC_ACM9,
    UDI_USB_HSIC_ACM10,
    UDI_USB_HSIC_ACM11,
    UDI_USB_HSIC_ACM12,
    UDI_USB_HSIC_ACM13,
	UDI_USB_HSIC_ACM14,
    UDI_USB_ACM_MAX                /* Must be used at the end, for boundary values */
}UDI_ACM_DEV_TYPE;

/* Ncm */
typedef enum tagUDI_NCM_DEV_TYPE
{
    UDI_USB_NCM_NDIS,
    UDI_USB_NCM_CTRL,
    UDI_USB_HSIC_NCM0,
    UDI_USB_HSIC_NCM1,
    UDI_USB_HSIC_NCM2,

    UDI_USB_NCM_MAX                /* Must be used at the end, for boundary values */
}UDI_NCM_DEV_TYPE;

/* Icc */
typedef enum tagUDI_ICC_DEV_TYPE
{
    UDI_ICC_GUOM0 = 16,
    UDI_ICC_GUOM1,
    UDI_ICC_GUOM2,
    UDI_ICC_GUOM3,
    UDI_ICC_GUOM4,
    UDI_ICC_GUOM5,

    UDI_ICC_MAX                /* Must be used at the end, for boundary values */
}UDI_ICC_DEV_TYPE;


/* Device ID number definition */
typedef enum tagUDI_DEVICE_ID
{
    /* USB ACM */
    UDI_ACM_CTRL_ID =  UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_CTRL),
    UDI_ACM_AT_ID =    UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_AT),
    UDI_ACM_SHELL_ID = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_SHELL),
    UDI_ACM_LTE_DIAG_ID = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_LTE_DIAG),
    UDI_ACM_OM_ID = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_OM),
    UDI_ACM_MODEM_ID = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_MODEM),
    UDI_ACM_GPS_ID      = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_GPS),     /*Hiso*/
    UDI_ACM_3G_GPS_ID   = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_3G_GPS),  /*Ashell*/
    UDI_ACM_3G_PCVOICE_ID = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_3G_PCVOICE), /*Reservation*/
    UDI_ACM_PCVOICE_ID    = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_PCVOICE),    /*Reservation*/
    /*UDI_ACM_3G_AT_ID      = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_3G_AT),    //3G AT*/
    /*UDI_ACM_3G_MODEM_ID   = UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_ACM_3G_MODEM), //3G PPP*/
    UDI_ACM_HSIC_ACM0_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM0),
    UDI_ACM_HSIC_ACM1_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM1),
    UDI_ACM_HSIC_ACM2_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM2),
    UDI_ACM_HSIC_ACM3_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM3),
    UDI_ACM_HSIC_ACM4_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM4),
    UDI_ACM_HSIC_ACM5_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM5),
    UDI_ACM_HSIC_ACM6_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM6),
    UDI_ACM_HSIC_ACM7_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM7),
    UDI_ACM_HSIC_ACM8_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM8),
    UDI_ACM_HSIC_ACM9_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM9),
    UDI_ACM_HSIC_ACM10_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM10),
    UDI_ACM_HSIC_ACM11_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM11),
    UDI_ACM_HSIC_ACM12_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM12),
    UDI_ACM_HSIC_ACM13_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM13),
	UDI_ACM_HSIC_ACM14_ID=UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, UDI_USB_HSIC_ACM14),

    /* USB NCM */
    UDI_NCM_NDIS_ID =  UDI_BUILD_DEV_ID(UDI_DEV_USB_NCM, UDI_USB_NCM_NDIS),
    UDI_NCM_CTRL_ID =  UDI_BUILD_DEV_ID(UDI_DEV_USB_NCM, UDI_USB_NCM_CTRL),
    UDI_NCM_HSIC_NCM0_ID =  UDI_BUILD_DEV_ID(UDI_DEV_USB_NCM, UDI_USB_HSIC_NCM0),
    UDI_NCM_HSIC_NCM1_ID =  UDI_BUILD_DEV_ID(UDI_DEV_USB_NCM, UDI_USB_HSIC_NCM1),
    UDI_NCM_HSIC_NCM2_ID =  UDI_BUILD_DEV_ID(UDI_DEV_USB_NCM, UDI_USB_HSIC_NCM2),

    /* Uart */
    UDI_UART_0_ID =  UDI_BUILD_DEV_ID(UDI_DEV_UART, 0),
    UDI_UART_1_ID =  UDI_BUILD_DEV_ID(UDI_DEV_UART, 1),

    /* Icc */
    UDI_ICC_IFC_ID  =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, 0),
    UDI_ICC_IPM_ID  =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, 1),
    UDI_ICC_LMSP_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, 2),

    UDI_ICC_GUOM0_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, UDI_ICC_GUOM0),
    UDI_ICC_GUOM1_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, UDI_ICC_GUOM1),
    UDI_ICC_GUOM2_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, UDI_ICC_GUOM2),
    UDI_ICC_GUOM3_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, UDI_ICC_GUOM3),
    UDI_ICC_GUOM4_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, UDI_ICC_GUOM4),
    UDI_ICC_GUOM5_ID =  UDI_BUILD_DEV_ID(UDI_DEV_ICC, UDI_ICC_GUOM5),

    /* MUST BE LAST */
    UDI_INVAL_DEV_ID = 0xFFFF
} UDI_DEVICE_ID;


/* Device opening parameters */
typedef struct tagUDI_OPEN_PARAM
{
    UDI_DEVICE_ID devid;                        /* Device ID */
    void   *pPrivate;                            /* Module-specific data */
} UDI_OPEN_PARAM;

/* IOCTL command code, the required command code is added here */
typedef enum tagUDI_IOCTL_CMD_TYPE
{
    UDI_IOCTL_SET_WRITE_CB = 0xF001,            /* Set a start code value to prevent conflicts with system definitions */
    UDI_IOCTL_SET_READ_CB,

    UDI_IOCTL_INVAL_CMD = 0xFFFFFFFF
} UDI_IOCTL_CMD_TYPE;

/**************************************************************************
  Function declaration
**************************************************************************/
/*****************************************************************************
*Function name: udi_get_capability
*
*Function Description: Obtain the current device supported features according to the device ID
*
*Input parameters: devId: Device ID
*Output parameters: None
*Return Value: Supported Feature Value
*****************************************************************************/
BSP_S32 udi_get_capability(UDI_DEVICE_ID devId);

/*****************************************************************************
*Function name: udi_open
*
*Function Description: Open the device (data channel)
*
*Input parameters: pParam: Device opening configuration parameters
*Output parameters: None
*Return Value: -1: Failed /Other: Success
*****************************************************************************/
UDI_HANDLE udi_open(UDI_OPEN_PARAM *pParam);
#define DRV_UDI_OPEN(pParam)    udi_open(pParam)

/*****************************************************************************
*Function name: udi_close
*
*Function description: Turn off the device (data channel)
*
*Input parameters: handle: device handle
*Output parameters: None
*Return Value: Success/Failed
*****************************************************************************/
BSP_S32 udi_close(UDI_HANDLE handle);
#define DRV_UDI_CLOSE(handle)    udi_close(handle)

/*****************************************************************************
*Function name: udi_write
*
*Function description: Data writing
*
*Input parameters: handle: device handle
*pMemObj: buffer memory or memory linked list object
*u32Size: Data write size or memory linked list object can not be set
*Output parameters:
*
*Return Value: Completed bytes or Success/Failed
*****************************************************************************/
BSP_S32 udi_write(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size);
#define  DRV_UDI_WRITE(handle,pMemObj,u32Size)     udi_write (handle,pMemObj,u32Size)

/*****************************************************************************
*Function name: udi_read
*
*Function description: Data reading
*
*Input parameters: handle: device handle
*pMemObj: buffer memory or memory linked list object
*u32Size: Data read size or memory linked list object can not be set
*Output parameters:
*
*Return Value: Completed bytes or Success/Failed
*****************************************************************************/
BSP_S32 udi_read(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size);
#define DRV_UDI_READ(handle,pMemObj,u32Size)  udi_read(handle,pMemObj,u32Size)

/*****************************************************************************
*Function name: udi_ioctl
*
*Function Description: Data Channel Attribute Configuration
*
*Input parameters: handle: device handle
*u32Cmd: IOCTL command code
*pParam: Operation parameters
*Output parameters:
*
*Return Value: Success/Failed
*****************************************************************************/
BSP_S32 udi_ioctl(UDI_HANDLE handle, BSP_U32 u32Cmd, BSP_VOID* pParam);
#define DRV_UDI_IOCTL(handle,u32Cmd,pParam)    udi_ioctl(handle,u32Cmd,pParam)

/*************************UDI END*************************************/

/*************************MEMORY START********************************/

/**************************************************************************
  Macro definition
**************************************************************************/
/* Memory pool type, for Flags tagging */
typedef enum tagMEM_POOL_TYPE
{
    MEM_NORM_DDR_POOL = 0,
    MEM_ICC_DDR_POOL,
    MEM_ICC_AXI_POOL,
    MEM_POOL_MAX
}MEM_POOL_TYPE;

/**************************************************************************
  Interface statement
**************************************************************************/
BSP_VOID* BSP_Malloc(BSP_U32 u32Size, MEM_POOL_TYPE enFlags);
BSP_VOID* BSP_MallocDbg(BSP_U32 u32Size, MEM_POOL_TYPE enFlags, BSP_U8* pFileName, BSP_U32 u32Line);
BSP_VOID  BSP_Free(BSP_VOID* pMem);
BSP_VOID  BSP_FreeDbg(BSP_VOID* pMem, BSP_U8* pFileName, BSP_U32 u32Line);

/*****************************************************************************
*Function Name: BSP_MALLOC
*
*Function description: BSP dynamic memory allocation
*
*Input parameters: sz: allocated size (byte)
*flags: memory attributes (not used for the time being, reserved)
*Output parameters: None
*Return Value: Allocated memory pointer
*****************************************************************************/
#ifdef __BSP_DEBUG__
#define BSP_MALLOC(sz, flags) BSP_MallocDbg(sz, flags, __FILE__, __LINE__)
#else
#define BSP_MALLOC(sz, flags) BSP_Malloc(sz, flags)
#endif

/*****************************************************************************
*Function Name: BSP_FREE
*
*Function description: BSP dynamic memory release
*
*Input parameters: ptr: Dynamic memory pointer
*Output parameters: None
*Return Value: None
*****************************************************************************/
#ifdef __BSP_DEBUG__
#define BSP_FREE(ptr) BSP_FreeDbg(ptr, __FILE__, __LINE__)
#else
#define BSP_FREE(ptr) BSP_Free(ptr)
#endif



/*****************************************************************************
*Function Name: BSP_SFree
*
*Function description: BSP dynamic memory release (add spin lock protection, used in multi-core scenarios)
*
*Input parameters: pMem: Dynamic memory pointer
*Output parameters: None
*Return Value: None
*****************************************************************************/
BSP_VOID  BSP_SFree(BSP_VOID* pMem);
/*****************************************************************************
*Function Name: BSP_SMalloc
*
*Function description: BSP dynamic memory allocation (add spin lock protection, used in multi-core scenarios)
*
*Input parameters: u32Size: allocated size (byte)
*enFlags: Memory attributes (not used for the time being, reserved)
*Output parameters: None
*Return Value: Allocated memory pointer
*****************************************************************************/
BSP_VOID* BSP_SMalloc(BSP_U32 u32Size, MEM_POOL_TYPE enFlags);


#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_VOID* cacheDmaMalloc(BSP_U32  bytes);
#define DRV_CACHEDMAM_ALLOC(bytes) cacheDmaMalloc(bytes)
#else
#define DRV_CACHEDMAM_ALLOC(bytes) kmalloc(bytes, GFP_KERNEL)
#endif

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 cacheDmaFree(BSP_VOID*  pBuf);
#define DRV_CACHEDMAM_FREE(pBuf) cacheDmaFree(pBuf)
#else
#define DRV_CACHEDMAM_FREE(pBuf) kfree(pBuf)
#endif

/************************************************************************
 *FUNCTION
 *vmEnable
 *DESCRIPTION
 *Memory read and write protection function enable function
 *INPUTS
 *NONE
 *OUTPUTS
 *NONE
 *************************************************************************/
extern int vmEnable(int enable);
#define DRV_VM_ENABLE(flag)   vmEnable(flag)

/*****************************************************************************
 Function Name: vmStateSet
 Function Description: Set the address space status
 Input parameters: None

 Output parameters: None.
 Return Value: None
 Notes:
*****************************************************************************/
#define DRV_VM_STATESET(context, virtAdrs, len, stateMask, state) vmStateSet(context, virtAdrs, len, stateMask, state)

/*****************************************************************************
 Function Name: BSP_CACHE_DATA_FLUSH
 Function description:
 Input parameters:
 Output parameters: None
 Return Value: void
*****************************************************************************/
extern void BSP_CACHE_DATA_FLUSH(void * addr, int size);

/*************************MEMORY END**********************************/

/*************************SOCP START**********************************/

/**************************************************************************
  Macro definition
**************************************************************************/
#define SOCP_CODER_SRC_CHAN                 0x00
#define SOCP_CODER_DEST_CHAN                0x01
#define SOCP_DECODER_SRC_CHAN               0x02
#define SOCP_DECODER_DEST_CHAN              0x03

#define SOCP_MAX_ENCSRC_BACKNUM                 (1)
#define SOCP_MAX_ENCDST_BACKNUM                 (1)


#define SOCP_CHAN_DEF(chan_type, chan_id)   ((chan_type<<16)|chan_id)
#define SOCP_REAL_CHAN_ID(unique_chan_id)   (unique_chan_id & 0xFFFF)
#define SOCP_REAL_CHAN_TYPE(unique_chan_id) (unique_chan_id>>16)

#define SOCP_CODER_DEST_CHAN_0      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 0)
#define SOCP_CODER_DEST_CHAN_1      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 1)
#define SOCP_CODER_DEST_CHAN_2      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 2)
#define SOCP_CODER_DEST_CHAN_3      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 3)
#define SOCP_CODER_DEST_CHAN_4      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 4)
#define SOCP_CODER_DEST_CHAN_5      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 5)
#define SOCP_CODER_DEST_CHAN_6      SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, 6)

#define SOCP_DECODER_SRC_CHAN_0     SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN, 0)
#define SOCP_DECODER_SRC_CHAN_1     SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN, 1)
#define SOCP_DECODER_SRC_CHAN_2     SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN, 2)
#define SOCP_DECODER_SRC_CHAN_3     SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN, 3)

#define SOCP_CODER_SRC_CHAN_BASE            0x00000000
#define SOCP_CODER_DEST_CHAN_BASE           0x00010000
#define SOCP_DECODER_SRC_CHAN_BASE          0x00020000
#define SOCP_DECODER_DEST_CHAN_BASE         0x00030000

/**************************************************************************
  Error code definition
**************************************************************************/
#define BSP_ERR_SOCP_BASE            BSP_DEF_ERR(BSP_MODU_SOCP, 0)
#define BSP_ERR_SOCP_NULL            (BSP_ERR_SOCP_BASE + 0x1)
#define BSP_ERR_SOCP_NOT_INIT        (BSP_ERR_SOCP_BASE + 0x2)
#define BSP_ERR_SOCP_MEM_ALLOC       (BSP_ERR_SOCP_BASE + 0x3)
#define BSP_ERR_SOCP_SEM_CREATE      (BSP_ERR_SOCP_BASE + 0x4)
#define BSP_ERR_SOCP_TSK_CREATE      (BSP_ERR_SOCP_BASE + 0x5)
#define BSP_ERR_SOCP_INVALID_CHAN    (BSP_ERR_SOCP_BASE + 0x6)
#define BSP_ERR_SOCP_INVALID_PARA    (BSP_ERR_SOCP_BASE + 0x7)
#define BSP_ERR_SOCP_NO_CHAN         (BSP_ERR_SOCP_BASE + 0x8)
#define BSP_ERR_SOCP_SET_FAIL        (BSP_ERR_SOCP_BASE + 0x9)
#define BSP_ERR_SOCP_TIMEOUT         (BSP_ERR_SOCP_BASE + 0xa)
#define BSP_ERR_SOCP_NOT_8BYTESALIGN (BSP_ERR_SOCP_BASE + 0xb)
#define BSP_ERR_SOCP_CHAN_RUNNING    (BSP_ERR_SOCP_BASE + 0xc)
#define BSP_ERR_SOCP_CHAN_MODE       (BSP_ERR_SOCP_BASE + 0xd)
#define BSP_ERR_SOCP_DEST_CHAN       (BSP_ERR_SOCP_BASE + 0xe)
#define BSP_ERR_SOCP_DECSRC_SET      (BSP_ERR_SOCP_BASE + 0xf)
/**************************************************************************
  Structural definition
**************************************************************************/


typedef enum tagSOCP_EVENT_E
{
    SOCP_EVENT_PKT_HEADER_ERROR         = 0x1,    /* Baotou detection error, "hisi" */
    SOCP_EVENT_OUTBUFFER_OVERFLOW       = 0x2,    /* Purpose buffer overflow */
    SOCP_EVENT_RDBUFFER_OVERFLOW        = 0x4,    /* R dbuffer overflow */
    SOCP_EVENT_DECODER_UNDERFLOW        = 0x8,    /* Decode source buffer underflow */
    SOCP_EVENT_PKT_LENGTH_ERROR         = 0x10,   /* Decode packet length detection error */
    SOCP_EVENT_CRC_ERROR                = 0x20,   /* Decode CRC verification error */
    SOCP_EVENT_DATA_TYPE_ERROR          = 0x40,   /* Decoded data type error*/
    SOCP_EVENT_HDLC_HEADER_ERROR        = 0x80,   /* Decode HDLC detection error */
    SOCP_EVENT_BUTT
}SOCP_EVENT_E;

/* Data structure pattern of encoded source */
typedef enum tagSOCP_ENCSRC_CHNMODE_E
{
    SOCP_ENCSRC_CHNMODE_CTSPACKET       = 0,    /* Continuous packet ring buffer */
    SOCP_ENCSRC_CHNMODE_FIXPACKET,              /* Fixed-length packet ring buffer */
    SOCP_ENCSRC_CHNMODE_LIST,                   /* Chain ring buffer */
    SOCP_ENCSRC_CHNMODE_BUTT
}SOCP_ENCSRC_CHNMODE_E;

/* Data structure pattern of decode source */
typedef enum tagSOCP_DECSRC_CHNMODE_E
{
    SOCP_DECSRC_CHNMODE_BYTES        = 0,       /* Continuous byte ring buffer */
    SOCP_DECSRC_CHNMODE_LIST,                   /* Chain ring buffer */
    SOCP_DECSRC_CHNMODE_BUTT
}SOCP_DECSRC_CHNMODE_E;

/* Timeout selection and enable data structure */
typedef enum tagSOCP_TIMEOUT_EN_E
{
    SOCP_TIMEOUT_BUFOVF_DISABLE        = 0,       /* Buffer overflows, no interruption is reported */
    SOCP_TIMEOUT_BUFOVF_ENABLE,                   /* Buffer overflow, timeout count reporting interrupt */
    SOCP_TIMEOUT_TRF,                             /* Transmission interrupt timeout count */
    SOCP_TIMEOUT_BUTT
}SOCP_TIMEOUT_EN_E;

/* Enumerate values ??of different priority levels for channels of the same type*/
typedef enum tagSOCP_CHAN_PRIORITY_E
{
    SOCP_CHAN_PRIORITY_0     = 0,               /* Minimum priority*/
    SOCP_CHAN_PRIORITY_1,                       /* Next Lower Priority*/
    SOCP_CHAN_PRIORITY_2,                       /* Next-high priority*/
    SOCP_CHAN_PRIORITY_3,                       /* Highest priority*/
    SOCP_CHAN_PRIORITY_BUTT
}SOCP_CHAN_PRIORITY_E;

/* Data type enumeration value*/
typedef enum tagSOCP_DATA_TYPE_E
{
    SOCP_DATA_TYPE_0            = 0,            /* LTE OAM data */
    SOCP_DATA_TYPE_1,                           /* GU OAM data */
    SOCP_DATA_TYPE_2,                           /* reserve */
    SOCP_DATA_TYPE_3,                           /* reserve */
    SOCP_DATA_TYPE_BUTT
}SOCP_DATA_TYPE_E;


/* BBP digital acquisition mode, data discarding or overwriting */
typedef enum tagSOCP_BBP_DS_MODE_E
{
    SOCP_BBP_DS_MODE_DROP           = 0,        /* Data discard */
    SOCP_BBP_DS_MODE_OVERRIDE,                  /* Data coverage */
    SOCP_BBP_DS_MODE_BUTT
}SOCP_BBP_DS_MODE_E;

/* Encoding source channel data type enable bit */
typedef enum tagSOCP_DATA_TYPE_EN_E
{
    SOCP_DATA_TYPE_EN           = 0,        /* data type enabled, default value */
    SOCP_DATA_TYPE_DIS,                     /* data type not enabled */
    SOCP_DATA_TYPE_EN_BUTT
}SOCP_DATA_TYPE_EN_E;

/* Encoding source channel debug enable bit */
typedef enum tagSOCP_ENC_DEBUG_EN_E
{
    SOCP_ENC_DEBUG_DIS          = 0,       /* debug is not enabled, default value */
    SOCP_ENC_DEBUG_EN,                     /* debug enable */
    SOCP_ENC_DEBUG_EN_BUTT
}SOCP_ENC_DEBUG_EN_E;

/* Decode path packet length configuration structure */
typedef struct tagSOCP_DEC_PKTLGTH_S
{
    BSP_U32                 u32PktMax;         /*Maximum package length*/
    BSP_U32                 u32PktMin;         /*Minimum package length value*/
}SOCP_DEC_PKTLGTH_S;


/* General source channel buffer structure definition*/
typedef struct tagSOCP_SRC_SETBUF_S
{
    BSP_U32                 u32InputStart;      /* Enter the channel start address*/
    BSP_U32                 u32InputEnd;        /* Enter the channel end address*/
    BSP_U32                 u32RDStart;         /* RD buffer start address*/
    BSP_U32                 u32RDEnd;           /* RD buffer end address*/
    BSP_U32                 u32RDThreshold;     /* RD buffer data reporting threshold*/
}SOCP_SRC_SETBUF_S;

/* General purpose channel buffer structure definition*/
typedef struct tagSOCP_DST_SETBUF_S
{
    BSP_U32                 u32OutputStart;     /* Output channel start address*/
    BSP_U32                 u32OutputEnd;       /* Output channel end address*/
    BSP_U32                 u32Threshold;       /* Output channel threshold value */
}SOCP_DST_SETBUF_S;

/* Encoding source channel structure definition*/
typedef struct tagSOCP_CODER_SRC_CHAN_S
{
    BSP_U32                 u32DestChanID;      /* Target channel id*/
    BSP_U32                 u32BypassEn;        /* Channel bypass enable*/
    SOCP_DATA_TYPE_E        eDataType;          /* Data type, specifying the data encapsulation protocol, used to multiplex multiple platforms*/
#if defined (CHIP_BB_6920CS)
    SOCP_DATA_TYPE_EN_E     eDataTypeEn;        /* Data type enable bit*/
    SOCP_ENC_DEBUG_EN_E     eDebugEn;           /* Debug bit enable*/
#endif
    SOCP_ENCSRC_CHNMODE_E   eMode;              /* Channel data mode*/
    SOCP_CHAN_PRIORITY_E    ePriority;          /* Channel priority*/
    SOCP_SRC_SETBUF_S       sCoderSetSrcBuf;
}SOCP_CODER_SRC_CHAN_S;

/* Definition of the configuration structure of the encoding purpose*/
typedef struct tagSOCP_CODER_DEST_CHAN_S
{
#if defined (CHIP_BB_6920CS)
    BSP_U32                 u32EncDstThrh;     /* Encode the threshold threshold of the destination channel, used when arbitrating the channel*/
#endif
    SOCP_DST_SETBUF_S       sCoderSetDstBuf;
}SOCP_CODER_DEST_CHAN_S;

/* Decode the source channel structure definition*/
typedef struct tagSOCP_DECODER_SRC_CHAN_S
{
#if defined (CHIP_BB_6920CS)
    SOCP_DATA_TYPE_EN_E     eDataTypeEn;        /* Data type enable bit*/
#endif
    SOCP_DECSRC_CHNMODE_E   eMode;              /* Channel Mode*/
    SOCP_SRC_SETBUF_S       sDecoderSetSrcBuf;
}SOCP_DECODER_SRC_CHAN_S;

/* Decode the destination channel structure definition*/
typedef struct tagSOCP_DECODER_DEST_CHAN_S
{
    BSP_U32                 u32SrcChanID;       /* Data source channel id*/
    SOCP_DATA_TYPE_E        eDataType;          /* Data type, specifying the data encapsulation protocol, used to multiplex multiple platforms*/
    SOCP_DST_SETBUF_S       sDecoderDstSetBuf;
}SOCP_DECODER_DEST_CHAN_S;

/* General buffer description structure definition*/
typedef struct tagSOCP_BUFFER_RW_S
{
    BSP_CHAR   *pBuffer;                        /* Buffer pointer*/
    BSP_U32     u32Size;                        /* Available buffer size*/
    BSP_CHAR   *pRbBuffer;                      /* Rewind buffer pointer*/
    BSP_U32     u32RbSize;                      /* Rewind buffer size*/
}SOCP_BUFFER_RW_S;

/* Decode the source channel error count structure definition*/
typedef struct tagSOCP_DECODER_ERROR_CNT_S
{
    BSP_U32     u32PktlengthCnt;                /* Packet length detection error count*/
    BSP_U32     u32CrcCnt;                      /* Crc verification error count*/
    BSP_U32     u32DataTypeCnt;                 /* Decoded data type verification error count*/
    BSP_U32     u32HdlcHeaderCnt;               /* 0x7 eCheck error*/
}SOCP_DECODER_ERROR_CNT_S;

/* Decode path packet length configuration structure */
typedef struct tagSOCP_ENCSRC_RSVCHN_SCOPE_S
{
    BSP_U32                 u32RsvIDMin;        /*The minimum value of the encoding source reserved channel id*/
    BSP_U32                 u32RsvIDMax;        /*The maximum value of the channel id of the encoding source is retained*/
}SOCP_ENCSRC_RSVCHN_SCOPE_S;

#define SOCP_OM_CHANNEL_CNT    3
/*******************************************************************************
 Structure name: SOCP_LOG_EXC_INFO_STRU
 Agreement Form:
 ASN.1 Description:
 Structure description: LOG2.0 OM data reset information saving
 Special Note: Space for the structure length is allocated in dual-core shared memory
             When changing this structure, please make sure to update EXCH_A_C_SOCP_LOG_SIZE in excdrv.h synchronously.
*******************************************************************************/
typedef struct
{
    UINT32 logOnFlag;    
    UINT32 dataBaseAddr[SOCP_OM_CHANNEL_CNT];
    UINT32 dataLen[SOCP_OM_CHANNEL_CNT];
    UINT32 dataUnreelAddr[SOCP_OM_CHANNEL_CNT];
    UINT32 dataUnreelLen[SOCP_OM_CHANNEL_CNT]; 
} SOCP_LOG_EXC_INFO_STRU;

typedef struct SOCP_ENC_DST_BUF_LOG_CFG
{
    BSP_U32     guWaterMark;    /* Socp encoding destination channel data transmission waterline, gu channel */
    BSP_U32     lWaterMark;     /* Socp encoding destination channel data transmission waterline, l channel */
    BSP_U32     overTime;       /* Socp encoding destination channel data transmission timeout time */
    BSP_U32     flushFlag;      /* Socp encoding destination channel data output flag immediately */
    BSP_U32     logOnFlag;      /* Lo g2.0 function startup flag */
} SOCP_ENC_DST_BUF_LOG_CFG_STRU;

typedef BSP_S32 (*socp_event_cb)(BSP_U32 u32ChanID, SOCP_EVENT_E u32Event, BSP_U32 u32Param);
typedef BSP_S32 (*socp_read_cb)(BSP_U32 u32ChanID);
typedef BSP_S32 (*socp_rd_cb)(BSP_U32 u32ChanID);

/**************************************************************************
  Function declaration
**************************************************************************/
/*****************************************************************************
 Function Name: BSP_SOCP_SleepIn
 Function Description: SOCP enters sleep
 Input parameters: pu32SrcChanID: encoded source channel ID list
             uSrcChanNum: Number of encoded source channels
             pu32DstChanID: encoded destination channel ID list
             uSrcChanNum: Number of encoding destination channels
 Output parameters: None.
 Return Value: SOCP_OK: Sleep successfully.
             Others: Failed to enter sleep
*****************************************************************************/
extern BSP_U32 DRV_SOCP_SLEEPIN(BSP_U32 *pu32SrcChanID, BSP_U32 uSrcChanNum, BSP_U32 *pu32DstChanID, BSP_U32 uDstChanNum);

/*****************************************************************************
 Function Name: DRV_SOCP_SLEEPOUT
 Function description: SOCP exits sleep
 Input parameters: pu32SrcChanID: encoded source channel ID list
             uSrcChanNum: Number of encoded source channels
             pu32DstChanID: encoded destination channel ID list
             uSrcChanNum: Number of encoding destination channels
 Output parameters: None.
 Return Value: SOCP_OK: Sleep successfully.
             Others: Failed to enter sleep
*****************************************************************************/
extern BSP_U32 DRV_SOCP_SLEEPOUT(BSP_U32 *pu32SrcChanID, BSP_U32 uSrcChanNum, BSP_U32 *pu32DstChanID, BSP_U32 uDstChanNum);

/*****************************************************************************
 Function Name: BSP_SOCP_CoderAllocSrcChan
 Function Description: This interface completes the allocation of the SOCP encoder source channel, sets the channel properties according to the encoder source channel parameters, connects to the target channel, and returns the function execution result.
 Input parameters: pSrcAttr: Encoder source channel parameter structure pointer.
             pSrcChanID: The source channel ID applied for.
 Output parameters: None.
 Return Value: SOCP_OK: The encoding source channel is allocated successfully.
             SOCP_ERROR: Encoding source channel allocation failed.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_CoderAllocSrcChan(SOCP_CODER_SRC_CHAN_S *pSrcAttr, BSP_U32 *pSrcChanID);
#define DRV_SOCP_CoderAllocSrcChan(pSrcAttr, pSrcChanID)    BSP_SOCP_CoderAllocSrcChan(pSrcAttr, pSrcChanID)

/*****************************************************************************
 Function Name: BSP_SOCP_CoderSetDestChanAttr
 Function Description: This interface completes the configuration of a certain encoding target channel and returns the result of function execution.
 Input parameters: u32DestChanID: The target channel ID of the SOCP encoder.
             pDestAttr: SOCP encoder target channel parameter structure pointer.
 Output parameters: None.
 Return Value: SOCP_OK: The encoding destination channel is set successfully.
             SOCP_ERROR: The encoding destination channel setting failed.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_CoderSetDestChanAttr(BSP_U32 u32DestChanID, SOCP_CODER_DEST_CHAN_S *pDestAttr);
#define DRV_SOCP_CoderSetDestChanAttr(u32DestChanID, pDestAttr)    BSP_SOCP_CoderSetDestChanAttr(u32DestChanID, pDestAttr)

/*****************************************************************************
 Function Name: BSP_SOCP_DecoderAllocDestChan
 Function description: This interface completes the allocation of the target channel of the SOCP decoder.
                Set the channel properties according to the decoding target channel parameters.
                And connect the source channel to return the function execution result.
 Input parameters: pAttr: Decoder target channel parameter structure pointer
                         pDestChanID: The target channel ID applied for
 Output parameters: None.
 Return Value: SOCP_OK: The decoding destination channel is assigned successfully.
                             SOCP_ERROR: The decoding destination channel setting failed.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_DecoderAllocDestChan (SOCP_DECODER_DEST_CHAN_S *pAttr, BSP_U32 *pDestChanID);
#define  DRV_SOCP_DecoderAllocDestChan(pAttr ,pDestChanID)  BSP_SOCP_DecoderAllocDestChan(pAttr ,pDestChanID)

/*****************************************************************************
 Function Name: BSP_SOCP_DecoderSetSrcChanAttr
 Function Description: This interface completes the configuration of a certain decoding source channel and returns the result of function execution.
 Input parameters: u32SrcChanID: Decoder source channel ID
                         pInputAttr: Decoder source channel parameter structure pointer
 Output parameters: None.
 Return Value: SOCP_OK: The decoding source channel is set successfully
                             SOCP_ERROR: Decoding source channel setting failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_DecoderSetSrcChanAttr ( BSP_U32 u32SrcChanID,SOCP_DECODER_SRC_CHAN_S *pInputAttr);
#define  DRV_SOCP_DecoderSetSrcChanAttr(u32SrcChanID, pInputAttr)   BSP_SOCP_DecoderSetSrcChanAttr(u32SrcChanID, pInputAttr)

/*****************************************************************************
 Function Name: BSP_SOCP_DecoderGetErrCnt
 Function Description: This interface gives the count value of four abnormal situations in the decoding channel.
 Input parameters: u32ChanID: Decoder channel ID
                         pErrCnt: Decoder exception count structure pointer
 Output parameters: None.
 Return Value: SOCP_OK: Return exception count successfully
                             SOCP_ERROR: Returns exception count failed
*****************************************************************************/
extern  BSP_S32 BSP_SOCP_DecoderGetErrCnt (BSP_U32 u32ChanID, SOCP_DECODER_ERROR_CNT_S *pErrCnt);
#define  DRV_SOCP_DecoderGetErrCnt(u32ChanID, pErrCnt)   BSP_SOCP_DecoderGetErrCnt ( u32ChanID, pErrCnt)

/*****************************************************************************
 Function Name: BSP_SOCP_FreeChannel
 Function Description: This interface releases the assigned codec channel according to the channel ID.
 Input parameters: u32ChanID:ChanID.
 Output parameters: None.
 Return Value: SOCP_OK: Channel release was successfully released.
             SOCP_ERROR: Channel release failed.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_FreeChannel(BSP_U32 u32ChanID);
#define  DRV_SOCP_FreeChannel(u32ChanID)  BSP_SOCP_FreeChannel(u32ChanID)

/*****************************************************************************
 Function Name: BSP_SOCP_RegisterEventCB
 Function Description: This interface registers event callback functions for a given channel.
 Input parameters: u32ChanID:ChanID.
             EventCB: Event callback function, refer to the socp_event_cb function definition
 Output parameters: None.
 Return Value: SOCP_OK: The event callback function was registered successfully.
             SOCP_ERROR: The registration event callback function failed.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_RegisterEventCB(BSP_U32 u32ChanID, socp_event_cb EventCB);
#define  DRV_SOCP_RegisterEventCB( u32ChanID,  EventCB) BSP_SOCP_RegisterEventCB( u32ChanID,  EventCB)

/*****************************************************************************
 Function Name: BSP_SOCP_Start
 Function Description: This interface is used for source channels, start encoding or decoding.
 Input parameters: u32SrcChanID: Source channel ID.
 Output parameters: None.
 Return Value: SOCP_OK: Encoding or decoding is started successfully.
             SOCP_ERROR: Encoding or decoding failed to start.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_Start(BSP_U32 u32SrcChanID);
#define  DRV_SOCP_Start( u32SrcChanID)   BSP_SOCP_Start( u32SrcChanID)

/*****************************************************************************
 Function Name: BSP_SOCP_Stop
 Function Description: This interface is used for source channels, stop encoding or decoding.
 Input parameters: u32SrcChanID: Source channel ID.
 Output parameters: None.
 Return Value: SOCP_OK: Encoding or decoding has stopped successfully.
             SOCP_ERROR: Encoding or decoding stop failed.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_Stop(BSP_U32 u32SrcChanID);
#define DRV_SOCP_Stop( u32SrcChanID)  BSP_SOCP_Stop( u32SrcChanID)


/*****************************************************************************
 Function Name: BSP_SOCP_SetTimeout
 Function Description: This interface sets the timeout threshold.
 Input parameters: u32Timeout: Timeout threshold

 Output parameters: None.
 Return Value: SOCP_OK: Set the timeout threshold successfully.
                             SOCP_ERROR: Failed to set the timeout threshold
*****************************************************************************/
extern BSP_S32 BSP_SOCP_SetTimeout (SOCP_TIMEOUT_EN_E eTmOutEn, BSP_U32 u32Timeout);
#define DRV_SOCP_SetTimeout(eTmOutEn,u32Timeout)   BSP_SOCP_SetTimeout(eTmOutEn,u32Timeout)

/*****************************************************************************
 Function Name: BSP_SOCP_SetDecPktLgth
 Function description: Set the limit value of the decoding packet length
 Input parameters: pPktlgth: Extreme value of the decoded packet length

 Output parameters: None.
 Return Value: SOCP_OK: Set successfully.
                    Other values: Setting failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_SetDecPktLgth(SOCP_DEC_PKTLGTH_S *pPktlgth);
#define DRV_SOCP_SetDecPktLgth(pPktlgth)   BSP_SOCP_SetDecPktLgth(SOCP_DEC_PKTLGTH_S *pPktlgth)

/*****************************************************************************
 Function Name: BSP_SOCP_SetDebug
 Function description: Set the debug mode of the decoding source channel
 Input parameters: u32ChanID:ChanID
                  u32DebugEn: debug logo
 Output parameters: None.
 Return Value: SOCP_OK: Set successfully.
                     Other values: Setting failed
*****************************************************************************/
BSP_S32 BSP_SOCP_SetDebug(BSP_U32 u32DestChanID, BSP_U32 u32DebugEn);
#define DRV_SOCP_SetDebug(u32ChanID, u32DebugEn)   BSP_SOCP_SetDebug(BSP_U32 u32DestChanID, BSP_U32 u32DebugEn)

/*****************************************************************************
 Function Name: BSP_SOCP_ChnSoftReset
 Function Description: Source Channel Soft Reset
 Input parameters: u32ChanID:ChanID
 Output parameters: None.
 Return Value: SOCP_OK: Set the reset is successful.
                     Other values: Reset failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_ChnSoftReset(BSP_U32 u32ChanID);
#define DRV_SOCP_ChnSoftReset(u32ChanID)   BSP_SOCP_ChnSoftReset(BSP_U32 u32ChanID)

/*****************************************************************************
 Function Name: BSP_SOCP_GetWriteBuff
 Function Description: This interface is used to obtain a write data buffer.
 Input parameters: u32SrcChanID: Source channel ID
                  pBuff: :Write data buffer

 Output parameters: None.
 Return Value: SOCP_OK: Get the buffer to write data successfully.
                             SOCP_ERROR: Failed to get the data write buffer
*****************************************************************************/
extern  BSP_S32 BSP_SOCP_GetWriteBuff( BSP_U32 u32SrcChanID, SOCP_BUFFER_RW_S *pBuff);
#define  DRV_SOCP_GetWriteBuff( u32SrcChanID, pBuff)  BSP_SOCP_GetWriteBuff( u32SrcChanID, pBuff)

/*****************************************************************************
 Function Name: BSP_SOCP_WriteDone
 Function Description: This interface is used for writing data and provides the length of the data written.
 Input parameters: u32SrcChanID: Source channel ID
                  u32WrtSize: length of the written data
 Output parameters: None.
 Return Value: SOCP_OK: The data was successfully written.
                             SOCP_ERROR: Failed to write data
*****************************************************************************/
extern   BSP_S32  BSP_SOCP_WriteDone(BSP_U32 u32SrcChanID, BSP_U32 u32WrtSize);
#define  DRV_SOCP_WriteDone(u32SrcChanID, u32WrtSize)  BSP_SOCP_WriteDone(u32SrcChanID, u32WrtSize)

/*****************************************************************************
 Function Name: BSP_SOCP_RegisterRdCB
 Function Description: This interface is used to register a callback function for reading data from the RD buffer.
 Input parameters: u32SrcChanID: Source channel ID
                  RdCB: Event callback function
 Output parameters: None.
 Return Value: SOCP_OK: The RD ring buffer read data callback function was successfully registered.
                             SOCP_ERROR: Registering RD ring buffer read data callback function failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_RegisterRdCB(BSP_U32 u32SrcChanID, socp_rd_cb RdCB);
#define DRV_SOCP_RegisterRdCB( u32SrcChanID,  RdCB) BSP_SOCP_RegisterRdCB( u32SrcChanID,  RdCB)

/*****************************************************************************
 Function Name: BSP_SOCP_GetRDBuffer
 Function description: This interface is used to obtain the data pointer of the RD buffer.
 Input parameters: u32SrcChanID: Source channel ID
                  pBuff: RD buffer
 Output parameters: None.
 Return Value: SOCP_OK: Getting the RD ring buffer successfully
                             SOCP_ERROR: Failed to obtain RD ring buffer
*****************************************************************************/
extern  BSP_S32 BSP_SOCP_GetRDBuffer( BSP_U32 u32SrcChanID,SOCP_BUFFER_RW_S *pBuff);
#define DRV_SOCP_GetRDBuffer(  u32SrcChanID,pBuff)  BSP_SOCP_GetRDBuffer(  u32SrcChanID,pBuff)


/*****************************************************************************
 Function Name: BSP_SOCP_ReadRDDone
 Function Description: This interface is used to notify the SOCP driver of the upper layer to actually read data from the RD buffer.
 Input parameters: u32SrcChanID: Source channel ID
                  u32RDSize: The actual data length read from the RD buffer
 Output parameters: None.
 Return Value: SOCP_OK: Read data in RDbuffer successfully
                             SOCP_ERROR: Failed to read data in RDbuffer
*****************************************************************************/
extern BSP_S32 BSP_SOCP_ReadRDDone(BSP_U32 u32SrcChanID, BSP_U32 u32RDSize);
#define DRV_SOCP_ReadRDDone( u32SrcChanID,  u32RDSize)   BSP_SOCP_ReadRDDone( u32SrcChanID,  u32RDSize)

/*****************************************************************************
 Function Name: BSP_SOCP_RegisterReadCB
 Function Description: This interface is used to register a callback function for reading data.
 Input parameters: u32DestChanID: Target channel ID
                  ReadCB: Event callback function
 Output parameters: None.
 Return Value: SOCP_OK: Registered data read callback function successfully
                             SOCP_ERROR: Registering the read data callback function failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_RegisterReadCB( BSP_U32 u32DestChanID, socp_read_cb ReadCB);
#define   DRV_SOCP_RegisterReadCB(  u32DestChanID,  ReadCB)  BSP_SOCP_RegisterReadCB(  u32DestChanID,  ReadCB)

/*****************************************************************************
 Function Name: BSP_SOCP_RegisterReadCB
 Function Description: This interface is used to obtain the read data buffer pointer.
 Input parameters: u32DestChanID: Target channel ID
                  ReadCB: Read data buffer
 Output parameters: None.
 Return Value: SOCP_OK: The read data buffer was successfully obtained.
                             SOCP_ERROR: The read data buffer was successfully obtained.
*****************************************************************************/
extern BSP_S32 BSP_SOCP_GetReadBuff( BSP_U32 u32DestChanID,SOCP_BUFFER_RW_S *pBuffer);
#define   DRV_SOCP_GetReadBuff(  u32DestChanID,  pBuffer)  BSP_SOCP_GetReadBuff(  u32DestChanID,  pBuffer)

/*****************************************************************************
 Function Name: BSP_SOCP_ReadDataDone
 Function Description: This interface is used by the upper layer to tell the SOCP driver to read the actual data from the target channel.
 Input parameters: u32DestChanID: Target channel ID
                  u32ReadSize: The length of the read data
 Output parameters: None.
 Return Value: SOCP_OK: Read data successfully.
                             SOCP_ERROR: failed to read data
*****************************************************************************/
extern BSP_S32 BSP_SOCP_ReadDataDone(BSP_U32 u32DestChanID,BSP_U32 u32ReadSize);
#define  DRV_SOCP_ReadDataDone( u32DestChanID, u32ReadSize)  BSP_SOCP_ReadDataDone( u32DestChanID, u32ReadSize)

/*****************************************************************************
 Function Name: BSP_SOCP_SetBbpEnable
 Function Description: Enable or stop the BBP channel.
 Input parameters: bEnable:Channel ID
 Output parameters: None.
 Return Value: SOCP_OK: Set successfully.
                   Other values: Setting failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_SetBbpEnable(BSP_BOOL bEnable);
#define  DRV_SOCP_SetBbpEnable( bEnable)  BSP_SOCP_SetBbpEnable(BSP_BOOL bEnable)

/*****************************************************************************
 Function Name: BSP_SOCP_SetBbpDsMode
 Function description: Set the BBP DS channel data overflow processing mode.
 Input parameters: eDsMode: Processing mode setting when DS channel data overflow
 Output parameters: None.
 Return Value: SOCP_OK: Set successfully.
                   Other values: Setting failed
*****************************************************************************/
extern BSP_S32 BSP_SOCP_SetBbpDsMode(SOCP_BBP_DS_MODE_E eDsMode);
#define  DRV_SOCP_SetBbpDsMode( eDsMode)  BSP_SOCP_SetBbpDsMode(SOCP_BBP_DS_MODE_E eDsMode)

/*****************************************************************************
*Function Name: BSP_SOCP_GetRsvChnScope
*Function Description: Obtain the range of reserved channel ID
*Input parameters: None
*Output parameters: pScope, the ID range of the encoding source reserved channel
*Return value:
*****************************************************************************/
extern BSP_S32 BSP_SOCP_GetRsvChnScope(SOCP_ENCSRC_RSVCHN_SCOPE_S *pScope);
#define  DRV_SOCP_GetRsvChnScope(pScope)  BSP_SOCP_GetRsvChnScope(SOCP_ENCSRC_RSVCHN_SCOPE_S *pScope)

/*****************************************************************************
*Function Name: DRV_SOCP_SET_HIFICHAN
*Function Description: Assign fixed channels
*Input parameters: Destination channel ID
*Output parameters: None
*Return value:
*****************************************************************************/
extern BSP_S32 DRV_SOCP_SET_HIFICHAN(BSP_U32 u32DestChanID);

/*****************************************************************************
*Function Name: DRV_SOCP_START_HIFICHAN
*Function Description: Start the fixed channel
*Input parameters: None
*Output parameters: None
*Return value:
*****************************************************************************/
extern BSP_VOID DRV_SOCP_START_HIFICHAN(BSP_VOID);

/*****************************************************************************
*Function Name: BSP_SOCP_StartDsp
*Function description:enable DSP channel
*Input parameters: 
*Output parameters: None
*Return value:
*****************************************************************************/
BSP_VOID BSP_SOCP_StartDsp(BSP_VOID);


/*****************************************************************************
*Function Name: BSP_SOCP_StopDsp
*Function description:disable DSP channel
*Input parameters: 
*Output parameters: None
*Return value:
*****************************************************************************/
BSP_S32 BSP_SOCP_StopDsp(BSP_VOID);

/*****************************************************************************
*Function Name: BSP_SOCP_StartTdDsp
*Function description:enable Tds DSP channel
*Input parameters: 
*Output parameters: None
*Return value:
*****************************************************************************/
BSP_VOID BSP_SOCP_StartTdDsp(BSP_VOID);


/*****************************************************************************
*Function Name: BSP_SOCP_StopTdDsp
*Function description:disable Tds DSP channel
*Input parameters: 
*Output parameters: None
*Return value:
*****************************************************************************/
BSP_S32 BSP_SOCP_StopTdDsp(BSP_VOID);


/*****************************************************************************
*Function name: BSP_SOCP_GetLogCfg
*
*Function Description: Read LOG2.0 configuration
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: SOCP_ENC_DST_BUF_LOG_CFG_STRU *
*****************************************************************************/
extern SOCP_ENC_DST_BUF_LOG_CFG_STRU * BSP_SOCP_GetLogCfg();

/*****************************************************************************
*Function Name: BSP_SOCP_EncDstBufFlush
*
*Function description: SOCP encoding purpose buffer cache data is output immediately
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: BSP_S32 BSP_OK:Success BSP_ERROR:Failed 
*****************************************************************************/
extern BSP_S32 BSP_SOCP_EncDstBufFlush();
#define DRV_SOCP_EncDstBufFlush() BSP_SOCP_EncDstBufFlush()
/*****************************************************************************
*Function Name: BSP_SOCP_SaveEncDstBufInfo
*
*Function description: Save the buffer cache data information for SOCP encoding purpose
*
*Input parameters: SOCP_LOG_EXC_INFO_STRU *logExcInfo
*
*Output parameters: None
*
*Return Value: BSP_S32 BSP_OK:Success BSP_ERROR:Failed 
*****************************************************************************/
extern BSP_S32 BSP_SOCP_SaveEncDstBufInfo(SOCP_LOG_EXC_INFO_STRU * logExcInfo);


/*************************SOCP END************************************/

/*************************CIPHER START**********************************/

/* Error code */
#define CIPHER_SUCCESS                  0

#define CHN_BUSY                    0
#define CHN_FREE                    1

/* Cipher supports the types of algorithms, please note that the enum value is consistent with the register setting value. */
typedef enum tagCIPHER_ALGORITHM_E
{
    CIPHER_ALG_NULL = 0,        /* No algorithm */
    CIPHER_ALG_SNOW3G  = 1,     /* Sno w3 g algorithm */
    CIPHER_ALG_AES_128 = 2,     /* Ae s128 algorithm */
    CIPHER_ALG_AES_192 = 3,     /* Ae s192 algorithm */
    CIPHER_ALG_AES_256 = 4,     /* Ae s256 algorithm */
    CIPHER_ALG_BUTTOM
}CIPHER_ALGORITHM_E;

typedef enum tagKDF_SHA_KEY_SOURCE_E
{
    SHA_KEY_SOURCE_DDR,             //sha_key comes from the address space specified by input point

    SHA_KEY_SOURCE_KEYRAM,          //Sha key comes from internal key ram

    SHA_KEY_SOURCE_FOLLOW,          //Sha key comes from the sha key of the last kdf operation

    SHA_KEY_SOURCE_RESULT,          //Sha key comes from the result of the last kdf operation

    SHA_KEY_SOURCE_MAX
}KDF_SHA_KEY_SOURCE_E;

typedef enum tagKDF_SHA_S_SOURCE_E
{
    SHA_S_SOURCE_DDR,               //sha_s comes from the address space specified by input point

    SHA_S_SOURCE_KEYRAM,            //Sha s comes from internal key ram

    SHA_S_SOURCE_MAX
}KDF_SHA_S_SOURCE_E;

/**************************************************************************
  Enumeration definition
**************************************************************************/
enum CIPHER_SECURITY_CHANNEL_ENUM
{
    CIPHER_SECURITY_CHANNEL_0            = 0,                /*Channel 0*/
    CIPHER_SECURITY_CHANNEL_1            = 1,                /*Channel 1*/
    CIPHER_SECURITY_CHANNEL_2            = 2,                /*Channel 2*/
    CIPHER_SECURITY_CHANNEL_3            = 3,                /*Channel 3*/

    CIPHER_SECURITY_CHANNEL_BUTT
};

/* Channel number*/
#define LTE_SECURITY_CHANNEL_NAS    CIPHER_SECURITY_CHANNEL_2
#define LTE_SECURITY_CHANNEL_RRC    CIPHER_SECURITY_CHANNEL_2
#define LTE_SECURITY_CHANNEL_UL_SRB CIPHER_SECURITY_CHANNEL_1
#define LTE_SECURITY_CHANNEL_DL_SRB CIPHER_SECURITY_CHANNEL_1
#define LTE_SECURITY_CHANNEL_UL_DRB CIPHER_SECURITY_CHANNEL_0
#define LTE_SECURITY_CHANNEL_DL_DRB CIPHER_SECURITY_CHANNEL_3

/* Cipher operation classification */
typedef enum tagCIPHER_SINGLE_OPT_E
{
    /* Individual operation type, individual enumeration values ??are consistent with register settings */
    CIPHER_OPT_ENCRYPT = 0x0,   /* encryption */
    CIPHER_OPT_DECRYPT = 0x1,   /* Decryption */
    CIPHER_OPT_PROTECT_SMAC = 0x2,   /* Protection operation, output mac length is 2 byte */
    CIPHER_OPT_PROTECT_LMAC = 0x3,   /* Protection operation, output mac length is 4 byte */
    CIPHER_OPT_CHECK_PRE_SMAC  = 0x4,   /* Check operation, the length of the mac is 2 byte, the position is in front of the data */
    CIPHER_OPT_CHECK_POST_SMAC = 0x5,   /* Check operation, the length of mac is 2 byte, the position is behind the data */
    CIPHER_OPT_CHECK_PRE_LMAC  = 0x6,   /* Check operation, the mac length is 4 byte, the position is in front of the data */
    CIPHER_OPT_CHECK_POST_LMAC = 0x7,   /* Check operation, the length of mac is 4 byte, the position is behind the data */
    CIPHER_SINGLE_OPT_BUTTOM
}CIPHER_SINGLE_OPT_E;


/* CIPHER related operations classification *///PDCP related operations are arbitrary combination

typedef enum tagCHPHER_RELA_OPT_E
{
    /*  Associated operation type */
    CIPHER_PDCP_PRTCT_ENCY,     /*Pdcp protection encryption (uplink)*/
    CIPHER_PDCP_DECY_CHCK,      /*Pdcp decryption check (down line)*/

    CIPHER_NAS_ENCY_PRTCT,      /* Nas layer encryption protection (uplink) */
    CIPHER_NAS_CHCK_DECY,       /* Nas layer check decryption (down line) */

	CIPHER_RELA_OPT_BUTTOM
}CIPHER_RELA_OPT_E;


/* CIPHER key length, note that the enum value is consistent with the register setting value */
typedef enum tagCIPHER_KEY_LEN_E
{
    CIPHER_KEY_L128 = 0,        /* Key length 128 bit */
    CIPHER_KEY_L192 = 1,        /* Key length 192 bit */
    CIPHER_KEY_L256 = 2,        /* Key length 256 bit */
    CIPHER_KEY_LEN_BUTTOM
}CIPHER_KEY_LEN_E;


/* Cipher operation submission type */
typedef enum tagCIPHER_SUBMIT_TYPE_E
{
    CIPHER_SUBM_NONE = 0,           /* No notice */
    CIPHER_SUBM_BLK_HOLD = 1,       /* Loop query waiting for completion */
    CIPHER_SUBM_CALLBACK = 2,       /* Callback function notification */
    CIPHER_SUBM_BUTTOM
}CIPHER_SUBMIT_TYPE_E;

/* Cipher notification type */
typedef enum tagCIPHER_NOTIFY_STAT_E
{
    CIPHER_STAT_OK = 0,           /* Completed successfully */
    CIPHER_STAT_CHECK_ERR = 1,    /* Integrity check error */
    CIPHER_STAT_BUTTOM
}CIPHER_NOTIFY_STAT_E;

typedef enum CIPHER_HDR_BIT_ENUM
{
    CIPHER_HDR_BIT_TYPE_0              = 0,     /*The corresponding header or additional header byte length is 0, which means that this function is not activated.*/
    CIPHER_HDR_BIT_TYPE_5              = 1,     /*The corresponding header or additional header byte length is 1, which means that this function is activated. The content of the corresponding additional header is the lower 5 bits of the count value, which participates in the protection.*/
    CIPHER_HDR_BIT_TYPE_7              = 2,     /*The corresponding header or additional header byte length is 1, which means that this function is activated. The content of the corresponding additional header is the lower 7 bits of the count value, which participates in the protection.*/
    CIPHER_HDR_BIT_TYPE_12             = 3,     /*The corresponding header or additional header byte length is 2, which means that this function is activated. The content of the corresponding additional header is the lower 12 bits of the count value, which participates in the protection.*/

    CIPHER_APPEND_HDR_BIT_TYPE_5       = 4,     /*The corresponding header or additional header byte length is 1, which means that this function is activated. The content of the corresponding additional header is the lower 5 digits of the count value. It does not participate in the protection, only moves*/
    CIPHER_APPEND_HDR_BIT_TYPE_7       = 5,     /*The corresponding header or additional header byte length is 1, which means that this function is activated. The content of the corresponding additional header is the lower 7 bits of the count value. It does not participate in the protection, only moves*/
    CIPHER_APPEND_HDR_BIT_TYPE_12      = 6,     /*The corresponding header or additional header byte length is 2, which means that this function is activated. The content of the corresponding additional header is the lower 12 bits of the count value. It does not participate in the protection, only moves*/

    CIPHER_HDR_BIT_TYPE_BUTT
}CIPHER_HDR_E;

/* Algorithm configuration information */
typedef struct tagCIHPER_ALG_INFO_S
{
    BSP_U32 u32KeyIndexSec;
    BSP_U32 u32KeyIndexInt;
    CIPHER_ALGORITHM_E enAlgSecurity;          /* Selection of safe operation algorithm, aec algorithm or sno w3 g algorithm */
    CIPHER_ALGORITHM_E enAlgIntegrity;         /* Selection of integrity operation algorithm, aec algorithm or sno w3 g algorithm */
}CIHPER_ALGKEY_INFO_S;


/*Package acceleration configuration information*/
typedef struct tagACC_SINGLE_CFG_S
{
    BSP_U32 u32BearId;
    BSP_U32 u32AppdHeaderLen;
    BSP_U32 u32HeaderLen;
    BSP_U32 u32Count;
    BSP_BOOL bMemBlock;                 /* Is it a single memory block? */
    BSP_U32 u32BlockLen;                /* If it is a single memory, you need to know the length*/
    BSP_U32 u32Offset;                  /* Use data to offset the packet start address*/
    BSP_U32 u32OutLen;                  /* Data length used*/
    BSP_U32 u32Aph;                      /*Attached Baotou Domain*/
    CIHPER_ALGKEY_INFO_S stAlgKeyInfo;  /* Algorithm key configuration information for individual operation */
} ACC_SINGLE_CFG_S;

/* Cipher configuration information settings */
typedef struct tagCIHPER_SINGLE_CFG_S
{
    CIPHER_SINGLE_OPT_E enOpt;          /* Individual operation type (pure dma operation ignores the following algorithm configuration) */
    BSP_U8 u8BearId;
    BSP_U8 u8Direction;
    CIPHER_HDR_E enAppdHeaderLen;
    CIPHER_HDR_E enHeaderLen;
    BSP_U32 u32Count;
    BSP_BOOL bMemBlock;                 /* Is it a single memory block? */
	BSP_U32 u32BlockLen;                /* If it is a single memory, you need to know the length*/
    CIHPER_ALGKEY_INFO_S stAlgKeyInfo;  /* Algorithm key configuration information for individual operation */
} CIHPER_SINGLE_CFG_S;

typedef struct tagCIHPER_RELA_CFG_S
{
    CIPHER_RELA_OPT_E enOpt;             /* Associated operation type (pure dma operation ignores the following algorithm configuration) */
    BSP_U8 u8BearId;
    BSP_U8 u8Direction;
    CIPHER_HDR_E enAppdHeaderLen;
    CIPHER_HDR_E enHeaderLen;
    BSP_U32 u32Count;
    BSP_BOOL bMemBlock;                 /* Is it a single memory block? */
	BSP_U32 u32BlockLen;                /* If it is a single memory, you need to know the length*/
    CIHPER_ALGKEY_INFO_S stAlgKeyInfo;  /* Algorithm key configuration information for the first step of association operation */

} CIHPER_RELA_CFG_S;


typedef struct tagS_CONFIG_INFO_S
{
    KDF_SHA_S_SOURCE_E   enShaSSource;      //Sha s Source

    BSP_U32              u32ShaSIndex;       //When Sha s comes from key ram, its index in key ram

    BSP_U32              u32ShaSLength;     //The length of Sha s

    BSP_VOID             *pSAddr;           //
}S_CONFIG_INFO_S;

typedef enum tagCIPHER_KEY_OUTPUT_E
{
    CIPHER_KEY_NOT_OUTPUT,    //No key output

    CIPHER_KEY_OUTPUT,        //Output key

    CIPHER_KEY_OUTPUT_BUTTOM
}CIPHER_KEY_OUTPUT_E;

typedef BSP_U32 CIPHER_KEY_LEN_E_U32;
typedef BSP_U32 CIPHER_KEY_OUTPUT_E_U32;

/* Get the structure of key length and key content*/
typedef struct
{
    CIPHER_KEY_LEN_E_U32    enKeyLen;//The length of the key to be read is passed in from the upper layer

    BSP_VOID                *pKeyAddr;//Key value storage address

    CIPHER_KEY_LEN_E_U32    *penOutKeyLen;//This address is used to store the length of the actual returned key

}KEY_GET_S;

/* When Key makes, get the length and key value of the key*/
typedef struct
{
    CIPHER_KEY_OUTPUT_E_U32 enKeyOutput; //Indicates whether to output key into st key get

    KEY_GET_S stKeyGet;
}KEY_MAKE_S;


/* Complete callback function type definition */
/*
u32ChNum: channel number;
u32SourAddr: Source address
u32DestAddr: Destination address
enStatus: The status of the notification, such as: Completed /Data transmission error /Data completion check error
u32Private: User private data;
*/
typedef BSP_VOID (*CIPHER_NOTIFY_CB_T)(BSP_U32 u32ChNum, BSP_U32 u32SourAddr,
              BSP_U32 u32DestAddr, CIPHER_NOTIFY_STAT_E enStatus, BSP_U32 u32Private);

typedef BSP_VOID (*CIPHER_FREEMEM_CB_T)(BSP_VOID* pMemAddr);

/**************************************************************************
  KDF related
**************************************************************************/

typedef struct tagKEY_CONFIG_INFO_S
{
    KDF_SHA_KEY_SOURCE_E enShaKeySource;    //Sha key source

    BSP_U32              u32ShaKeyIndex;     //When Sha key comes from key ram, its index in key ram

    BSP_VOID             *pKeySourceAddr;   //
}KEY_CONFIG_INFO_S;


/*****************************************************************************
*Function Name: BSP_Accelerator_DMA
*
*Function Description: Configuration descriptor for the specified channel, used for simple DMA transfer
*
*Input parameters: u32BdFifoAddr: BDFIFO home address
*pInMemMgr: Enter the buffer home address
*pOutMemMgr: Output buffer home address
*Output parameters: None
*Return Value: Success/Failed
*****************************************************************************/
BSP_S32 BSP_Accelerator_DMA(BSP_U32 u32BdFifoAddr, const BSP_VOID* pInMemMgr, const BSP_VOID* pOutMemMgr,ACC_SINGLE_CFG_S *pstCfg);

/*****************************************************************************
*Function Name: BSP_Accelerator_Cipher
*
*Function Description: Configure the descriptor for the specified channel, used for operations that require Cipher to perform encryption.
*
*Input parameters: u32BdFifoAddr: BDFIFO home address
*pInMemMgr: Enter the buffer home address
*pOutMemMgr: Output buffer home address
*pstCfg: Cipher configuration information
*Output parameters: None
*Return Value: Success/Failed
*****************************************************************************/
BSP_S32 BSP_Accelerator_Cipher(BSP_U32 u32BdFifoAddr, const BSP_VOID* pInMemMgr, const BSP_VOID* pOutMemMgr,
               ACC_SINGLE_CFG_S *pstCfg);


/**************************************************************************
 Function declaration
**************************************************************************/

/*****************************************************************************
*Function Name: BSP_CIPHER_RegistNotifyCB
*
*Function Description: Register notification callback function
*
*Input parameters: pFunNotifyCb: ??Notification callback function
*
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*: Failed:CIPHER_NOT_INIT
*****************************************************************************/
BSP_S32 BSP_CIPHER_RegistNotifyCB(CIPHER_NOTIFY_CB_T pFunNotifyCb);

/*****************************************************************************
*Function Name: BSP_CIPHER_FreeMemCB
*
*Function description: Register Memory release callback function
*
*Input parameters: u32Chn: Channel number
*bSrc: 0: Input Buffer /1: Output Buffer
*pFunFreeMemCb: Memory releases callback function
*
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*: Failed:CIPHER_NOT_INIT
*: CIPHER_INVALID_CHN
*****************************************************************************/
BSP_S32 BSP_CIPHER_RegistFreeMemCB(BSP_U32 u32Chn, BSP_BOOL bSrc, CIPHER_FREEMEM_CB_T pFunFreeMemCb);

/*****************************************************************************
*Function Name: BSP_CIPHER_Purge
*
*Function description: Clear all CIPHER tasks of currently specified channels (blocking the interface, returning after completing the purge)
*
*Input parameter: u32Chn: Channel number to be cleared
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*: Failed:CIPHER_NOT_INIT
*: CIPHER_INVALID_CHN
*: CIPHER_TIME_OUT
*****************************************************************************/
BSP_S32 BSP_CIPHER_Purge(BSP_U32 u32Chn);

/*****************************************************************************
*Function Name: BSP_CIPHER_SingleSubmitTask
*
*Function Description: Starts the operation of the CIPHER specified channel for separate operation.
*
*Input parameters: u32Chn: Channel number
*pInMemMgr: Enter buffer memory management structure pointer
*pOutMemMgr: Output buffer memory management structure pointer
*pstCfg: Cipher operation configuration properties
*pstSubmAttr: Submitted properties
*u32Private:Private data
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*Failed:CIPHER_NOT_INIT
*CIPHER_INVALID_CHN
*CIPHER_NULL_PTR
*CIPHER_ALIGN_ERROR
*CIPHER_INVALID_ENUM
*            CIPHER_PURGING
*            CIPHER_FIFO_FULL
*            CIPHER_TIME_OUT
*****************************************************************************/
BSP_S32 BSP_CIPHER_SingleSubmitTask(BSP_U32 u32Chn, const BSP_VOID* pInMemMgr, const BSP_VOID* pOutMemMgr,
                   CIHPER_SINGLE_CFG_S *pstCfg, CIPHER_SUBMIT_TYPE_E enSubmAttr, BSP_U32 u32Private);

/*****************************************************************************
*Function Name: BSP_CIPHER_RelaSubmitTask
*
*Function Description: Starts the work of the CIPHER specified channel for associated operations.
*
*Input parameters: u32Chn: Channel number
*pInMemMgr: Enter buffer memory management structure pointer
*pOutMemMgr: Output buffer memory management structure pointer
*pstCfg: Cipher operation configuration properties
*enSubmAttr: Submitted properties
*u32Private:Private data
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*Failed:CIPHER_NOT_INIT
*CIPHER_INVALID_CHN
*CIPHER_NULL_PTR
*CIPHER_ALIGN_ERROR
*CIPHER_INVALID_ENUM
*            CIPHER_PURGING
*            CIPHER_FIFO_FULL
*            CIPHER_TIME_OUT
*****************************************************************************/
BSP_S32 BSP_CIPHER_RelaSubmitTask(BSP_U32 u32Chn, const BSP_VOID* pInMemMgr, const BSP_VOID* pOutMemMgr,
                   CIHPER_RELA_CFG_S *pstCfg, CIPHER_SUBMIT_TYPE_E enSubmAttr, BSP_U32 u32Private);

/*****************************************************************************
*Function Name: BSP_CIPHER_SetKeyIndex
*
*Function Description: Set the key value
*
*Input parameters: pKeyAddr: Key address
*enKeyLen : Key length
*u32KeyIndex: Key Index
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*: Failed:CIPHER_NOT_INIT
*: CIPHER_NULL_PTR
*: CIPHER_INVALID_ENUM
*: CIPHER_INVALID_KEY
*****************************************************************************/
BSP_S32 BSP_CIPHER_SetKey(const BSP_VOID* pKeyAddr,CIPHER_KEY_LEN_E enKeyLen,BSP_U32 u32KeyIndex);

/*****************************************************************************
*Function Name: BSP_CIPHER_GetKey
*
*Function description: Obtain the address of the Key according to KeyIndex
*
*Input parameters: u32KeyIndex: Key index
*Output parameters: None
*Return Value: Success: The address of the Key obtained by KeyIndex
*: Failed: Failed to obtain the address where the Key value is located, return 0
*****************************************************************************/
BSP_S32 BSP_CIPHER_GetKey(BSP_U32 u32KeyIndex, KEY_GET_S *pstKeyGet);

/*****************************************************************************
*Function Name: CIPHER_DataBuffPara
*
*Function description: Obtain upper-level data information and set the information
*
*Input parameters: u32Chn: Channel number
*bSrc: TRUE: The attribute configuration of the source address /FALSE: The attribute configuration of the destination address
*u32BufOft: Data buffer pointer offset
*u32LenOft: Buffer length offset
*NextOft: Next node offset
*Output parameters: None
*Return Value: Success: CIPHER_SUCCESS
*: Failed:CIPHER_NOT_INIT
*: CIPHER_INVALID_CHN
*****************************************************************************/
BSP_S32 BSP_CIPHER_SetDataBufPara(BSP_U32 u32Chn, BSP_BOOL bSrc, BSP_U32 u32BufOft,
                                 BSP_U32 u32LenOft, BSP_U32 u32NextOft);

/*****************************************************************************
*Function Name: CIPHER_GetCmplSrcBuff
*
*Function description: Get the source data address that has been completed
*
*Input parameters: u32ChNum: Channel number
*Output parameters: pu32SourAddr: The source address for the operation completed;
*pu32DestAddr: The destination address for the operation to be completed
*penStatus: The state of operation completion
*pu32Private: Returned private data
*Return Value: Success: CIPHER_SUCCESS
*: Failed:CIPHER_NOT_INIT
*: CIPHER_INVALID_CHN
*: CIPHER_NULL_PTR
*: CIPHER_INVALID_RD
*****************************************************************************/
BSP_S32 BSP_CIPHER_GetCmplSrcBuff(BSP_U32 u32Chn, BSP_U32 *pu32SourAddr,
             BSP_U32 *pu32DestAddr, CIPHER_NOTIFY_STAT_E *penStatus, BSP_U32 *pu32Private);

/*****************************************************************************
*Function Name: BSP_Accelerator_GetBdFifoAddr
*
*Function description: Used to obtain the currently available BDFIFO home address
*
*Input parameters: None
*
*Output parameters: None
*Return Value: BDFIFO home address
*****************************************************************************/
BSP_U32 BSP_Accelerator_GetBdFifoAddr(void);

/*****************************************************************************
*Function Name: BSP_Accelerator_GetStatus
*
*Function Description: Get the current channel status
*
*Input parameters: None
*Output parameters: None
*Return Value: Channel is free/idle
*****************************************************************************/
BSP_S32 BSP_Accelerator_GetStatus(void);

/*****************************************************************************
*Function Name: BSP_Accelerator_Enable
*
*Function Description: Enable Packet Group Acceleration
*
*Input parameters: u32BdFifoAddr: BDFIFO home address
*
*Output parameters: None
*Return Value: Success/Failed
*****************************************************************************/
BSP_S32 BSP_Accelerator_Enable(BSP_U32 u32BdFifoAddr);

/*****************************************************************************
*Function Name: BSP_KDF_KeyRamRead()
*
*Function Description: Read data from Key Ram
*
*Input parameter: enKeyIndex: Key index, start reading data from this index
*u32Length: Read data length, in units of Byte
*Output parameters: pDestAddr: Destination address, store the data read from KeyRam to this location
*Return Value: Success: BSP_OK
*: Failed: BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_KDF_KeyRamRead(BSP_VOID *pDestAddr, BSP_U32 u32KeyIndex ,BSP_U32 u32ReadLength);

/*****************************************************************************
*Function Name: BSP_KDF_KeyRamWrite
*
*Function Description: Write data to KeyRam
*
*Input parameters: enKeyIndex: Key index, start writing data from this index
*pSourAddr: Source address, data address written to KeyRam
*u32Length: Data length
*
*Output parameters: None
*Return Value: Success: BSP_OK
*: Failed: BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_KDF_KeyRamWrite(BSP_U32 u32KeyIndex, BSP_VOID *pSourAddr, BSP_U32 u32Length);


/*****************************************************************************
*Function Name: BSP_KDF_KeyMake
*
*Function description: KDF calculation, used to generate new Keys
*
*Input parameters: stKeyCfgInfo: Parameter sha_key attribute configuration during KDF operation
            : stKeyCfgInfo: Parameter sha_s attribute configuration during KDF operation
            : enDestIndex : The position where the Key generated by the KDF operation is placed in KeyRam
*Output parameters: None
*Return Value: Success: BSP_OK
*: Failed:BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_KDF_KeyMake(KEY_CONFIG_INFO_S *pstKeyCfgInfo, S_CONFIG_INFO_S *pstSCfgInfo,
                        BSP_U32 u32DestIndex, KEY_MAKE_S *pstKeyMake);



/**************************************************************************
  Macro definition
**************************************************************************/

/* Print Level */
#define CIPHER_PRNT_NOT           BSP_LOG_LEVEL_MAX       /* No information is printed */
#define CIPHER_PRNT_ERR           BSP_LOG_LEVEL_ERROR     /* Print only error messages */
#define CIPHER_PRNT_WAR           BSP_LOG_LEVEL_WARNING   /* Print only errors, alarm messages*/
#define CIPHER_PRNT_LOG           BSP_LOG_LEVEL_DEBUG     /* Print debugging, alarms, error messages */

/* Note that the cipher error code must be negative */
#define CIPHER_ERROR_BASE               0x80000800

typedef enum tagCIPHER_ERR_CODE_E
{
    CIPHER_ERR_CODE_NULL_PTR = 1,
    CIPHER_ERR_CODE_NO_MEM ,
    CIPHER_ERR_CODE_NOT_INIT ,
    CIPHER_ERR_CODE_FIFO_FULL ,
    CIPHER_ERR_CODE_INVALID_CHN ,
    CIPHER_ERR_CODE_INVALID_OPT ,
    CIPHER_ERR_CODE_ALIGN_ERROR ,
    CIPHER_ERR_CODE_PURGING ,
    CIPHER_ERR_CODE_TIME_OUT,
    CIPHER_ERR_CODE_INVALID_ENUM,
    CIPHER_ERR_CODE_INVALID_RD,
    CIPHER_ERR_CODE_RD_NULL,
    CIPHER_ERR_CODE_INVALID_KEY,
    CIPHER_ERR_CODE_CHECK_ERROR,
    CIPHER_ERR_CODE_BDLEN_ERROR,
    CIPHER_ERR_CODE_INVALID_NUM,
    CIPHER_ERR_CODE_NO_KEY,
    CIPHER_ERR_CODE_KEYLEN_ERROR
}CIPHER_ERR_CODE_E;

#define CIPHER_NULL_PTR      ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NULL_PTR))
#define CIPHER_NO_MEM        ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NO_MEM))
#define CIPHER_NOT_INIT      ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NOT_INIT))
#define CIPHER_FIFO_FULL     ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_FIFO_FULL))
#define CIPHER_INVALID_CHN   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_CHN))
#define CIPHER_INVALID_OPT   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_OPT))
#define CIPHER_ALIGN_ERROR   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_ALIGN_ERROR))
#define CIPHER_PURGING       ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_PURGING))
#define CIPHER_TIME_OUT      ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_TIME_OUT))
#define CIPHER_INVALID_ENUM  ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_ENUM))
#define CIPHER_INVALID_RD    ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_RD))
#define CIPHER_RDQ_NULL      ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_RD_NULL))
#define CIPHER_INVALID_KEY   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_KEY))
#define CIPHER_CHECK_ERROR   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_CHECK_ERROR))
#define CIPHER_BDLEN_ERROR   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_BDLEN_ERROR))
#define CIPHER_INVALID_NUM   ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_NUM))
#define CIPHER_NO_KEY        ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NO_KEY))
#define CIPHER_KEYLEN_ERROR  ((BSP_S32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_KEYLEN_ERROR))
#define CIPHER_UNKNOWN_ERROR ((BSP_S32)(CIPHER_ERROR_BASE|0xff))



/*************************CIPHER END **********************************/

/*************************WIFI START**********************************/

/*****************************************************************************
 Function Name: WifiCalDataLen
 Function description: WIFI calculation requires data block memory
 Input parameters: usLen -User application data length Len
 Output parameters: None
 Return Value: Total length of the applied data area
 Calling the function:
 Called function:

 Modify history:
  1. Date: December 22, 2011
    Modify content: New generated function

*****************************************************************************/
extern unsigned short BSP_WifiCalDataLen(unsigned short usLen);
#define DRV_WifiCalDataLen(usLen) BSP_WifiCalDataLen(usLen)

/*****************************************************************************
 Function Name: WIFI_TEST_CMD
 Function Description: Test Command
 Input parameters: cmdStr; command string
 Output parameters: None
 Return value: None
*****************************************************************************/
extern void WIFI_TEST_CMD(char * cmdStr);

/*****************************************************************************
 Function Name: WIFI_GET_TCMD_MODE
 Function Description: Get the mode of the test command
 Input parameters: None
 Output parameters: 16: Calibration test mode
             17: Indicates that it is in transmit mode
             18: Indicates the reception mode
 Return value: The state result value of the function execution
*****************************************************************************/
extern int WIFI_GET_TCMD_MODE(void);

/*****************************************************************************
 Function Name: WIFI_POWER_START
 Function description: Power on WIFI
 Input parameters: None
 Output parameters: None
 Return value: 0: execute ok
              1: executed failed
*****************************************************************************/
extern int WIFI_POWER_START(void);

/*****************************************************************************
 Function Name: WIFI_POWER_SHUTDOWN
 Function description: Power off by WIFI
 Input parameters: None
 Output parameters: None
 Return value: 0: execute ok
              1: executed failed
*****************************************************************************/
extern int WIFI_POWER_SHUTDOWN(void);

/*****************************************************************************
 Function Name: WIFI_GET_STATUS
 Function description: WIFI status acquisition
 Input parameters: None
 Output parameters: None
 Return value: 0: wifi is off
              1: wifi is in normal mode
              2: wifi is in tcmd mode
*****************************************************************************/
extern int WIFI_GET_STATUS(void);

/*****************************************************************************
 Function Name: WIFI_GET_RX_DETAIL_REPORT
 Function description: get result of rx report: totalPkt, GoodPkt, ErrorPkt
 Input parameters: None
 Output parameters: totalPkt, goodPkt, badPkt
 Return value: None
*****************************************************************************/
extern void WIFI_GET_RX_DETAIL_REPORT(int* totalPkt,int* goodPkt,int* badPkt);

/*****************************************************************************
 Function Name: WIFI_GET_RX_PACKET_REPORT
 Function description: get result of rx ucast&mcast packets
 Input parameters: None
 Output parameters: ucastPkts, mcastPkts
 Return value: None
*****************************************************************************/
extern void  WIFI_GET_RX_PACKET_REPORT(unsigned int *ucastPkts, unsigned int *mcastPkts);

/*****************************************************************************
 Function Name: WIFI_GET_PA_CUR_MODE
 Function description: get the current PA mode of the wifi chip
 Input parameters: None
 Output parameters: None
 Return value: 0: (WIFI_ONLY_PA_MODE) WIFI chip is in PA mode
             1: (WIFI_ONLY_NOPA_MODE) WIFI chip is in no PA mode
             -1: wifi chip is in absolute mode
*****************************************************************************/
extern int WIFI_GET_PA_CUR_MODE(void);

/*****************************************************************************
 Function Name: WIFI_GET_PA_MODE
 Function description: get the support PA mode of wifi chip
 Input parameters: None
 Output parameters: None
 Return value: 0: (WIFI_ONLY_PA_MODE) WIFI support only PA mode
             1: (WIFI_ONLY_NOPA_MODE) WIFI support only no PA mode
             2: (WIFI_PA_NOPA_MODE) WIFI support both PA & no PA mode
             -1: failed
*****************************************************************************/
extern int WIFI_GET_PA_MODE(void);

/*****************************************************************************
 Function Name: WIFI_SET_PA_MODE
 Function description: set the PA mode of wifi chip
 Input parameters: 0: (WIFI_ONLY_PA_MODE) WIFI support only PA mode
             1: (WIFI_ONLY_NOPA_MODE) WIFI support only no PA mode
             2: (WIFI_PA_NOPA_MODE) WIFI support both PA & no PA mode
 Output parameters: None
 Return value: 0: success
             -1: failed
*****************************************************************************/
extern int WIFI_SET_PA_MODE(int wifiPaMode);

/*****************************************************************************
 Function Name: DRV_WIFI_DATA_RESERVED_TAIL
 Function description: WIFI calculation requires data block memory
 Input parameters: usLen -User application data length Len
 Output parameters: None
 Return Value: The length reserved at the end of the data area
*****************************************************************************/
extern unsigned int DRV_WIFI_DATA_RESERVED_TAIL(unsigned int len);

/*h00106354 20120201 Incoming flow control interface add start */
/*****************************************************************************
 Function Name: DRV_WIFI_SET_RX_FCTL
 Function description: Set the WIFI receiving flow control identifier
 Input parameters: para1, para2
 Output parameters: None
 Return Value: BSP_OK/BSP_ERROR
*****************************************************************************/
extern unsigned long DRV_WIFI_SET_RX_FCTL(unsigned long para1, unsigned long para2);

/*****************************************************************************
 Function Name: DRV_WIFI_CLR_RX_FCTL
 Function description: Clear the WIFI receiving flow control identifier
 Input parameters: para1, para2
 Output parameters: None
 Return Value: 1: Yes
             0: None
*****************************************************************************/
extern unsigned long DRV_WIFI_CLR_RX_FCTL(unsigned long para1, unsigned long para2);

/*****************************************************************************
 Function Name: DRV_AT_GET_USER_EXIST_FLAG
 Function description: Returns whether there is currently a USB connection or a WIFI user connection (C core call)
 Input parameters: None
 Output parameters: None
 Return Value: 1: Yes
             0: None
*****************************************************************************/
extern unsigned long  DRV_AT_GET_USER_EXIST_FLAG(void);


unsigned long USB_ETH_DrvSetRxFlowCtrl   (unsigned long ulParam1, unsigned long ulParam2);
unsigned long USB_ETH_DrvClearRxFlowCtrl (unsigned long ulParam1, unsigned long ulParam2);
unsigned long USB_ETH_DrvSetHostAssembleParam(unsigned long ulHostOUtTimeout);
unsigned long USB_ETH_DrvSetDeviceAssembleParam(unsigned long ulEthTxMinNum,
											    unsigned long ulEthTxTimeout,
											    unsigned long ulEthRxMinNum,
											    unsigned long ulEthRxTimeout);
/*h00106354 20120201 Incoming flow control interface add end */

/*************************WIFI END************************************/

/*************************AUDIO START*********************************/

/*AUDIO IOCTL interface command word*/
#define AUDIO_IOCTL_BASE                    (0x30000000)
#define IOCTL_AUDIO_SAMPLE_RATE_SET         (AUDIO_IOCTL_BASE+1)
#define IOCTL_AUDIO_PCM_MODE_SET            (AUDIO_IOCTL_BASE+7)
#define IOCTL_AUDIO_IN_DEV_SELECT           (AUDIO_IOCTL_BASE+11)
#define IOCTL_AUDIO_OUT_DEV_SELECT          (AUDIO_IOCTL_BASE+12)
#define IOCTL_AUDIO_OUT_DEV_UNSELECT        (AUDIO_IOCTL_BASE+13)
#define IOCTL_AUDIO_IN_DEV_UNSELECT         (AUDIO_IOCTL_BASE+14)
#define IOCTL_AUDIO_VOICE_OPEN              (AUDIO_IOCTL_BASE+23)
#define IOCTL_AUDIO_VOICE_CLOSE             (AUDIO_IOCTL_BASE+24)

typedef enum
{
    NODEV=0x00,
    HEADSET=0x01,
    HANDSET=0x02,
    HANDFREE=0x04
}AUDIO_DEV_TYPE;
typedef enum
{
    SAMPLE_RATE_8K=0,
    SAMPLE_RATE_11K025=1,
    SAMPLE_RATE_12K=2,
    SAMPLE_RATE_16K=3,
    SAMPLE_RATE_22K05=4,
    SAMPLE_RATE_24K=5,
    SAMPLE_RATE_32K=6,
    SAMPLE_RATE_44K1=7,
    SAMPLE_RATE_48K=8,
    SAMPLE_RATE_INVALID=0xFFFF
}AUDIO_SAMPLE_RATE;


/********************************************************************************************************
 Function Name: BSP_AUDIO_CodecOpen
 Function Description: Open an audio device and initialize codec.
 Input parameter: devname: This parameter is fixed to "/dev/codec0".
                           flags: This parameter indicates read and write mode, fixed to 0.
                           mode: This parameter indicates the new file creation method, fixed to 0. .
  Output parameters: None.
 Return Value: Non-negative Value: Device Handle;
             -1: The operation failed.
********************************************************************************************************/
extern int BSP_AUDIO_CodecOpen(const char *devname, int flags, int mode);
#define DRV_CODEC_OPEN(devname, flags, mode)    BSP_AUDIO_CodecOpen(devname, flags, mode)

/********************************************************************************************************
 Function Name: BSP_AUDIO_Codec_ioctl
 Function description: codec device command word control.
 Enter parameters: devid: device handle.
                           cmd: command word.
                           arg: Command parameter.
  Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int BSP_AUDIO_Codec_ioctl(int devid, int cmd, int arg);
#define DRV_CODEC_IOCTL(devid, cmd, arg)   BSP_AUDIO_Codec_ioctl(devid, cmd, arg)

/*************************AUDIO END***********************************/

/*************************USB START***********************************/
/* The maximum number of ports enumerated */
#define DYNAMIC_PID_MAX_PORT_NUM        17

typedef  enum
{
    FLOW_CON,     /* Flow control        */
    PDP_STATUS,   /* Pdp activation results */
    SIGNAL_LEVEL, /* Signal strength    */
    PKT_STATICS,  /* Traffic statistics    */
    AT_CMD,       /* At command      */
    PS_NOTIFY,    /* Notify command  */
    RESERVE       /* reserve        */
}NDIS_NOTICE;

typedef unsigned long (*pPCSCFUNC)(unsigned long CmdType, unsigned char *pApdu, unsigned long ApduLen);

/* Nv item 50091 structure, representing the port form to be enumerated by the device */
typedef struct
{
    unsigned long ulStatus;
    unsigned char aucFirstPortStyle[DYNAMIC_PID_MAX_PORT_NUM];  /* Device switch front port pattern */
    unsigned char aucRewindPortStyle[DYNAMIC_PID_MAX_PORT_NUM]; /* After switching device port pattern */
    unsigned char reserved[22];
}DRV_DYNAMIC_PID_TYPE_STRU;

typedef enum
{
    WWAN_WCDMA    = 1,/*Wcdma mode*/
    WWAN_CDMA    = 2  /*Cdma mode*/
}NDIS_WWAN_MODE;


/*Definition of pin signals*/
#define IO_CTRL_FC                      (0x02)
#define IO_CTRL_DSR                     (0x20)
#define IO_CTRL_DTR                     (0x01)
#define IO_CTRL_RFR                     (0x04)
#define IO_CTRL_CTS                     (0x10)
#define IO_CTRL_RI                      (0x40)
#define IO_CTRL_DCD                     (0x80)

typedef int  (*pComRecv)(unsigned char  uPortNo, unsigned char* pData, unsigned short uslength);

typedef int  (*pUartRecv)(unsigned char uPortNo, unsigned char* pData, unsigned short uslength);

typedef long (*pUsbStatus)(unsigned char ucStatus);

typedef long  (*pUsbState)(unsigned char ucStatus);

typedef int  (*pComStatusNotify)(unsigned char uStatus);

/* MODEM pipe angle signal structure definition*/
typedef struct tagMODEM_MSC_STRU
{
    BSP_U32 OP_Dtr     :    1;          /*DTR CHANGE FLAG*/
    BSP_U32 OP_Dsr     :    1;          /*DSR CHANGE FLAG*/
    BSP_U32 OP_Cts     :    1;          /*CTSCHANGE FLAG*/
    BSP_U32 OP_Rts     :    1;          /*RTS CHANGE FLAG*/
    BSP_U32 OP_Ri      :    1;          /*RI CHANGE FLAG*/
    BSP_U32 OP_Dcd     :    1;          /*DCD CHANGE FLAG*/
    BSP_U32 OP_Fc      :    1;          /*FC CHANGE FLAG*/
    BSP_U32 OP_Brk     :    1;          /*BRK CHANGE FLAG*/
    BSP_U32 OP_Spare   :    24;         /*Reserve*/
    BSP_U8   ucDtr;                     /*DTR  VALUE*/
    BSP_U8   ucDsr;                     /*DSR  VALUE*/
    BSP_U8   ucCts;                     /*DTS VALUE*/
    BSP_U8   ucRts;                     /*RTS  VALUE*/
    BSP_U8   ucRi;                      /*RI VALUE*/
    BSP_U8   ucDcd;                     /*DCD  VALUE*/
    BSP_U8   ucFc;                      /*FC  VALUE*/
    BSP_U8   ucBrk;                     /*BRK  VALUE*/
    BSP_U8   ucBrkLen;                  /*BRKLEN VALUE*/
} MODEM_MSC_STRU, *PMODEM_MSC_STRU,AT_DCE_MSC_STRU;

/*Protocol stack bulk data reception callback function type definition*/
typedef int (*USB_MODEM_RECV_CALL)
(
    unsigned char ucPortType,          /*Port number*/
    unsigned char ucDlci,                 /*Link number*/
    unsigned char *pData,               /*Data pointer*/
    unsigned short usLen                /*Data length*/
);


/*Definition of callback function for protocol stack message channel state change data reception*/
typedef int (*USB_MODEM_AT_Sig)
(
    unsigned char ucPortType,          /*Port number*/
    unsigned char ucDlci,                   /*Link number*/
    MODEM_MSC_STRU* data  /*Pointer to the MODEM tube angle signal structure*/
);


/*Protocol stack chain breakage and chain building callback function*/
typedef int (*USB_MODEM_IN_OUT)
(
    unsigned char ucPortType,           /*Port number*/
    unsigned char ucDlci                   /*Link number*/
);

/*Definition of high-performance copy function of protocol stack*/
typedef void (*USB_MODEM_COPY)
(
    unsigned char   *pDest,
    unsigned char   *pSrc,
    unsigned long   ulLen
);


typedef struct
{
    unsigned int    GateWayIpAddr;  /* Pdp activation to obtain remote gateway IP address      */
    unsigned int    LocalIpAddr;    /* Pdp activation to obtain IP address assigned to modem */
    unsigned int    MaskAddr;       /* Subnet mask */
    unsigned int    speed;          /* Connection speed, unit bps */
    unsigned char   ActiveSatus;    /* Activation result, 0 is successful, others are failure */
}NDIS_PDP_STRU,*pNDIS_PDP_STRU;


typedef struct
{
    unsigned int    currentTx;    /* Current tx */
    unsigned int    currentRx;    /* Current rx */
    unsigned int    maxTx;        /* Max tx */
    unsigned int    maxRx;        /* Max rx */
}NDIS_RATE_STRU,*pNDIS_RATE_STRU;


typedef struct
{
    unsigned int     TxOKCount;         /*Number of packets sent*/
    unsigned int     RxOKCount;         /*Number of packets received*/
    unsigned int     TxErrCount;        /*Send error*/
    unsigned int     RxErrCount;        /*Receive error*/
    unsigned int     TxOverFlowCount;   /*Send overflow and lost packets*/
    unsigned int     RxOverFlowCount;   /*Receive overflow and lost packets*/
    unsigned int     MaxTx;         /*Send rate*/
    unsigned int     MaxRx;         /*Receive rate*/
} NDIS_PKT_STATISTICS, *pNDIS_PKT_STATISTICS;

typedef struct
{
    unsigned char   *pucAtCmd;          /*Pointer to the at command*/
    unsigned short   usLen;             /*The length of the At command*/
}NDIS_AT_CMD_STRU, *pNDIS_AT_CMD_STRU;

/* The structure used by the signal reporting function*/
typedef struct
{
    NDIS_NOTICE                 type;
    union
    {
        NDIS_PDP_STRU           pdp;
        NDIS_PKT_STATISTICS     pktStatics;
        unsigned char           flowFlag;   /*  1: Flow control, 0: De-flow control */
        int                     dbm;
        NDIS_AT_CMD_STRU        atCmd;
    }NDIS_STATUS_DATA;

} NDIS_PRO_STRU, *pNDIS_PRO_STRU;

typedef int (*USB_NDIS_RECV_CALL)
(
    unsigned char *pData, /* Receive buffer pointer, content is IP message */
    unsigned short usLen  /* Data length */
);

typedef int (*USB_NDIS_BRK)
(
    void
);

typedef int (*USB_NDIS_EST)
(
    unsigned char *pData /* Receive buffer pointer, the content is the connection configuration management parameters */
);

typedef int (*USB_NDIS_PKT_STATISTICS_PERIOD)
(
    unsigned int period /*Units */
);

typedef int (*USB_NDIS_GET_RATE)
(
    NDIS_RATE_STRU  *pRate
);

typedef int (*USB_NDIS_AT_CMD_RECV)
(
    unsigned char *pBuff,   /*Pointer to the at command*/
    unsigned short usLen    /*At command length*/
);

typedef unsigned int (*USB_NDIS_GET_WWAN_MODE)  /*Get the current network mode cdma/wcdma*/
(
    void
);

typedef unsigned long (*USB_NET_DEV_SET_MAX_TX_PACKET_NUMBER)
(
    unsigned long ulNumber
);

typedef void (*IPC_RECV_HANDLER)(unsigned char ucCidNo, unsigned char *pstData, unsigned int ulLength );

typedef int (*IPC_COM_RECV_HANDLER)(unsigned char uPortNo, unsigned char* pData, unsigned short uslength);

typedef unsigned long (*MNTN_ERRLOGREGFUN)(char * cFileName,unsigned int ulFileId, unsigned int ulLine,
                unsigned int ulErrNo, void * pBuf, unsigned int ulLen);

/*****************************************************************************
 Function Name: BSP_USB_SetPid
 Function description:
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern int BSP_USB_SetPid(unsigned char u2diagValue);
#define DRV_SET_PID(u2diagValue)    BSP_USB_SetPid(u2diagValue)

/*****************************************************************************
Function name: BSP_USB_PortTypeQuery
Function description: Query the port form value of the current device enumeration
Input parameters: stDynamicPidType Port pattern
Output parameters: stDynamicPidType Port pattern
Return value: 0: Query successful
           Others: Query failed
*****************************************************************************/
unsigned int BSP_USB_PortTypeQuery(DRV_DYNAMIC_PID_TYPE_STRU *pstDynamicPidType);
#define DRV_SET_PORT_QUIRY(a)   BSP_USB_PortTypeQuery(a)

/*****************************************************************************
Function name: BSP_USB_PortTypeValidCheck
Function description: Provided to the upper layer query device port form configuration legality interface
           1. The port is supported, 2. It contains PCUI port, 3. There are no duplicate ports, 4. The number of endpoints does not exceed 16.
           5. The first device is not a MASS class
Input parameters: pucPortType port form configuration
           ulPortNum Number of port patterns
Return value: 0: Port form is legal
           Others: Illegal port form
*****************************************************************************/
unsigned int BSP_USB_PortTypeValidCheck(unsigned char *pucPortType, unsigned long ulPortNum);
#define DRV_USB_PORT_TYPE_VALID_CHECK(pucPortType, ulPortNum)  \
                    BSP_USB_PortTypeValidCheck(pucPortType, ulPortNum)

/*****************************************************************************
Function name: BSP_USB_GetAvailabePortType
Function description: Provided to the upper layer to query the current device to support port form list interface
Input parameters: ulPortMax protocol stack supports the maximum number of port patterns
Output parameters: pucPortType Supported port patterns list
           pulPortNum supports the number of port patterns
Return value: 0: Acquisition of port form list successfully
           Others: Failed to get the port pattern list
*****************************************************************************/
unsigned int BSP_USB_GetAvailabePortType(unsigned char *pucPortType,
                            unsigned long *pulPortNum, unsigned long ulPortMax);
#define DRV_USB_GET_AVAILABLE_PORT_TYPE(pucPortType, pulPortNum, ulPortMax)  \
                BSP_USB_GetAvailabePortType(pucPortType, pulPortNum, ulPortMax)

/*****************************************************************************
 Function Name: BSP_USB_ATProcessRewind2Cmd
 Function description: rewind2 CMD processing.
 Input parameters: pData: Data.
 Output parameters: None
 Return Value: false(0): Processing failed
             tool(1): Processing successfully
*****************************************************************************/
extern int BSP_USB_ATProcessRewind2Cmd(unsigned char *pData);
#define DRV_PROCESS_REWIND2_CMD(pData)    BSP_USB_ATProcessRewind2Cmd(pData)

/*****************************************************************************
 Function Name: BSP_USB_GetDiagModeValue
 Function Description: Obtain the device type.
 Input parameters: None.
 Output parameters: ucDialmode: 0 -Dialing with Modem; 1 -Dialing with NDIS; 2 -Modem and NDIS coexist
              ucCdcSpec: 0 -Modem/NDIS complies with CDC specifications; 1 -Modem complies with CDC specifications;
                           2 -NDIS complies with CDC specifications; 3 -Modem/NDIS complies with CDC specifications
 Return Value: VOS_OK/VOS_ERR
*****************************************************************************/
extern BSP_S32 BSP_USB_GetDiagModeValue(unsigned char *pucDialmode,
                             unsigned char *pucCdcSpec);
#define DRV_GET_DIAG_MODE_VALUE(pucDialmode, pucCdcSpec)    \
                             BSP_USB_GetDiagModeValue(pucDialmode, pucCdcSpec)

/*****************************************************************************
 Function Name: BSP_USB_GetPortMode
 Function description: Obtain port pattern mode, gateway docking requirements, and pile driving.
 Enter parameters: .
 Output parameters:
 Return Value:
*****************************************************************************/
extern unsigned char BSP_USB_GetPortMode(char*PsBuffer, unsigned long*Length );
#define DRV_GET_PORT_MODE(PsBuffer, Length)    BSP_USB_GetPortMode(PsBuffer,Length)

/*****************************************************************************
 Function Name: BSP_USB_GetU2diagDefaultValue
 Function description: Obtain the default u2diag value of the port
 Input parameters: None.
 Output parameters: None.
 Return value: u2diag value

*****************************************************************************/
extern BSP_U32 BSP_USB_GetU2diagDefaultValue(void);
#define DRV_GET_U2DIAG_DEFVALUE()    BSP_USB_GetU2diagDefaultValue()

/*****************************************************************************
 Function Name: ErrlogRegFunc
 Function description: USB MNTN registration exception log interface, pile driving in Porting project
 Input parameters: None.
 Output parameters: None.
 Return value: None

*****************************************************************************/
//extern void ErrlogRegFunc(MNTN_ERRLOGREGFUN pRegFunc);

extern void MNTN_ERRLOG_REG_FUNC(MNTN_ERRLOGREGFUN pRegFunc);

/*****************************************************************************
 Function Name: getSystemResetInfo
 Function Description: Obtain system reset information
 Input parameters: None

 Output parameters: None.
 Return Value: None
 Notes:
*****************************************************************************/
extern void * GET_SYSTEM_RESET_INFO(void);

/*****************************************************************************
*Function Name: DRV_DSP_AHB_RESET
*Function Description: ZSP AHB bus reset
*Input parameters: None
*Output parameters: None
*Return Value: None
*Other Instructions: None
*****************************************************************************/
extern void DRV_DSP_AHB_RESET(void);

/*****************************************************************************
*Function Name: DRV_DSP_AHB_RESET_CANCEL
*Function Description: ZSP AHB bus dereset
*Input parameters: None
*Output parameters: None
*Return Value: None
*Other Instructions: None
*****************************************************************************/
extern void DRV_DSP_AHB_RESET_CANCEL(void);


/*****************************************************************************
 Function Name: BSP_USB_UdiagValueCheck
 Function description: This interface is used to check the legality of USB morphological values ??in NV items.
 Input parameters: None.
 Output parameters: None.
 Return Value: 0: OK;
            -1: ERROR
*****************************************************************************/
extern int BSP_USB_UdiagValueCheck(unsigned long DiagValue);
#define DRV_UDIAG_VALUE_CHECK(DiagValue)     BSP_USB_UdiagValueCheck(DiagValue)
#define DRV_U2DIAG_VALUE_CHECK(DiagValue)     BSP_USB_UdiagValueCheck(DiagValue)

/*****************************************************************************
 Function Name: BSP_USB_GetLinuxSysType
 Function description: This interface is used to check whether the PC side is Linux to avoid the problem of failure of secondary dialing in the Linux background.
 Input parameters: None.
 Output parameters: None.
 Return value: 0: Linux;
            -1: Non-Linux.
*****************************************************************************/
extern int BSP_USB_GetLinuxSysType(void) ;
#define DRV_GET_LINUXSYSTYPE()    BSP_USB_GetLinuxSysType()

/********************************************************
Function description: Returns the device form value that supports (sel=1) or does not support (sel=0) PCSC in the current device list
Function function:
Input parameters: sel
          0: Return the current device form value without PCSC through parameter dev_type
          1: Return the current value with PCSC as the device form through the parameter dev_type
Output parameters: dev_type Writes the required device form value, and if not, the value will not be written.
          Device form values ??stored in NV
Output parameters: pulDevType The device form value corresponding to ulCurDevType, if no return value is 1.
Return value:
          0: Query the corresponding device form value;
          1: No response device form value was found.
********************************************************/
int BSP_USB_PcscInfoSet(unsigned int  ulSel, unsigned int  ulCurDevType, unsigned int *pulDevType);
#define DRV_USB_PCSC_INFO_SET(ulSel, ulCurDevType, pulDevType)    BSP_USB_PcscInfoSet(ulSel, ulCurDevType, pulDevType)

/*The message identification that can be used to capture the Tcp/ip protocol stack*/
enum IPS_MNTN_TRACE_IP_MSG_TYPE_ENUM
{
    /* IP packets can be maintained and measured and reported */
    ID_IPS_TRACE_IP_ADS_UL                  = 0xD030,
    ID_IPS_TRACE_IP_ADS_DL                  = 0xD031,
    ID_IPS_TRACE_IP_USB_UL                  = 0xD032,
    ID_IPS_TRACE_IP_USB_DL                  = 0xD033,

    ID_IPS_TRACE_IP_MSG_TYPE_BUTT
};

/********************************************************
Function description: Protocol stack registration USB reports SKB key information to SDT callback function
Function function:
Input parameters: pFunc: SKB key information report to SDT callback function pointer
Output parameters: None
Output parameters: None
Return value: 0: Success
          1: Failure
********************************************************/
typedef void (*USB_IPS_MNTN_TRACE_CB_T)(struct sk_buff *skb,unsigned short usType);
extern unsigned int BSP_USB_RegIpsTraceCB(USB_IPS_MNTN_TRACE_CB_T pFunc);
#define DRV_USB_REG_IPS_TRACECB(x) BSP_USB_RegIpsTraceCB(x)

/********************************************************
Function description: Protocol stack registration USB enable notification callback function input
Function function:
Input parameters: pFunc: USB enable callback function input pointer
Output parameters: None
Output parameters: None
Return value: 0: Success
          1: Failure
********************************************************/
typedef void (*USB_UDI_ENABLE_CB_T)(void);
extern unsigned int BSP_USB_RegUdiEnableCB(USB_UDI_ENABLE_CB_T pFunc);
#define DRV_USB_REGUDI_ENABLECB(x) BSP_USB_RegUdiEnableCB(x)

/********************************************************
Function description: Register USB to enable notification callback function input
Function function:
Input parameters: pFunc: USB enable callback function input pointer
Output parameters: None
Output parameters: None
Return value: 0: Success
          1: Failure
********************************************************/
typedef void (*USB_UDI_DISABLE_CB_T)(void);
extern unsigned int BSP_USB_RegUdiDisableCB(USB_UDI_DISABLE_CB_T pFunc);
#define DRV_USB_REGUDI_DISABLECB(x) BSP_USB_RegUdiDisableCB(x)

/********************************************************
Function description: Protocol stack registration HSIC enable notification callback function input
Function function:
Input parameters: pFunc: HSIC enable callback function input pointer
Output parameters: None
Output parameters: None
Return value: 0: Success
          1: Failure
********************************************************/
typedef void (*HSIC_UDI_ENABLE_CB_T)(void);
extern unsigned int BSP_HSIC_RegUdiEnableCB(HSIC_UDI_ENABLE_CB_T pFunc);
#define DRV_HSIC_REGUDI_ENABLECB(x) BSP_HSIC_RegUdiEnableCB(x)

/********************************************************
Function description: Protocol stack registration HSIC to enable notification callback function input
Function function:
Input parameters: pFunc: HSIC enable callback function input pointer
Output parameters: None
Output parameters: None
Return value: 0: Success
          1: Failure
********************************************************/
typedef void (*HSIC_UDI_DISABLE_CB_T)(void);
extern unsigned int BSP_HSIC_RegUdiDisableCB(HSIC_UDI_DISABLE_CB_T pFunc);
#define DRV_HSIC_REGUDI_DISABLECB(x) BSP_HSIC_RegUdiDisableCB(x)
/********************************************************
Function description: Protocol stack query HSIC enumeration status
Function function:
Input parameters: None
Output parameters: None
Output parameters: None
Return value: 1: Enumeration is completed
          0: Enumeration not completed
********************************************************/
extern unsigned int BSP_GetHsicEnumStatus(void);
#define DRV_GET_HSIC_ENUM_STATUS() BSP_GetHsicEnumStatus()

/********************************************************
Function description: Protocol stack query whether HSIC supports NCM
Function function:
Input parameters: None
Output parameters: None
Output parameters: None
Return value: 1: Support NCM
          0: NCM is not supported
********************************************************/
extern int BSP_USB_HSIC_SupportNcm(void);
#define DRV_USB_HSIC_SUPPORT_NCM() BSP_USB_HSIC_SupportNcm()

/********************************************************
Function description: TTF query pre-application SKB Num
Function function:
Input parameters: None
Output parameters: None
Output parameters: None
Return value: SKB Num
********************************************************/
extern BSP_U32 BSP_AcmPremallocSkbNum(void);
#define DRV_GET_PREMALLOC_SKB_NUM() BSP_AcmPremallocSkbNum()

/********************************************************
Function name: BSP_UDI_FindVcom
Function description: Query whether the specified UDI virtual serial port exists in the current device form
Input parameters: UDI_DEVICE_ID enumeration value, that is, the virtual serial port ID to be queried
Output parameters: None
Return value:
           0: The current device form does not support querying virtual serial ports;
           1: The current device form supports querying virtual serial port.
Notes: None
********************************************************/
extern int BSP_UDI_FindVcom(UDI_DEVICE_ID enVCOM);

/*****************************************************************************
*Function Name: DRV_USB_RegEnumDoneForMsp
*Function Description: Provided to MSP to register the notification function after USB enumeration is completed
*Input parameters: pFunc: Enumeration completion callback function pointer
*Output parameters: None
*Return Value: 0: Successfully registered, waiting for the enumeration completion notification;
*1: The USB has been enumerated and the USB part can be initialized directly;
*-1: Failed, non-USB form, no USB driver
*****************************************************************************/
extern signed int BSP_USB_RegEnumDoneForMsp(void *pFunc);
#define DRV_USB_RegEnumDoneForMsp   BSP_USB_RegEnumDoneForMsp

/*****************************************************************************
*Function Name: BSP_USB_RegEnumDoneForPs
*Function description: Provided to PS to register the notification function after USB enumeration is completed
*Input parameters: pFunc: Enumeration completion callback function pointer
*Output parameters: None
*Return Value: 0: Successfully registered, waiting for the enumeration completion notification;
*1: The USB has been enumerated and the USB part can be initialized directly;
*-1: Failed, non-USB form, no USB driver
*****************************************************************************/
extern signed int BSP_USB_RegEnumDoneForPs(void *pFunc);
#define DRV_USB_RegEnumDoneForPs    BSP_USB_RegEnumDoneForPs

/********************************************************
Function description: Returns the device form value in the current NV item, whether PCSC is supported
Function function:
Input parameters: dev_type is the device morphology value (OAM reads from NV)

Output parameters: None.
Return value:
          0: dev_type does not support PCSC devices;
          1: dev_type supports PCSC devices
********************************************************/
/*unsigned int pcsc_info_quiry(unsigned int ulDevType);*/
#define DRV_USB_PCSC_INFO_QUIRY(a)            DRV_OK

typedef unsigned long (*pFunAPDUProcess)(unsigned long CmdType, unsigned char *pApdu, unsigned long ApduLen);
typedef unsigned long (*GetCardStatus)(void);

typedef struct
{
    pFunAPDUProcess   pFuncApdu;
    GetCardStatus  pFuncGetCardStatus;
}BSP_CCID_REGFUNC;

/*****************************************************************************
 Function name: pcsc_usim_int
 Function Description: Register PCSC command receiving function
 Input parameters:
 Output parameters: None
 Return Value: void
*****************************************************************************/
extern void pcsc_usim_int(pFunAPDUProcess pFun1, GetCardStatus pFun2);
#define DRV_PCSC_REG_CALLBACK(pFun1, pFun2) pcsc_usim_int((pFun1), (pFun2))


/*****************************************************************************
 Function name: pcsc_usim_ctrl_cmd
 Function Description: PCSC command reply function
 Input parameters:
 Output parameters: None
 Return Value: void
*****************************************************************************/
extern unsigned long pcsc_usim_ctrl_cmd(unsigned long CmdTpye, unsigned long Result,
                                        unsigned char *Buf, unsigned long Length);
#define DRV_PCSC_SEND_DATA(CmdType, Result, Buffer, Length) pcsc_usim_ctrl_cmd(CmdType, Result, Buffer, Length)

/* Porting is temporarily defined as empty */
#define DRV_COM_RCV_CALLBACK_REGI(uPortNo,pCallback)    DRV_OK


/*****************************************************************************
 Function name: pcsc_usim_ctrl_cmd
 Function Description: PCSC command reply function
 Input parameters:
 Output parameters: None
 Return Value: void
*****************************************************************************/


/*Lightning card has not been developed yet. Temporary API Piling begin*/
/*****************************************************************************
 Function Name: BSP_USB_RndisAppEventDispatch
 Function Description: This interface is used to notify the APP of the corresponding USB plug-in and unplugging events
 Output parameters: usb event.
 Return Value: None
*****************************************************************************/
extern void BSP_USB_RndisAppEventDispatch(unsigned ulStatus) ;
#define DRV_RNDIS_APP_ENENT_DISPATCH(ulStatus)    BSP_USB_RndisAppEventDispatch(ulStatus)

/************************************************************************
 *FUNCTION
 *rndis_app_event_dispatch
 *DESCRIPTION
 *Lightning card version control application for dialing or disconnecting dialing
 *INPUTS
 *Make dialing or disconnect dialing instructions
 *OUTPUTS
 *NONE
 *************************************************************************/
extern VOID rndis_app_event_dispatch(unsigned int ulStatus);
extern VOID DRV_AT_SETAPPDAILMODE(unsigned int ulStatus);


/************************************************************************
 *FUNCTION
 *PDP_ACT_DRV_CALLBACK
 *DESCRIPTION
 *After PDP is activated, the soft callback function is called. It was originally used as the loading symbol table information on the lightning card, but now it is driven
 *INPUTS
 *NONE
 *OUTPUTS
 *NONE
 *************************************************************************/
extern int PDP_ACT_DRV_CALLBACK(void);

typedef void(* USB_NET_DEV_SWITCH_GATEWAY)
(
    void
);

/*****************************************************************************
 Function Name: BSP_USB_NASSwitchGatewayRegExtFunc
 Function description: This interface is used for NAS registration and switching gateway notification callback function
 Input parameters: callback interface.
 Output parameters: None.
 Return Value: 0: Success
                       Nonzero: Failure
*****************************************************************************/
extern int BSP_USB_NASSwitchGatewayRegFunc(USB_NET_DEV_SWITCH_GATEWAY switchGwMode) ;
#define DRV_USB_NAS_SWITCH_GATEWAY_REGFUNC(switchGwMode)    BSP_USB_NASSwitchGatewayRegFunc(switchGwMode)
/*Lightning card has not been developed yet. Temporary API piling end*/

/*************************USB END*************************************/


/*************************If Start ****************************/

#define PS_L2_UL_TRACE           (0)
#define PS_L2_DL_TRACE           (1)

#define PROCESS_ENTER(pADDRESS)  do { } while (0)
#define PROCESS_EXIT(pADDRESS)   do { } while (0)

typedef enum{
     ADDRTYPE8BIT,
     ADDRTYPE16BIT,
     ADDRTYPE32BIT
}ENADDRTYPE;

typedef enum{
     BOARD_TYPE_LTE_ONLY    = 0,
     BOARD_TYPE_GUL
}BOARD_TYPE_E;

typedef enum{
     BSP_MODEM_CORE         = 0,
     BSP_APP_CORE
}BSP_CORE_TYPE_E;

typedef enum{
     PV500_CHIP             = 0,
     V7R1_CHIP              = 1,
     PV500_PILOT_CHIP,
     V7R1_PILOT_CHIP
}BSP_CHIP_TYPE_E;

/******************************************************************************
*Function     :   BSP_OM_RegRead
*
*Description  :
*
*Input        :
*
*Output       :   无
*
*return       :
******************************************************************************/
BSP_S32 BSP_OM_RegRead(BSP_U32 u32RegAddr, ENADDRTYPE enAddrType, BSP_U32 *pu32Value);


/******************************************************************************
*Function     :   BSP_OM_RegWrite
*
*Description  :
*
*Input        :
*
*Output       :   无
*
*return       :
******************************************************************************/
BSP_S32 BSP_OM_RegWrite(BSP_U32 u32RegAddr, ENADDRTYPE enAddrType, BSP_U32 u32Value);

/******************************************************************************
*Function     :   BSP_OM_SoftReboot
*
*Description  :
*
*Input        :
*
*Output       :   无
*
*return       :
******************************************************************************/
BSP_VOID BSP_OM_SoftReboot(void);

/*****************************************************************************
*Function Name: BSP_OM_GetBoardType
*
*Function Description: Get the board type
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: BSP Board Type Enumeration
*
*Other Instructions: None
*
*****************************************************************************/
BOARD_TYPE_E BSP_OM_GetBoardType(BSP_VOID);

/*****************************************************************************
*Function Name: BSP_GetHostCore
*
*Function Description: Query the current CPU master-slave core type
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: CPU master-slave core type
*
*Other Instructions: None
*
*****************************************************************************/
BSP_CORE_TYPE_E BSP_GetHostCore(BSP_VOID);

/*****************************************************************************
*Function Name: BSP_OM_GetChipType
*
*Function Description: Get the chip type
*
*Input parameters: BSP_VOID
*
*Output parameters: None
*
*Return Value: Chip Type
*PV500_CHIP:PV500 chip
*V7R1_CHIP: V7R1 chip
*
*Other Instructions: None
*
*****************************************************************************/
BSP_CHIP_TYPE_E BSP_OM_GetChipType(BSP_VOID);

typedef struct tagBSP_OM_NET_S
{
    BSP_U32 u32NetRxStatOverFlow;       /* Receive fifo overflow statistics count */
    BSP_U32 u32NetRxStatPktErr;         /* Receive total error packet count */
    BSP_U32 u32NetRxStatCrcErr;         /* Receive CRC error packet count */
    BSP_U32 u32NetRxStatLenErr;         /* Receive invalid length packet count */
    BSP_U32 u32NetRxNoBufInt;           /* Receive no buffer interrupt count */
    BSP_U32 u32NetRxStopInt;            /* Receive stop interrupt count */
    BSP_U32 u32NetRxDescErr;            /* Receive descriptor error */

    BSP_U32 u32NetTxStatUnderFlow;      /* Send fifo underflow count */
    BSP_U32 u32NetTxUnderFlowInt;       /* Send fifo underflow interrupt count */
    BSP_U32 u32NetTxStopInt;            /* Send stop interrupt count */
    BSP_U32 u32NetTxDescErrPs;          /* Send descriptor error (ps) */
    BSP_U32 u32NetTxDescErrOs;          /* Send descriptor error (os) */
    BSP_U32 u32NetTxDescErrMsp;         /* Send descriptor error (msp) */

    BSP_U32 u32NetFatalBusErrInt;      /* Bus Error*/
}BSP_OM_NET_S;

/******************************************************************************
*Function     :   BSP_OM_NET
* 
*Description  :   
* 
*Input        :  
* 
*Output       :   无
* 
*return       :   
******************************************************************************/
BSP_VOID BSP_OM_NET(BSP_OM_NET_S *pstNetOm);

/*************************Although **********************************/

/*************************MNTN START**********************************/
/*Versioninfo i data structure maximum effective character length*/
#define VER_MAX_LENGTH                  30

/*Mem version ctrl interface operation type*/
#define VERIONREADMODE                  0
#define VERIONWRITEMODE                 1

#define OM_SAVE_EXCHFILE_ING            0
#define OM_SAVE_EXCHFILE_END            1


/*Component Type*/
typedef enum
{
    VER_BOOTLOAD = 0,
    VER_BOOTROM =1,
    VER_NV =2 ,
    VER_VXWORKS =3,
    VER_DSP =4 ,
    VER_PRODUCT_ID =5 ,
    VER_WBBP =6 ,
    VER_PS =7,
    VER_OAM =8,
    VER_GBBP =9 ,
    VER_SOC =10,
    VER_HARDWARE =11,
    VER_SOFTWARE =12,
    VER_MEDIA =13,
    VER_APP =14,
    VER_ASIC =15,
    VER_RF =16,
    VER_PMU =17,
    VER_PDM = 18,
    VER_PRODUCT_INNER_ID = 19,
    VER_INFO_NUM =20
}COMP_TYPE_I;

/* Three-color lamp status number*/
enum
{
    LED_LIGHT_SYSTEM_STARTUP,
    LED_LIGHT_POWER_ON,
    LED_LIGHT_G_REGISTERED,
    LED_LIGHT_W_REGISTERED,
    LED_LIGHT_G_CONNECTED,
    LED_LIGHT_W_CONNNECTED,
    LED_LIGHT_H_CONNNECTED,
    LED_LIGHT_OFFLINE,
    LED_LIGHT_FORCE_UPDATING,
    LED_LIGHT_NORMAL_UPDATING,
    LED_LIGHT_UPDATE_FAIL,
    LED_LIGHT_UPDATE_SUCCESS,
    LED_LIGHT_UPDATE_FILEFAIL,
    LED_LIGHT_UPDATE_NVFAIL,
    LED_LIGHT_SIM_ABSENT,
    LED_LIGHT_SHUTDOWN,
    LED_LIGHT_G_PLMN_SEARCH,
    LED_LIGHT_W_PLMN_SEARCH,
    LED_LIGHT_L_REGISTERED,
    LED_LIGHT_STATE_MAX = 32
};/*the state of LED */

enum SECURE_SUPPORT_STATUS_I
{
    SECURE_NOT_SUPPORT = 0,
    SECURE_SUPPORT = 1
};

enum SECURE_ENABLE_STATUS_I
{
    SECURE_DISABLE = 0,
    SECURE_ENABLE = 1
};

typedef enum                /* Cache type */
{
    PS_OSAL_INSTRUCTION_CACHE ,
    PS_OSAL_DATA_CACHE
} PS_OSAL_CACHE_TYPE_I;

typedef int  (*OM_SAVE_FUNC)(int funcType, unsigned char *data, unsigned int* ulLength);

typedef struct
{
    unsigned char CompId;              /* Component number: see comp type */
    unsigned char CompVer[VER_MAX_LENGTH+1];         /* Maximum version length 30 characters + \0 */
}VERSIONINFO_I;

typedef struct
{
    unsigned char enHSDSCHSupport;          /*Does the hardware support hsdpa*/
    unsigned char ucHSDSCHPhyCategory;      /*The maximum hsdpa capability level supported by hardware*/
    unsigned char enEDCHSupport;            /*Does the hardware support hsupa*/
    unsigned char ucEDCHPhyCategory;        /*The maximum hsupa capability level supported by hardware*/
    unsigned int  reserved;                 /*Reservation*/
}SUPPORT_MAX_HSPA_CAPA_STRU;

/*****************************BSP_MspProcReg  begin******************************/
typedef enum tagMSP_PROC_ID_E
{
    OM_REQUEST_PROC = 0,
    OM_ADDSNTIME_PROC = 1,
    OM_PRINTF_WITH_MODULE = 2,
    OM_PRINTF = 3,

    OM_PRINTF_GET_MODULE_IDLEV = 4,
    OM_READ_NV_PROC = 5,
    OM_WRITE_NV_PROC = 6,
    OM_MNTN_ERRLOG = 7, 

    MSP_PROC_REG_ID_MAX
}MSP_PROC_ID_E;

/*Rf power supply type*/
typedef enum tagRF_VOLTAGE
{
    RF_VOLTAGE_18V,                                                          
    RF_VOLTAGE_22V,
    RF_VOLTAGE_MAX
}RF_VOLTAGE_E;


typedef void (*BSP_MspProc)(void);
/*****************************************************************************
*Function name: DRV_MSP_PROC_REG
*
*Function description: Registration function provided by DRV to OM
*
*Input parameters: MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc
*Output parameters: NA
*
*Return Value: NA
*
*Other instructions: Reliable and measurable interface functions
*
*****************************************************************************/
extern void DRV_MSP_PROC_REG(MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc);

/********************************************************************************************************
 Function Name: BSP_FS_GetDskspc
 Function Description: Get file system space.
 Enter parameters: path: file system path.
 Output parameters: DskSpc: Total space.
                           UsdSpc: Used space.
                           VldSpc: Free space.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int BSP_FS_GetDskspc(const char *path,unsigned int *DskSpc,unsigned int  *UsdSpc,  unsigned int *VldSpc);
#define DRV_FILE_GET_DISKSPACE(path,DskSpc,UsdSpc,VldSpc)    BSP_FS_GetDskspc(path,DskSpc,UsdSpc,VldSpc)


/*****************************************************************************
 Function Name: BSP_MNTN_GetFlashSpec
 Function description: Get flash information
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern int BSP_MNTN_GetFlashSpec(unsigned char * pFlashInfo, unsigned int ulLength);
#define DRV_GET_FLASH_INFO(pFlashInfo, usLength)    BSP_MNTN_GetFlashSpec (pFlashInfo, usLength )


/************************************************************************
 *FUNCTION
 *      max_freeblock_size_get
 *DESCRIPTION
 *      get memory max free block size
 *INPUTS
 *      无
 *OUTPUTS
 *      max free block size
 *************************************************************************/
extern int DRV_GET_FREE_BLOCK_SIZE(void);

/*****************************************************************************
 Function Name: BSP_MNTN_UartRecvCallbackRegister
 Function Description: Provides API interface function for upper-level applications to register data and receive callback function pointers.
 Input parameters: uPortNo: serial port instance number.
             pCallback: callback function pointer.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int  BSP_MNTN_UartRecvCallbackRegister(unsigned char  uPortNo, pUartRecv pCallback);
#define DRV_UART_RCV_CALLBACK_REGI(uPortNo,pCallback)\
                BSP_MNTN_UartRecvCallbackRegister(uPortNo,pCallback)

/*************************************************
 Function Name: BSP_MNTN_ProductTypeGet
 Function Description: Return to the current product type
 Input parameters: None
 Output parameters: None
 Return Value: 0:STICK
             1:MOD
             2:E5
             3:CPE
*************************************************/
extern BSP_U32 BSP_MNTN_ProductTypeGet(void);
#define DRV_PRODUCT_TYPE_GET()   BSP_MNTN_ProductTypeGet()


/*************************************************
 Function Name: DRV_HKADC_BAT_VOLT_GET
 Function Description: Return to the current battery voltage value
 Input parameters: pslData: Battery voltage value
 Output parameters: pslData: Battery voltage value
 Return Value: 0: Acquisition successfully
            -1: Failed to obtain
*************************************************/
extern BSP_S32 DRV_HKADC_BAT_VOLT_GET(BSP_S32 *ps32Data);


/*****************************************************************************
*Function Name: BSP_HKADC_PaValueGet
*
*Function description:
*
*Input parameters:
*Output parameters: None
*
*Return Value: None
*
*Modification record: 2011-3-29 wuzechun creat
*
*****************************************************************************/
BSP_S32 BSP_HKADC_PaValueGet( BSP_U16 *pusValue );


#define BUILD_DATE_LEN  12
#define BUILD_TIME_LEN  12

typedef struct  tagUE_SW_BUILD_VER_INFO_STRU
{
    BSP_U16 ulVVerNo;        // Part V

    BSP_U16 ulRVerNo;        // Part R

    BSP_U16 ulCVerNo;        // Part C

    BSP_U16 ulBVerNo;        // Part B

    BSP_U16 ulSpcNo;         // Spc part

    BSP_U16 ulCustomVer;     // Customers need to customize the part, for hardware interface, low 8bit PCB number, high 8bit HW number

    BSP_U32 ulProductNo;     // such as porting,CPE, ...

    BSP_S8 acBuildDate[BUILD_DATE_LEN];  // Build date,

    BSP_S8 acBuildTime[BUILD_TIME_LEN];  // Build time

} UE_SW_BUILD_VER_INFO_STRU; // Build version



typedef struct {
    BSP_U32   bandGU;         	/*Supported gu frequency bands*/
    BSP_U32   bamdLTELow;         /*Supports LTE frequency band, b1~b32*/
    BSP_U32   bamdLTEHigh;        /*Support LTE frequency band, b33~b64*/
}BAND_INFO_T;

typedef struct {
    BSP_U16  index;         /*Hardware version number value (large version number 1 + large version number 2), distinguishing between different products*/
    BSP_U16   hwIdSub;         /*Hardware subversion number, distinguishing the different versions of the product*/
    BSP_CHAR* name;           /*Internal product name*/
    BSP_CHAR* namePlus;       /*Internal product name plus*/
    BSP_CHAR* hwVer;          /*Hardware version name*/
    BSP_CHAR* dloadId;        /*Name used in the upgrade*/
    BSP_CHAR* productId;      /*External product name*/
    BAND_INFO_T  band;            /*Frequency bands supported by the product*/
    BAND_INFO_T  bandDiv;         /*Product Supported Collection*/
    BSP_CHAR reserve[4];      /*Reservation*/
}PRODUCT_INFO_T;

/*****************************************************************************
*Function Name: BSP_HKADC_PaValueGet
*
*Function description:
*
*Input parameters:
*Output parameters: None
*
*Return Value: None
*
*Modification record: 2011-3-29 wuzechun creat
*
*****************************************************************************/
const UE_SW_BUILD_VER_INFO_STRU* BSP_GetBuildVersion(BSP_VOID);


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
BSP_S32 BSP_HwGetHwVersion (char* pFullHwVersion, BSP_U32 ulLength);


/*****************************************************************************
*Function Name: BSP_GetSupportBands
*
*Function Description: Get supported frequency bands
*
*Input parameters: BAND_INFO_T *pBandInfo, frequency band information structure pointer
*Output parameters:
*
 Return Value: 0: Correct, non-0: Failed
*
*Other instructions:
*
*****************************************************************************/
BSP_S32 BSP_GetSupportBands( BAND_INFO_T *pBandInfo);

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
BSP_BOOL BSP_HwIsSupportWifi(BSP_VOID);

/*****************************************************************************
 Function Name: hkadcBatADCRead
 Function Description: Obtain the battery voltage sampling value
 Input parameters: None
 Output parameters: pTemp: A pointer to the battery temperature.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern BSP_S32 DRV_GET_BATTERY_ADC(BSP_S32 * pslData);


/*****************************************************************************
 Function Name: BSP_PMU_LDOOFF
 Function Description: This interface is used to turn off the voltage output of a certain LDO.
 Input parameters: ucLDO: Indicates a certain LDO voltage. The value range is 1 to 12.
 Output parameters: None.
 Return Value: None.
 Note: LDO6 is controlled by hardware, and the software does not support the operation of LDO6. If it is turned off for LDO1, the system will shut down.
*****************************************************************************/
extern void BSP_PMU_LDOOFF(BSP_U8 u8LDO);
#define DRV_PM_LDO_OFF(u8LDO)                 BSP_PMU_LDOOFF(u8LDO)

/*****************************************************************************
 Function Name: BSP_PMU_LDOON
 Function Description: This interface is used to set the voltage output of a certain LDO.
 Input parameters: ucLDO: Indicates a certain LDO voltage. The value range is 2 to 12.
 Output parameters: None.
 Return Value: None.
 Note: Operations of LDO1 and LDO6 are not supported. LDO1 is automatically turned on when the system is powered on, and once it is turned off, the entire system will be powered off; LDO6 is controlled by hardware pins.
*****************************************************************************/
extern void BSP_PMU_LDOON(BSP_U8 u8LDO);
#define DRV_PM_LDO_ON(u8LDO)                  BSP_PMU_LDOON(u8LDO)

/************************************
Function Name: BSP_PMU_AptEnable
Function Description: Enable the APT function of PMU
Input parameters: None
Output parameters: Return to setting status
Return Value: BSP_OK, Set successfully
		Other values: Setting failed
**************************************/
extern BSP_S32 BSP_PMU_AptEnable(BSP_VOID);
#define DRV_PMU_APT_ENABLE()    BSP_PMU_AptEnable()

/************************************
Function Name: BSP_PMU_AptDisable
Function description: De-enable the APT function of PMU
Input parameters: None
Output parameters: Return to setting status
Return Value: BSP_OK, Set successfully
		Other values: Setting failed
**************************************/
extern BSP_S32 BSP_PMU_AptDisable(BSP_VOID);
#define DRV_PMU_APT_DISABLE()    BSP_PMU_AptDisable()

/************************************
Function Name: BSP_PMU_AptIsEnable
Function Description: Check whether the APT function of PMU is enabled
Input parameters: None
Output parameters: Return to setting status
Return Value: BSP_TURE, enable APT
		    BSP_FALSE, APT not enabled
**************************************/
extern BSP_BOOL  BSP_PMU_AptIsEnable(BSP_VOID);
#define DRV_PMU_APT_ISENABLE()    BSP_PMU_AptIsEnable()

extern BSP_U32 DRV_ADJUST_RF_VOLTAGE(RF_VOLTAGE_E enRfVol, BSP_U32 u32Voltage);

/*****************************************************************************
 Function Name: BSP_ONOFF_DrvPowerOff
 Function description: Power down directly on the board
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void DRV_POWER_OFF(void);


/*****************************************************************************
 Function Name: BSP_MNTN_GetGsmPATemperature
 Function Description: Obtain the GSM PA temperature
 Input parameters: Temprature
             hkAdcTalble
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_GetGsmPATemperature(int *temperature, unsigned short *hkAdcTable);
#define DRV_GET_PA_GTEMP(temperature, hkAdcTable)   \
                BSP_MNTN_GetGsmPATemperature(temperature, hkAdcTable)

/*****************************************************************************
 Function Name: getWcdmaPATemperature
 Function Description: Obtain WCDMA PA temperature
 Input parameters: Temprature
             hkAdcTalble
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_GetWcdmaPATemperature (int *temperature, unsigned short *hkAdcTable);
#define  DRV_GET_PA_WTEMP(temperature, hkAdcTable)  \
              BSP_MNTN_GetWcdmaPATemperature(temperature, hkAdcTable)

/*****************************************************************************
 Function Name: BSP_MNTN_GetSIMTemperature
 Function Description: Get the SIM card temperature
 Input parameters: pusHkAdcTable: electromagnetic voltage meter
 Output parameters: pTemp: A pointer to the temperature of the SIM card.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_GetSIMTemperature(int *plTemp, unsigned short *pusHkAdcTable);
#define DRV_GET_SIM_TEMP(plTemp, pusHkAdcTable)   \
               BSP_MNTN_GetSIMTemperature(plTemp, pusHkAdcTable)

/*****************************************************************************
 Function Name: void BSP_PWRCTRL_DfsCpuUp(void)
 Function description: Set CPU freq to 450M
 Input parameters: 
 Output parameters: 
 Return Value: 
*****************************************************************************/
extern void BSP_PWRCTRL_DfsCpuUp(void);
#define BSP_PWRCTRL_DFS_CPUUP() BSP_PWRCTRL_DfsCpuUp()

/*****************************************************************************
 Function Name: void BSP_PWRCTRL_DfsThrProOpen(void)/
 			 void BSP_PWRCTRL_DfsThrProClose(void)
 Function description: open CPU freq to THREE PROFILES MODE
 Input parameters: 
 Output parameters: 
 Return Value: 
*****************************************************************************/

extern void BSP_PWRCTRL_DfsThrProOpen(void);
#define BSP_PWRCTRL_DFS_THRPRO_OPEN() BSP_PWRCTRL_DfsThrProOpen()
/*****************************************************************************
 Function Name: void BSP_PWRCTRL_DfsThrProClose(void)
 Function description: CLOSE CPU freq to THREE PROFILES MODE
 Input parameters: 
 Output parameters: 
 Return Value: 
*****************************************************************************/
extern void BSP_PWRCTRL_DfsThrProClose(void);
#define BSP_PWRCTRL_DFS_THRPRO_CLOSE() BSP_PWRCTRL_DfsThrProClose()

/*****************************************************************************
Function:   BSP_PWRCTRL_GetCpuFreq(void)
Description:Get CPU frequency
Input:      None
Output:     None
Return:     None
Others:
*****************************************************************************/
extern int BSP_PWRCTRL_GetCpuFreq();
#define BSP_PWRCTRL_GET_CPUFREQ() BSP_PWRCTRL_GetCpuFreq()


/*****************************************************************************
 Function Name: BSP_DFS_GetCurCpuLoad
 Function Description: Query the current CPU
 Input parameters: pu32AcpuLoad ACPUload pointer
             pu32CcpuLoad CCPUload pointer
 Output parameters: pu32AcpuLoad ACPUload pointer
             pu32CcpuLoad CCPUload pointer
 Return Value: 0: The operation was successful;
            -1: The operation failed.
*****************************************************************************/
extern BSP_U32 BSP_DFS_GetCurCpuLoad(BSP_U32 *pu32AcpuLoad,BSP_U32 *pu32CcpuLoad);
#define DRV_GET_CUR_CPU_LOAD(pu32AcpuLoad,pu32CcpuLoad) BSP_DFS_GetCurCpuLoad(pu32AcpuLoad,pu32CcpuLoad)


/*****************************************************************************
 Function Name: BSP_GU_GetVerTime
 Function Description: Get the version compilation time
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern char *BSP_GU_GetVerTime(void);
#define DRV_GET_VERSION_TIME()    BSP_GU_GetVerTime()

/*****************************************************************************
 Function Name: BSP_MNTN_MemVersionCtrl
 Function description: Component version read and write interface.
 Input parameters: pcData: When Mode is read, it is the memory address of the component version information returned by the caller to save;
                     When Mode is written, prepare the address of the written component version information for the caller.
             ucLength: When Mode is read, it is the memory size of the component version information returned by the caller to save;
                       When Mode is written, the number of characters (excluding '\0') of the written component version information is prepared for the caller.
             ucType: Version information ID,
                     0: BOOTLOAD;
                     1: BOOTROM;
                     2: NV;
                     3: VXWORKS;
                     4: DSP;
                     5; CDROMISO;
                     6: PHY;
                     7: PS;
                     8: COM;
                     9: RF;
                     10: SOC;
                     11: HARDWARE;
                     12: SOFTWARE;
                     13: MEDIA;
                     14: APP;
                     15: INFO_NUM.
             ucMode: 0: Read component version information of the specified ID; 1: Write component version information of the specified ID.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_MemVersionCtrl(signed char *pcData, unsigned char ucLength, unsigned char ucType, unsigned char ucMode);
#define DRV_MEM_VERCTRL(pcData,ucLength,ucType,ucMode)  \
                          BSP_MNTN_MemVersionCtrl(pcData,ucLength,ucType,ucMode)

/********************************************************************************************************
 Function Name: getHwVersionIndex
 Function description: Get the assembled hardware index value of the hardware ID read by HKADC
 Input parameters: None
 Output parameters: Index value of hardware ID

 Return value: Non-1: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int getHwVersionIndex(void);
#define DRV_GET_HW_VERSION_INDEX()       getHwVersionIndex()

/*****************************************************************************
 Function Name: BSP_MNTN_GetFullHwVersion
 Function description: Hardware full version reading interface.
 Input parameters: pFullHwVersion: The memory address of the caller's memory full version information to be saved;
             ulLength: The memory size of the complete hardware version information to be saved by the caller;
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_GetFullHwVersion(char * pFullHwVersion,unsigned int ulLength);
#define  DRV_GET_FULL_HW_VER(pFullHwVersion,ulLength)  \
               BSP_MNTN_GetFullHwVersion(pFullHwVersion,ulLength)

/*****************************************************************************
 Function Name: BSP_MNTN_GetProductIdInter
 Function description: Product name full version reading interface.
 Input parameters: pProductIdInter: The memory address of the full version of the product name to be saved by the caller;
             ulLength: The memory size of the full version of the product name to be saved by the caller;
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_GetProductIdInter(char * pProductIdInter, unsigned int ulLength);
#define  DRV_GET_PRODUCTID_INTER_VER(pProductIdInter,ulLength)  \
              BSP_MNTN_GetProductIdInter(pProductIdInter,ulLength)

/*****************************************************************************
 Function Name: BSP_MNTN_VersionQueryApi
 Function Description: Query the version number of all components.
 Input parameters: ppVersionInfo: The version information address to be saved.
 Output parameters: ucLength: The number of bytes of data to be returned.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_VersionQueryApi(void ** ppVersionInfo, unsigned int * ulLength);
#define  DRV_VER_QUERY(ppVersionInfo,ulLength)  \
              BSP_MNTN_VersionQueryApi (ppVersionInfo,ulLength)



typedef enum DRV_SHUTDOWN_REASON_tag_s
{
    DRV_SHUTDOWN_LOW_BATTERY,           /* Low battery power                 */
    DRV_SHUTDOWN_BATTERY_ERROR,         /* Battery abnormality                   */
    DRV_SHUTDOWN_POWER_KEY,             /* Press and hold the Power key to turn off the machine          */
    DRV_SHUTDOWN_TEMPERATURE_PROTECT,   /* Overtemperature protection shutdown               */
    DRV_SHUTDOWN_LOW_TEMP_PROTECT,
    DRV_SHUTDOWN_RESET,                 /* System soft reset                 */
    DRV_SHUTDOWN_CHARGE_REMOVE,         /* Unplug the charger in the power-off charging mode */
    DRV_SHUTDOWN_UPDATE,                /* Shut down and enter upgrade mode         */
    DRV_SHUTDOWN_BUTT
}DRV_SHUTDOWN_REASON_ENUM;

#ifdef FEATURE_HUAWEI_MBB_CHG
/*Battery (battery) actively reported events*/
typedef enum _GPIO_BAT_EVENT
{
    GPIO_BAT_CHARGING_ON_START = 0,  /* In power-on mode, start charging */
    GPIO_BAT_CHARGING_OFF_START,     /* In shutdown mode, start charging */   
    GPIO_BAT_LOW_POWER,              /* Report this event when the battery is low */
    GPIO_BAT_CHARGING_UP_STOP,       /* Report this event when the power is turned on and stopped charging */
    GPIO_BAT_CHARGING_DOWN_STOP,     /* Report this event when the power is shut down and charging is stopped. */
    GPIO_BAT_LOW_POWEROFF = 9,           /* Shutdown caused by low battery power, report this event*//*BEGIN: PN: New specifications for normalized charging Modified by d00135750, 2012/12/20   */
    GPIO_BAT_EVENT_MAX               /* If the event value is not less than this value, it is an illegal event */
}GPIO_BATTERY_EVENT;

/*Battery high temperature reporting incident*/
typedef enum _TEMP_EVENT
{
   GPIO_TEMP_BATT_LOW,   /* Battery cryogenic events */      
   GPIO_TEMP_BATT_HIGH,   /* Battery high temperature events */ 
   GPIO_TEMP_BATT_NORMAL, /* Battery high temperature release event */
   GPIO_TEMP_BATT_MAX     /* If the event value is not less than this value, it is an illegal event*/
}GPIO_TEMP_EVENT;

/*Events reported by Key*/
typedef enum _GPIO_KEY_EVENT
{
    GPIO_KEY_WAKEUP = 0,           /* Short press any key to report this event to light up the screen */    
    GPIO_KEY_WLAN,                 /* Used to turn on/off wlan devices, different versions use different keys */
    GPIO_KEY_WPS,                  /* When you hold down the wps key (more than 5 seconds), this event will be reported to activate the wps function. */
    GPIO_KEY_FACTORY_RESTORE,      /* When you hold the reset key for a long time, this event will be reported to restore the factory settings. */
    GPIO_KEY_POWER_OFF,            /* When you hold the power key for a long time, this event will be reported and used to shut down the machine. */
    GPIO_KEY_UPDATE,               /* When the system needs to be upgraded, report this event to be used to upgrade the system */
    GPIO_KEY_SSID,                 /* When the wps key is short pressed (less than 5 seconds), it is used to display ssid, only some versions need it */
    GPIO_KEY_WPS_PROMPT,           /* When the wps key is short pressed (more than 1 second), it is used to display the wps prompt, only some versions need it. */ 
    GPIO_KEY_POWER_SHORT,          /* Press the power button for a short time, and release the button within 500 milliseconds. */
    GPIO_KEY_WPS_SHORT,            /* Press the wps button for a short time, and release the button within 500 milliseconds */
    GPIO_KEY_RESET_SHORT,          /* Press the Reset button for a short time, release the button within 500 milliseconds */
    GPIO_KEY_WPS_POWER,            /* Press WPS and POWER keys together for 2 seconds */
    GPIO_KEY_WPS_POWER_UP,         /* The WPS and POWER keys are pressed together for 2 seconds and then lifted before 5 seconds, or lifted within 500 milliseconds after 5 seconds. */
    GPIO_KEY_POWER_ONE_SECOND,     /* Press Power for 1 second */
    GPIO_KEY_POWER_ONE_SECOND_UP,  /* Press Power for 1 second and then lift it before 2 seconds */
    GPIO_KEY_WPS_ONE_SECOND,       /*WPS short press 1 second event, mainly used to switch WiFi Offload*/
    GPIO_KEY_WPS_DIAL= 16,         /*WPS short press releases WPS dialing events, mainly used for key dialing*/  
#ifdef FEATURE_FUNC_FAST_BOOT
    /*The platform provides 5 seconds of quick shutdown events*/
    GPIO_KEY_SHORT_POWER_OFF = 17,
    /*The platform provides 5 seconds of quick boot event*/
    GPIO_KEY_SHORT_POWER_ON = 18,
 #endif
    GPIO_KEY_EVENT_MAX             /* If the event value is not less than this value, it is an illegal event */
}GPIO_KEY_EVENT;
#endif /* Feature huawei mbb chg */

/*****************************************************************************
 Function Name: DRV_SHUT_DOWN
 Function description: Board shutdown
 Input parameters: None
 Output parameters: eReason: shutdown reason
 Return Value: None
*****************************************************************************/
void drvShutdown( DRV_SHUTDOWN_REASON_ENUM eReason );
void DRV_SHUT_DOWN( DRV_SHUTDOWN_REASON_ENUM eReason );

typedef enum tagMNTN_FILE_TYPE_E
{
	MNTN_TYPE_ZSP = 0,
	MNTN_TYPE_HIFI,
    MNTN_TYPE_MAX
}MNTN_FILE_TYPE_E;

/*****************************************************************************
 Function Name: BSP_MNTN_ABBSelfCheck
 Function description: Return the abb self-test result
 Input parameters: None
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_ABBSelfCheck(void);
#define DRV_ABB_SELFCHECK()    BSP_MNTN_ABBSelfCheck()

/*****************************************************************************
 Function Name: BSP_MNTN_PmuSelfCheck
 Function description: PMU self-test.
 Input parameters: None.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_PmuSelfCheck(void);
#define DRV_PMU_SELFCHECK()    BSP_MNTN_PmuSelfCheck()

/*****************************************************************************
 Function Name: BSP_MNTN_I2cSelfCheck
 Function description: Return I2c self-test result
 Input parameters: None
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: This interface is only used in PS PC projects and is not currently used and is reserved for the time being.
*****************************************************************************/
extern int BSP_MNTN_I2cSelfCheck(void);
#define DRV_I2C_SELFCHECK()    BSP_MNTN_I2cSelfCheck()

/*****************************************************************************
 Function Name: BSP_MNTN_SpiSelfCheck
 Function description: Return spi self-test results
 Input parameters: None
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: This interface is only used in PS PC projects and is not currently used and is reserved for the time being.
*****************************************************************************/
extern int BSP_MNTN_SpiSelfCheck(void);
#define DRV_SPI_SELFCHECK()    BSP_MNTN_SpiSelfCheck()

/*****************************************************************************
 Function Name: BSP_MNTN_GetCodecFuncStatus
 Function Description: Get CODEC function status
 Input parameters: None
 Output parameters: None
 Return Value: 1: CODEC exists
             0: CODEC does not exist
*****************************************************************************/
extern  int BSP_MNTN_GetCodecFuncStatus(void);
#define DRV_GET_CODEC_FUNC_STATUS()    BSP_MNTN_GetCodecFuncStatus()

/*****************************************************************************
 Function Name: MNTN_RFGLockStateGet
 Function Description: Read the GSM RF locked state.
 Input parameters: None.
 Output parameters: Status: Stores the output value of the locked state.
                        0: TX or RX is locked.
                        1: TX and RX are not locked;

 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_RFGLockStateGet(unsigned int *Status);
#define DRV_GET_RF_GLOCKSTATE(Status)    BSP_MNTN_RFGLockStateGet(Status)

/****************************************************************************
 Function Name: BSP_MNTN_LedStateFuncReg
 Function Description: This interface is a hook function provided to the protocol stack, which is used to obtain the lighting status of the protocol stack.
 Input parameters: The protocol stack obtains the pointer to the lighting state function.
 Output parameters: None.
 Return Value: None.
 Notes: None.

******************************************************************************/
extern void BSP_MNTN_LedStateFuncReg (pFUNCPTR p);
#define DRV_LED_STATE_FUNREG(p)    BSP_MNTN_LedStateFuncReg(p)

/*****************************************************************************
 Function Name: BSP_MNTN_LedFlush
 Function description: Tri-color light settings.
 Input parameters: status: status of the three-color light, see LED_STATUS_I
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_LedFlush(unsigned long state);
#define DRV_LED_FLUSH(state)    BSP_MNTN_LedFlush(state)

/*****************************************************************************
 Function Name: BSP_MNTN_LedControlAddressGet
 Function description: Get customized lighting data
 Input parameters: None
 Output parameters: None.
 Return Value: The global variable address of the lighting control data
*****************************************************************************/
extern unsigned int BSP_MNTN_LedControlAddressGet(void);
#define DRV_LED_GET_ADDRESS()   BSP_MNTN_LedControlAddressGet()

/*****************************************************************************
 Function Name: BSP_MNTN_BootForceloadModeCheck
 Function Description: Force load mode query.
 Input parameters: None.
 Output parameters: None.
 Return value: 0: Forced loading;
             1: Non-force loading.
*****************************************************************************/
extern unsigned int BSP_MNTN_BootForceloadModeCheck(void);
#define DRV_BOOT_FORCELOAD_MODE_CHECK()    BSP_MNTN_BootForceloadModeCheck()

/*****************************************************************************
 Function Name: BSP_MNTN_BootFlagConfig
 Function Description: Configure the startup flags in the Bootline data to be fast self-start, countdown to 7 seconds self-start or manual start.
 Input parameters: ulBootFlag: Not defined yet.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern  int BSP_MNTN_BootFlagConfig(unsigned int ulBootFlag);
#define DRV_BOOTFLAG_CFG(ulBootFlag)    BSP_MNTN_BootFlagConfig(ulBootFlag)

/*****************************************************************************
 Function Name: BSP_MNTN_CheckArmTCM
 Function description: TCM detection
 Input parameters: None.
 Output parameters: None.
 Return value: 0xffff0000: The detection is successful, TCM is normal
            Address: The address where the error was detected (0-0x5ffff)
            0xffffffff: Detection failed

*****************************************************************************/
extern unsigned int BSP_MNTN_CheckArmTCM(void);
#define DRV_CHECK_ARM_TCM()    BSP_MNTN_CheckArmTCM()

/*****************************************************************************
 Function Name: BSP_MNTN_ExchOMSaveRegister
 Function description: Registered to the bottom soft trace save function, used to reset the board,
             Save the current signaling and log information.
 Input parameters: funcType: Function ID
             pFunc:Registered callback function
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: The same funcType can only register one callback function, and repeated registration will override the original function
*****************************************************************************/
extern int BSP_MNTN_ExchOMSaveRegister(int funcType, OM_SAVE_FUNC *pFunc);
#define DRV_SAVE_REGISTER(funcType, pFunc)    BSP_MNTN_ExchOMSaveRegister(funcType, pFunc)

/*****************************************************************************
 Function Name: BSP_MNTN_ExchOMRegisterTdsPhy
 Function description: TDS is registered to the bottom soft memory address, which is used to record TDS physical layer information to the crash BIN file when the crash is frozen. Called during initialization
 Input parameters: ulLength: Memory length
            tphyaddr: Starting address
 Output parameters: None
 Return Value: 
 Notes:
*****************************************************************************/
extern void BSP_MNTN_ExchOMRegisterTdsPhy(BSP_U32 ulLength,char * tphyaddr);


/*****************************************************************************
 Function Name: BSP_MNTN_SystemError
 Function Description: The system is hot-started.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_SystemError(int modId, int arg1, int arg2, char * arg3, int arg3Length);
#define DRV_SYSTEM_ERROR(modId, arg1, arg2, arg3, arg3Length)\
                   BSP_MNTN_SystemError(modId, arg1, arg2, arg3, arg3Length)

/*****************************************************************************
 Function Name: BSP_MNTN_ExchMemMalloc
 Function Description: Together with the systemOsaError interface, it is used to record large blocks of reset information.
             Currently, it supports up to 128k.
 Input parameters: ulSize -Memory size
 Output parameters: None.
 Return Value: Memory Address
*****************************************************************************/
extern unsigned int BSP_MNTN_ExchMemMalloc(unsigned int ulSize);
#define DRV_EXCH_MEM_MALLOC(ulSize)    BSP_MNTN_ExchMemMalloc(ulSize)

/*****************************************************************************
 Function Name: BSP_MNTN_OmExchFileSave
 Function description: Used to record the exception file interaction between ARM and DSP
 Input parameters: address: buffer address
                length: storage length
                IsFileEnd: 1 means the last piece of the file, 0 means the non-last piece
 Output parameters: None.
 Return Value: 0 is OK.
*****************************************************************************/
extern int BSP_MNTN_OmExchFileSave(void * address, unsigned long length,unsigned char IsFileEnd,unsigned char type);
#define DRV_EXCH_FILE_SAVE(address, length, IsFileEnd, type) BSP_MNTN_OmExchFileSave(address, length, IsFileEnd, type)

/*****************************************************************************
 Function Name: BSP_MNTN_GetHwGpioInfo
 Function description: Get flash information
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern int BSP_MNTN_GetHwGpioInfo(unsigned char *pGpioInfo, unsigned long usLength );
#define DRV_GET_GPIO_INFO(pGpioInfo, usLength)    BSP_MNTN_GetHwGpioInfo(pGpioInfo, usLength )

/*****************************************************************************
 Function Name: BSP_MNTN_Int4ToString
 Function Description: Convert the register value to ASCII characters
 Input parameters: register value, and the storage space for converted characters, the maximum space shall not exceed 30byte
 Output parameters: Convert characters
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_MNTN_Int4ToString(unsigned int value, char * string);
#define DRV_MEM_VERCONVERT(value,string)    BSP_MNTN_Int4ToString(value, string)

/*****************************************************************************
 Function Name: BSP_MNTN_HeapInfoGet
 Function Description: Get memory information on the single board side.
 Enter parameters: allocSize: The allocated heap memory size, unit byte.
             totalSize: The total size of heap memory, unit byte.
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: Operation failed.
*****************************************************************************/
extern int BSP_MNTN_HeapInfoGet(unsigned int *allocSize, unsigned int *totalSize);
#define DRV_GET_HEAPINFO(allocSize, totalSize)    BSP_MNTN_HeapInfoGet(allocSize, totalSize)

/*****************************************************************************
 Function Name: BSP_MNTN_GetSupportBands
 Function Description: Get current support from Soft.
 Input parameters: pusWBands -WCDMA frequency band value
             pusGBands -GSM's band value

 Output parameters: pusWBands -WCDMA frequency band value
             pusGBands -GSM's band value

 Return Value: 0: Correct, non-0: Failed
             Which frequency bands are supported by Bit bits.
*****************************************************************************/
extern int BSP_MNTN_GetSupportBands(unsigned short *pusWBands, unsigned short *pusGBands);
#define DRV_GET_SUPPORT_BANDS(pusWBands, pusGBands)    BSP_MNTN_GetSupportBands(pusWBands, pusGBands)

/*****************************************************************************
 Function Name: BSP_MNTN_GetSupportDivBands
 Function description: Obtain the currently supported hierarchical frequency band from the bottom soft
 Input parameters: N/A

 Output parameters: pDivBands -Return to the hierarchical frequency band supported by the current board, and the frequency band is bit
 			      Streaming arrangement (same as frequency band query of W and G)

 Return Value: 0: Correct, non-0: Failed
*****************************************************************************/
extern int BSP_MNTN_GetSupportDivBands(unsigned short* pDivBands);
#define DRV_GET_SUPPORT_DIVBANDS(pDivBands)    BSP_MNTN_GetSupportDivBands(pDivBands)

/********************************************************************************************************
 Function Name: BSP_MNTN_WriteSysBackupFile
 Function description: Backup ZSP and NV data to FLASH
 Input parameters: None
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int BSP_MNTN_WriteSysBackupFile(void);
#define DRV_BACKUP_SYS_FILE()    BSP_MNTN_WriteSysBackupFile()

/*****************************************************************************
 Function Name: DRV_MNTN_GetExtAntenLockState
 Function Description: Get the connection status between wired and wireless
 Input parameters: None
 Output parameters: 0 -Wireless connection
             1 -Wired connection
 Return Value: 0 -Success
             Others are failures

*****************************************************************************/
extern int DRV_MNTN_GetExtAntenLockState(unsigned int *Status);
#define DRV_GET_ANTEN_LOCKSTATE(Status)    DRV_MNTN_GetExtAntenLockState(Status)

/*****************************************************************************
 Function Name: BSP_MNTN_ExtAntenIntInstall
 Function description: interrupt registration function to obtain the current antenna state
 Input parameters: routine -interrupt handling function
             para -reserved fields
 Output parameters: None
 Return Value: void

*****************************************************************************/
extern void BSP_MNTN_ExtAntenIntInstall(void* routine, int para);
#define DRV_ANTEN_INT_INSTALL(routine, para)    BSP_MNTN_ExtAntenIntInstall(routine, para)

/*****************************************************************************
 Function Name: BSP_MNTN_ExchHookFuncAdd
 Function description: Task switching and interrupt hook registration
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void BSP_MNTN_ExchHookFuncAdd(void);
#define DRV_EXCH_HOOK_FUNC_ADD()    BSP_MNTN_ExchHookFuncAdd()

/*****************************************************************************
 Function Name: BSP_MNTN_ExchHookFuncDelete
 Function description: Task switching and interrupt hook logout
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void BSP_MNTN_ExchHookFuncDelete(BSP_CHAR* file, BSP_U32 line);
#define DRV_EXCH_HOOK_FUNC_DELETE()    BSP_MNTN_ExchHookFuncDelete(__FILE__, __LINE__)

/*****************************************************************************
 Function Name: BSP_MNTN_ExcStackPeakRecord
 Function description: Record task information that exceeds the task stack.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_ExcStackPeakRecord(void);
#define DRV_STACK_HIGH_RECORD()    BSP_MNTN_ExcStackPeakRecord()

/*****************************************************************************
 Function Name: BSP_MNTN_SubIntHook
 Function description: Secondary interrupt service hook function registration. Related
 Input parameters: p_Func: hook function pointer.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_SubIntHook(pFUNCPTR p_Func);
#define DRV_VICINT_HOOK_ADD(p_Func)    BSP_MNTN_SubIntHook(p_Func)

/*****************************************************************************
 Function Name: BSP_MNTN_SubIntHookDel
 Function Description: Secondary interrupt service hook function logout. Related
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_SubIntHookDel(void);
#define DRV_VICINT_HOOK_DEL()    BSP_MNTN_SubIntHookDel()

/*****************************************************************************
 Function Name: BSP_MNTN_IntBreakInHook
 Function description: First-level interrupt entry hook function registration. Related
 Input parameters: p_Func:hook function.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_IntBreakInHook(pFUNCPTR p_Func);
#define DRV_VICINT_IN_HOOK_ADD(p_Func)   BSP_MNTN_IntBreakInHook(p_Func)

/*****************************************************************************
 Function Name: BSP_MNTN_IntBreakInHookDel
 Function description: First-level interrupt entry hook function logout. Related
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_IntBreakInHookDel(void);
#define DRV_VICINT_IN_HOOK_DEL()    BSP_MNTN_IntBreakInHookDel()

/*****************************************************************************
 Function Name: BSP_MNTN_IntBreakOutHook
 Function description: First-level interrupt exit hook function registration. Related to maintaining and measuring, V7 drives piles first
 Input parameters: p_Func: hook function.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_IntBreakOutHook(pFUNCPTR p_Func);
#define DRV_VICINT_OUT_HOOK_ADD(p_Func)    BSP_MNTN_IntBreakOutHook(p_Func)

/*****************************************************************************
 Function Name: vicIntBreakOutHookDel
 Function Description: First-level interrupt exit hook function logout. Related
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_IntBreakOutHookDel(void);
#define DRV_VICINT_OUT_HOOK_DEL()    BSP_MNTN_IntBreakOutHookDel()

/*****************************************************************************
 Function Name: BSP_MNTN_IntLvlChgHook
 Function description: Level 1 interrupt level change hook function registration. Related
 Input parameters: p_Func: hook function.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_IntLvlChgHook(pFUNCPTR p_Func);
#define DRV_VICINT_LVLCHG_HOOK_ADD(p_Func)    BSP_MNTN_IntLvlChgHook(p_Func)

/*****************************************************************************
 Function Name: BSP_MNTN_IntLvlChgHookDel
 Function description: Level 1 interrupt level change hook function logout. Related
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_MNTN_IntLvlChgHookDel(void);
#define DRV_VICINT_LVLCHG_HOOK_DEL()    BSP_MNTN_IntLvlChgHookDel()

/*****************************************************************************
 Function Name: BSP_MNTN_TFUPIfNvBackup
 Function description: Before upgrading using the TF card, use this interface to determine whether you need to back up NV items.
 Input parameters: None
 Output parameters: None
 Return Value: 1: Backup is required
             0: No backup required
*****************************************************************************/
extern int BSP_MNTN_TFUPIfNeedNvBackup(void);
#define DRV_TF_NVBACKUP_FLAG()    BSP_MNTN_TFUPIfNeedNvBackup()

/*****************************************************************************
 Function Name: BSP_MNTN_GetSDDevName
 Function Description: Get the SD device name in the device list
 Input parameters: None.
 Output parameters: None.
 Return Value: SD_DEV_TYPE_STR structure pointer.
*****************************************************************************/
extern SD_DEV_TYPE_STR * BSP_MNTN_GetSDDevName(void);
#define DRV_GET_SD_PATH()     BSP_MNTN_GetSDDevName()

/*****************************************************************************
 Function Name: BSP_PROD_GetRecoverNvInfo
 Function description: Obtain the NV information currently required to be covered from the bottom soft
 Input parameters: N/A

 Output parameters: ppNvInfo -an array pointing to NV information structure
             pulNvNum -Number of arrays

 Return Value: 0: Correct, non-0: Failed
*****************************************************************************/
extern  int BSP_PROD_GetRecoverNvInfo(void **ppNvInfo, unsigned long *pulNvNum);
#define  DRV_GET_RECOVER_NV_INFO(ppNvInfo, pulNvNum)  \
                BSP_PROD_GetRecoverNvInfo(ppNvInfo, pulNvNum)


/*****************************************************************************
 Function Name: BSP_MNTN_GETSDSTATUS
 Function Description: Return to the SD card in-location status
 Input parameters: None.
 Output parameters: None.
 Return value: 1 in bit
            0 is not in position

*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern int BSP_MNTN_GETSDSTATUS(void);
#define DRV_SDMMC_GET_STATUS()    BSP_MNTN_GETSDSTATUS()
#else
extern int BSP_SDMMC_GetSDStatus(void);
#define DRV_SDMMC_GET_STATUS()    BSP_SDMMC_GetSDStatus()
#endif

/*****************************************************************************
 Function Name: SDIO_read_write_blkdata
 Function description: SD card read and write interface
 Input parameters: nCardNo SD card number
                dwBlkNo Block Number
                nBlkCount read and write size
                pbDataBuff read and write buffer
                nFlags Read/write
 Output parameters: None.
 Return value: 0: Read and write successfully
            Non-0: Error code
*****************************************************************************/
extern int BSP_MNTN_SDRWBlkData(int nCardNo,unsigned int dwBlkNo,
                   unsigned int nBlkCount, unsigned char *pbDataBuff,int nFlags);
#define DRV_SDMMC_BLK_DATA_RW(nCardNo,dwBlkNo,nBlkCount,pbDataBuff,nFlags)   \
             BSP_MNTN_SDRWBlkData(nCardNo,dwBlkNo,nBlkCount,pbDataBuff, nFlags)


/***********************************Memory copy optimization*********************************************/
/*****************************************************************************
 Function Name: __rt_memcpy
 Function description: Assembly version of memcpy function
 Input parameters: Dest: Destination address
             Src: Source address
             Count: The size of the copy data
 Output parameters: None.
 Return Value: Destination address.
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern void * __rt_memcpy(void * Dest, const void * Src, unsigned long Count);
#define DRV_RT_MEMCPY(Dest,Src,Count)       __rt_memcpy(Dest,Src,Count)
#else
#define DRV_RT_MEMCPY(Dest,Src,Count)       memcpy(Dest,Src,Count)
#endif



/************************************The remaining macros are defined as empty functions************************************************/
/*****************************************************************************
 Function Name: UsbStatusCallbackRegister
 Function Description: Record pCallBack into global variables. SD card power consumption-related interface, V7 drives piles first
 Input parameters: None.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
 /*extern int  UsbStatusCallbackRegister(pUsbState pCallBack);*/
#define DRV_USB_STATUS_CALLBACK_REGI(pCallBack)              DRV_OK

/*****************************************************************************
 Function Name: CicomClkEnable
 Function description: This routine enable CICOM IP clock gating. Low power consumption related, V7 pile driving
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
/*extern void CicomClkEnable(void);*/
#define DRV_CICOM_CLK_ENABLE()

/*****************************************************************************
 Function Name: CicomClkDisable
 Function description: This routine disable CICOM IP clock gating. Low power consumption related, V7 pile driving
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
/*extern void CicomClkDisable(void);*/
#define DRV_CICOM_CLK_DISABLE()

/*****************************************************************************
 Function Name: HdlcClkEnable
 Function description: This routine enable HDLC IP clock gating. Low power consumption related, V7 pile driving
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
/*extern void HdlcClkEnable(void);*/
#define DRV_HDLC_CLK_ENABLE()

/*****************************************************************************
 Function Name: HdlcClkDisable
 Function description: This routine disable HDLC IP clock gating. Low power consumption related, V7 pile driving
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
/*extern void HdlcClkDisable(void);*/
#define DRV_HDLC_CLK_DISABLE()

/*****************************************************************************
 Function Name: DRV_MEM_READ
 Function description: Query memory at a 32-bit width and output 4 bytes of data. Maintainable and measurable, V7 drives piles first
 Input parameters: ulAddress: Query the address space address, the address must be within the memory range, otherwise -1 will be returned.
 Output parameters: pulData: Pointer to the query content.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int DRV_MEM_READ(unsigned int ulAddress, unsigned int* pulData);

/*****************************************************************************
 Function Name: DrvLogInstall
 Function Description: Print function registration. Maintainable and measurable, V7 drives piles first
 Input parameters: fptr registered function pointer
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
/*extern int DrvLogInstall(PVOIDFUNC fptr, PVOIDFUNC fptr1, PVOIDFUNC fptr2, PVOIDFUNC fptr3, PVOIDFUNC fptr4);*/
#define DRV_LOG_INSTALL(fptr, fptr1, fptr2, fptr3, fptr4)     DRV_OK

/*****************************************************************************
*Function Name: BSP_MNTN_GetBattState
*
*Function Description: Get the battery in place
*
*Input parameters: None
*Output parameters: None
*
*Return Value: Success: 0, 1, 2
              Failed: Negative
*
*
*****************************************************************************/
extern BSP_S32 BSP_MNTN_GetBattState(void);
#define DRV_MNTN_GET_BATT_STATE() BSP_MNTN_GetBattState()

/*****************************************************************************
 Function name: pwrctrl_wpa_pwr_up
 Function description: RF power down
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void pwrctrl_wpa_pwr_up(void);
#define DRV_WPA_POWERUP()

/*****************************************************************
Function: free_mem_size_get
Description:
    get mem size
Input:
    N/A
Output:
    N/A
Return:
    free mem size
*******************************************************************/
extern unsigned long free_mem_size_get(void);
#define FREE_MEM_SIZE_GET() free_mem_size_get()

/*****************************************************************************
 Function Name: BSP_DMR_ATAnalyze
 Function description: NAS query dynamic memory usage information and query through AT command
 Input parameters: None.
 Output parameters: None.
 Return Value: 0 for success, -1 for failure
*****************************************************************************/
extern int BSP_DMR_ATANALYZE(void * pstMem,unsigned int uiSize, unsigned int * puiModIdSum);

/*************************MNTN END************************************/

/*************************DPM START***********************************/

#define PWRCTRL_GET_SUCCESS         (0x00)
#define PWRCTRL_GET_PARAINVALID     (0x01)
#define PWRCTRL_ONORLOCK            (0x10)
#define PWRCTRL_OFFORUNLOCK         (0x20)

typedef unsigned long (*pULULFUNCPTR)( unsigned long ulPara );

typedef struct
{
    pULULFUNCPTR pRestoreFunction;
    pULULFUNCPTR pEnableFunction;
    pULULFUNCPTR pDisableFunction;
    pFUNCPTR2    pDfsSetFunc;
    pULULFUNCPTR pDfsReleaseFunc;
}DPM_SLEEP_CALLBACK_STRU;

/*Wang Wei's way*/
typedef  enum
{
    PWRCTRL_DFS_USB = 0,
    PWRCTRL_DFS_SD  = 1,
    PWRCTRL_DFS_PS1 = 2,
    PWRCTRL_DFS_HIFI = 3,  /*Hifi*/
    PWRCTRL_DFS_BUTT
} PWC_DFS_ID_E;

typedef enum
{
    DFS_PROFILE_0 = 0,            /*Frequency level 1*/
    DFS_PROFILE_1 = 1,            /*Frequency level 2*/
    DFS_PROFILE_2 = 2,            /*Frequency level 3*/
    DFS_PROFILE_3 = 3,            /*Frequency level 4*/
    DFS_PROFILE_4 = 4,            /*Frequency level 5*/
    DFS_PROFILE_5 = 5,
    DFS_PROFILE_6 = 6,
    DFS_PROFILE_7 = 7,
    DFS_PROFILE_8 = 8,
    DFS_PROFILE_BUTT
} PWC_DFS_PROFILE_E;


typedef enum tagPWC_COMM_MODE_E
{
    PWC_COMM_MODE_WCDMA,
    PWC_COMM_MODE_GSM,
    PWC_COMM_MODE_LTE,
    PWC_COMM_MODE_TDS,
    PWC_COMM_MODE_BUTT
}PWC_COMM_MODE_E;

typedef enum tagPWC_COMM_MODULE_E
{
    PWC_COMM_MODULE_PA,
    PWC_COMM_MODULE_RF,
    PWC_COMM_MODULE_BBP_SOC,
    PWC_COMM_MODULE_BBP_PWR,
    PWC_COMM_MODULE_DSP,
    PWC_COMM_MODULE_ABB,
    PWC_COMM_MODULE_HIFI,
    PWC_COMM_MODULE_BUTT
}PWC_COMM_MODULE_E;

/* Sleep voting id */
typedef enum tagPWC_CLIENT_ID_E
{
    PWRCTRL_SLEEP_SCI       = 0x100,
    PWRCTRL_SLEEP_PS        = 0x101,
    PWRCTRL_SLEEP_BBPMASTER = 0x102,
    PWRCTRL_SLEEP_CICOM     = 0x103,
    PWRCTRL_SLEEP_NV        = 0x104,
	PWRCTRL_SLEEP_RNIC      = 0x105,
	PWRCTRL_SLEEP_APP		= 0x106,
	PWRCTRL_SLEEP_TDSPS     = 0x107,
    PWRCTRL_SLEEP_DMA       = 0x108,
    PWRCTRL_SLEEP_FLASH     = 0x109,
    PWRCTRL_SLEEP_SD        = 0x10A,
    PWRCTRL_SLEEP_FTM       = 0x10B,
    PWRCTRL_SLEEP_OAM       = 0x10C,
    PWRCTRL_SLEEP_SOCP      = 0x10D,
    PWRCTRL_SLEEP_SPECIALTIMER        = 0x10E,	
    PWRCTRL_SLEEP_GUDSP_EDMA = 0x10E, 

    PWRCTRL_SLEEP_BOOT      = 0x110,
    PWRCTRL_SLEEP_ACPU      = 0x111,
    PWRCTRL_SLEEP_HSIC      = 0x112,

    PWRCTRL_LIGHTSLEEP_WIFI = 0x113, /* Hsic can't lose power */
    PWRCTRL_SLEEP_WIFI      = 0x114,
    PWRCTRL_SLEEP_USB       = 0x115,
    PWRCTRL_SLEEP_LCD       = 0x116,
    PWRCTRL_SLEEP_KEY     	= 0x117,
    PWRCTRL_SLEEP_ACM     	= 0x118,
    PWRCTRL_SLEEP_ICC     	= 0x119,
    PWRCTRL_SLEEP_NAS     	= 0x11a,
    PWRCTRL_SLEEP_LTEPS     = 0x11b,
    PWRCTRL_SLEEP_IFC		= 0x11c,
	PWRCTRL_SLEEP_CHG		= 0x11d,
    /* Test */
    PWRCTRL_TEST_LIGHTSLEEP = 0x11E,
    PWRCTRL_TEST_DEEPSLEEP  = 0x11F,
    PWRCTRL_CLIENT_BUTT     = 0x120
} PWC_CLIENT_ID_E;

typedef struct
{
    PWRCTRLFUNCPTRVOID  pFuncDsIn;
    PWRCTRLFUNCPTRVOID  pFuncDsOut;
}PWC_DS_SOCP_CB_STRU;

typedef enum tagPWC_TEMP_PROTECT_E
{
    PWC_TEMP_OLED,
    PWC_TEMP_BATTERY,
	PWC_TEMP_L_PA,
    PWC_TEMP_G_PA,
    PWC_TEMP_W_PA,
    PWC_TEMP_SIM_CARD,
    PWC_TEMP_DCXO,
    /* BEGIN PN:N/A,Modified by c00191475, 2013/01/08*/
    PWC_TEMP_SURFACE,/*Surface enumeration quantity definition*/
    /* END   PN:N/A,Modified by c00191475, 2013/01/08*/
    PWC_TEMP_BUTT
}PWC_TEMP_PROTECT_E;

typedef struct
{
    PWRCTRLFUNCPTR drx_slow;
} PWC_SLEEP_CALLBACK_STRU;


typedef enum tagPWRCTRL_MODULE_E
{
    PWRCTRL_MODU_BOOTROM,       /*0x0*/
    PWRCTRL_MODU_EFUSE,
    PWRCTRL_MODU_SSDMA,
    PWRCTRL_MODU_LBBP,
    PWRCTRL_MODU_LDSP,    	
    PWRCTRL_MODU_HSUART,
    PWRCTRL_MODU_EMI,  
    PWRCTRL_MODU_SDCC,
    PWRCTRL_MODU_USBHSIC,
    PWRCTRL_MODU_SOCP,
    PWRCTRL_MODU_GMAC,          /* 0 */
    PWRCTRL_MODU_SCI0,
    PWRCTRL_MODU_SCI1,
    PWRCTRL_MODU_UICC,
    PWRCTRL_MODU_USBOTG_BC,
    PWRCTRL_MODU_MMC0,              /*0x10*/
    PWRCTRL_MODU_MMC1,
    PWRCTRL_MODU_MMC2,
    PWRCTRL_MODU_USBOTG,
    PWRCTRL_MODU_WDT,
    PWRCTRL_MODU_TCSSI,
    PWRCTRL_MODU_IPF,
    PWRCTRL_MODU_CIPHER,
    PWRCTRL_MODU_BBPCOMM,
    PWRCTRL_MODU_TENSI,             /*0x1a*/
    PWRCTRL_MODU_CORESIGHT,
    PWRCTRL_MODU_ACP,
    PWRCTRL_MODU_I2C,
    PWRCTRL_MODU_SPI1,
    PWRCTRL_MODU_DMAC,
    PWRCTRL_MODU_LSIO,                  /*0x20*/
    PWRCTRL_MODU_HIFI,
    PWRCTRL_MODU_TSENSOR,
    PWRCTRL_MODU_CICOM,
    PWRCTRL_MODU_EDMA,
    PWRCTRL_MODU_HDLC,
    PWRCTRL_MODU_DM,
    PWRCTRL_MODU_DWSSI0,
    PWRCTRL_MODU_WGBBP,
    PWRCTRL_MODU_ZSP,
    PWRCTRL_MODU_NANDC,
    PWRCTRL_MODU_SSI0,
    PWRCTRL_MODU_SSI1,
    PWRCTRL_MODU_SSI2,
    PWRCTRL_MODU_SSI3,
    PWRCTRL_MODU_SPI0,
    PWRCTRL_MODU_MST,
    PWRCTRL_MODU_ALL,   /* Represents all modules */
    PWRCTRL_MODU_MAX    /* Boundary value */
}PWRCTRL_MODULE_E;
/************************************************************************
 *FUNCTION
 *PWRCTRL_BSP_GuDsp_StatusInfo
 *DESCRIPTION
 *Used to query GUDSP_ZSPcore reset, dereset time and number of times
 *INPUTS
 *GuZspAddr: GU_ZSP pointer address for saving reset and dereset information
 *OUTPUTS
           BSP_ERROR: The pointer is illegal.
 *************************************************************************/
 BSP_U32 PWRCTRL_BSP_GuDsp_StatusInfo(BSP_U32 *GuZspAddr);

/************************************************************************
 *FUNCTION
 *PWRCTRL_BSP_RTT_StatusInfo
 *DESCRIPTION
 *BBP status query
 *INPUTS
 *NONE
 *OUTPUTS
          BSP_BBP_ERROR: Not accessible
          BSP_BBP_OK: accessible
 *************************************************************************/
BSP_U32  PWRCTRL_BSP_RTT_StatusInfo (BSP_VOID);


/*****************************************************************************
*Function Name: BSP_PWRCTRL_TimerClose
*
*Function description: SOC peripheral clock shutdown interface
*
*Input parameters: 
*Output parameters: 
*Return value:
*
*Modify record: 

*****************************************************************************/
BSP_S32 BSP_PWRCTRL_TimerClose(PWRCTRL_MODULE_E enModu);

/*****************************************************************************
*Function Name: BSP_PWRCTRL_TimerOpen
*
*Function description: SOC peripheral clock enable interface
*
*Input parameters: 
*Output parameters: 
*Return value:
*
*Modify record: 

*****************************************************************************/
BSP_S32 BSP_PWRCTRL_TimerOpen(PWRCTRL_MODULE_E enModu);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_DEEPSLEEP_FOREVER
 Function description: AARM CARM power down interface
 Input parameters: None
 Output parameters: None
 Return Value: None

*****************************************************************************/
extern void DRV_PWRCTRL_DEEPSLEEP_FOREVER(void );

/*****************************************************************************
 Function Name: DRV_PWRCTRL_SLEEP_CALLBACK_REG
 Function description: Register PWC function for soft use
 Input parameters: Function to be registered
 Output parameters: None
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int DRV_PWRCTRL_SLEEP_CALLBACK_REG(PWC_SLEEP_CALLBACK_STRU pCallback);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_GET_TEMP
 Function Description: Get the temperature of OLED, battery, GPA, WPA, SIM card, DCXO.
 Input parameters: enModule:PWC_TEMP_PROTECT_E
             hkAdcTable:Voltage Temperature Conversion Table
 Output parameters: pslData: The temperature value converted to the stored voltage
 Return Value: HKADC_OK/HKADC_ERROR.
*****************************************************************************/
/*V3R2 CS f00164371, this interface is not isolated for the time being, because the SFT platform returns 0, and the actual return of the film needs to be implemented.
CS_SFT macros need to be used to distinguish them, but the CS_SFT macros of different components are not necessarily unified, so they cannot be blocked directly.
采用在Internal isolation of interface code*/
extern BSP_S32 DRV_PWRCTRL_GET_TEMP(PWC_TEMP_PROTECT_E enModule, BSP_U16 *hkAdcTable,BSP_S32 *pslData);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_AHB_BAK_ADDR_ALLOC
 Function description: The arm power-up and down function requires backup of AHB data. This interface applies for data storage address and length
 Input parameters: length Pointer to save the size of the application space
 Output parameters: None
 Return Value: The address of the application space.
*****************************************************************************/
/*V3R2 CS f00164371, this interface is not isolated for the time being*/
extern void * DRV_PWRCTRL_AHB_BAK_ADDR_ALLOC(unsigned int * length);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_GetCcpuLoadCB
 Function description: arm The callback function provided to TTF
 Input parameters: pFunc:TTF function pointer
 Output parameters: None
 Return Value: The address of the application space.
*****************************************************************************/
extern void DRV_PWRCTRL_GetCcpuLoadCB(PWRCTRLFUNCPTR pFunc );

/*****************************************************************************
 Function Name: DRV_PWRCTRL_GetCpuLoad
 Function description: arm cpu occupancy query function
 Input parameters: None
 Output parameters: ulAcpuLoad:Acpu occupancy address.
             ulCcpuLoad:Ccpu occupancy address.
 Return value: 0/1.
*****************************************************************************/
extern unsigned int DRV_PWRCTRL_GetCpuLoad(unsigned int *ulAcpuLoad,unsigned int *ulCcpuLoad);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_PWRUP
 Function description: Power-on control of PA, RF, BBP, DSP, ABB, and HIFI under W mode, G mode, and L mode.
 Input parameters: enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
/*V3R2 CS f00164371, this interface is not isolated for the time being, because HIFI loading requires it, it is isolated inside the function.*/
extern BSP_U32 DRV_PWRCTRL_PWRUP (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_PWRDOWN
 Function description: Power-down control of PA, RF, BBP, DSP, ABB, and HIFI under W mode, G mode, L mode.
 Input parameters: enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
/*V3R2 CS f00164371, this interface is not isolated for the time being, because HIFI loading requires it, it is isolated inside the function.*/
extern BSP_U32 DRV_PWRCTRL_PWRDOWN (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule);

/*****************************************************************************
 Function Name: BSP_PWRCTRL_PwrStatusGet
 Function description: Up-down power status query for PA, RF, BBP, DSP, and ABB under W mode, G mode, and L mode.
 Input parameters: enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 Output parameters: None
 Return Value: PWRCTRL_GET_SUCCESS/PWRCTRL_GET_PARAINVALID/PWRCTRL_ONORLOCK/PWRCTRL_OFFORUNLOCK
*****************************************************************************/
extern  BSP_U32 DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_PLL_ENABLE
 Function description: PLL enables PA, DSP, and ABB in W mode, G mode, and L mode.
 Input parameters: enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_PLL_ENABLE (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_PLL_DISABLE
 Function description: PLL of PA, DSP, and ABB under W mode, G mode, and L mode is enabled.
 Input parameters: enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_PLL_DISABLE (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_DSP_RESET
 Function description: DSP reset interface in L-mode.
 Input parameters: 
 Output parameters: None
 Return Value: 
*****************************************************************************/
extern BSP_VOID DRV_PWRCTRL_DSP_RESET();

/*****************************************************************************
 Function Name: BSP_PWRCTRL_PllStatusGet
 Function description: PLL status query of PA, DSP, and ABB under W mode, G mode, and L mode.
 Input parameters: enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 Output parameters: None
 Return Value: PWRCTRL_GET_SUCCESS/PWRCTRL_GET_PARAINVALID/PWRCTRL_ONORLOCK/PWRCTRL_OFFORUNLOCK
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_PllStatusGet (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule);

/************************************************************************
 *FUNCTION
 *BSP_PWRCTRL_ZspSleepFlagStore
 *DESCRIPTION
 *ZSP sleep flag save interface
 *INPUTS
 *       
 *OUTPUTS
 *NONE
 *************************************************************************/
extern BSP_VOID DRV_ZSP_SLEEP_FALG_STORE(BSP_VOID);

/************************************************************************
 *FUNCTION
 *BSP_PWRCTRL_ZspSleepFlagRead
 *DESCRIPTION
 *ZSP sleep flag read interface
 *INPUTS
 *       
 *OUTPUTS
 *NONE
 *************************************************************************/
extern BSP_U32 DRV_ZSP_SLEEP_FALG_READ(BSP_VOID);


/*****************************************************************************
 Function Name: DRV_PWRCTRL_SLEEPVOTE_LOCK
 Function description: The peripheral prohibits sleep voting interface.
 Input parameters: enClientId:PWC_CLIENT_ID_E
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_SLEEPVOTE_LOCK(PWC_CLIENT_ID_E enClientId);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_SLEEPVOTE_UNLOCK
 Function Description: Peripheral allows sleep voting interface.
 Input parameters: enClientId:PWC_CLIENT_ID_E
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWC_CLIENT_ID_E enClientId);

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_SET_PRF_CCPU
Description:Set the System Min Profile
Input:      ulClientID: The Vote Module Client
            enProfile:  The Min Profile Value
Output:     None
Return:     The Vote Result
Others:
*****************************************************************************/
extern BSP_U32  DRV_PWRCTRL_DFS_SET_PRF_CCPU(PWC_DFS_ID_E ulClientID, PWC_DFS_PROFILE_E enProfile);

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_RLS_PRF_CCPU
Description:Release the Vote Result
Input:      ulClientID: The Vote Module Client
Output:     None;
Return:     The Vote Result
Others:
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_DFS_RLS_PRF_CCPU(PWC_DFS_ID_E ulClientID);

/*****************************************************************************
Function:   DRV_PWRCTRL_SLEEP_IN_CB
Description:
Input:
Output:     None;
Return:
Others:
*****************************************************************************/
extern int DRV_PWRCTRL_SLEEP_IN_CB(PWC_DS_SOCP_CB_STRU stFunc);

/*****************************************************************************
 Function Name: BSP_PWRCTRL_UsbLowPowerEnter
 Function description: USB enters the low-power interface
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void DRV_PWRCTRL_USB_LOWPOWER_ENTER(void);

/*****************************************************************************
 Function Name: DRV_PWRCTRL_USB_LOWPOWER_EXIT
 Function Description: USB exits the low-power interface
 Input parameters: None
 Output parameters: None
 Return Value: None
*****************************************************************************/
extern void DRV_PWRCTRL_USB_LOWPOWER_EXIT(void);


/*****************************************************************************
 Function Name: BSP_32K_GetTick
 Function description: 32K clock corresponds to Tick query interface
 Input parameters: None
 Output parameters: None
 Return Value: 32K clock corresponding to Tick value
*****************************************************************************/
extern BSP_U32 BSP_32K_GetTick( BSP_VOID );


/*****************************************************************************
Function:   BSP_PWC_GetMeansFlag
Description:
Input:
Output:     the means flag value;
Return:
Others:
*****************************************************************************/
extern BSP_S32 BSP_PWC_GetMeansFlag(PWC_COMM_MODE_E enCommMode);
#define DRV_PWC_GET_MEANSFLAG(enCommMode) BSP_PWC_GetMeansFlag(enCommMode)

extern BSP_VOID BSP_PWC_MspVoteRegister(FUNCPTR routine);
extern BSP_VOID BSP_PWC_SocpVoteRegister(FUNCPTR routine);
extern BSP_VOID BSP_PWC_SocpRestoreRegister(FUNCPTR routine);

/*****************************************************************************
 Function: BSP_PWC_SetTimer4WakeSrc
 Description: Set timer4 as wakeup source
 Input: 
 Return : void
 Other: 
*****************************************************************************/
extern VOID BSP_PWC_SetTimer4WakeSrc(VOID);

/*****************************************************************************
 Function: BSP_PWC_DelTimer4WakeSrc
 Description: Setting timer4 is not used as a wake-up source 
 Input:  
            : 
 Return : void
 Other: 
*****************************************************************************/
extern VOID BSP_PWC_DelTimer4WakeSrc(VOID);


/*****************************************************************************
 Function Name: BSP_PWRCTRL_StandbyStateCcpu/BSP_PWRCTRL_StandbyStateAcpu
 Function description: AT^PSTANDBY
 Input parameters:
 Output parameters:
 Return value:
*****************************************************************************/
extern unsigned int BSP_PWRCTRL_StandbyStateAcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime);
extern unsigned int BSP_PWRCTRL_StandbyStateCcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime);
#define DRV_PWRCTRL_STANDBYSTATEACPU(ulStandbyTime, ulSwitchTime)	BSP_PWRCTRL_StandbyStateAcpu(ulStandbyTime, ulSwitchTime)
#define DRV_PWRCTRL_STANDBYSTATECCPU(ulStandbyTime, ulSwitchTime)	BSP_PWRCTRL_StandbyStateCcpu(ulStandbyTime, ulSwitchTime)



/*************************DPM END*************************************/

/*************************ABB START***********************************/

/*****************************************************************************
 Function Name: BSP_ABB_RegGet
 Function Description: Get the ABB register value
 Input parameters: usInstruction: Register address to be read
             uspData: Store register value
 Output parameters:
 Return Value:
*****************************************************************************/
extern int BSP_ABB_RegGet(unsigned char usInstruction, unsigned char *uspData);
#define DRV_ABB_REG_VALUE_GET(usInstruction,uspData)    BSP_ABB_RegGet (usInstruction,uspData)

/*************************ABB END*************************************/

/*************************SYSCTRL START*******************************/
/*****************************************************************************
 Function Name: BSP_GUSYS_GuDspCountInfo
 Function description: This interface implements the dereset operation of ZSP module, including zsp BRG and bus
 Input parameters: *GuDspAddr is used to save counting information
 Output parameters: None.
 Return Value: BSP_ERROR: The address is illegal
                       BSP_OK: The address is legal
         Xu Jingcui x00221564 2012.11.20
*****************************************************************************/
extern unsigned int BSP_GUSYS_GuDspCountInfo(unsigned int *GuDspAddr);
#define DRV_DSP_RESET_GET_INFO(GuDspAddr)    BSP_GUSYS_GuDspCountInfo (GuDspAddr)

/*****************************************************************************
 Function Name: BSP_GUSYS_DspReset
 Function description: This interface implements reset operations of ZSP module, including zsp core, bus and peripherals.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_DspReset(void);
#define DRV_DSP_ZONE_RESET()    BSP_GUSYS_DspReset()

/*****************************************************************************
 Function Name: BSP_GUSYS_DspResetCancel
 Function description: This interface implements the dereset operation of the ZSP module, including zsp core, bus and peripherals.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_DspResetCancel(void);
#define DRV_DSP_ZONE_RESET_CANCEL()    BSP_GUSYS_DspResetCancel()

/*****************************************************************************
 Function Name: BSP_GUSYS_ABBGSMRxCtrl
 Function Description: This interface implements ABB GSM reception control.
 Input parameters: ucStatus: 0: Close; 1: Open.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern unsigned int BSP_GUSYS_ABBGSMRxCtrl(unsigned char ucStatus);
#define DRV_ABB_GRX_CTRL(ucStatus)    BSP_GUSYS_ABBGSMRxCtrl(ucStatus)

/*****************************************************************************
 Function Name: ABBWCDMARxCtrl
 Function Description: This interface implements ABB WCDMA reception control.
 Input parameters: ucStatus: 0: Close; 1: Open.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern unsigned int BSP_GUSYS_ABBWCDMARxCtrl(unsigned char ucStatus);
#define DRV_ABB_WRX_CTRL(ucStatus)    BSP_GUSYS_ABBWCDMARxCtrl(ucStatus)

/*****************************************************************************
 Function Name: ABBTxCtrl
 Function Description: This interface implements ABB transmission control.
 Input parameters: ucStatus: 0: Close; 1: Open.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
 Note: ABB sending control does not distinguish between W/G modes.
*****************************************************************************/
extern int BSP_GUSYS_ABBTxCtrl(unsigned char ucStatus);
#define DRV_ABB_TX_CTRL(ucStatus)    BSP_GUSYS_ABBTxCtrl(ucStatus)

/*****************************************************************************
 Function Name: BSP_GUSYS_WcdmaBbpPllEnable
 Function Description: This interface implements the WCDMA BBP PLL to enable.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_WcdmaBbpPllEnable(void);
#define DRV_WBBP_PLL_ENABLE()    BSP_GUSYS_WcdmaBbpPllEnable()

/*****************************************************************************
 Function Name: BSP_GUSYS_WcdmaBbpPllDisable
 Function Description: This interface implements the shutdown of WCDMA BBP PLL.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_WcdmaBbpPllDisable(void);
#define DRV_WBBP_PLL_DSABLE()    BSP_GUSYS_WcdmaBbpPllDisable()

/*****************************************************************************
 Function Name: BSP_GUSYS_GsmBbpPllEnable
 Function Description: This interface implements the enablement of GSM BBP PLL.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_GsmBbpPllEnable(void);
#define DRV_GBBP_PLL_ENABLE()    BSP_GUSYS_GsmBbpPllEnable()

/*****************************************************************************
 Function Name: BSP_GUSYS_GsmBbpPllDisable
 Function Description: This interface implements the shutdown of GSM BBP PLL.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_GsmBbpPllDisable(void);
#define DRV_GBBP_PLL_DISABLE()    BSP_GUSYS_GsmBbpPllDisable()

/*****************************************************************************
 Function Name: BSP_GUSYS_RFLdoOn
 Function description: This interface implements WCDMA and GSM RF LDO power-up.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_RFLdoOn(void);
#define DRV_RF_LDOUP()    BSP_GUSYS_RFLdoOn()

/*****************************************************************************
 Function Name: BSP_GUSYS_RFLdoDown
 Function description: This interface implements power-down of WCDMA and GSM RF LDO.
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_RFLdoDown(void);
#define DRV_RF_LDODOWN()    BSP_GUSYS_RFLdoDown()

/*****************************************************************************
 Function Name: BSP_GUSYS_WcdmaPllStatusGet
 Function Description: Read the stable state of WCDMA BBP PLL.
 Input parameters: None.
 Output parameters: None.
 Return Value:
                     0: Stable
                     1: Not stable
*****************************************************************************/
extern unsigned long BSP_GUSYS_WcdmaPllStatusGet(void);
#define DRV_GET_WCDMA_PLL_STATUS()    BSP_GUSYS_WcdmaPllStatusGet()

/*****************************************************************************
 Function Name: BSP_GUSYS_GsmPllStatusGet
 Function Description: Read the stable state of GSM BBP PLL.
 Input parameters: None.
 Output parameters: None.
 Return Value:
                     0: Stable
                     1: Not stable
*****************************************************************************/
extern unsigned long BSP_GUSYS_GsmPllStatusGet(void);
#define DRV_GET_GSM_PLL_STATUS()    BSP_GUSYS_GsmPllStatusGet()

/*****************************************************************************
 Function Name: BSP_GUSYS_DspPllStatusGet
 Function Description: Read the DSP PLL stable state.
 Input parameters: None.
 Output parameters: None.
 Return Value:
                     0: Stable
                     1: Not stable
*****************************************************************************/
extern unsigned long BSP_GUSYS_DspPllStatusGet(void);
#define DRV_GET_DSP_PLL_STATUS()    BSP_GUSYS_DspPllStatusGet()


/*****************************************************************************
 Function Name: BSP_GUSYS_ArmPllStatusGet
 Function Description: Read the stable state of the ARM PLL.
 Input parameters: None.
 Output parameters: None.
 Return Value:
                     0: Stable
                     1: Not stable
*****************************************************************************/
extern unsigned int BSP_GUSYS_ArmPllStatusGet(void);
#define DRV_GET_ARM_PLL_STATUS()    BSP_GUSYS_ArmPllStatusGet()

/*****************************************************************************
 Function Name: BSP_GUSYS_BBPAutoAdjust
 Function Description: Initialize the self-calibration enable of ABB
 Input parameters: uiSysMode: 0: WCDMA mode; 1: GSM mode.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_GUSYS_BBPAutoAdjust(unsigned char ucAdjustMode);
#define DRV_BBP_AUTO_ADJUST(ucAdjustMode)    BSP_GUSYS_BBPAutoAdjust(ucAdjustMode)

/*****************************************************************************
 Function Name: BSP_GUSYS_DspPllEnable
 Function Description: Open DSP PLL
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_DspPllEnable(void);
#define DRV_DSP_PLL_ENABLE()    BSP_GUSYS_DspPllEnable()

/*****************************************************************************
 Function Name: BSP_GUSYS_DspPllDisable
 Function Description: Turn off DSP PLL
 Input parameters: None.
 Output parameters: None.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUSYS_DspPllDisable(void);
#define DRV_DSP_PLL_DISABLE()    BSP_GUSYS_DspPllDisable()


#define WCDMA_MODE      0
#define GSM_MODE        1
#define LTE_MODE        2
#define TDS_MODE        3

/*****************************************************************************
 Function Name: BSP_GUSYS_ModeSwitchs
 Function description: This interface implements WCDMA/GSM system mode switching.
 Input parameters: uiSysMode: 0: Switch to WCDMA mode; 1: Switch to GSM mode.
 Output parameters: None.
 Return Value: 0: The operation was successful;
             -1: The operation failed.
*****************************************************************************/
extern int BSP_GUSYS_ModeSwitch(unsigned int uiSysMode);
#define DRV_SYSMODE_SWITCH(uiSysMode)    BSP_GUSYS_ModeSwitch(uiSysMode)

/*************************SYSCTRL START*******************************/

/*************************GUDSP Loading START***************************/

/*****************************************************************************
 Function Name: BSP_GUDSP_LayerInfoGet
 Function description: Get physical layer loading information.
 Input parameters: None.
 Output parameters: pulAddr: cache for storing the physical layer loading information.
 Return Value: None.
*****************************************************************************/
extern void BSP_GUDSP_LayerInfoGet(unsigned long* pulAddr, unsigned long* pulLength);
#define DRV_PHY_SLOAD_INFO_GET(pulAddr,pulLength)    BSP_GUDSP_LayerInfoGet(pulAddr,pulLength)

/*****************************************************************************
 Function Name: BSP_GUDSP_CommonInfoGet
 Function description: Obtain the physical layer BSS COMMON segment information.
 Input parameters: None.
 Output parameters: pulAddr: cache for storing the physical layer loading information.
 Return Value: 0: Success, -1: Failed.
*****************************************************************************/
extern int BSP_GUDSP_CommonInfoGet(unsigned long* pulAddr, unsigned long* pulLength);
#define DRV_DSP_COMMON_INFO_GET(pulAddr,pulLength)    BSP_GUDSP_CommonInfoGet(pulAddr,pulLength)

/*****************************************************************************
 Function Name: BSP_GUDSP_Load
 Function Description: Complete the GU ZSP loading function: Load the ZSP static segment from DDR to ZSP TCM.
 Input parameters: None.
 Output parameters: None.
 Return Value: 0: Success, -1: Failed.
*****************************************************************************/
extern int BSP_GUDSP_Load(void);
#define DRV_GUDSP_LOAD()    BSP_GUDSP_Load()

/*************************GUDSP Loading END************************************/

/*************************EDMA START**********************************/

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
typedef void (*channel_isr)(BSP_U32 channel_arg, BSP_U32 int_status);

/*Define peripheral request number*/
typedef enum _BALONG_DMA_REQ
{
    EDMA_DWSSI0_RX = 0,
    EDMA_DWSSI0_TX,
    EDMA_DWSSI1_RX,
    EDMA_DWSSI1_TX,
    EDMA_HIFISIO_RX,
    EDMA_HIFISIO_TX,
    EDMA_HSSPI_RX,
    EDMA_HSSPI_TX,
    EDMA_HSUART_RX,
    EDMA_HSUART_TX,
    EDMA_UART0_RX,
    EDMA_UART0_TX,
    EDMA_UART1_RX,
    EDMA_UART1_TX,
    EDMA_UART2_RX,
    EDMA_UART2_TX,
    EDMA_SCI_RX,
    EDMA_SCI_TX,
    EDMA_UART3_RX,
    EDMA_UART3_TX,
    EDMA_MMC0,     /*20*/
    EDMA_MMC1,
    EDMA_MMC2,
    EDMA_MEMORY,    /*memory to memory, virtual peripheral request line*/
    EDMA_REQ_MAX    /*If the device request is not less than this value, it is an illegal request*/
} BALONG_DMA_REQ;

/* The parameter int_status of function void (*channel_isr)(BSP_U32 channel_arg, BSP_U32 int_status) is int_status,
    function int balong_dma_channel_init (BALONG_DMA_REQ req, channel_isr pFunc,
                     UINT32 channel_arg, UINT32 int_flag) parameter int_flag
    为以下Several interrupt types, can be combined */
#define BALONG_DMA_INT_DONE           1          /*Dma transmission completion interrupt*/
#define BALONG_DMA_INT_LLT_DONE       2          /*Chain DMA node transmission complete interrupt*/
#define BALONG_DMA_INT_CONFIG_ERR     4          /*Interrupts caused by Dma configuration errors*/
#define BALONG_DMA_INT_TRANSFER_ERR   8          /*Interrupt caused by Dma transmission error*/
#define BALONG_DMA_INT_READ_ERR       16         /*Interrupts caused by Dma linked list read error*/

/* Edmac transmission direction definition*/
#define BALONG_DMA_P2M      1
#define BALONG_DMA_M2P      2
#define BALONG_DMA_M2M      3

/* Edmac flow control and transmission types*/
typedef enum tagEDMA_TRANS_TYPE
{
    MEM_MEM_DMA = 0x00,    /* Memory to memory, dma flow control*/
    MEM_PRF_DMA = 0x01,        /* Memory and peripherals, dma flow control*/
    MEM_PRF_PRF = 0x10        /* Memory and peripherals, peripheral flow control*/
} EDMA_TRANS_TYPE;

/* Channel status */

#define   EDMA_CHN_FREE          1   /* Idle channel */
#define   EDMA_CHN_BUSY          0   /* Busy channel */


/* Edma transmission bit width, source and destination addresses are constrained to the same value */
#define   EDMA_TRANS_WIDTH_8       0x0   /* 8 bit width*/
#define   EDMA_TRANS_WIDTH_16      0x1   /* 16 bit width*/
#define   EDMA_TRANS_WIDTH_32      0x2   /* 32bit bit width*/
#define   EDMA_TRANS_WIDTH_64      0x3   /* 64bit bit width*/

/*  EDMA burst length, the value range is 0~15, and the burn length represented is 1~16*/
#define   EDMA_BUR_LEN_1    0x0    /* Burst length, that is, the number of transmissions is 1*/
#define   EDMA_BUR_LEN_2    0x1    /* Burst length, that is, the number of transmissions is 2*/
#define   EDMA_BUR_LEN_3    0x2   /* Burst length, that is, the number of transmissions is 3*/
#define   EDMA_BUR_LEN_4    0x3   /* Burst length, that is, the number of transmissions is 4*/
#define   EDMA_BUR_LEN_5    0x4   /* Burst length, that is, the number of transmissions is 5*/
#define   EDMA_BUR_LEN_6    0x5   /* Burst length, that is, the number of transmissions is 6*/
#define   EDMA_BUR_LEN_7    0x6   /* Burst length, that is, the number of transmissions is 7*/
#define   EDMA_BUR_LEN_8    0x7   /* Burst length, that is, the number of transmissions is 8*/
#define   EDMA_BUR_LEN_9    0x8   /* Burst length, that is, the number of transmissions is 9*/
#define   EDMA_BUR_LEN_10   0x9   /* Burst length, that is, the number of transmissions is 10*/
#define   EDMA_BUR_LEN_11   0xa   /* Burst length, that is, the number of transmissions is 11*/
#define   EDMA_BUR_LEN_12   0xb   /* Burst length, that is, the number of transmissions is 12*/
#define   EDMA_BUR_LEN_13   0xc   /* Burst length, that is, the number of transmissions is 13*/
#define   EDMA_BUR_LEN_14   0xd   /* Burst length, that is, the number of transmissions is 14*/
#define   EDMA_BUR_LEN_15   0xe   /* Burst length, that is, the number of transmissions is 15*/
#define   EDMA_BUR_LEN_16   0xf   /* Burst length, that is, the number of transmissions is 16*/


/* The specific bits corresponding to EDMA are used for EDMA register configuration macros
       EDMAC_BASIC_CONFIG, BALONG_DMA_SET_LLI, BALONG_DMA_SET_CONFIG Usage*/
#define EDMAC_TRANSFER_CONFIG_SOUR_INC      (0X80000000)
#define EDMAC_TRANSFER_CONFIG_DEST_INC      (0X40000000)

#define EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH( _len )  ((BSP_U32)((_len)<<24))
#define EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH( _len )  ((BSP_U32)((_len)<<20))

#define EDMAC_TRANSFER_CONFIG_SOUR_WIDTH( _len )  ((BSP_U32)((_len)<<16))
#define EDMAC_TRANSFER_CONFIG_DEST_WIDTH( _len )  ((BSP_U32)((_len)<<12))

#define EDMAC_TRANSFER_CONFIG_REQUEST( _ulReg )    ( (_ulReg ) << 4)
#define EDMAC_TRANSFER_CONFIG_FLOW_DMAC( _len )    ((BSP_U32)((_len)<<2))

#define EDMAC_TRANSFER_CONFIG_INT_TC_ENABLE            ( 0x2 )
#define EDMAC_TRANSFER_CONFIG_INT_TC_DISABLE           ( 0x0 )

#define EDMAC_TRANSFER_CONFIG_CHANNEL_ENABLE           ( 0x1 )
#define EDMAC_TRANSFER_CONFIG_CHANNEL_DISABLE          ( 0x0 )

#define EDMAC_TRANSFER_CONFIG_EXIT_ADD_MODE_A_SYNC     ( 0UL )
#define EDMAC_TRANSFER_CONFIG_EXIT_ADD_MODE_AB_SYNC    ( 0x00008000 )

#define EDMAC_NEXT_LLI_ENABLE       0x2           /* Bit 1 */

/*Node information during chain transmission*/
typedef struct _BALONG_DMA_CB
{
    volatile BSP_U32 lli;     /*Point to the next lli*/
    volatile BSP_U32 bindx;
    volatile BSP_U32 cindx;
    volatile BSP_U32 cnt1;
    volatile BSP_U32 cnt0;   /*Data length per node for block transmission or LLI transmission <= 65535 bytes*/
    volatile BSP_U32 src_addr; /*Physical address*/
    volatile BSP_U32 des_addr; /*Physical address*/
    volatile BSP_U32 config;
} BALONG_DMA_CB;

#define P2M_CONFIG   (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_PRF_DMA) | EDMAC_TRANSFER_CONFIG_DEST_INC)
#define M2P_CONFIG   (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_PRF_DMA) | EDMAC_TRANSFER_CONFIG_SOUR_INC)
#define M2M_CONFIG   (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_MEM_DMA) | EDMAC_TRANSFER_CONFIG_SOUR_INC | EDMAC_TRANSFER_CONFIG_DEST_INC)

#define EDMAC_BASIC_CONFIG(burst_width, burst_len) \
               ( EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH(burst_len) | EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH(burst_len) \
               | EDMAC_TRANSFER_CONFIG_SOUR_WIDTH(burst_width) | EDMAC_TRANSFER_CONFIG_DEST_WIDTH(burst_width) )

/*Addr: Physical address*/
#define BALONG_DMA_SET_LLI(addr, last)   ((last)?0:(EDMAC_MAKE_LLI_ADDR(addr) | EDMAC_NEXT_LLI_ENABLE))

#define BALONG_DMA_SET_CONFIG(req, direction, burst_width, burst_len) \
                 ( EDMAC_BASIC_CONFIG(burst_width, burst_len) | EDMAC_TRANSFER_CONFIG_REQUEST(req) \
                 | EDMAC_TRANSFER_CONFIG_INT_TC_ENABLE | EDMAC_TRANSFER_CONFIG_CHANNEL_ENABLE \
                 | ((direction == BALONG_DMA_M2M)?M2M_CONFIG:((direction == BALONG_DMA_P2M)?P2M_CONFIG:M2P_CONFIG)))

/**************************************************************************
  Macro definition
**************************************************************************/

/*Error code definition*/
#define DMAC_SUCCESS                       BSP_OK
#define DMA_FAIL                          BSP_ERROR

#define DMA_ERROR_BASE                          -100
#define DMA_CHANNEL_INVALID                     (DMA_ERROR_BASE-1)
#define DMA_TRXFERSIZE_INVALID                  (DMA_ERROR_BASE-2)
#define DMA_SOURCE_ADDRESS_INVALID              (DMA_ERROR_BASE-3)
#define DMA_DESTINATION_ADDRESS_INVALID         (DMA_ERROR_BASE-4)
#define DMA_MEMORY_ADDRESS_INVALID              (DMA_ERROR_BASE-5)
#define DMA_PERIPHERAL_ID_INVALID               (DMA_ERROR_BASE-6)
#define DMA_DIRECTION_ERROR                     (DMA_ERROR_BASE-7)
#define DMA_TRXFER_ERROR                        (DMA_ERROR_BASE-8)
#define DMA_LLIHEAD_ERROR                       (DMA_ERROR_BASE-9)
#define DMA_SWIDTH_ERROR                        (DMA_ERROR_BASE-0xa)
#define DMA_LLI_ADDRESS_INVALID                 (DMA_ERROR_BASE-0xb)
#define DMA_TRANS_CONTROL_INVALID               (DMA_ERROR_BASE-0xc)
#define DMA_MEMORY_ALLOCATE_ERROR               (DMA_ERROR_BASE-0xd)
#define DMA_NOT_FINISHED                        (DMA_ERROR_BASE-0xe)
#define DMA_CONFIG_ERROR                        (DMA_ERROR_BASE-0xf)


/*******************************************************************************
  Function name: BSP_S32 balong_dma_init(void)
  Function description: DMA initialization program, hook interrupt
  Input parameters: None
  Output parameters: None
  Return value: 0
*******************************************************************************/
extern BSP_S32 balong_dma_init(void);

/*******************************************************************************
  Function name: int balong_dma_current_transfer_address(UINT32 channel_id)
  Function description: Obtain the memory address currently transmitted by a certain channel
  Input parameters: channel_id: channel ID, call the return value of balong_dma_channel_init function
  Output parameters: None
  Return value: Success: The memory address currently transmitted by the channel
                Failed: Negative
*******************************************************************************/
extern int balong_dma_current_transfer_address(BSP_U32 channel_id);
#define DRV_EDMA_CURR_TRANS_ADDR(channel_id)  balong_dma_current_transfer_address(channel_id)

/*******************************************************************************
  Function name: int balong_dma_channel_stop(UINT32 channel_id)
  Function Description: Stop the specified DMA channel
  Input parameters: channel_id: channel ID, call the return value of balong_dma_channel_init function
  Output parameters: None
  Return value: Success: The memory address currently transmitted by the channel
                Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_stop(BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_STOP(channel_id)  balong_dma_channel_stop(channel_id)


/*******************************************************************************
  Function name: BALONG_DMA_CB *balong_dma_channel_get_lli_addr(UINT32 channel_id)
  Function description: Get the starting address of the linked list control block of the specified DMA channel
  Input parameters: channel_id: channel ID, call balong_dma_channel_init function return value
  Output parameters: None
  Return value: Success: 0
               Failed: Negative
*******************************************************************************/
extern BALONG_DMA_CB *balong_dma_channel_get_lli_addr (BSP_U32 channel_id);
#define DRV_EDMA_CHAN_GET_LLI_ADDR(channel_id)  balong_dma_channel_get_lli_addr(channel_id)


/******************************************************************************
  Function name: int balong_dma_channel_init (BALONG_DMA_REQ req,
                  channel_isr pFunc, UINT32 channel_arg, UINT32 int_flag)
  Function description: Assign channels according to peripheral number, register channel interrupt callback function, initialize transmission completion semaphore,
               Write peripheral number to config register
  Input parameters: req: peripheral request number
               pFunc: DMA channel interrupt handling function registered by the upper layer module, NULL indicates that it is not registered
               channel_arg: pFunc entry parameter 1,
                             pFunc is NULL, this parameter is not required to be set
               int_flag: The entry parameter 2 of pFunc, the interrupt type generated, the value range is
                        BALONG_DMA_INT_DONE, BALONG_DMA_INT_LLT_DONE,
                       ?BALONG_DMA_INT_CONFIG_ERR, BALONG_DMA_INT_TRANSFER_ERR,
                        One of BALONG_DMA_INT_READ_ERR, or a combination.
                        pFunc is NULL, this parameter is not required to be set
  Output parameters: None
  Return value: Success: Channel number
               Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_init (BALONG_DMA_REQ req, channel_isr pFunc, BSP_U32 channel_arg, BSP_U32 int_flag);
#define DRV_EDMA_CHANNEL_INIT(req, pFunc,channel_arg,int_flag) balong_dma_channel_init(req, pFunc,channel_arg,int_flag)

/*******************************************************************************
  Function name: int balong_dma_channel_set_config (UINT32 channel_id,
                       UINT32 direction, UINT32 burst_width, UINT32 burst_len)
  Function description: During non-chain DMA transmission, call this function to configure channel parameters
               This function is not required when transmitting chain DMA.
  Input parameters: channel_id: channel ID, call the return value of balong_dma_channel_init function
               direction: DMA transmission direction, the values ??are BALONG_DMA_P2M, BALONG_DMA_M2P,
                           BALONG_DMA_M2M one
               burst_width: Take the values ??of 0, 1, 2, and 3, and the burst bit width represented is 8, 16, 32, and 64bit
               burst_len: The value range is 0~15, and the burn length is 1~16
  Output parameters: None
  Return value: Success: 0
               Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_set_config (BSP_U32 channel_id, BSP_U32 direction,BSP_U32 burst_width, BSP_U32 burst_len);
#define DRV_EDMA_CHANNEL_CONFIG(channel_id, direction,burst_width,burst_len)  balong_dma_channel_set_config(channel_id, direction,burst_width,burst_len)

/*******************************************************************************
  Function name: int balong_dma_channel_start (UINT32 channel_id, UINT32 src_addr,
                       UINT32 des_addr, UINT32 len)
  Function description: Start a synchronous DMA transmission, and return only after the DMA transmission is completed.
               When using this function, no interrupt handling function is required
  Input parameters: channel_id: channel ID, call balong_dma_channel_init function return value
               src_addr: The data transmission source address, must be a physical address
               des_addr: The destination address of the data transmission must be a physical address
               len: data transmission length, unit: bytes; the maximum length of data transmitted at one time is 65535 bytes
  Output parameters: None
  Return value: Success: 0
               Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_start (BSP_U32 channel_id, BSP_U32 src_addr, BSP_U32 des_addr, BSP_U32 len);
#define DRV_EDMA_CHANNEL_START(channel_id,src_addr,des_addr,len)  balong_dma_channel_start(channel_id,src_addr,des_addr,len)


/*******************************************************************************
  Function name: int balong_dma_channel_async_start (UINT32 channel_id,
                unsigned int src_addr, unsigned int des_addr, unsigned int len)
  Function Description: Start an asynchronous DMA transmission. After starting DMA transmission, return. Don't wait for the DMA transmission to complete.
               When using this function, register the interrupt processing function and handle the DMA in the interrupt processing function
Transmission completion event
               Alternatively, do not register the interrupt handling function and use the balong_dma_channel_is_idle function to query
               Is DMA transmission completed?
  Input parameters: channel_id: channel ID, call balong_dma_channel_init function return value
               src_addr: The data transmission source address, must be a physical address
               des_addr: The destination address of the data transmission must be a physical address
               len: data transmission length, unit: bytes; the maximum length of data transmitted at one time is 65535 bytes
  Output parameters: None
  Return value: Success: 0
               Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_async_start (BSP_U32 channel_id, BSP_U32 src_addr, BSP_U32 des_addr, BSP_U32 len);
#define DRV_EDMA_CHANNEL_ASYNC_START(channel_id,src_addr,des_addr,len) balong_dma_channel_async_start(channel_id,src_addr,des_addr,len)


/*******************************************************************************
  Function name: int balong_dma_channel_lli_start (UINT32 channel_id)
  Function Description: Start chain DMA transmission. Return only after all node transmissions of chain DMA are completed.
               The maximum data transmission length of each node of a chain DMA is 65535 bytes.
               Note: Before calling this function, you must set up the linked list control block.
  Input parameters: channel_id: channel ID, call balong_dma_channel_init function return value
  Output parameters: None
  Return value: Success: 0
               Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_lli_start (BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_lli_START(channel_id)   balong_dma_channel_lli_start(channel_id)

/*******************************************************************************
  Function name: int balong_dma_channel_lli_start (UINT32 channel_id)
  Function Description: Start chain DMA transmission and return immediately without waiting for DMA transmission to complete.
               The maximum data transmission length of each node of a chain DMA is 65535 bytes.
               Note: Before calling this function, you must set up the linked list control block.
  Input parameters: channel_id: channel ID, call balong_dma_channel_init function return value
  Output parameters: None
  Return value: Success: 0
               Failed: Negative
*******************************************************************************/
extern BSP_S32 balong_dma_channel_lli_async_start (BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_lli_ASYNC_START(channel_id)   balong_dma_channel_lli_async_start(channel_id)


/******************************************************************************
*
  Function name: int balong_dma_channel_is_idle (UINT32 channel_id)
  Function description: Query whether the DMA channel is idle
  Input parameters: channel_id: channel ID, call the return value of balong_dma_channel_init function
  Output parameters: None
  Return value: 0: Channel is busy
                1: The channel is idle
                Negative number: Failed
*******************************************************************************/
extern BSP_S32 balong_dma_channel_is_idle (BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_IS_IDLE(chanel_id) balong_dma_channel_is_idle(chanel_id)
#endif

/*************************EDMA END************************************/

/*************************SEC START***********************************/

/*************************************************
 Function Name: DRV_SECURE_SUPPORT
 Function Description: Whether the current version supports secure booting
 Input parameters: unsigned char *pData
 Output parameters: unsigned char *pData
 Return Value: OK/ERROR
*************************************************/
extern BSP_S32 DRV_SECURE_SUPPORT(BSP_U8 *pu8Data);

/*************************************************
 Function Name: DRV_SECURE_ALREADY_USE
 Function Description: Query whether the current version has enabled secure boot
 Input parameters: unsigned char *pData
 Output parameters: unsigned char *pData
 Return Value: OK/ERROR
*************************************************/
extern BSP_S32 DRV_SECURE_ALREADY_USE(BSP_U8 *pu8Data);

/*************************************************
 Function Name: DRV_START_SECURE
 Function Description: Enable Safe Boot
 Input parameters:
 Output parameters:
 Return Value: OK/ERROR
*************************************************/
extern BSP_S32 DRV_START_SECURE(void);


/*************************************************
 Function Name: DRV_SEC_CHECK
 Function description: Security mirror judgment
 Input parameters:
 Output parameters:
 Return Value: 0: Not a secure image
                           1: It is a security mirror
                           Others: Error
 Calling the function:
 Called function:

 Modify history:
    Date: March 7, 2011
   Modify content: New generated function

*************************************************/
extern BSP_U32 DRV_SEC_CHECK(void);

/*************************SEC END*************************************/

/*************************MD5 related START*********************************/

/*****************************************************************************
 Function Name: VerifySIMLock
 Function Description: Determine whether the current unlock code is correct.
 Input parameters: imei -IMEI number of single board
             unlockcode -Unlock code
 Output parameters: None.
 Return Value: 1: Unlocked successfully
             0: Unlock failed
*****************************************************************************/
extern int VerifySIMLock(char* UnlockCode, char* Key);
#define DRV_CARDLOCK_MD5_VERIFY(unlockcode, imei)  VerifySIMLock(unlockcode, imei)

/*******************************************************
  Function name: GetAuthVer
  Call relationship: Produce Tool
  enter:
  Return value:
    Description: SIMLock manager module version number
    Type: integer value
    If 1, it means that the password is obtained using an earlier version. That is, version 1.0.
                    If it is 2, it means version 2.0, and the version after this optimization is adopted to obtain the password.
                    Others are reserved values.
 Modify history:
  1. Date: April 23, 2011
    Modified content: SIMLock merge

********************************************************/
extern int GetAuthVer(void);
#define DRV_GET_AUTH_VER()  GetAuthVer()

/*************************MD5 related END******************************************/

/*************************SYNC START**********************************/

/**************************************************************************
  Error code definition
**************************************************************************/
#define BSP_ERR_SYNC_BASE                (BSP_S32)(0x80000000 | (BSP_DEF_ERR(BSP_MODU_SYNC, 0)))
#define BSP_ERR_SYNC_TIMEOUT             (BSP_ERR_SYNC_BASE + 0x0)

/**************************************************************************
  Structural definition
**************************************************************************/
typedef enum tagSYNC_MODULE_E
{
    SYNC_MODULE_MEM     = 0,
    SYNC_MODULE_NANDC,
    SYNC_MODULE_PMU,
    SYNC_MODULE_SOCP,
    SYNC_MODULE_DMA,
    SYNC_MODULE_IPF,
    SYNC_MODULE_NV,
    SYNC_MODULE_YAFFS,
    SYNC_MODULE_MSP,
    SYNC_MODULE_GPIO,
    SYNC_MODULE_CSHELL,
    SYNC_MODULE_MNTN,
	SYNC_MODULE_MSPNV,
	SYNC_MODULE_ONLINE,
	SYNC_MODULE_CHG,
	SYNC_MODULE_TFUPDATE,
    SYNC_MODULE_BUTT
}SYNC_MODULE_E;

/**************************************************************************
  Function declaration
**************************************************************************/
BSP_S32 BSP_SYNC_Lock(SYNC_MODULE_E u32Module, BSP_U32 *pState, BSP_U32 u32TimeOut);
BSP_S32 BSP_SYNC_UnLock(SYNC_MODULE_E u32Module, BSP_U32 u32State);
BSP_S32 BSP_SYNC_Wait(SYNC_MODULE_E u32Module, BSP_U32 u32TimeOut);
BSP_S32 BSP_SYNC_Give(SYNC_MODULE_E u32Module);

/*************************SYNC END************************************/

/*******************************Chg module start**********************************/

/*****************************************************************************
 Function Name: BSP_CHG_GetCbcState
 Function Description: Return to battery status and power
 Input parameters:pusBcs 0: Battery is being powered 1: Connected with the battery Battery is not powered 2: Not connected with the battery
                          pucBcl 0: Insufficient power or not connected to the battery
 Output parameters: pusBcs 0: Battery is being powered 1: Connected with the battery Battery is not powered 2: Not connected with the battery
                          pucBcl 0: Insufficient power or not connected to the battery
 Return value: 0 Operation successful
                         -1 Operation failed

*****************************************************************************/
extern int BSP_CHG_GetCbcState(unsigned char *pusBcs,unsigned char *pucBcl);
extern int DRV_CHG_GET_CBC_STATE(unsigned char *pusBcs,unsigned char *pucBcl);

typedef enum CHARGING_STATE_ENUM_tag
{
        CHARGING_INIT = -1,
        NO_CHARGING_UP = 0,  /*Powered on and not charged*/
        CHARGING_UP ,              /*Turn on and charge*/
        NO_CHARGING_DOWN ,  /*Shut down and not charged*/
        CHARGING_DOWN         /*Shut down and not charged*/
}CHARGING_STATE_ENUM;

typedef enum BATT_LEVEL_ENUM_tag
{
        BATT_INIT = -2,
        BATT_LOW_POWER =-1,    /*Low battery*/
        BATT_LEVEL_0,                 /*0 grid battery capacity*/
        BATT_LEVEL_1,                  /*1-grid battery capacity*/
        BATT_LEVEL_2,                   /*2-grid battery capacity*/
        BATT_LEVEL_3,                    /*3-grid battery capacity*/
        BATT_LEVEL_4,                    /*4-grid battery capacity*/
        BATT_LEVEL_MAX
}BATT_LEVEL_ENUM;

typedef struct BATT_STATE_tag
{
    CHARGING_STATE_ENUM  charging_state;
    BATT_LEVEL_ENUM      battery_level;
    #ifdef FEATURE_HUAWEI_MBB_CHG
    GPIO_TEMP_EVENT      batt_temp_state;
	#endif
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
    int                       batt_percent;
#endif
}BATT_STATE_T;

/*AT command enumeration type*/
typedef enum
{
    CHG_AT_BATTERY_CHECK,        /* Battery calibration*/
    CHG_AT_BATTERY_LEVEL,        /* Battery battery */
    CHG_AT_BATTERY_ADC,          /* Battery Temperature Protection*/
    CHG_AT_BATTERY_INVALID
} ENUM_CHG_ATCM_TYPE;

typedef enum ENUM_POWER_ON_MODE_tag
{
    POWER_ON_INVALID = 0,          	/* Invalid boot mode*/
    POWER_ON_NORMAL,       	 	/* Normal boot mode*/
    POWER_ON_CHARGING,       	/* Shut down charging mode*/
    POWER_ON_MAX
} ENUM_POWER_ON_MODE;
/*****************************************************************************
 Function Name: BSP_CHG_GetBatteryState
 Function description: Obtain the underlying battery status information
 Input parameters:battery_state battery information
 Output parameters:battery_state battery information
 Return value: 0 Operation successful
                         -1 Operation failed

*****************************************************************************/
extern int BSP_CHG_GetBatteryState(BATT_STATE_T *battery_state);
extern int DRV_CHG_GET_BATTERY_STATE(BATT_STATE_T *battery_state);

/*****************************************************************************
 Function Name: BSP_CHG_ChargingStatus
 Function Description: Check whether it is currently charging
 Input parameters: None
 Output parameters: None
 Return value: 0 Not charged
                          1 Charging
*****************************************************************************/
extern int BSP_CHG_ChargingStatus(void);
extern int DRV_CHG_GET_CHARGING_STATUS(void);

/*****************************************************************************
 Function Name: BSP_CHG_StateSet
 Function Description: Enable or disable charging
 Input parameters: ulState 0: Charging is prohibited
 						1: Enable charging
 Output parameters: None
 Return value: None
*****************************************************************************/
extern void DRV_CHG_STATE_SET(unsigned long ulState);

/*****************************************************************************
 Function Name: BSP_CHG_StateGet
 Function description: Query
 Input parameters:
 Output parameters: None
 Return value: None
*****************************************************************************/
extern BSP_S32 BSP_CHG_StateGet(void);
#define DRV_CHG_STATE_GET()    BSP_CHG_StateGet()

/*****************************************************************************
 Function Name: BSP_CHG_Sply
 Function description: Query
 Input parameters:
 Output parameters: None
 Return value: None
*****************************************************************************/
extern BSP_S32 BSP_CHG_Sply(void);
#define DRV_CHG_BATT_SPLY()    BSP_CHG_Sply()

#ifdef FEATURE_HUAWEI_MBB_CHG
/* 	AT read interface for chg normalzaton*/
extern int BSP_TBAT_Read(ENUM_CHG_ATCM_TYPE atID, void *pItem);
extern int BSP_TBAT_Write(ENUM_CHG_ATCM_TYPE atID, void *pItem);
#else
/* 	At read interface*/
extern BSP_S32 BSP_TBAT_Read(ENUM_CHG_ATCM_TYPE atID, void *pItem);
extern BSP_S32 BSP_TBAT_Write(ENUM_CHG_ATCM_TYPE atID, void *pItem);
#endif /* Feature huawei mbb chg */

/**********************************************************************
Function Name: BSP_TBAT_CHRStGet
Function description: TBAT AT^TCHRENABLE? Is it necessary to recharge the power

Input parameters: None
Output parameters: None
Return Value: 1: Need to recharge
			      0: No need to recharge
Notes: None
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRStGet(void);

/**********************************************************************
Function Name: BSP_TBAT_CHRSuply
Function description: TBAT AT^TCHRENABLE=4 recharge power implementation
            			Recharge the power supply and LCD display pattern is required
Input parameters: None
Output parameters: None
Return Value:
Notes: None
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRSply(void);

/**********************************************************************
Function Name: BSP_ONOFF_StartupModeGet
Function description: A core judges the switch mode
            			
Input parameters: None
Output parameters: None
Return Value:
Notes: None
***********************************************************************/
ENUM_POWER_ON_MODE BSP_ONOFF_StartupModeGet( void );
/*****************************************************************************
*Function Name: BSP_PMU_BattCali
*
*Function description: Reduce the single-board current interface before battery calibration, call the AT 
*
*Input parameters: 
*Output parameters: 
*
*Return value: 
*
*Other instructions: 
*
*****************************************************************************/
extern void BSP_PMU_BattCali(void);


/*************************CHG Module END****************************************/

/*************************ICC BEGIN***********************************/
/**************************************************************************
  Macro definition
**************************************************************************/
#define ICC_CHAN_NUM_MAX            (32)

#define ICC_CHAN_PRIORITY_HIGH      (0)
#define ICC_CHAN_PRIORITY_LOW       (255)
#define ICC_CHAN_PRIORITY_DEFAULT   ICC_CHAN_PRIORITY_LOW

#define ICC_CHAN_FIFO_MIN           (32)
#define ICC_CHAN_FIFO_MAX           (65536)
#define ICC_CHAN_FIFO_SIZE1         (1024)
#define ICC_CHAN_FIFO_SIZE2         (2048)
#define ICC_CHAN_FIFO_SIZE3         (4096)

/**************************************************************************
  Structural definition
**************************************************************************/
typedef enum tagICC_CHAN_STATE_E
{
    ICC_CHAN_STATE_CLOSED   = 0,
    ICC_CHAN_STATE_OPENED,
    ICC_CHAN_STATE_CLOSEING,
    ICC_CHAN_STATE_BUTT
}ICC_CHAN_STATE_E;

typedef enum tagICC_CHAN_MODE_E{
    ICC_CHAN_MODE_STREAM    = 0,
    ICC_CHAN_MODE_PACKET,
    ICC_CHAN_MODE_BUTT
}ICC_CHAN_MODE_E;

typedef enum tagICC_IOCTL_CMD_E{
    ICC_IOCTL_SET_WRITE_CB  = 0,
    ICC_IOCTL_SET_READ_CB,
    ICC_IOCTL_SET_EVENT_CB,
    ICC_IOCTL_GET_STATE,
    ICC_IOCTL_SET_BUTT
}ICC_IOCTL_CMD_E;


typedef enum tagICC_EVENT_E{
    ICC_EVENT_CLOSE         = 0,
    ICC_EVENT_RESET,
    ICC_EVENT_BUTT
}ICC_EVENT_E;


typedef BSP_U32 (*icc_event_cb)(BSP_U32 u32ChanID, BSP_U32 u32Event, BSP_VOID* Param);
typedef BSP_U32 (*icc_write_cb)(BSP_U32 u32ChanID);
typedef BSP_U32 (*icc_read_cb)(BSP_U32 u32ChanID, BSP_S32 u32Size);

typedef struct tagICC_CHAN_ATTR_S{
    BSP_U32             u32FIFOInSize;
    BSP_U32             u32FIFOOutSize;
    BSP_U32             u32Priority;
    ICC_CHAN_MODE_E     enChanMode;
    BSP_U32             u32TimeOut;
    icc_event_cb        event_cb;
    icc_write_cb        write_cb;
    icc_read_cb         read_cb;
}ICC_CHAN_ATTR_S;

/**************************************************************************
  Function declaration
**************************************************************************/
BSP_S32 BSP_ICC_Open(BSP_U32 u32ChanId, ICC_CHAN_ATTR_S *pChanAttr);
BSP_S32 BSP_ICC_Close(BSP_U32 u32ChanId);
BSP_S32 BSP_ICC_Write(BSP_U32 u32ChanId, BSP_U8* pData, BSP_S32 s32Size);
BSP_S32 BSP_ICC_Read(BSP_U32 u32ChanId, BSP_U8* pData, BSP_S32 s32Size);
BSP_S32 BSP_ICC_Ioctl(BSP_U32 u32ChanId, BSP_U32 cmd, BSP_VOID *param);
BSP_U32 BSP_ICC_CanSleep(BSP_U32 u32Flag);

/**************************************************************************
  Error code definition
**************************************************************************/
#define BSP_ERR_ICC_BASE                (BSP_S32)(0x80000000 | (BSP_DEF_ERR(BSP_MODU_ICC, 0)))
#define BSP_ERR_ICC_NOT_INIT            (BSP_ERR_ICC_BASE + 0x1)
#define BSP_ERR_ICC_NOT_CONNECT         (BSP_ERR_ICC_BASE + 0x2)
#define BSP_ERR_ICC_NULL                (BSP_ERR_ICC_BASE + 0x3)
#define BSP_ERR_ICC_NO_MEM              (BSP_ERR_ICC_BASE + 0x4)
#define BSP_ERR_ICC_INVALID_CHAN        (BSP_ERR_ICC_BASE + 0x5)
#define BSP_ERR_ICC_INVALID_PARAM       (BSP_ERR_ICC_BASE + 0x6)
#define BSP_ERR_ICC_EXIST               (BSP_ERR_ICC_BASE + 0x7)
#define BSP_ERR_ICC_NOT_EXIST           (BSP_ERR_ICC_BASE + 0x8)
#define BSP_ERR_ICC_OPENED              (BSP_ERR_ICC_BASE + 0x9)
#define BSP_ERR_ICC_NOT_OPEN            (BSP_ERR_ICC_BASE + 0xa)
#define BSP_ERR_ICC_NO_SEM              (BSP_ERR_ICC_BASE + 0xb)
#define BSP_ERR_ICC_NEGOTIATE_FAIL      (BSP_ERR_ICC_BASE + 0xc)
#define BSP_ERR_ICC_BUFFER_FULL         (BSP_ERR_ICC_BASE + 0xd)
#define BSP_ERR_ICC_BUFFER_EMPTY        (BSP_ERR_ICC_BASE + 0xe)
#define BSP_ERR_ICC_INVALID_PACKET      (BSP_ERR_ICC_BASE + 0xf)
#define BSP_ERR_ICC_PACKET_SIZE         (BSP_ERR_ICC_BASE + 0x10)
#define BSP_ERR_ICC_USER_BUFFER_SIZE    (BSP_ERR_ICC_BASE + 0x11)

/*************************ICC END*************************************/



/*************************IFC START*************************************/
/* Module id enumeration*/
typedef enum tagIFC_MODULE_E
{
    IFC_MODULE_BSP=  0,
    IFC_MODULE_LPS=  1,
    IFC_MODULE_GUPS= 2,
    IFC_MODULE_LMSP = 3,
    IFC_MODULE_GUOM = 4,
    IFC_MODULE_BUTT
} IFC_MODULE_E;


/* Ifc callback function structure*/
/* P msg body: The parameter corresponding to the function id of this function, the first four bytes must be guaranteed to be module id*/
/* U32 len:p msg body length*/
typedef BSP_S32 (*BSP_IFC_REG_FUNC)(BSP_VOID *pMsgBody,BSP_U32 u32Len);

/*****************************************************************************
*Function Name: BSP_IFC_RegFunc
*
*Function description: Register the IFC callback function interface
*
*Input parameters: IFC_MODULE_E enModuleId Module ID
*BSP_IFC_REG_FUNC pFunc callback function pointer
*Output parameters: None
*
*Return Value: BSP_SUCCESS
*BSP_ERR_MODULE_NOT_INITED
*BSP_ERR_INVALID_PARA
*
*Other Instructions: None
*
*****************************************************************************/
BSP_U32 BSP_IFC_RegFunc(IFC_MODULE_E enModuleId, BSP_IFC_REG_FUNC pFunc);


/*****************************************************************************
*Function Name: BSP_IFC_Send
*
*Function description: IFC message sending interface
*
*Input parameters: BSP_VOID *pMspBody Send message content
*Length of BSP_U32 u32Len pMspBody
*Output parameters: None
*
*Return Value: BSP_SUCCESS
*BSP_ERR_MODULE_NOT_INITED
*BSP_ERR_INVALID_PARA
*BSP_ERR_IFC_SEND_FAIL
*
*Other Instructions: None
*
*****************************************************************************/
BSP_U32 BSP_IFC_Send(BSP_VOID * pMspBody, BSP_U32 u32Len);

/*************************IFC END*************************************/

/*************************Build Optimization START******************************/
/*************************Memory allocation START******************************/

/* Types of each memory segment in Ddr */
typedef enum tagBSP_DDR_SECT_TYPE_E
{
    BSP_DDR_SECT_TYPE_TTF = 0x0,
    BSP_DDR_SECT_TYPE_ARMDSP,
    BSP_DDR_SECT_TYPE_UPA,
    BSP_DDR_SECT_TYPE_CQI,
    BSP_DDR_SECT_TYPE_APT,
    BSP_DDR_SECT_TYPE_ET,
    BSP_DDR_SECT_TYPE_BBPMASTER,
    BSP_DDR_SECT_TYPE_NV,
    BSP_DDR_SECT_TYPE_DICC,
    BSP_DDR_SECT_TYPE_WAN,
    BSP_DDR_SECT_TYPE_SHARE_MEM,
    BSP_DDR_SECT_TYPE_EXCP,
    BSP_DDR_SECT_TYPE_HIFI,
    BSP_DDR_SECT_TYPE_TDS_LH2,
    BSP_DDR_SECT_TYPE_BUTTOM
}BSP_DDR_SECT_TYPE_E;


/* Types of each memory segment in Axi */
typedef enum tagBSP_AXI_SECT_TYPE_E
{
    BSP_AXI_SECT_TYPE_ACORE_DEEPSLEEP = 0x0,
    BSP_AXI_SECT_TYPE_FLASH_SEM,
    BSP_AXI_SECT_TYPE_IFC,
    BSP_AXI_SECT_TYPE_ICC,
    BSP_AXI_SECT_TYPE_MEMMGR_FLAG,
    BSP_AXI_SECT_TYPE_DYNAMIC,
    BSP_AXI_SECT_TYPE_SOFT_FLAG,
    BSP_AXI_SECT_TYPE_IPF,
    BSP_AXI_SECT_TYPE_TEMPERATURE,
    BSP_AXI_SECT_TYPE_ONOFF,
    BSP_AXI_SECT_TYPE_DICC,
    BSP_AXI_SECT_TYPE_HIFI,
    BSP_AXI_SECT_TYPE_PTABLE,
    BSP_AXI_SECT_TYPE_RESERVE,
    BSP_AXI_SECT_TYPE_DLOAD_AUTOINSTALL,
    BSP_AXI_SECT_TYPE_DLOAD,
    BSP_AXI_SECT_TYPE_GUDSP_LOG,
    BSP_AXI_SECT_TYPE_BUTTOM
}BSP_AXI_SECT_TYPE_E;


/* Memory segment properties */
typedef enum tagBSP_DDR_SECT_ATTR_E
{
    BSP_DDR_SECT_ATTR_CACHEABLE = 0x0,
    BSP_DDR_SECT_ATTR_NONCACHEABLE,
    BSP_DDR_SECT_ATTR_BUTTOM
}BSP_DDR_SECT_ATTR_E;


/* Enumeration that defines whether the virtual and real addresses are the same */
typedef enum tagBSP_DDR_SECT_PVADDR_E
{
    BSP_DDR_SECT_PVADDR_EQU = 0x0,
    BSP_DDR_SECT_PVADDR_NOT_EQU,
    BSP_DDR_SECT_PVADDR_BUTTOM
}BSP_DDR_SECT_PVADDR_E;


/* Query structure of Ddr memory segment */
typedef struct tagBSP_DDR_SECT_QUERY
{
    BSP_DDR_SECT_TYPE_E     enSectType;
    BSP_DDR_SECT_ATTR_E     enSectAttr;
    BSP_DDR_SECT_PVADDR_E   enPVAddr;
    BSP_U32                 ulSectSize;
}BSP_DDR_SECT_QUERY;


/* Details of Ddr memory segment */
typedef struct tagBSP_DDR_SECT_INFO
{
    BSP_DDR_SECT_TYPE_E    enSectType;
    BSP_DDR_SECT_ATTR_E    enSectAttr;
    BSP_U32                ulSectVirtAddr;
    BSP_U32                ulSectPhysAddr;
    BSP_U32                ulSectSize;
}BSP_DDR_SECT_INFO;


/* Details of Axi memory segments */
typedef struct tagBSP_AXI_SECT_INFO
{
    BSP_AXI_SECT_TYPE_E    enSectType;
    BSP_U32                ulSectVirtAddr;
    BSP_U32                ulSectPhysAddr;
    BSP_U32                ulSectSize;
}BSP_AXI_SECT_INFO;

/*****************************************************************************
 Function Name: BSP_DDR_GetSectInfo
 Function description: DDR memory segment query interface
 Input parameters: pstSectQuery: The memory segment type and attribute that need to be queried
 Output parameters: pstSectInfo: The query memory segment information
 Return value: BSP_OK/BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_DDR_GetSectInfo(BSP_DDR_SECT_QUERY *pstSectQuery, BSP_DDR_SECT_INFO *pstSectInfo);
#define DRV_GET_FIX_DDR_ADDR(pstSectQuery, pstSectInfo) BSP_DDR_GetSectInfo(pstSectQuery, pstSectInfo)

/*****************************************************************************
 Function Name: BSP_AXI_GetSectInfo
 Function description: AXI memory segment query interface
 Input parameters: enSectType: The memory segment type that needs to be queried
 Output parameters: pstSectInfo: The query memory segment information
 Return value: BSP_OK/BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_AXI_GetSectInfo(BSP_AXI_SECT_TYPE_E enSectType, BSP_AXI_SECT_INFO *pstSectInfo);
#define DRV_GET_FIX_AXI_ADDR(enSectType, pstSectInfo) BSP_AXI_GetSectInfo(enSectType, pstSectInfo)

/*************************Memory allocation END************************************/

/**********************************IP base address and interrupt number query START*******************/

/* Define all ip types that need to be queried */
typedef enum tagBSP_IP_TYPE_E
{
    BSP_IP_TYPE_SOCP = 0x0,
    BSP_IP_TYPE_CICOM0,
    BSP_IP_TYPE_CICOM1,
    BSP_IP_TYPE_HDLC,
    BSP_IP_TYPE_BBPMASTER,
    BSP_IP_TYPE_ZSP_ITCM,
    BSP_IP_TYPE_ZSP_DTCM,
    BSP_IP_TYPE_AHB,
    BSP_IP_TYPE_WBBP,
    BSP_IP_TYPE_WBBP_DRX,
    BSP_IP_TYPE_GBBP,
    BSP_IP_TYPE_GBBP_DRX,
    BSP_IP_TYPE_ZSPDMA,
    BSP_IP_TYPE_SYSCTRL,
    BSP_IP_TYPE_TDSSYS,
    BSP_IP_TYPE_ZSPDHI,
    BSP_IP_TYPE_BUTTOM
}BSP_IP_TYPE_E;

/*****************************************************************************
 Function Name: BSP_GetIPBaseAddr
 Function description: IP base address query
 Input parameters: enIPType: The IP type to be queried
 Output parameters: None
 Return value: The query IP base address
*****************************************************************************/
BSP_U32 BSP_GetIPBaseAddr(BSP_IP_TYPE_E enIPType);
#define DRV_GET_IP_BASE_ADDR(enIPType)  BSP_GetIPBaseAddr(enIPType)


/* The interrupt type that needs to be queried */
typedef enum tagBSP_INT_TYPE_E
{
    BSP_INT_TYPE_RTC = 0,
    BSP_INT_TYPE_WDT,
    BSP_INT_TYPE_USBOTG,
    BSP_INT_TYPE_UICC,
    BSP_INT_TYPE_IPF_C,
    BSP_INT_TYPE_IPF_A,
    BSP_INT_TYPE_SOCP_A,
    BSP_INT_TYPE_SOCP_C,
    BSP_INT_TYPE_CICOM0,
    BSP_INT_TYPE_CICOM1,
    BSP_INT_TYPE_HDLC,
    BSP_INT_TYPE_BBPMASTER,
    BSP_INT_TYPE_GBBP,          /*G bbp int level*/
    BSP_INT_TYPE_GBBP_AWAKE,    /*G bbp awake int level*/
    BSP_INT_TYPE_WBBP_0MS,      /*W bbp 0 ms int level*/
    BSP_INT_TYPE_WBBP_AWAKE,    /*W bbp awake int level*/
    BSP_INT_TYPE_WBBP_SWITCH,   /*W bbp clock switch int level*/
    BSP_INT_TYPE_INT_OSRTC,     /*Rtc dual timer int level*/
    BSP_INT_TYPE_INT_SIMI,      /*Soc smim int level*/
    BSP_INT_TYPE_INT_ZSP_DOG,   /*Zsp wdg int level*/
    BSP_INT_TYPE_INT_HIFI_DOG,  /*Hifi wdg int level*/
    BSP_INT_TYPE_INT_DRX_TIMER, /**/
    BSP_INT_TYPE_BUTTOM
}BSP_INT_TYPE_E;


/*****************************************************************************
 Function Name: BSP_GetIntNO
 Function description: Interrupt number query
 Input parameters: enIntType: The interrupt type that needs to be queried
 Output parameters: None
 Return value: the query interrupt number
*****************************************************************************/
BSP_S32 BSP_GetIntNO(BSP_INT_TYPE_E enIntType);
#define DRV_GET_INT_NO(enIntType)    BSP_GetIntNO(enIntType)


/**********************************IP base address, interrupt number query END*************************/

/*************************************************Check whether the module supports START*******************/

/* Does the current version support a module? */
typedef enum tagBSP_MODULE_SUPPORT_E
{
    BSP_MODULE_SUPPORT     = 0,
    BSP_MODULE_UNSUPPORT   = 1,
    BSP_MODULE_SUPPORT_BUTTOM
}BSP_MODULE_SUPPORT_E;

/* The type of module to query */
typedef enum tagBSP_MODULE_TYPE_E
{
    BSP_MODULE_TYPE_SD = 0x0,
    BSP_MODULE_TYPE_CHARGE,
    BSP_MODULE_TYPE_WIFI,
    BSP_MODULE_TYPE_OLED,
    BSP_MODULE_TYPE_HIFI,
    BSP_MODULE_TYPE_POWER_ON_OFF,
    BSP_MODULE_TYPE_HSIC,
    BSP_MODULE_TYPE_LOCALFLASH,    
    BSP_MODULE_TYPE_BUTTOM
}BSP_MODULE_TYPE_E;

/*****************************************************************************
 Function Name: BSP_CheckModuleSupport
 Function Description: Query whether the module supports it
 Input parameters: enModuleType: The module type that needs to be queried
 Output parameters: None
 Return value: BSP_MODULE_SUPPORT or BSP_MODULE_UNSUPPORT
*****************************************************************************/
BSP_MODULE_SUPPORT_E BSP_CheckModuleSupport(BSP_MODULE_TYPE_E enModuleType);

#define DRV_GET_BATTERY_SUPPORT()   BSP_CheckModuleSupport(BSP_MODULE_TYPE_CHARGE)

#define DRV_GET_WIFI_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_WIFI)

#define DRV_GET_SD_SUPPORT()        BSP_CheckModuleSupport(BSP_MODULE_TYPE_SD)

#define DRV_GET_OLED_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_OLED)

#define DRV_GET_HIFI_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_HIFI)

#define DRV_GET_HSIC_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_HSIC)

#define DRV_GET_LOCAL_FLASH_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_LOCALFLASH)
/********************************************************/

/*************************Virtual and real address translation START******************************/

/*****************************************************************************
 Function Name: DRV_DDR_VIRT_TO_PHY
 Function description: DDR memory virtual address to real address conversion
 Input parameters: ulVAddr; virtual address
 Output parameters: None
 Return value: real address
*****************************************************************************/
unsigned int DRV_DDR_VIRT_TO_PHY(unsigned int ulVAddr);

/*****************************************************************************
 Function Name: DRV_DDR_PHY_TO_VIRT
 Function description: DDR memory virtual address to real address conversion
 Input parameters: ulPAddr; real address
 Output parameters: None
 Return value: virtual address
*****************************************************************************/
unsigned int DRV_DDR_PHY_TO_VIRT(unsigned int ulPAddr);

/*****************************************************************************
 Function Name: TTF_VIRT_TO_PHY
 Function description: Transform TTF memory virtual address to real address
 Input parameters: ulVAddr; virtual address
 Output parameters: None
 Return value: real address
*****************************************************************************/
extern unsigned int TTF_VIRT_TO_PHY(unsigned int ulVAddr);

/*****************************************************************************
 Function Name: TTF_PHY_TO_VIRT
 Function description: Transform TTF memory virtual address to real address
 Input parameters: ulPAddr; real address
 Output parameters: None
 Return value: virtual address
*****************************************************************************/
extern unsigned int TTF_PHY_TO_VIRT(unsigned int ulPAddr);

/*****************************************************************************
 Function Name: IPF_VIRT_TO_PHY
 Function description: Convert the virtual address of the IPF register to the real address
 Input parameters: ulVAddr; virtual address
 Output parameters: None
 Return value: real address
*****************************************************************************/
extern unsigned int IPF_VIRT_TO_PHY(unsigned int ulVAddr);

/*****************************************************************************
 Function Name: IPF_PHY_TO_VIRT
 Function description: Convert the virtual address of the IPF register to the real address
 Input parameters: ulPAddr; real address
 Output parameters: None
 Return value: virtual address
*****************************************************************************/
extern unsigned int IPF_PHY_TO_VIRT(unsigned int ulPAddr);

/*************************Virtual and real address translation END************************************/

/***********************************New interface for OAM components START*******************/

/*****************************************************************************
 Function Name: BSP_GUDSP_ShareAddrGet
 Function description: Get information about the physical layer shared address segment.
 Input parameters: None.
 Output parameters: pulAddr: The cache of the shared address segment information of the physical layer.
 Return Value: None.
*****************************************************************************/
extern int BSP_GUDSP_ShareAddrGet(unsigned int * pulAddrInTcm, unsigned int * pulAddrInDdr, unsigned int * pulLength);
#define DRV_DSP_SHARE_ADDR_GET(pulAddrInTcm,pulAddrInDdr,pulLength)    BSP_GUDSP_ShareAddrGet(pulAddrInTcm,pulAddrInDdr,pulLength)

/*****************************************************************************
 Function Name: BSP_UpateDSPShareInfo
 Function Description: Update the physical layer BSS COMMON segment information.
 Input parameters: None.
 Output parameters: None
 Return Value: 0: Success, -1: Failed.
*****************************************************************************/
extern int BSP_GUDSP_UpateShareInfo(void);
#define DRV_BSP_UPDATE_DSP_SHAREINFO()  BSP_GUDSP_UpateShareInfo()

/* DMA migration of downlink service data is completed, and this interrupt only corresponds to the modem core*/
typedef BSP_VOID (*BSPBBPIntDlTbFunc)(BSP_VOID);
/* Bbp subframe interrupt processing function, this interrupt only corresponds to the modem kernel*/
typedef BSP_VOID (*BSPBBPIntTimerFunc)(BSP_VOID);

/*****************************************************************************
*Function Name: BSP_BBPIntTimerRegCb
*
*Function description: Called by PS, used to register a callback with a 1ms timed interrupt to the bottom soft
*
*Input parameters: 
*Output parameters: None
*
*Return Value: VOID
*
*Modified record: March 7, 2011 wangjing creat
*****************************************************************************/
extern BSP_VOID BSP_BBPIntTimerRegCb(BSPBBPIntTimerFunc pFunc);

/*****************************************************************************
*Function Name: BSP_BBPIntTimerClear
*
*Function description: Called by PS, used to clear 1ms timing interrupt
*
*Input parameters: None
*Output parameters: None
*
*Return Value: VOID
*
*Modified record: May 31, 2011 wangjing creat
*****************************************************************************/
extern BSP_VOID BSP_BBPIntTimerClear(void);

/*****************************************************************************
*Function Name: BSP_BBPIntTimerEnable
*
*Function description: Called by PS, used to turn on 1ms timing interrupt
*
*Input parameters: None
*Output parameters: None
*
*Return Value: VOID
*
*Modified record: March 7, 2011 wangjing creat
*****************************************************************************/
extern BSP_S32 BSP_BBPIntTimerEnable(void);


/*****************************************************************************
*Function Name: BSP_BBPIntTimerDisable
*
*Function description: Called by PS, used to close 1ms timing interrupt
*
*Input parameters: None
*Output parameters: None
*
*Return Value: VOID
*
*Modified record: March 7, 2011 wangjing creat
*****************************************************************************/
extern BSP_VOID BSP_BBPIntTimerDisable(void);

/*****************************************************************************
*Function Name: BSP_BBPGetCurTime
*
*Function description: Called by PS to obtain the precise system time
*
*Input parameters: None
*Output parameters: BSP_U32 u32CurTime: Current time
*
*Return Value: BSP_OK: Acquisition successfully
*BSP_ERR_INVALID_PARA: Failed to obtain
*
*Modified record: March 7, 2011 wangjing creat
*****************************************************************************/
extern BSP_U32 BSP_BBPGetCurTime(BSP_U64 *pCurTime);

/*****************************************************************************
*Function Name: BSP_BBPIntDlTbRegCb
*
*Function description: Called by PS, used to register downlink data to the bottom soft soft register to complete the callback of interruption
*
*Input parameters: 
*Output parameters: None
*
*Return Value: VOID
*
*Modified record: March 7, 2011 wangjing creat
*****************************************************************************/
extern BSP_VOID BSP_BBPIntDlTbRegCb(BSPBBPIntDlTbFunc pFunc);

/*****************************************************************************
*Function Name: BSP_GetSysFrame
*
*Function description: get system frame num 
*
*Input parameters: None
*Output parameters:
*
*Return Value: frame num/0xffff
*
*Modified record: April 18, 2012 wangjing creat
*****************************************************************************/
extern BSP_U32 BSP_GetSysFrame(BSP_VOID);

/*****************************************************************************
*Function Name: BSP_GetSysSubFrame
*
*Function description: get sub system frame num 
*
*Input parameters: None
*Output parameters:
*
*Return Value: frame num/0xffff
*
*Modified record: April 18, 2012 wangjing creat
*****************************************************************************/
extern BSP_U32 BSP_GetSysSubFrame(BSP_VOID);

/*****************************************************************************
 Function Name: BSP_GetSliceValue
 Function Description: Get the value of the Slice timer.
 Input parameters: None.
 Output parameters: None
 Return Value: The value of the timer.
*****************************************************************************/
extern unsigned int BSP_GetSliceValue(BSP_VOID);
#define DRV_GET_SLICE()   BSP_GetSliceValue()

/*****************************************************************************
 Function Name: BSP_StartHardTimer
 Function Description: The value of a timer is started.
 Input parameters: None.
 Output parameters: None
 Return Value: The value of the timer.
*****************************************************************************/
extern BSP_VOID BSP_StartHardTimer( BSP_U32 value );
#define DRV_STATR_HARD_TIMER(value)   BSP_StartHardTimer(value)

/*****************************************************************************
 Function Name: BSP_StartHardTimer
 Function Description: The value of a timer is started.
 Input parameters: None.
 Output parameters: None
 Return Value: The value of the timer.
*****************************************************************************/
extern BSP_VOID BSP_StopHardTimer(BSP_VOID);
#define DRV_STOP_HARD_TIMER()   BSP_StopHardTimer()

/*****************************************************************************
 Function Name: BSP_GetHardTimerCurTime
 Function Description: Gets the remaining value of a timer.
 Input parameters: None.
 Output parameters: None
 Return Value: The remaining value of the timer.
*****************************************************************************/
extern BSP_U32 BSP_GetHardTimerCurTime(BSP_VOID);
#define DRV_GET_TIMER_CUR_TIME()   BSP_GetHardTimerCurTime()

/*****************************************************************************
 Function Name: BSP_ClearTimerINT
 Function Description: Clear interrupt from a timer
 Input parameters: None.
 Output parameters: None
 Return Value: The remaining value of the timer.
*****************************************************************************/
extern BSP_VOID BSP_ClearTimerINT(BSP_VOID);
#define DRV_CLEAR_TIMER_INT()   BSP_ClearTimerINT()

/*****************************************************************************
 Function Name: BSP_StartHardTimer
 Function Description: The value of a timer is started.
 Input parameters: None.
 Output parameters: None
 Return Value: The value of the timer.
*****************************************************************************/
extern BSP_VOID BSP_StartDrxTimer( BSP_U32 value );
#define DRV_STATR_DRX_TIMER(value)   BSP_StartDrxTimer(value)

/*****************************************************************************
 Function Name: BSP_StartHardTimer
 Function Description: The value of a timer is started.
 Input parameters: None.
 Output parameters: None
 Return Value: The value of the timer.
*****************************************************************************/
extern BSP_VOID BSP_StopDrxTimer(BSP_VOID);
#define DRV_STOP_DRX_TIMER()   BSP_StopDrxTimer()

/*****************************************************************************
 Function Name: BSP_GetHardTimerCurTime
 Function Description: Gets the remaining value of a timer.
 Input parameters: None.
 Output parameters: None
 Return Value: The remaining value of the timer.
*****************************************************************************/
extern BSP_U32 BSP_GetDrxTimerCurTime(BSP_VOID);
#define DRV_GET_DRX_TIMER_CUR_TIME()   BSP_GetDrxTimerCurTime()

/*****************************************************************************
 Function Name: BSP_ClearTimerINT
 Function Description: Clear interrupt from a timer
 Input parameters: None.
 Output parameters: None
 Return Value: The remaining value of the timer.
*****************************************************************************/
extern BSP_VOID BSP_ClearDrxTimerINT(BSP_VOID);
#define DRV_CLEAR_DRX_TIMER_INT()   BSP_ClearDrxTimerINT()

/*****************************************************************************
*Function Name: BSP_PWC_SetDrxTimerWakeSrc
*Function Description: Set DRX timer as wakeup source
*Input parameters: 
*Output parameters: 
*Return value:
*Modify record: 
*****************************************************************************/
extern BSP_VOID BSP_PWC_SetDrxTimerWakeSrc(VOID);
#define DRV_SET_DRX_TIMER_WAKE_SRC() BSP_PWC_SetDrxTimerWakeSrc()
/*****************************************************************************
*Function Name: BSP_PWC_DelDrxTimerWakeSrc
*Function Description: Setting DRX timer is not used as a wake-up source
*Input parameters: 
*Output parameters: 
*Return value:
*Modify record: 
*****************************************************************************/
extern BSP_VOID BSP_PWC_DelDrxTimerWakeSrc(VOID);
#define DRV_DEL_DRX_TIMER_WAKE_SRC() BSP_PWC_DelDrxTimerWakeSrc()
/**************DRX Alignment Timer20******************/

/*****************************************************************************
 Function Name: BSP_ClearZSPWatchDogInt
 Function Description: Clear interrupts from a ZSP WatchDog
 Input parameters: None.
 Output parameters: None
 Return Value: Whether the interrupt needs to be processed
*****************************************************************************/
extern BSP_BOOL BSP_ClearZSPWatchDogInt(BSP_VOID);
#define DRV_CLEAR_ZSPDOG_INT()   BSP_ClearZSPWatchDogInt()

/*****************************************************************************
 Function Name: BSP_ClearHIFIWatchDogInt
 Function Description: Clear an interrupt from a HIFI WatchDog
 Input parameters: None.
 Output parameters: None
 Return Value: Whether the interrupt needs to be processed
*****************************************************************************/
extern BSP_BOOL BSP_ClearHIFIWatchDogInt(BSP_VOID);
#define DRV_CLEAR_HIFIDOG_INT()   BSP_ClearHIFIWatchDogInt()

/*****************************************************************************
 Function Name: BSP_ZspWatchDogIntEnalbe
 Function Description: Enable interrupts of a ZSP WatchDog
 Input parameters: None.
 Output parameters: None
 Return Value: Whether the interrupt needs to be processed
*****************************************************************************/
extern BSP_VOID BSP_ZSPWatchDogIntEnalbe(int level);
#define DRV_ZSPDOG_INT_ENABLE(level) BSP_ZSPWatchDogIntEnalbe(level)

/*****************************************************************************
 Function Name: BSP_ZspWatchDogIntEnalbe
 Function Description: Enable interrupts of a ZSP WatchDog
 Input parameters: None.
 Output parameters: None
 Return Value: Whether the interrupt needs to be processed
*****************************************************************************/
extern BSP_VOID BSP_HIFIWatchDogIntEnalbe(int level);
#define DRV_HIFIDOG_INT_ENABLE(level) BSP_HIFIWatchDogIntEnalbe(level)

/*****************************************************************************
 Function Name: BSP_SendNMIInterrupt
 Function Description: Send an interrupt to NMI
 Input parameters: bit bit of NMI interrupt
 Output parameters: None
 Return Value: Whether the interrupt needs to be processed
*****************************************************************************/
extern BSP_VOID BSP_SendNMIInterrupt(unsigned int SocBitNO, unsigned int ZspBitNO);
#define DRV_SEND_NMI_INT(SocBitNO,ZspBitNO) BSP_SendNMIInterrupt(SocBitNO,ZspBitNO)

/*****************************************************************************
 Function Name: BSP_InitPlatformVerInfo
 Function description: Initialize the version number of the chip
 Input parameters: None
 Output parameters: none
 Return Value: void
*****************************************************************************/
extern BSP_VOID BSP_InitPlatformVerInfo(BSP_VOID);
#define DRV_INIT_PLATFORM_VER() BSP_InitPlatformVerInfo()

/******************************************************************************
* Function:  DRV_START_MODEGET
* Description:
* History:
********************************************************************************/
extern BSP_S32  DRV_START_MODEGET(void);

/*****************************************************************************
 Function Name: BSP_GetPlatformInfo
 Function description: Get the version number of the chip
 Input parameters: None
 Output parameters: u32PlatformInfo: chip version number
 Return Value: void
*****************************************************************************/
extern BSP_VOID BSP_GetPlatformInfo(unsigned long *u32PlatformInfo);
#define DRV_GET_PLATFORM_INFO(u32PlatformInfo) BSP_GetPlatformInfo(u32PlatformInfo)

/*The partition name of the file system that writes SDT information in the production line version*/
#define MANUFACTURE_ROOT_PATH "/manufacture"

/*****************************************************************************
 Function Name: drvOnLineUpdateResult
 Function description: WebUI module function registration.
 Input parameters: None
 Output parameters: None.
 Return Value: None
 Notes:
*****************************************************************************/
extern void drvOnLineUpdateResult(void);
#define DRV_ONLINE_UPDATE_RESULT()    drvOnLineUpdateResult()

/******************************************************************************
*
  Function name: BSP_S32 nand_get_bad_block (BSP_U32 *len, BSP_U32 **ppBadBlock)
  Function Description: Query all FLASH bad blocks of the entire NAND
  Input parameters: None
  Output parameters: pNum: Returns the number of bad blocks
                ppBadBlock: array pointer, return index index of all bad blocks
  Return value: 0: Query successful
                Negative number: query failed
*******************************************************************************/
extern BSP_S32 nand_get_bad_block(BSP_U32 *pNum, BSP_U32 **ppBadBlock);
#define NAND_GET_BAD_BLOCK(pNum, ppBadBlock) nand_get_bad_block(pNum, ppBadBlock)

/******************************************************************************
*
  Function name: BSP_VOID nand_free_bad_block_mem(BSP_U32*pBadBlock)
  Function description: The bad block memory applied through the nand_get_bad_block interface is applied by the bottom soft and is called by the protocol stack.
                This interface is released.
  Input parameters: pBadBlock
  Output parameters: None
  Return value: BSP_VOID
*******************************************************************************/
extern BSP_VOID  nand_free_bad_block_mem(BSP_U32* pBadBlock);
#define NAND_FREE_BAD_BLOCK_MEM(pBadBlock)  nand_free_bad_block_mem(pBadBlock)

#define NAND_MFU_NAME_MAX_LEN    16
#define NAND_DEV_SPEC_MAX_LEN    32

typedef struct
{
    BSP_U32           MufId;                                         /* Manufacturer ID */
    BSP_U8      aucMufName[NAND_MFU_NAME_MAX_LEN];             /* Manufacturer name string */
    BSP_U32           DevId;                                         /* Device ID */
    BSP_U8      aucDevSpec[NAND_DEV_SPEC_MAX_LEN];             /* Device specification string */
}NAND_DEV_INFO_S;

/******************************************************************************
*
  Function name: BSP_S32 nand_get_dev_info (NAND_DEV_INFO_S *pNandDevInfo)
  Function description: Query NAND device related information: including manufacturer ID, manufacturer name, device ID, device specifications
  Input parameters: None
  Output parameters: pNandDevInfo Structure for storing NAND device related information
  Return value: 0: Query successful
                Negative number: query failed
*******************************************************************************/
extern BSP_S32 nand_get_dev_info(NAND_DEV_INFO_S *pNandDevInfo);
#define NAND_GET_DEV_INFO(pNandDevInfo) nand_get_dev_info(pNandDevInfo)


/******************************************************************************
*Function: BSP_NANDF_Read
*
*Description: Read the specified address and length content in Nand Flash into the specified space
*
*Input: u32FlashAddr The source address of the data to be read
*: u32NumByte Read data length, unit in bytes
*
*Output: pRamAddr The address of the data read
*
*return: Whether the read operation is successful or not
******************************************************************************/
BSP_S32 BSP_NANDF_Read(BSP_VOID* pRamAddr, BSP_U32 u32FlashAddr, BSP_U32 u32NumByte);

/******************************************************************************
*Function : BSP_NANDF_Write
*
*Description: Write the specified address and length content to the specified FLASH address
*
*Input: pRamAddr Write operation source address
*: u32NumByte Data length, unit in bytes
*
*Output: u32FlashAddr write operation destination address
*
*return: Whether the write operation is successful or not
******************************************************************************/
BSP_S32 BSP_NANDF_Write(BSP_U32 u32FlashAddr, BSP_VOID *pRamAddr,  BSP_U32 u32NumByte);

/******************************************************************************
*Function : BSP_NANDF_Erase
*
*Description: Erase the block where the specified Flash address is located
*
*Input: u32address The ID of the block to be erased
*
*Output: None
*
*return: Whether the erase operation is successful or not
******************************************************************************/
BSP_S32 BSP_NANDF_Erase(BSP_U32 u32BlockID);

BSP_U32 nand_isbad(BSP_U32 blockID, BSP_U32 *flag);

/********************************************************************************************************
 Function Name: USB_otg_switch_signal_set
 Function Description: Used for USB channel switching GPIO pin control when BALONG and K3 docking
 Input parameters: group number, pin number and value
 Output parameters: None

 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int USB_otg_switch_signal_set(UINT8 ucGroup, UINT8 ucPin, UINT8 ucValue);
#define DRV_GPIO_SET(group, pin, value) USB_otg_switch_signal_set(group, pin, value)

#define DRV_GPIO_USB_SWITCH     200
#define DRV_GPIO_HIGH           1
#define DRV_GPIO_LOW            0

/********************************************************************************************************
 Function Name: USB_otg_switch_set
 Function description: Used to turn on or off the USB PHY on the MODEM side when connecting BALONG and K3
 Input parameters: On or Off
 Output parameters: None

 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int USB_otg_switch_set(UINT8 ucValue);
#define DRV_USB_PHY_SWITCH_SET(value) USB_otg_switch_set(value)

#define AT_USB_SWITCH_SET_VBUS_VALID            1
#define AT_USB_SWITCH_SET_VBUS_INVALID          2

#define USB_SWITCH_ON       1
#define USB_SWITCH_OFF      0

/********************************************************************************************************
 Function Name: USB_otg_switch_get
 Function description: Used for MODEM side USB PHY status query when BALONG and K3 docking
 Input parameters: None
 Output parameters: Return to USB PHY enabled status

 Return Value: 0: The operation was successful;
             -1: The operation failed.
********************************************************************************************************/
extern int USB_otg_switch_get(UINT8 *pucValue);
#define DRV_USB_PHY_SWITCH_GET(value) USB_otg_switch_get(value)

/*************************New interface for OAM components END***************************/

/************************ Build Optimization END*************************************/

enum GPIO_OPRT_ENUM
{
    GPIO_OPRT_SET = 0,
    GPIO_OPRT_GET,
    GPIO_OPRT_BUTT
};


/* IOCTL CMD Definition */
#define ACM_IOCTL_SET_WRITE_CB      0x7F001000
#define ACM_IOCTL_SET_READ_CB       0x7F001001
#define ACM_IOCTL_SET_EVT_CB        0x7F001002
#define ACM_IOCTL_SET_FREE_CB       0x7F001003

#define ACM_IOCTL_WRITE_ASYNC       0x7F001010
#define ACM_IOCTL_GET_RD_BUFF       0x7F001011
#define ACM_IOCTL_RETURN_BUFF       0x7F001012
#define ACM_IOCTL_RELLOC_READ_BUFF  0x7F001013
#define ACM_IOCTL_SEND_BUFF_CAN_DMA 0x7F001014

#define ACM_IOCTL_IS_IMPORT_DONE    0x7F001020
#define ACM_IOCTL_WRITE_DO_COPY     0x7F001021

/* Modem control command code */
#define ACM_MODEM_IOCTL_SET_MSC_READ_CB 0x7F001030
#define ACM_MODEM_IOCTL_MSC_WRITE_CMD   0x7F001031
#define ACM_MODEM_IOCTL_SET_REL_IND_CB  0x7F001032
#define ACM_IOCTL_FLOW_CONTROL  		0x7F001035

/* UDI IOCTL Command ID */
#define UDI_ACM_IOCTL_SET_READ_CB           ACM_IOCTL_SET_READ_CB
#define UDI_ACM_IOCTL_GET_READ_BUFFER_CB    ACM_IOCTL_GET_RD_BUFF
#define UDI_ACM_IOCTL_RETUR_BUFFER_CB       ACM_IOCTL_RETURN_BUFF

#define UART_IOCTL_SET_WRITE_CB      0x7F001000
#define UART_IOCTL_SET_READ_CB       0x7F001001
#define UART_IOCTL_SET_EVT_CB        0x7F001002
#define UART_IOCTL_SET_FREE_CB       0x7F001003
#define UART_IOCTL_WRITE_ASYNC       0x7F001010
#define UART_IOCTL_GET_RD_BUFF       0x7F001011
#define UART_IOCTL_RETURN_BUFF       0x7F001012
#define UART_IOCTL_RELLOC_READ_BUFF  0x7F001013
#define UART_IOCTL_SEND_BUFF_CAN_DMA 0x7F001014
#define UART_IOCTL_SET_WRITE_TIMEOUT 0x7F001015
#define UART_IOCTL_IS_IMPORT_DONE    0x7F001020
#define UDI_UART_IOCTL_SET_READ_CB           UART_IOCTL_SET_READ_CB
#define UDI_UART_IOCTL_GET_READ_BUFFER_CB    UART_IOCTL_GET_RD_BUFF
#define UDI_UART_IOCTL_RETUR_BUFFER_CB       UART_IOCTL_RETURN_BUFF

/* Asynchronous data transmission and reception structure */
typedef struct tagACM_WR_ASYNC_INFO
{
    char* pBuffer;
    unsigned int u32Size;
    void* pDrvPriv;
}ACM_WR_ASYNC_INFO;

/* Acm device event type */
typedef enum tagACM_EVT_E
{
    ACM_EVT_DEV_SUSPEND = 0,        /* The device cannot read and write (mainly used for the state of event callback function) */
    ACM_EVT_DEV_READY = 1,          /* The device can read and write (mainly used for the state of event callback function) */
    ACM_EVT_DEV_BOTTOM
}ACM_EVT_E;

typedef enum tagACM_IOCTL_FLOW_CONTROL_E
{
    ACM_IOCTL_FLOW_CONTROL_DISABLE = 0,      /* resume receiving data from ACM port */
    ACM_IOCTL_FLOW_CONTROL_ENABLE      /* stop receiving data from ACM port */
}ACM_IOCTL_FLOW_CONTROL_E;

/* Read the buffer information */
typedef struct tagACM_READ_BUFF_INFO
{
    unsigned int u32BuffSize;
    unsigned int u32BuffNum;
}ACM_READ_BUFF_INFO;

/*************************NCM START***********************************/

typedef struct tagNCM_PKT_S
{
    BSP_U8 *pBuffer;       /* Buffer pointer*/
    BSP_U32  u32BufLen;      /* Buffer length */
}NCM_PKT_S;

/* Ncm device type enumeration*/
typedef enum tagNCM_DEV_TYPE_E
{
    NCM_DEV_DATA_TYPE,      /* Data channel type, ps use*/
    NCM_DEV_CTRL_TYPE       /* Control channel, use msp transmission at command*/
}NCM_DEV_TYPE_E;

/* Uplink package function pointer */
typedef BSP_VOID (*USBUpLinkRxFunc)(UDI_HANDLE handle, BSP_VOID * pPktNode);

/* Package package release function pointer */
typedef BSP_VOID (*USBFreePktEncap)(BSP_VOID *PktEncap);

/* USB IOCTL Enumeration */
typedef enum tagNCM_IOCTL_CMD_TYPE_E
{
    NCM_IOCTL_NETWORK_CONNECTION_NOTIF,      /* 0x0, is the ncm network connected?*/
    NCM_IOCTL_CONNECTION_SPEED_CHANGE_NOTIF, /* 0x1,ncm device negotiated network card speed*/
    NCM_IOCTL_SET_PKT_ENCAP_INFO,            /* 0x2, set the package format*/
    NCM_IOCTL_REG_UPLINK_RX_FUNC,            /* 0x3, register uplink package callback function*/
    NCM_IOCTL_REG_FREE_PKT_FUNC,             /* 0x4, register the package callback function for release package*/
    NCM_IOCTL_FREE_BUFF,                     /* 0x5, release the bottom soft buffer*/
    NCM_IOCTL_GET_USED_MAX_BUFF_NUM,         /* 0x6, get the maximum number of ncm buffers that the upper layer can occupy*/
    NCM_IOCTL_GET_DEFAULT_TX_MIN_NUM,        /* 0x7, get the default number of packets. If the threshold exceeds this threshold, an ncm transmission will be started.*/
    NCM_IOCTL_GET_DEFAULT_TX_TIMEOUT,        /* 0x8, get the default packet timeout time, after which time, ncm transmission will be started*/
    NCM_IOCTL_GET_DEFAULT_TX_MAX_SIZE,       /* 0x9, get the default packet byte threshold. If this threshold exceeds it, an ncm transmission will be started.*/
    NCM_IOCTL_SET_TX_MIN_NUM,                /* 0xa, set the threshold for the number of packets. If this threshold exceeds it, an ncm transmission will be started.*/
    NCM_IOCTL_SET_TX_TIMEOUT,                /* 0xb, set the packet timeout time, after which the ncm transmission will be started*/
    NCM_IOCTL_SET_TX_MAX_SIZE,               /* 0xc, the command word is no longer used. Set the packet byte threshold, exceeding this threshold will start an ncm transmission*/
    NCM_IOCTL_GET_RX_BUF_SIZE,               /* 0xd, get the size of the packetized buffer*/
    NCM_IOCTL_FLOW_CTRL_NOTIF,               /* 0xe, flow control switch*/
    NCM_IOCTL_REG_AT_PROCESS_FUNC,           /* 0xf, register the at command to handle the callback function*/
    NCM_IOCTL_AT_RESPONSE,                   /* 0x10, at command response*/
    NCM_IOCTL_REG_CONNECT_STUS_CHG_FUNC,     /* 0x11, the notification callback function for the registration network card status change*/
    NCM_IOCTL_SET_PKT_STATICS,               /* 0x12, configuration statistics*/
       /*【BSP New Requirements USB NCM】PS requires bottom soft to provide NCM flow control status query interface*/
    NCM_IOCTL_GET_FLOWCTRL_STATUS,           /* 0x13 Query NCM flow control status*/
    /* END:   Modified by liumengcun, 2011-4-21 */

      /* New requirements for PS and MSP, providing querying the current packet threshold number interface and NCM callback function to add NCM device ID parameters when NCM is suspended*/
    NCM_IOCTL_GET_CUR_TX_MIN_NUM,              /* 0x14 Get the current number of packets*/
    NCM_IOCTL_GET_CUR_TX_TIMEOUT,               /* 0x15 Get the current packet timeout time*/
    NCM_IOCTL_IPV6_DNS_NOTIF,              /*0x16 IPV6 DNS active reporting*/
    /* END:   Modified by liumengcun, 2011-6-23 */
    /* BEGIN: Modified by liumengcun, 2011-7-20 Support IPV6 DNS configuration*/
    NCM_IOCTL_SET_IPV6_DNS,                     /* 0x16 Configure IPV6 DNS*/
    /* END:   Modified by liumengcun, 2011-7-20 */
    /* BEGIN: Modified by liumengcun, 2011-8-10 New MSP requirements*/
    NCM_IOCTL_CLEAR_IPV6_DNS,                     /* 0x17 Clear IPV6 DNS cache on the board, param's command word has no meaning, just don't fill in the blank pointer*/
    NCM_IOCTL_GET_NCM_STATUS,                     /* 0x18 Get the NCM network card status enable:TRUE(1);disable:FALSE(0) */
    /* END:   Modified by liumengcun, 2011-8-10 */

    NCM_IOCTL_SET_ACCUMULATION_TIME,

    /* BEGIN: Modified by baoxianchun, 2012-5-17 GU PS New Requirements*/
	NCM_IOCTL_SET_RX_MIN_NUM,		/*Configure the threshold for receiving packages*/
	NCM_IOCTL_SET_RX_TIMEOUT,			/*Configure the timeout of the packet collection*/
    /* END: Modified by baoxianchun, 2012-5-17 GU PS New Requirements*/

    NCM_IOCTL_REG_NDIS_RESP_STATUS_FUNC,   /* Ndis channel at command status processing callback function */
}NCM_IOCTL_CMD_TYPE_E;

/* Ncm connection status enumeration, ncm ioctl network connection notif command word corresponding parameter enumeration*/
typedef enum tagNCM_IOCTL_CONNECTION_STATUS_E
{
    NCM_IOCTL_CONNECTION_LINKDOWN,      /* Ncm network disconnection*/
    NCM_IOCTL_CONNECTION_LINKUP         /* Ncm network connection*/
}NCM_IOCTL_CONNECTION_STATUS_E;

/* Ncm connection speed structure, ncm ioctl connection speed change notif command word corresponding parameter structure*/
typedef struct tagNCM_IOCTL_CONNECTION_SPEED_S
{
    BSP_U32 u32DownBitRate;
    BSP_U32 u32UpBitRate;
}NCM_IOCTL_CONNECTION_SPEED_S;

/* Package encapsulation structure, ncm ioctl set pkt encap info command word corresponding parameter structure*/
typedef struct tagNCM_PKT_ENCAP_INFO_S
{
    BSP_S32 s32BufOft;      /* Buf offset */
    BSP_S32 s32LenOft;      /* Len offset */
    BSP_S32 s32NextOft;     /* Next Offset */
}NCM_PKT_ENCAP_INFO_S;

/* AT command reply data pointer and length NCM_IOCTL_AT_RESPONSE*/
typedef struct tagNCM_AT_RSP_S
{
    BSP_U8* pu8AtAnswer;
    BSP_U32 u32Length;
} NCM_AT_RSP_S;
typedef struct tagNCM_IPV6_DNS_NTF_S
{
    BSP_U8* pu8Ipv6DnsNtf;
    BSP_U32 u32Length;
} NCM_AT_IPV6_DNS_NTF_S;

/* The At command receives a function pointer, which is a synchronous interface, corresponding to the ncm ioctl reg at process func command word*/
typedef BSP_VOID (*USBNdisAtRecvFunc)(BSP_U8 * pu8Buf, BSP_U32 u32Len);

/* Ncm flow control switch enumeration, ncm ioctl network connection notif command word corresponding parameter enumeration*/
typedef enum tagNCM_IOCTL_FLOW_CTRL_E
{
    NCM_IOCTL_FLOW_CTRL_ENABLE,      /* Turn on flow control*/
    NCM_IOCTL_FLOW_CTRL_DISABLE      /* Turn off flow control*/
}NCM_IOCTL_FLOW_CTRL_E;

/* Ncm network card status change notification enumeration, ncm ioctl reg connect stus chg func command word corresponding parameter enumeration*/
typedef enum tagNCM_IOCTL_CONNECT_STUS_E
{
    NCM_IOCTL_STUS_CONNECT,      /* Building a chain*/
    NCM_IOCTL_STUS_BREAK         /* The network card is disconnected, the link is broken*/
}NCM_IOCTL_CONNECT_STUS_E;

/* Network card status switching notification function, corresponding to ncm ioctl reg connect stus chg func command word*/
typedef BSP_VOID (*USBNdisStusChgFunc)(NCM_IOCTL_CONNECT_STUS_E enStatus, BSP_VOID * pBuffer);

/* Ndis channel at command status processing callback function */
typedef BSP_VOID (*USBNdisRespStatusCB)(BSP_VOID *pBuffer, BSP_U32 status);

/*  Download various statistical information of uploading and downloading packages,
    不发送For PC, passively wait for PC to read, NCM_IOCTL_SET_PKT_STATICS command word corresponding parameter structure*/
typedef struct tagNCM_IOCTL_PKT_STATISTICS_S
{
    BSP_U32     u32TxOKCount;         /*Number of packets sent*/
    BSP_U32     u32RxOKCount;         /*Number of packets received*/
    BSP_U32     u32TxErrCount;        /*Send error*/
    BSP_U32     u32RxErrCount;        /*Receive error*/
    BSP_U32     u32TxOverFlowCount;   /*Send overflow and lost packets*/
    BSP_U32     u32RxOverFlowCount;   /*Receive overflow and lost packets*/
    BSP_U32     u32CurrentTx;         /*Send rate*/
    BSP_U32     u32CurrentRx;         /*Receive rate*/
} NCM_IOCTL_PKT_STATISTICS_S;

/* BEGIN: Modified by liumengcun, 2011-7-20 IPV6 DNS configuration structure, NCM_IOCTL_SET_IPV6_DNS corresponding parameter structure*/
#define BSP_NCM_IPV6_DNS_LEN     32
 typedef struct tagNCM_IPV6DNS_S  /* 0x16 Configure IPV6 DNS*/
 {
     BSP_U8 * pu8Ipv6DnsInfo;/* 32 bytes, the lower 16 bytes represent primary dns; the higher 16 bytes represent secondary dns.*/
     BSP_U32 u32Length;
 } NCM_IPV6DNS_S;
/* END:   Modified by liumengcun, 2011-7-20 */
typedef struct tagNCM_PKT_INFO_S
{
    BSP_U32 u32PsRcvPktNum;              /* Number of packages received and delivered to PS*/
    BSP_U32 u32RcvUnusedNum;             /* The number of packets dropped if the packet is not in compliance with the requirements of PS*/
    BSP_U32 u32NcmSendPktNum;            /* Number of packages*/
}NCM_PKT_INFO_S;


/*************************TIMER BEGIN*****************************/

/*****************************************************************************
*函 数 名  : BSP_USRCLK_Connect
*
*功能描述  : This routine specifies the interrupt service routine to be called
*            at each clock interrupt.  It does not enable usr clock interrupts.
*
*输入参数  : FUNCPTR routine   routine to be called at each clock interrupt
              BSP_S32 arg           argument with which to call routine
              BSP_S32 s32UsrClkid      which user clock the interrup routine belongs to
*输出参数  : 无
*返 回 值  : OK, or ERROR if the routine cannot be connected to the interrupt.
*
*Modified record: January 20, 2009 liumengcun creat
*****************************************************************************/
extern BSP_S32  BSP_USRCLK_Connect(FUNCPTR routine, BSP_S32 arg, BSP_S32 s32UsrClkid);

/*****************************************************************************
*Function Name: BSP_USRCLK_Disable
*
*Function description: This routine disables user clock interrupts.
*
*Input parameters: BSP_S32 s32UsrClkid which user clock the interrup routine belongs to
*Output parameters: None
*Return Value: OK, or ERROR
*
*Modified record: January 20, 2009 liumengcun creat
*****************************************************************************/
extern BSP_S32   BSP_USRCLK_Disable (BSP_S32 s32UsrClkid);

/*****************************************************************************
*Function Name: BSP_USRCLK_Enable
*
*Function description: This routine enables user clock interrupts.
*
*Input parameters: BSP_S32 s32UsrClkid which user clock the interrup routine belongs to
*Output parameters: None
*Return Value: OK, or ERROR
*
*Modified record: January 20, 2009 liumengcun creat
*****************************************************************************/
extern BSP_S32    BSP_USRCLK_Enable (BSP_S32 s32UsrClkid);

/*****************************************************************************
*Function Name: BSP_USRCLK_RateSet
*
*Function description: This routine sets the interrupt rate of the usr clock.
*
*Input parameters: BSP_S32 ticksPerSecond number of clock interrupts per second
              BSP_S32 s32UsrClkid which user clock the interrup routine belongs to
*Output parameters: None
*Return Value: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
*Modified record: January 20, 2009 liumengcun creat
*****************************************************************************/
extern BSP_S32  BSP_USRCLK_RateSet(BSP_S32 ticksPerSecond, BSP_S32 s32UsrClkid);

/*****************************************************************************
 Function Name: DRV_AXI_VIRT_TO_PHY
 Function description: Transform virtual address in AXI to real address
 Input parameters: ulVAddr; virtual address
 Output parameters: None
 Return value: virtual address
*****************************************************************************/
extern unsigned int DRV_AXI_VIRT_TO_PHY(unsigned int ulVAddr);

/*****************************************************************************
 Function Name: DRV_AXI_PHY_TO_VIRT
 Function description: Transform real address to virtual address in AXI
 Input parameters: ulVAddr; real address
 Output parameters: None
 Return value: virtual address
*****************************************************************************/
extern unsigned int DRV_AXI_PHY_TO_VIRT(unsigned int ulPAddr);

/*****************************************************************************
*Function Name: BSP_IPM_FreeBspBuf
*
*Function description: Release the memory interface
*
*Input parameters: BSP_U8 *pBuf Pointers that need to be released
*
*Output parameters: None
*Return Value: None
*
*Modification record: January 27, 2011 Lu Ting Created
*****************************************************************************/
BSP_VOID BSP_IPM_FreeBspBuf(BSP_U8 *pBuf);

/*for create_crypto_key,hash algorithm enum*/
typedef enum 
{
    CREATE_CRYPTO_KEY_ALGORITHM_MD5 = 0x0,
    CREATE_CRYPTO_KEY_ALGORITHM_SHA1,
    CREATE_CRYPTO_KEY_ALGORITHM_MAX
}CREATE_CRYPTO_KEY_ALGORITHM;

/*for crypto_hash,hash algorithm enum*/
typedef enum 
{
    CRYPTO_ALGORITHM_MD5 = 0x0,
    CRYPTO_ALGORITHM_SHA1,
    CRYPTO_ALGORITHM_MAX
}CRYPTO_HASH_ALGORITHM;

/*for crypto_encrypt,aes algorithm enum*/
typedef enum 
{
    CRYPTO_ENCRYPT_ALGORITHM_AES_ECB = 0x0,
    CRYPTO_ENCRYPT_ALGORITHM_MAX
}CRYPTO_ENCRYPT_ALGORITHM;



/*****************************************************************************
*Function name: create_crypto_key
*
*Function Description: Use the input data and HUK to generate a key.
*Currently supports MD5, and SHA-1 algorithms. How to generate a key: put HUK and input
*data are connected as input to the MD5 or SHA-1 algorithm to calculate its HASH value
*
*Input parameters: data: Input parameters. Stores the data used to generate the key.
*len: Enter parameters. Store input data length (bytes)
*algorithm: Enter parameters. An algorithm for generating a key.
*key: Output parameter. Stores the generated key.
*Note: Its length must be no less than 16 bytes. Because the key is 16 bytes.
*klen: Input and output parameters. As input parameters, the key
*The length of the buffer. As an output parameter, the length of the generated key is stored. (byte)
*Output parameters: 
*
*Return Value: BSP_OK-encryption was successful; BSP_ERROR-encryption failed
*
*Other instructions: Memory is requested by the caller
*
*****************************************************************************/
int create_crypto_key(char *data, int len, CREATE_CRYPTO_KEY_ALGORITHM algorithm, char *key, int *klen);
#define CREATE_CRYPTO_KEY(data,len,algorithm,key,klen)  create_crypto_key(data,len,algorithm,key,klen)

/*****************************************************************************
*Function name: crypto_hash
*
*Function Description: Calculate the HASH value of the input data.
*Currently supports MD5, SHA-1 algorithms. The output HASH length is 16 bytes
*
*Input parameters: data: Input parameters. Stores data for calculating HASH values.
*len: Enter parameters. Store input data length (bytes)
*algorithm: Enter parameters. HASH algorithm.
*hash: output parameters. Stores the generated HASH value.
*hlen: Input and output parameters. As an input parameter, the length of the buffer where the HASH value is stored. (byte)
*As an output parameter, store the length of the generated HASH value.
*Output parameters: 
*
*Return Value: BSP_OK-encryption was successful; BSP_ERROR-encryption failed
*
*Other instructions: Memory is requested by the caller
*
*****************************************************************************/
int crypto_hash(char *data, int len, CRYPTO_HASH_ALGORITHM algorithm, char *hash, int *hlen);
#define CRYPTO_HASH(data,len,algorithm,hash,hlen)  crypto_hash(data,len,algorithm,hash,hlen)

/*****************************************************************************
*Function name: crypto_encrypt
*
*Function Description: Encrypt the input data using the specified key and the specified algorithm, and output the encrypted data.
                             Currently supports AES-ECB algorithm.
*
*Input parameters: data: Input parameters. Stores data that needs to be encrypted.
*len: Enter parameters. Store input data length (bytes)
*algorithm: Enter parameters. HASH algorithm.
*key: Enter parameters. Store the encryption key.
*klen: Enter parameters. The length of the key. (byte)
*cipher_data: Output parameter. Store encrypted data.
*cipher_len: Input and output parameters. As input parameters, the length of the buffer where the cipher text is stored. (byte)
*As an output parameter, the length of the generated ciphertext is stored.
*Output parameters: 
*
*Return Value: BSP_OK-encryption was successful; BSP_ERROR-encryption failed
*
*Other instructions: Memory is requested by the caller
*
*****************************************************************************/
int crypto_encrypt (char *data, int len, CRYPTO_ENCRYPT_ALGORITHM algorithm, char *key, int klen, char *cipher_data, int *cipher_len);
#define CRYPTO_ENCRYPT(data,len,algorithm,key,klen,cipher_data,cipher_len)  \
crypto_encrypt(data,len,algorithm,key,klen,cipher_data,cipher_len)

/*****************************************************************************
*Function name: crypto_decrypt
*
*Function Description: Use the specified key and the specified algorithm to decrypt the input data and output the decrypted data.
*Currently supports AES-ECB algorithm.
*
*Input parameters: 
*cipher_data: The buffer of data to be stored.
*cipher_len: The actual length of the data to be decrypted. (byte)
*algorithm: The decryption algorithm used is only available for AES-ECB.
*key: key buffer.
*klen: key buffer length. (byte)
*len: The buffer size of the decrypted data storage buffer. (byte)(No checking)
*
*Output parameters: 
*data: Decrypted data.
*len: The length of the decrypted data. (byte)
*
*Return Value: BSP_OK: Decrypted successfully.
*BSP_ERROR: Decryption failed.
*
*Other description: len is an input/output parameter, and the memory used by the passed len variable must be writeable.
*So avoid passing function call results like sizeof() directly.
*
*****************************************************************************/
extern int crypto_decrypt (char *cipher_data,int cipher_len,CRYPTO_ENCRYPT_ALGORITHM algorithm, char *key, int klen, char *data, int *len);
#define CRYPTO_DECRYPT(cipher_data,cipher_len,algorithm, key, klen, data, len)  \
crypto_decrypt(cipher_data,cipher_len,algorithm, key, klen, data, len)


/*****************************************************************************
*Function name: crypto_rsa_encrypt
*
*Function description: Use the remodeled RSA public key saved in NV (the same signature verification is required when reading)
*Encrypt the input data and output the encrypted data.
*Input parameters: data: Input parameters. Stores data that needs to be encrypted.
*len: Enter parameters. Store input data length (bytes)
*rsa_key:RSA public key
*rsa_len: RSA public key length (bytes)
*cipher_data: Output parameter. Store encrypted data.
*cipher_len: Input and output parameters. As input parameters, the length of the buffer where the cipher text is stored. (byte)
*As an output parameter, the length of the generated ciphertext is stored.
*Output parameters: 
*
*Return Value: BSP_OK-encryption was successful; BSP_ERROR-encryption failed
*
*Other instructions: Memory is requested by the caller
*
*****************************************************************************/
int crypto_rsa_encrypt (char *data, int len, char *rsa_key, int rsa_klen, char *cipher_data, int *cipher_len);
#define CRYPTO_RSA_ENCRYT(data,len,rsa_key,rsa_klen,cipher_data,cihper_len) \
crypto_rsa_encrypt(data,len,rsa_key,rsa_klen,cipher_data,cihper_len)

/*****************************************************************************
*Function name: crypto_rsa_decrypt
*
*Function Description: Decrypt the input data using the remodeled RSA public key saved in NV
*Output decrypted data.
*Input parameters: cipher_data: Input parameters. Store encrypted data.
*cipher_len: Enter parameters. The length of the buffer where the cipher text is stored. (byte)
*rsa_key:RSA public key
*rsa_len: RSA public key length (bytes)
*data: Output parameters. Store the data that needs to be decrypted.
*len: Input and output parameters. As input parameters, the length of the decrypted buffer (bytes) is stored as a input parameter.
*As output parameter, store the length of the generated plaintext
*Output parameters: 
*
*Return value: BSP_OK--Decryption was successful; BSP_ERROR-Decryption failed
*
*Other instructions: Memory is requested by the caller
*
*****************************************************************************/
int crypto_rsa_decrypt (char *cipher_data, int cipher_len, char *rsa_key, int rsa_klen, char *data, int *len);
#define CRYPTO_RSA_DECRYPT(cipher_data,cihper_len,rsa_key,rsa_klen,data,len) \
crypto_rsa_decrypt(cipher_data,cihper_len,rsa_key,rsa_klen,data,len)

/*****************************************************************************
*Function name: crypto_rand
*
*Function description: Random number generation interface
*Input parameters: rand_data: Random number storage buffer
*len: The expected number of random number bytes
*
*Output parameters: 
*
*Return value: BSP_OK--Get random number successfully; BSP_ERROR-Get failed
*
*Other instructions: Memory is requested by the caller
*
*****************************************************************************/
int crypto_rand (char *rand_data, int len);
#define CRYPTO_RAND(rand_data,len)  crypto_rand(rand_data,len)

/*************************************************
 Function Name: efuseWriteHUK
 Function description: HUK writes efuse interface
 Input parameters: pBuf: write content; len: write length (bytes)
 Output parameters: 
 Return Value: OK/ERROR
 Calling the function:
 Called function:

 Modify history:
    Date: March 27, 2012
   Modify content: New generated function

*************************************************/
int efuseWriteHUK(char *pBuf,unsigned int len);
#define EFUSE_WRITE_HUK(pBuf,len) efuseWriteHUK(pBuf,len)

/*************************************************
 Function Name: CheckHukIsValid
 Function description: Determine whether HUK is valid. If it is 0, it is invalid. If it is not 0, it is valid.
 Input parameters: None
 Output parameters:
 Return Value: BSP_FALSE:HUK is invalid; BSP_TRUE:HUK is valid
 Calling the function:
 Called function:

 Modify history:
   Date: March 27, 2012
   Modify content: New generated function

*************************************************/
int CheckHukIsValid(void);
/*****************************************************************************
 Function Name: DRV_SMS_AWAKE_OLED_ANTISLEEP
 Function description: The API interface that opposes the system sleep when the A core wakes up the OLED after receiving the SMS message, for call to the SMS module.
 Input parameters: None
 Output parameters: None
 Return Value: PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_SMS_AWAKE_OLED_ANTISLEEP(void);
#define DRV_CHECK_HUK_IS_VALID() CheckHukIsValid()

extern long bsp_sys_creat(const char  *pathname, int mode);
#define DRV_SYS_CREAT(pathname,mode) bsp_sys_creat(pathname,mode)

extern long bsp_sys_open(const char  *filename,int flags, int mode);
#define DRV_SYS_OPEN(filename,flags,mode) bsp_sys_open(filename,flags,mode)

extern long bsp_sys_close(unsigned int fd);
#define DRV_SYS_CLOSE(fd) bsp_sys_close(fd)

extern long bsp_sys_lseek(unsigned int fd, unsigned int offset,unsigned int origin);
#define DRV_SYS_LSEEK(fd,offset,origin) bsp_sys_lseek(fd,offset,origin)

extern long bsp_sys_read(unsigned int fd, char  *buf, unsigned int count);
#define DRV_SYS_READ(fd, buf, count) bsp_sys_read(fd, buf, count)

extern long bsp_sys_write(unsigned int fd, const char  *buf,unsigned int count);
#define DRV_SYS_WRITE(fd, buf, count) bsp_sys_write(fd, buf, count)

extern long bsp_sys_mkdir(const char  *pathname, int mode);
#define DRV_SYS_MKDIR(pathname, mode) bsp_sys_mkdir(pathname, mode)

extern long bsp_sys_rmdir(const char  *pathname);
#define DRV_SYS_RMDIR(pathname) bsp_sys_rmdir(pathname)

extern long bsp_sys_unlink(const char  *pathname);
#define DRV_SYS_UNLINK(pathname) bsp_sys_unlink(pathname)

extern long bsp_sys_getdents(unsigned int fd,
          void  *dirent,
          unsigned int count);
#define DRV_SYS_GETDENTS(fd,dirent,count) bsp_sys_getdents(fd,dirent,count)

extern long bsp_sys_sync(void);
#define DRV_SYS_SYNC() bsp_sys_sync()

extern long bsp_sys_sync_file(unsigned int fd);
#define DRV_SYS_SYNC_FILE(fd) bsp_sys_sync_file(fd)



#if (FEATURE_UE_MODE_TDS == FEATURE_ON)

typedef enum tagTDS_IPC_INT_CORE_E
{
    TDS_IPC_CORE_ARM_ZSP1 = 0x1,
    TDS_IPC_CORE_ARM_ZSP2 = 0x2,
    TDS_IPC_CORE_BUTTOM   
}TDS_IPC_INT_CORE_E;


/*****************************************************************************
 *Function Name: BSP_TDS_GetDynTableAddr
 *
 *Function description: TDS Get the home address of the dynamic load table
 *
 *Input parameters: None
 *Output parameters: None
 *
 *Return Value: Addr: Dynamic table home address (DDR) /NULL: Fail
 *
 *Modify record:
 *****************************************************************************/
BSP_U32 BSP_TDS_GetDynTableAddr(BSP_VOID);

/*****************************************************************************
 *Function Name: BSP_TDS_TF_IntConnect
 *
 *Function description: TDS frame interrupt hook function
 *
 *Input parameters: routine: hook function pointer
 *parameter: parameter
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_S32 BSP_TDS_TF_IntConnect(VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
 *Function Name: BSP_TDS_TF_IntEnable
 *
 *Function Description: TDS frame interrupt enabled
 *
 *Input parameters: None
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_S32 BSP_TDS_TF_IntEnable(BSP_VOID);

/*****************************************************************************
 *Function Name: BSP_TDS_TF_IntDisable
 *
 *Function Description: TDS frame interrupt de-enabled
 *
 *Input parameters: None
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_S32 BSP_TDS_TF_IntDisable(BSP_VOID);

/*****************************************************************************
 *Function name: BSP_RunTdsDsDs
 *
 *Function description: Load ZSP (the current path read is: "/yaffs0/tds_zsp.bin")
 *
 *Input parameters: None
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_VOID ZSP_Init(BSP_VOID);


/*****************************************************************************
 *Function name: BSP_RunTdsDsDs
 *
 *Function description: Load ZSP (the current path read is: "/yaffs0/tds_zsp.bin")
 *
 *Input parameters: None
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_S32 BSP_RunTdsDsp(BSP_VOID);

/*****************************************************************************
 *Function Name: BSP_TDS_IPC_IntConnect
 *
 *Function Description: Register TDS MailBox interrupt
 *
 *Input parameters:
               BSP_U32 ulLvl: No need to use (to keep the interface form consistent)
               VOIDFUNCPTR routine interrupt service program
 *BSP_U32 parameter interrupt service program parameters
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_S32 BSP_TDS_IPC_IntConnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
*Function Name: BSP_IPC_IntSend
*
*Function Description: Send Interrupt
*
*Input parameters:
                IPC_INT_CORE_E enDstore does not need to be used (to keep the interface form consistent)
                BSP_U32 u32Msg Message to be sent
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modify record:
*****************************************************************************/
BSP_S32 BSP_TDS_IPC_IntSend(TDS_IPC_INT_CORE_E enDstCore, BSP_U32 u32Msg);

/*****************************************************************************
*Function Name: BSP_TDS_IPC_IntEnable
*
*Function Description: Enable MailBox interrupt
*
*Input parameters:
                BSP_U32 ulLvl does not need to be used (to keep the interface form consistent)
*Output parameters: None
*
*Return Value: OK&ERROR
*
*Modify record:
*****************************************************************************/
BSP_S32 BSP_TDS_IPC_IntEnable(IPC_INT_LEV_E ulLvl);

/*****************************************************************************
 *Function Name: BSP_TDS_IPC_IntDisable
 *
 *Function Description: Deactivate MailBox interrupt
 *
 *Input parameters:
                BSP_U32 ulLvl does not need to be used (to keep the interface form consistent)
 *Output parameters: None
 *
 *Return Value: OK&ERROR
 *
 *Modify record:
 *****************************************************************************/
BSP_S32 BSP_TDS_IPC_IntDisable (IPC_INT_LEV_E ulLvl);




/*************************TDS IPC END************************/
#endif 


/*****************************************************************************
*Function Name: BSP_LDSP_EDMA_MemRestore
*
*Function description: Restore LDSP mem
*
*Input parameters: 
*Output parameters: 
*Return value:
*
*Modify record: 

*****************************************************************************/
BSP_S32 BSP_LDSP_EDMA_MemRestore();


/*****************************************************************************
*Function Name: BSP_LDSP_EDMA_MemStore
*
*Function Description: Store LDSP mem
*
*Input parameters: 
*Output parameters: 
*Return value:
*
*Modify record: 

*****************************************************************************/
BSP_S32 BSP_LDSP_EDMA_MemStore();

/*****************************************************************************
*Function Name: BSP_LDSP_GetEdmaTaskState
*
*Function Description: Check whether the edma channel used by LDSP to backup TCM is idle
*
*Input parameters: 
*Output parameters: 
*Return value:
*
*Modify record: 

*****************************************************************************/
BSP_VOID BSP_LDSP_GetEdmaTaskState();

BSP_VOID BSP_GetEdmaTaskState();


/*****************************************************************************
*Function Name: DRV_Get_DspPsAddr
*Function description: Returns the 4-byte space first address allocated for DSP_PS in AXI
*Input parameters: None
*Output parameters: None
*Return Value: MEMORY_AXI_DSP_PS_ADDR
*Other Instructions: None
*****************************************************************************/
BSP_U32 DRV_Get_DspPsAddr();


/*****************************************************************************
*Function Name: DRV_Get_DspMspAddr
*Function description: Returns the 4-byte space first address allocated for DSP_MSP in AXI
*Input parameters: None
*Output parameters: None
*Return Value: MEMORY_AXI_DSP_MSP_ADDR
*Other Instructions: None
*****************************************************************************/
BSP_U32 DRV_Get_DspMspAddr();


/*******************************************************************************
  Function: BSP_Modem_OS_Status_Swtich
  Description: Indicates Modem Ready or non-Ready status

  Input: int enable
                Non-0: Set Modem in Ready state 
                0: Set Modem to be in a non-Ready state

  Output:
  Return: 0: Operation is successful
                -1: Operation failed
*******************************************************************************/
extern int BSP_Modem_OS_Status_Switch(int enable);

/*****************************************************************************
*Function Name: CHG_CHIP_HARD_READ 
*Function description: Get whether the single-board charging chip is OK
*Input parameters: None
*Output parameters: None
*Return Value: 0: Correct, -1: Incorrect
*Modification record: 2013-3-30 wanghaijie
*****************************************************************************/
extern BSP_S32 CHG_CHIP_HARD_READ (void);

#define DRV_OS_STATUS_SWITCH(enable) BSP_Modem_OS_Status_Switch(enable)
#pragma pack(0)


#endif /* end of __DRV_INTERFACE_H__ */

