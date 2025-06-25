/*************************************************************************
*Copyright (C) 1987-2020, Shenzhen Huawei Technologies Co., Ltd.
*
*File name: balong_tft_drv.c
*
*Author: wuzechun
*
*Description: TFT screen display, use 16-bit true color/128*128/high-level front/horizontal scanning when converting pictures
*
*Modify record: December 16, 2011 v1.00 wuzechun created
*
*************************************************************************/

#include <asm/io.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/device.h>
#include "BSP.h"
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <generated/FeatureConfig.h>

#include <linux/gpio.h>
#include <linux/bsp_pmu_drv.h>

#include "balong_tft_drv.h"
#include "fonts.h"
#include "images.h"

#if ( FEATURE_E5 == FEATURE_ON )		
#include "../../../arch/arm/mach-balong/mmi.h"
#endif
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
#include <mach/powerExchange.h>
#endif

#define LCD_TFT_DATA_LGTH  0x0f

#define emiCmdOut8(cmd)		(iowrite8(cmd, emi_buf_base_addr_v + EMI_BUF_WRT_CMD))
#define emiCmdOut16(cmd)		(iowrite16(cmd, emi_buf_base_addr_v + EMI_BUF_WRT_CMD))

#define emiDataOut8(data)		(iowrite8(data, emi_buf_base_addr_v + EMI_WRT_DATA_8BIT))
#define emiDataOut16(data)		(iowrite16(data, emi_buf_base_addr_v + EMI_WRT_DATA_16BIT))

#define emiCmdIn8()		(ioread8(emi_buf_base_addr_v + EMI_BUF_WRT_CMD))
#define emiCmdIn16()		(ioread16(emi_buf_base_addr_v + EMI_BUF_WRT_CMD))

#define emiDataIn8()		(ioread8(emi_buf_base_addr_v + EMI_WRT_DATA_8BIT))
#define emiDataIn16()		(ioread16(emi_buf_base_addr_v + EMI_WRT_DATA_16BIT))

#define TFT_DYNAMIC_MINOR 236
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
#define Dim_light        3
#endif

#if (FEATURE_E5 == FEATURE_ON)
#if (FEATURE_E5_UDP == FEATURE_ON)
#define BALONG_GPIO_LED_LIG    BALONG_GPIO_1(19)
#else
#define TFT_LCD_PWR_LDO     13
#define TFT_BKLIG_PMU_DR_NR 1
#endif
#endif
    
static BSP_U32 emi_ctrl_base_addr_v = 0;
static BSP_U32 emi_buf_base_addr_v = 0; /*lint !e551 */

/* Initialize the logo*/
static BSP_BOOL g_bEmiInit = BSP_FALSE;

static BSP_U32 g_bTftInit = BSP_FALSE;

/* Initialize the logo*/
BSP_BOOL g_bLcdInit = BSP_FALSE;

/*Whether the screen is currently on, TRUE: On FALSE: Off*/
static BSP_BOOL g_bIsLight = BSP_TRUE;

/* Global variables indicating timing*/
LCD_TYPE_E g_bLcdType = LCD_TYPE_STN;
LCD_IF_E   g_bLcdIf   = LCD_IF_EMI;
BSP_U32	rstCnt = 0;

/* Global variables indicating timing*/
static EMI_SCDUL_MODE_E g_bEmiScdul = EMI_TFT_8BIT;

/* Global variable indicating the interface bit width*/
static EMI_IF_WDTH_E g_bEmiIfWdth = EMI_WDTH_8BIT;

static BSP_U8 u8E0Data[LCD_TFT_DATA_LGTH] = {0x3F,0x1C, 0x18,0x25,0x1E,0x0C,0x42,0xD8,\
    0x2B,0x13,0x12,0x08,0x10,0x03,0x00};
static BSP_U8 u8E1Data[LCD_TFT_DATA_LGTH] = {0x00,0x23,0x27,0x0A,0x11,0x13,0x3D,0x72,\
    0x54,0x0C,0x1D,0x27,0x2F,0x3C,0x3F};

/* x/y position for user to refresh image */
static UINT32 x_position = 0;
static UINT32 y_position = 0;

#define SCREEN_BUFFER_SIZE (LCD_HIGHT*LCD_WIDTH*2)
/*The bad single board status flag was detected, and the single board battery temperature detection pull-up resistance dummy soldering*/
static BSP_BOOL is_ftm_bad_board = BSP_FALSE;  
/*Detected bad board, control command word for screen swiping*/
#define FTM_BAD_BOARD       0x00212897 

static BSP_BOOL refresh_tft_when_inited = BSP_FALSE; 

static unsigned char *buffer_t = NULL;
static volatile UINT32 g_ulPicCount = 0x00;  /*Picture Count*/

static volatile UINT32 g_ulOledPwrOnState = LCD_PWR_OFF_STATE;  /*Tft power-on state*/

static UINT32	tft_poweroff_init_flag = 0;

/* Synchronous semaphore*/
static struct semaphore	g_ulOledSemSyn;

/*Clear screen and swipe screen mutually exclusive semaphore*/
static struct semaphore	g_ulOledSemMux;

/* workqueue and worker for (power on)/(power off charge) animation */
static struct workqueue_struct	*tft_queue;
static struct delayed_work		tft_pwroff_chg_work;

/* task for displaying charge animation */
static struct task_struct *charge_task = NULL;
static struct platform_device *tft_dev = NULL;
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
static struct delayed_work        tft_pwron_ani_work;
static struct task_struct *power_on_ani_task = NULL;
static struct semaphore g_ulOledPowerOnAniSemSyn;
static volatile UINT32 g_ulPowerOnAniPicCount = 0x00;  /*Picture Count*/
static UINT32 tft_pwron_init_flag = 0;
BSP_S32 tftPwrOnAniInstall(BSP_VOID);
#endif

/*For ST*/
#undef  BALONG_LCD_SYSFS
#define BALONG_LCD_SYSFS

#undef HILCD_TRACE_LEVEL
#define HILCD_TRACE_LEVEL  (5)

#undef hitft_trace
#undef hitft_error
#define BALONG_LCD_DEBUG_TAG	"hitft"
#define hitft_trace(level, format, args...) do { \
	if((level) >= HILCD_TRACE_LEVEL) { \
		printk(format":"BALONG_LCD_DEBUG_TAG":%s:%d: \n", ## args, __FUNCTION__,  __LINE__); \
	} \
}while(0)


#define hitft_error(format, args...) do{ \
	printk(KERN_ERR""format":"BALONG_LCD_DEBUG_TAG" error:%s:%d: \n", ##args, __FUNCTION__, __LINE__); \
}while(0)
BSP_VOID tftReset(BSP_VOID);
BSP_S32 tftRefresh(UINT32 ulXStart, UINT32 ulYStart, UINT32 ulXOffset, UINT32 ulYOffset, UINT8 *pucBuffer);
BSP_S32 tftPwrOffChgAniInstall(BSP_VOID);
BSP_S32 balong_tft_ioctl(struct file *file, BSP_U32 cmd, unsigned long arg);
BSP_VOID tftClearWholeScreen(BSP_VOID);

#if (FEATURE_E5 == FEATURE_ON)
extern BSP_U32 BSP_CHGC_SplyStGet(void);
#endif

BSP_VOID cmd_out_direct(UINT8 cmd)
{
   printk("----cmd out direct");
   emiCmdOut8(cmd);
}
BSP_VOID data_out_direct(UINT8 data)
{
   printk("----data out direct");
   emiCmdOut8(data);
}


static unsigned char * g_chgImage[5] =
{
    (unsigned char *)gImage_chg_0,
    (unsigned char *)gImage_chg_1,
    (unsigned char *)gImage_chg_2,
    (unsigned char *)gImage_chg_3,
    (unsigned char *)gImage_chg_4

};



static unsigned char * g_power_on_display_image[6] =
{
    (unsigned char *)power_on_display_image0,
    (unsigned char *)power_on_display_image1,
    (unsigned char *)power_on_display_image2,
    (unsigned char *)power_on_display_image3,
    (unsigned char *)power_on_display_image4,
    (unsigned char *)power_on_display_image5

};


#endif

BSP_VOID emi_reg_set_bit(unsigned offset,unsigned bit)
{
  unsigned long flag = readl(V3R2_SC_VA_BASE + offset);
  flag |= (1<<bit);
  writel(flag,V3R2_SC_VA_BASE + offset);
}/*lint !e550*/

BSP_VOID emi_reg_clear_bit(unsigned offset,unsigned bit)
{
  unsigned long flag = readl(V3R2_SC_VA_BASE + offset);
  flag &= ~(1<<bit);    /*lint !e502 */
  writel(flag,V3R2_SC_VA_BASE + offset);
}/*lint !e550*/

void PWRCTRL_EMI_SoftClkStatus(void)
{
	unsigned int emi_clk = 0; /*clean lint e713*/
	emi_clk = BSP_PWRCTRL_SoftClkStatus(PWRCTRL_MODU_EMI);
	printk("EMI clk=%d\n", emi_clk);
}

void PWRCTRL_EMI_SoftClkDis(void)
{
	BSP_PWRCTRL_SoftClkDis(PWRCTRL_MODU_EMI);
}

void PWRCTRL_EMI_SoftClkEn(void)
{
	BSP_PWRCTRL_SoftClkEn(PWRCTRL_MODU_EMI);
}


int PWRCTRL_EMI_LowPowerEnter(void)
{
	PWRCTRL_EMI_SoftClkDis();
#if defined (FEATURE_LCD_ON)
    /*This function is used for LCD sleep voting and clock-off interfaces, and only voting functions are added at present*/
    BSP_PWRCTRL_SleepVoteUnLock(PWRCTRL_SLEEP_LCD);
#endif
	return BSP_OK;
}

int PWRCTRL_EMI_LowPowerExit(void)
{
	PWRCTRL_EMI_SoftClkEn();
#if defined (FEATURE_LCD_ON)
    /*This function is used for LCD sleep voting and clock-off interfaces, and only voting functions are added at present*/
	BSP_PWRCTRL_SleepVoteLock(PWRCTRL_SLEEP_LCD);
#endif
	return BSP_OK;
}

/*****************************************************************************
  Function    : tftInit
  Description : tftInit
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
BSP_S32 emiInit(BSP_VOID)
{
	printk("%s --------- start emi init\n", __func__);

    emi_ctrl_base_addr_v = (uint32_t)ioremap(EMI_REG_BASE_ADDR, 0x32);
    emi_buf_base_addr_v = (uint32_t)ioremap(EMI_BUF_BASE_ADDR, 0x1000); /* Video memory buff */

#if  (FEATURE_E5 == FEATURE_ON) && (FEATURE_E5_UDP != FEATURE_ON)
    /* Open ld o13 */
    BSP_PMU_LDOCtrl(TFT_LCD_PWR_LDO, BSP_TRUE);
    /* Current source is 9m a */
    BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR, 18);
#endif

   /*lint -e778*/
    /*Reset emi*/
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, 1); //
    
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, CRG_RST_DIS);
   
    /*Turn on the clock*/
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, CRG_CLK_OFFSET, 1, CRG_CLK_VALID);
 
    /* I8080 read enable signal*/
    /* I8080 write enable signal*/
    /* Interface command/data selection signal*/
    CRG_REG_SETBITS(IOS_CTRL71_OFFSET, 10, 1, 1); /*Enable as output, default is output*/
	/*lint +e778*/

#if defined(CHIP_BB_6920ES) /* Cs unified configuration */
    /* Reset and chip select signal PAD settings*/
    CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 20, 1, 1); /* Three conditions are met, this is not the default*/
    CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 23, 1, 0); /* Three conditions are met, default*/
    CRG_REG_SETBITS(IOS_CTRL99_OFFSET, 17, 1, 0); /* Three conditions are met, default*/
#endif

    /* Data signal*/
    /* No need to set*/
    
    /* Set chip selection signal low valid*/
    EMI_REG_SETBITS(EMI_CR0_OFFSET,1,1,0);
    
#if defined(CHIP_BB_6920ES) && (FEATURE_E5_UDP == FEATURE_ON)
    /* Turn on the backlight*/
    /* Setting up the pad of gpi o3 1*/
    CRG_REG_SETBITS(IOS_CTRL100_OFFSET, 14, 1, 0);
    CRG_REG_SETBITS(IOS_CTRL100_OFFSET, 15, 1, 1);
    CRG_REG_SETBITS(IOS_CTRL92_OFFSET, 23, 1, 0);
#endif

	return BSP_OK;
}
#if defined(CHIP_BB_6920ES)
#define LCD_ID0 BALONG_GPIO_1(LCD_VEC_ID_GPIO_0)
#define LCD_ID1 BALONG_GPIO_1(LCD_VEC_ID_GPIO_1)
#elif defined(CHIP_BB_6920CS)
#define LCD_ID0 BALONG_GPIO_4(LCD_VEC_ID_GPIO_0)
#define LCD_ID1 BALONG_GPIO_4(LCD_VEC_ID_GPIO_1)
#else 
#error "invalid lcd id"
#endif

BSP_S32 lcdVectIdGet (BSP_U16 *pu16VectId)
{
    BSP_U32 u32Data1 = 0;
    BSP_U32 u32Data2 = 0;
	static BSP_BOOL is_init = BSP_FALSE;
		
    if(!pu16VectId)
    {
    	hitft_error("invalid parameter\n");
        return BSP_ERROR;
    }
	
#if defined(CHIP_BB_6920ES)	
    /* Multiplex gpio 1 19/20 as gpio */    
	CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 18,1, 0);
	CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 19,1, 0);
#endif
	if(!is_init)
	{
		if(gpio_request(LCD_ID0, "LCD_ID0"))
		{
			hitft_error("Gpio is busy!!! \n");
		}
		/* Set gpio as input */
		gpio_direction_input(LCD_ID0);	
		
		if(gpio_request(LCD_ID1, "LCD_ID1"))
		{
			hitft_error("Gpio is busy!!! \n");
		}
		
		is_init = BSP_TRUE;
	}
	
 	gpio_direction_input(LCD_ID1); 

    /* Get input value */
	u32Data1 = gpio_get_value(LCD_ID0); /*lint !e732 */
	u32Data2 = gpio_get_value(LCD_ID1); /*lint !e732 */

	printk("LCD_ID0 %x,LCD_ID1 %x\n",u32Data1,u32Data2);
    /* Get the LCD manufacturer ID*/
    *pu16VectId = (BSP_U16)((1&u32Data1<<0)\
                     |((1&u32Data2)<<1));
    
    return BSP_OK;  
	
}

BSP_U32 lcdVecId(void)
{
	BSP_U16 id;
	lcdVectIdGet((BSP_U16*)&id);
	return id;
}

/*****************************************************************************
*Function name: tftCMInit
*
*Function description: CHIMEI TFT LCD initialization
*
*Input parameters: 
*
*Output parameters: None
*
*Return value: 
*****************************************************************************/
BSP_VOID tftCMInit(BSP_VOID)
{
	BSP_U32 i = 0;
    BSP_U16 ucId = 0;
    /* Wake up lcd */
    emiCmdOut8(0x11);
    
    /* Delay 120ms */
    msleep(120);

    /* Set pixel format*/
    emiCmdOut8(0x3A);
    emiDataOut8(0x55);
    
    /* Setting up GVDD and VCI1 */
    emiCmdOut8(0xC0);
    emiDataOut8(0x0C); /* GVDD 4.20V */
    emiDataOut8(0x05); /* VCI1 2.50V */

    msleep(10);

    /* Setting voltage supply */
    emiCmdOut8(0xC1);
    emiDataOut8(0x06); /* No 06 configuration?? by wuzechun */

    msleep(10);

    /* Set the amplifier current value, Medium High*/
    emiCmdOut8(0xC2);
    emiDataOut8(0x04);
    
    msleep(10);

    /* Set VCOM1, VCOMH:3.525V, VCOML:-1.225V */
    emiCmdOut8(0xC5);
    emiDataOut8(0x29);
    emiDataOut8(0x35);

    msleep(10);

    /* Set VCOM Offset*/
    emiCmdOut8(0xC7);
    emiDataOut8(0xD5);

    /* Gamma adjustment E0 and E1 enable*/
    emiCmdOut8(0xF2);
    emiDataOut8(0x01);

    /* Set e0*/
    emiCmdOut8(0xE0);
    for(i=0;i<LCD_TFT_DATA_LGTH;i++)
    {
        emiDataOut8(u8E0Data[i]);
    }

    emiCmdOut8(0xE1);
    for(i=0;i<LCD_TFT_DATA_LGTH;i++)
    {
        emiDataOut8(u8E1Data[i]);
    }
	
    /* Set the scanning direction*/
	emiCmdOut8(0x36);
#if defined(FEATURE_LCD_TURN_RIGHT)
    /*The screen display rotates ninety degrees clockwise*/
	emiDataOut8(0x68);	
#else
    emiDataOut8(0x08);
#endif
	
    /* Set the frame rate*/
    emiCmdOut8(0xB1);
    emiDataOut8(0x10);
    emiDataOut8(0x0A);

    /* Turn on the display*/
    emiCmdOut8(0x29);
    
    msleep(10);

    /*Memory Write, no need? by wuzechun*/
    emiCmdOut8(0x2C);
    
    msleep(120);
}

/*****************************************************************************
*Function name: tftTMInit
*
*Function description: ILITEK TFT LCD initialization
*
*Input parameters: 
*
*Output parameters: None
*
*Return value: 
*****************************************************************************/
BSP_VOID tftTMInit(void)
{
	int i = 0;

	BSP_U8 u8E0Data[LCD_TFT_DATA_LGTH] = {0x3F,0x1C, 0x18,0x25,0x1E,0x0C,0x42,0xD8,\
		0x2B,0x13,0x12,0x08,0x10,0x03,0x00};
	BSP_U8 u8E1Data[LCD_TFT_DATA_LGTH] = {0x00,0x23,0x27,0x0A,0x11,0x13,0x3D,0x72,\
		0x54,0x0C,0x1D,0x27,0x2F,0x3C,0x3F};

    /* Wake up lcd */
    emiCmdOut8(0x11);
	
    /* Delay 120ms */
    msleep(120);
   
    emiCmdOut8(0xEC);
    emiDataOut8(0x0C);
	
	/* set default gamma*/
    emiCmdOut8(0x26);
    emiDataOut8(0x4);
	
	/* set frame rate*/
    emiCmdOut8(0xB1);
    emiDataOut8(0x0A);
	emiDataOut8(0x14);

	/*set VRH1[4:0]&VC[2:0] for VCI1 & GVDD*/
    emiCmdOut8(0xC0);
    emiDataOut8(0x11);
	emiDataOut8(0x05);		

    emiCmdOut8(0xC1);
    emiDataOut8(0x2);
	
	/*set VMH[6:0]&VML[6:0] for VOMH&VCOML*/
    emiCmdOut8(0xC5);
    emiDataOut8(0x40);
	emiDataOut8(0x45);		

    emiCmdOut8(0xC7);
    emiDataOut8(0xC1);
	
	/*set Scan direction*/
    emiCmdOut8(0x2A);
    emiDataOut8(0x0);
	emiDataOut8(0x0);	
    emiDataOut8(0x0);
	emiDataOut8(0x7F);
	
    emiCmdOut8(0x2B);
    emiDataOut8(0x0);
	emiDataOut8(0x0);	
    emiDataOut8(0x0);
	emiDataOut8(0x7F);	

	/*set pixel*/
    emiCmdOut8(0x3A);
    emiDataOut8(0x55);
	
	/*set scanning direction &BGR*/
    emiCmdOut8(0x36);
#if defined(FEATURE_LCD_TURN_RIGHT)
    /*The screen display rotates ninety degrees clockwise*/
    emiDataOut8(0xA8);
#else
    emiDataOut8(0xC8);
#endif
	
	/*enable gamma bit*/	
    emiCmdOut8(0xF2);
    emiDataOut8(0x01);

	/*Gamma Setting*/
	emiCmdOut8(0xE0);
    for(i=0;i<LCD_TFT_DATA_LGTH;i++)
    {
	    emiDataOut8(u8E0Data[i]);
    }

    emiCmdOut8(0xE1);
    for(i=0;i<LCD_TFT_DATA_LGTH;i++)
    {
        emiDataOut8(u8E1Data[i]);
    }
		/*display on*/
    emiCmdOut8(0x29);
		
    msleep(10);
    /*Memory Write, no need? by wuzechun*/
    emiCmdOut8(0x2C);
    
    msleep(120);
}

/*****************************************************************************
*Function name: tftlcdInit
*
*Function description: TFT LCD initialization
*
*Input parameters: 
*
*Output parameters: None
*
*Return value: 
*****************************************************************************/
BSP_VOID tftlcdInit(BSP_VOID)
{
	BSP_U16 ucId = 0;

	lcdVectIdGet(&ucId); 
	
	if(LCD_ILITEK == ucId)
	{
		tftTMInit();
	}
	else if(LCD_CHIMEI == ucId)
	{
		tftCMInit();
	}
	else
	{
		hitft_error("invalid lcd vector id\n");
	}
}

/*****************************************************************************
*Function name: emisetScdul
*
*Function Description: Set the timing
*
*Input parameters: eSchdulMode: LCD type and interface mode
                  pScdulPara: timing parameter value
*
*Output parameters: None
*
*Return Value: Setting successful or failed
*****************************************************************************/
BSP_S32 emiSetScdul(EMI_SCDUL_MODE_E eSchdulMode, EMI_SCDUL_PARA_S *pScdulPara)
{
    g_bEmiScdul = eSchdulMode;
    
    /* Set the timing */
    if(eSchdulMode < EMI_SCHEDULING_BUTT)
    {
        // I8080 and M6800 are distinguished

        if(EMI_STN_8BIT_M6800 == eSchdulMode)
        {
            emi_reg_set_bit(CS_CTRL3_OFFSET, 1); /*Set to m6800 mode*/
        }
        else
        {
            emi_reg_clear_bit(CS_CTRL3_OFFSET, 1);
        }

        // 8-bit and 16-bit

        if(EMI_TFT_9BIT == eSchdulMode)
        {
            EMI_REG_SETBITS(EMI_CR0_OFFSET,3,2,1);
            EMI_REG_SETBITS(EMI_CR0_OFFSET,5,1,1);
			/*lint -e778*/
            CRG_REG_SETBITS(IOS_CTRL98_OFFSET,2,1,1); /* Default pad bit8-bit15*/
			/*lint +e778*/
        }
        else
        {
            EMI_REG_SETBITS(EMI_CR0_OFFSET,3,2,0);  
            EMI_REG_SETBITS(EMI_CR0_OFFSET,5,1,0);
        }

        switch(eSchdulMode)
        {
            case EMI_TFT_8BIT:
            case EMI_TFT_9BIT:
            {
                /* Set the read operation timing*/
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,0,10,pScdulPara->u32Wst1); /* Bus read access waiting time*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,16,8,pScdulPara->u32Wst3); /* The interval between two read and write accesses*/
                EMI_REG_SETBITS(EMI_WSTOENR0_OFFSET,0,4,pScdulPara->u32OEn); /* Cs valid to read signal effective delay*/
                EMI_REG_SETBITS(EMI_WSTOENR0_OFFSET,5,2,pScdulPara->u32OEnDelay); /* Time delay of read signal to chip select invalid*/

                /* Set the write operation timing*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,0,10,pScdulPara->u32Wst2); /* Bus write access waiting time*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,16,8,pScdulPara->u32Wst3); /* The interval between two read and write accesses*/
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,0,4,pScdulPara->u32WEn); /* Cs valid to write signal effective delay*/
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,4,2,pScdulPara->u32WEnDelay); /* Time delay of write signal to chip selection*/
            }
                break;
            case EMI_STN_8BIT_M6800:
            case EMI_STN_8BIT_I8080:
            {
                BSP_U32 u32UpDlyrSave   = 0;
                BSP_U32 u32DwnDlyrSave  = 0;
                BSP_U32 u32UpDlywSave   = 0;
                BSP_U32 u32DwnDlywSave  = 0;
                BSP_U32 u32UpDlyrTmp    = 0;
                BSP_U32 u32DwnDlyrTmp   = 0;
                BSP_U32 u32UpDlywTmp    = 0;
                BSP_U32 u32DwnDlywTmp   = 0;

                /* Save parameters */
                u32UpDlyrSave   = pScdulPara->u32UpDlyr;
                u32DwnDlyrSave  = pScdulPara->u32DdnDlyr;
                u32UpDlywSave   = pScdulPara->u32UpDlyw;
                u32DwnDlywSave  = pScdulPara->u32DdnDlyw;
                
                /* Set the read operation timing*/
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,0,10,pScdulPara->u32Wst1); /* Bus read access waiting time*/
                u32UpDlyrTmp = (u32UpDlyrSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,4,7,u32UpDlyrTmp); /* Preb signal pulls up the delay time of relative to cs valid signal, 7 bits lower*/
                u32UpDlyrTmp = ((u32UpDlyrSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,10,3,u32UpDlyrTmp); /* The Preb signal pulls up the delay time of the relative cs valid signal, which is 3 bits higher*/

                u32DwnDlyrTmp = (u32DwnDlyrSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,11,7,u32DwnDlyrTmp); /* Preb signal lowers the delay time of relative cs valid signal, lowers 7 bits*/
                u32DwnDlyrTmp = ((u32DwnDlyrSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,13,3,u32DwnDlyrTmp); /* Preb signal lowers the delay time of relative cs valid signal, 3 bits higher*/

                /* Set the read operation timing*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,0,10,pScdulPara->u32Wst2); /* Bus write access waiting time*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,16,8,pScdulPara->u32Wst3); /* Bus read and write access waiting time*/
                
                u32UpDlywTmp = (u32UpDlywSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,18,7,u32UpDlywTmp); /* Preb signal pulls up the delay time of relative to cs valid signal, 7 bits lower*/
                u32UpDlywTmp = ((u32UpDlywSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,16,3,u32UpDlywTmp); /* The Preb signal pulls up the delay time of the relative cs valid signal, which is 3 bits higher*/

                u32DwnDlywTmp = (u32DwnDlywSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,25,7,u32DwnDlywTmp); /* Preb signal lowers the delay time of relative cs valid signal, lowers 7 bits*/
                u32DwnDlywTmp = ((u32DwnDlywSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,19,3,u32DwnDlywTmp); /* Preb signal lowers the delay time of relative cs valid signal, 3 bits higher*/
                
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,0,4,pScdulPara->u32WEn); /* Cs valid to write signal effective delay*/
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,5,2,pScdulPara->u32WEnDelay); /* Time delay of write signal to chip selection*/
            }
                break;
            case EMI_LCD_8BIT:
            default:
                hitft_error("EMI SCHDULING MODE IS NOT SUPPORTED. \n");
                return  BSP_ERR_INVALID_PARA;  
        }
            
    }
    else
    {
        hitft_error("EMI SCHDULING MODE IS VALID. \n");
        return BSP_ERR_INVALID_PARA;
    }

    return BSP_OK;
}

/*****************************************************************************
*Function Name: emisetBackLight
*
*Function description: LCD backlight panel control
*
*Input parameters: BSP_TRUE: Turn on the backlight BSP_FALSE: Turn off the backlight
*
*Output parameters: None
*
*Return Value: Setting successful or failed
*****************************************************************************/
BSP_S32 emiSetBackLight(BSP_BOOL bOn)
{
#if (FEATURE_E5 == FEATURE_ON)
#if (FEATURE_E5_UDP == FEATURE_ON)
    if(gpio_request(BALONG_GPIO_LED_LIG, "Board_Type_detect"))
	{
		hitft_error("Gpio is busy!!! \n");
	}
	else
	{
	    gpio_direction_output(BALONG_GPIO_LED_LIG, 1);
        gpio_set_function(BALONG_GPIO_LED_LIG, GPIO_NORMAL);
    	gpio_set_value(BALONG_GPIO_LED_LIG, bOn == BSP_TRUE ? 0 : 1);
        
        gpio_free(BALONG_GPIO_LED_LIG);
	}
#else
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
    BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR,18);
#endif
    return BSP_PMU_DRCtrl(TFT_BKLIG_PMU_DR_NR, bOn);
#endif
#endif
    return BSP_OK;/*lint !e527*/
}
/*****************************************************************************
*Function name: stnlcdInit
*
*Function description: STN LCD initialization
*
*Input parameters: 
*
*Output parameters: None
*
*Return value: 
*****************************************************************************/
BSP_VOID stnlcdInit(BSP_VOID)
{
    // internal reset

    emiCmdOut8(0xe2);
    
    /* Delay 10ms */
    msleep(10);

    emiCmdOut8(0xc0);

    emiCmdOut8(0xa2);
    
    /* Delay 10ms */
    msleep(10);

    emiCmdOut8(0xa0);

    emiCmdOut8(0x24);
    
    msleep(10);

    emiCmdOut8(0x81);

    emiCmdOut8(0x19+0x0f);

    emiCmdOut8(0x2f);
    
    emiCmdOut8(0xa4);
    
    emiCmdOut8(0xc8);

    emiCmdOut8(0x2f);

    emiCmdOut8(0xf8);

    emiCmdOut8(0x01);
    
    emiCmdOut8(0x40);
    
    emiCmdOut8(0xa6);
    
    emiCmdOut8(0xaf);
}
/*****************************************************************************
*Function Name: LcdInit
*
*Function description: LCD initialization
*
*Input parameters: None
*
*Output parameters: None
*
*Return Value: Initialization succeeds or fails
*****************************************************************************/
BSP_S32 LcdInit(BSP_VOID)
{
    EMI_SCDUL_PARA_S sScdulPara;
    EMI_SCDUL_MODE_E eSdulMode;

    memset(&sScdulPara, 0, sizeof(EMI_SCDUL_PARA_S));
    eSdulMode = g_bEmiScdul;
    (BSP_VOID)emiInit();
	/* BEIGN ,Deleted by xuchao,2012/2/14 */
    //(BSP_VOID)tftReset(); The screen will flash once after opening
    /* END ,Deleted by xuchao,2012/2/14 */
    switch(eSdulMode)
    {
        case EMI_TFT_8BIT:
        case EMI_TFT_9BIT:
        {
            // Read 

            sScdulPara.u32Wst3 = 1; //20ns

            sScdulPara.u32Wst1 = 24;
            sScdulPara.u32OEnDelay = 2; 
            sScdulPara.u32OEn= 2;

            // Write 

            sScdulPara.u32Wst2= 3; //20ns

            sScdulPara.u32Wst3= 1;
            sScdulPara.u32WEnDelay = 0; 
            sScdulPara.u32WEn   = 1;

            (BSP_VOID)emiSetScdul(eSdulMode, &sScdulPara); /*lint !e718*/

            (BSP_VOID)tftlcdInit();
        }
            break;
        case EMI_STN_8BIT_M6800:
        case EMI_STN_8BIT_I8080:
        {
            // Read 

            sScdulPara.u32Wst1 = 10;
            sScdulPara.u32Wst3 = 3; //100ns

            sScdulPara.u32OEnDelay = 0; 
            sScdulPara.u32OEn  = 0;
            sScdulPara.u32UpDlyr = 1;
            sScdulPara.u32DdnDlyr= 9;

            // Write 

            sScdulPara.u32Wst2 = 10; //100ns

            sScdulPara.u32Wst3 = 3;
            sScdulPara.u32WEnDelay = 0; 
            sScdulPara.u32WEn  = 0;
            sScdulPara.u32UpDlyw = 2;
            sScdulPara.u32DdnDlyw= 7;

            emiSetScdul(eSdulMode, &sScdulPara); 

            stnlcdInit();
        }
            break;
        default:
            break;
    }
        
    return BSP_OK;
}
BSP_S32 LcdReInit(BSP_VOID)
{
    EMI_SCDUL_PARA_S sScdulPara;
    EMI_SCDUL_MODE_E eSdulMode;
    memset(&sScdulPara, 0, sizeof(EMI_SCDUL_PARA_S));
    eSdulMode = g_bEmiScdul;
    /* Open ld o13 */
  //  BSP_PMU_LDOCtrl(TFT_LCD_PWR_LDO, BSP_TRUE);
    /* Current source is 9mA */
  //  BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR, 6);


    /*Reset emi*/
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, 1); //
    
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, CRG_RST_DIS);
    /*Turn on the clock*/
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, CRG_CLK_OFFSET, 1, CRG_CLK_VALID);
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, 17, 1, CRG_CLK_VALID);
    /* Set chip selection signal low valid*/
    EMI_REG_SETBITS(EMI_CR0_OFFSET,1,1,0);
	/* BEIGN ,Deleted by xuchao,2012/2/14 */
    //(BSP_VOID)tftReset(); The screen will flash once after opening
    /* END ,Deleted by xuchao,2012/2/14 */
    switch(eSdulMode)
    {
        case EMI_TFT_8BIT:
        case EMI_TFT_9BIT:
        {
            // Read 

            sScdulPara.u32Wst3 = 1; //20ns

            sScdulPara.u32Wst1 = 24;
            sScdulPara.u32OEnDelay = 2; 
            sScdulPara.u32OEn= 2;

            // Write 

            sScdulPara.u32Wst2= 3; //20ns

            sScdulPara.u32Wst3= 1;
            sScdulPara.u32WEnDelay = 0; 
            sScdulPara.u32WEn   = 1;

            (BSP_VOID)emiSetScdul(eSdulMode, &sScdulPara); /*lint !e718*/
           // (bsp void)tftlcd init();

        }
            break;
        case EMI_STN_8BIT_M6800:
        case EMI_STN_8BIT_I8080:
        {
            // Read 

            sScdulPara.u32Wst1 = 10;
            sScdulPara.u32Wst3 = 3; //100ns

            sScdulPara.u32OEnDelay = 0; 
            sScdulPara.u32OEn  = 0;
            sScdulPara.u32UpDlyr = 1;
            sScdulPara.u32DdnDlyr= 9;

            // Write 

            sScdulPara.u32Wst2 = 10; //100ns

            sScdulPara.u32Wst3 = 3;
            sScdulPara.u32WEnDelay = 0; 
            sScdulPara.u32WEn  = 0;
            sScdulPara.u32UpDlyw = 2;
            sScdulPara.u32DdnDlyw= 7;
            emiSetScdul(eSdulMode, &sScdulPara); 

          // Stnlcd heat ();

        }
            break;
        default:
            break;
    }
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, 17, 1, 0);
    return BSP_OK;
}
EXPORT_SYMBOL(LcdReInit);

/*****************************************************************************
  Function    : tftReset
  Description : tftReset
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
BSP_VOID tftReset(BSP_VOID)
{
    /*lint -e778*/
    CRG_REG_SETBITS(CS_CTRL3_OFFSET, 0, 1, 0); /* Reset*/
    msleep(10);

    CRG_REG_SETBITS(CS_CTRL3_OFFSET, 0, 1, 1); /* Dereset*/
    msleep(50);
	/*lint +e778*/
	rstCnt++;
}

BSP_U32 tftRstCntGet(void)
{
	return rstCnt;
}

/*****************************************************************************
  Function    : tftPwrOn
  Description : tftPwrOn
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
BSP_VOID tftPwrOn(BSP_VOID)
{
/* BEIGN ,Modified by xuchao,2012/2/14 */
    PWRCTRL_EMI_LowPowerExit();
	if (LCD_PWR_ON_STATE != g_ulOledPwrOnState)
	{
		LcdInit();//Initialize first and then turn on the backlight, Xu Chao 2012-02-14

		g_ulOledPwrOnState = LCD_PWR_ON_STATE;
	}
	else
	{
		hitft_trace(5, "tft has been powered on.\n");
	}
	
	/*The screen is white just after initialization, and the backlight cannot be turned on immediately. You need to call this function and refresh the screen before turning on the backlight.*/
	//Emi set back light(bsp true);

	
/* END ,Modified by xuchao,2012/2/14 */
}
EXPORT_SYMBOL(tftPwrOn);

/*****************************************************************************
  Function    : tftPwrOff
  Description : tftPwrOff
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
BSP_VOID tftPwrOff(BSP_VOID)
{
	/*It is powered off, no need to power off again*/
	if (LCD_PWR_OFF_STATE == g_ulOledPwrOnState)
	{
		hitft_trace(5, "tft has been powered off.\n");
		return;
	}

	/*Turn off the backlight*/
	emiSetBackLight(BSP_FALSE);
	
	/* tft enter sleepin mode  */
	tftSleep();
	
#if  (FEATURE_E5 == FEATURE_ON) && (FEATURE_E5_UDP != FEATURE_ON)
    /* Close ld o13 */
#if defined(FEATURE_SHORT_ON_OFF)
    /* When the fake shutdown function is turned on, the screen enters sleep in mode and does not turn off LDO13. */
    tftClearWholeScreen();
#else
    BSP_PMU_LDOCtrl(TFT_LCD_PWR_LDO, BSP_FALSE);
#endif
#endif
	g_ulOledPwrOnState = LCD_PWR_OFF_STATE;
	PWRCTRL_EMI_LowPowerEnter();
}
EXPORT_SYMBOL(tftPwrOff);

/*****************************************************************************
  Function    : tftLight
  Description : tftLight
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
BSP_VOID tftLight(BSP_VOID)
{   
	BSP_S32 ret = 0;
	
	if (LCD_PWR_ON_STATE != g_ulOledPwrOnState)
	{
		return;
	}
	down(&g_ulOledSemMux);

	PWRCTRL_EMI_LowPowerExit();
    emiCmdOut8(0x11);
	msleep(120);
	/*display on*/
    emiCmdOut8(0x29);
	msleep(120);	

    /*Turn on the backlight*/
  
	ret = emiSetBackLight(BSP_TRUE);
	
	if( BSP_OK == ret )
	{
		g_bIsLight = BSP_TRUE;
	}
    else
    {
    	hitft_error("tftLight failed for emiSetBackLight fail.");
    }
	
	up(&g_ulOledSemMux);
}
EXPORT_SYMBOL(tftLight);

#if(FEATURE_ON == FEATURE_HUAWEI_VDF)

/*****************************************************************************
  Function    : tftDim
  Description : tftDim
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:       : 2013-07-04
*****************************************************************************/

BSP_VOID tftDim(BSP_VOID)
{

    BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR,Dim_light);
}
EXPORT_SYMBOL(tftDim);

#endif


/*****************************************************************************
  Function    : tftSleep
  Description : tftSleep
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
BSP_VOID tftSleep(BSP_VOID)
{
	BSP_S32 ret = 0;
	down(&g_ulOledSemMux);
    /*Turn off the backlight*/
	ret = emiSetBackLight(BSP_FALSE);
	if(BSP_OK == ret)
	{
		/*close display*/
		g_bIsLight = BSP_FALSE;
	    emiCmdOut8(0x10);
		PWRCTRL_EMI_LowPowerEnter();
	}
	else
	{
		hitft_error("tftSleep failed for emiSetBackLight fail.");
	}
	up(&g_ulOledSemMux);
}
EXPORT_SYMBOL(tftSleep);


/*****************************************************************************
  Function: tftInitImage
  Description: Display the startup initialization screen
  Calls: 
  Called By: 
  Input : NULL
  Output : NULL
  Return : NULL     
  Date: 2012-02-10
*****************************************************************************/
BSP_VOID tftInitImage(BSP_VOID)
{
	BSP_S32 ret = 0;
	/* BEIGN ,Modified by xuchao,2012/2/14 */
    tftClearWholeScreen();
	ret = tftRefreshByBit(0, 0,  LCD_WIDTH, LCD_HIGHT, (UINT8 *)gImage_init);
	/* END ,Modified by xuchao,2012/2/14 */
	if (0 > ret)
	{
		hitft_error("tftRefresh failed.");
	}	
}
EXPORT_SYMBOL(tftInitImage);

/*****************************************************************************
  Function: tftUpdateDisplay
  Description: tft displays the image of the upgrade successfully
  Calls: 
  Called By: 
  Input: UpdateStatus:TRUE:Upgrade successful FALSE:Upgrade failed
  Output : NULL
  Return : NULL      
*****************************************************************************/
void tftUpdateDisplay(int UpdateStatus)
{
    tftClearWholeScreen();
    if(UpdateStatus)
    {
        tftRefreshByBit(0, 40, 128, 48, (UINT8 *)g_aucUpdateOKIconPic);
    }
    else
    {
        tftRefreshByBit(0, 40, 128, 48, (UINT8 *)g_aucUpdateFailIconPic);
    }
    return;
}
EXPORT_SYMBOL(tftUpdateDisplay);

/*****************************************************************************
  Function    : tft_get_frame_buffer
  Description : tft_get_frame_buffer
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:          2010-09-16
*****************************************************************************/
static BSP_VOID tft_get_frame_buffer(unsigned char *buffer)
{
   unsigned int i = 0;
   hitft_trace(1,"-------------%s,%d--------- \n",__FUNCTION__,  __LINE__);
   copy_to_user(buffer, buffer_t, SCREEN_BUFFER_SIZE);
   for(i = 0; i< SCREEN_BUFFER_SIZE; i++)
   hitft_trace(1,"0x%x  ", buffer_t[i]);
   hitft_trace(1,"end of get frame buffer");
}

/*************************************************
*Function: tftClear
*Description: tft clear *
*Called By:AP
*Table Accessed:
*Table Updated:
*Input:
*UINT32 ulXStart, UINT32 ulYStart---Start coordinates
*UINT32 ulXEnd, UINT32 ulYEnd－-horizontal vertical offset
*Output:
*N/A
*Return:
*N/A
*************************************************/
BSP_S32 tftClear(UINT32 ulXStart, UINT32 ulYStart,UINT32 ulXOffset, UINT32 ulYOffset)
{
	UINT32 ulXloop = 0x00;
	UINT32 ulYloop = 0x00;
	UINT32 ulXEnd = (ulXStart + ulXOffset);
	UINT32 ulYEnd = (ulYStart + ulYOffset);
	
	if ((ulXEnd > LCD_WIDTH) || (ulYEnd > LCD_HIGHT))
	{
		hitft_error("tftClear para error!! ");
		return ERROR;
	}
    /*When a bad board is detected in factory mode, x is displayed. At this time, other workstations are prohibited from flushing the screen.*/
    if(BSP_TRUE == is_ftm_bad_board)
    {
        return OK;
    }
	
	/*Mutex semaphore protection is required*/
	down(&g_ulOledSemMux);
	
    /*Set the column address, the high position is in front*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*Set the line address, the high position is in front*/
	emiCmdOut8(0x2B);
    emiDataOut8((ulYStart>>8)&0xff);
    emiDataOut8(ulYStart&0xff);
    emiDataOut8((ulYEnd>>8)&0xff);
    emiDataOut8(ulYEnd&0xff);
    
	/*output follows RAM content*/
	emiCmdOut8(0x2C); 
	
	for (ulYloop = ulYStart; ulYloop < ulYEnd; ulYloop++)
	{
		for (ulXloop = ulXStart; ulXloop < ulXEnd; ulXloop++)
		{
			emiDataOut8(0x00);
			emiDataOut8(0x00);
		}
	}
    
	/* disable the EMI clock gating */
	/* Clock gating disable(clock gating emi); */
    if(NULL != buffer_t)
    {
        memset(buffer_t ,0 ,SCREEN_BUFFER_SIZE);
    }
	up(&g_ulOledSemMux);
	
	return OK;   
}
EXPORT_SYMBOL(tftClear);

BSP_S32 tftWhite(UINT32 ulXStart, UINT32 ulYStart,UINT32 ulXOffset, UINT32 ulYOffset)
{
	UINT32 ulXloop = 0x00;
	UINT32 ulYloop = 0x00;
	UINT32 ulXEnd = (ulXStart + ulXOffset);
	UINT32 ulYEnd = (ulYStart + ulYOffset);
	
	if ((ulXEnd > LCD_WIDTH) || (ulYEnd > LCD_HIGHT)  || (0 != (ulYStart % 8)) ||(0 != (ulYEnd % 8)) )
	{
		hitft_error("tftClear para error!! ");
		return ERROR;
	}
    /*When a bad board is detected in factory mode, x is displayed. At this time, other workstations are prohibited from flushing the screen.*/
    if(BSP_TRUE == is_ftm_bad_board)
    {
        return OK;
    }
	
	/*Mutex semaphore protection is required*/
	down(&g_ulOledSemMux);
	
    /*Set the column address, the high position is in front*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*Set the line address, the high position is in front*/
	emiCmdOut8(0x2B);
    emiDataOut8((ulYStart>>8)&0xff);
    emiDataOut8(ulYStart&0xff);
    emiDataOut8((ulYEnd>>8)&0xff);
    emiDataOut8(ulYEnd&0xff);
    
	/*output follows RAM content*/
	emiCmdOut8(0x2C); 
	
	for (ulYloop = ulYStart; ulYloop < ulYEnd; ulYloop++)
	{
		for (ulXloop = ulXStart; ulXloop < ulXEnd; ulXloop++)
		{
			emiDataOut8(0xFF);
			emiDataOut8(0xFF);
		}
	}
    
	/* disable the EMI clock gating */
	/* Clock gating disable(clock gating emi); */
	memset(buffer_t ,0xFF ,SCREEN_BUFFER_SIZE);
	up(&g_ulOledSemMux);
	
	return OK;   
}
EXPORT_SYMBOL(tftWhite);


/*************************************************
* Function:  tftClearWholeScreen
* Description: tft clear *
* Called By:AP
* Table Accessed:
* Table Updated:
* Input:
*        N/A
* Output:
*        N/A
* Return:
*        N/A
*************************************************/
BSP_VOID tftClearWholeScreen(BSP_VOID)
{
	BSP_S32 ret = 0;
	ret = tftClear(0, 0, LCD_WIDTH, LCD_HIGHT);
	if (0 > ret)
	{
		hitft_error("tftClear failed.");
	}
	return;
}
EXPORT_SYMBOL(tftClearWholeScreen);

/*************************************************
*Function: tftRefresh
*Description: tft refresh *
*Called By:AP
*Table Accessed:
*Table Updated:
*Input:
*UINT32 ulXStart, UINT32 ulYStart---Start coordinates
*UINT32 ulXOffset, UINT32 ulYOffset---horizontal vertical offset
*UINT8 *pucBuffer－Display data

*Output:
*N/A
*Return:
*N/A
*************************************************/
BSP_S32 tftRefresh(UINT32 ulXStart, UINT32 ulYStart, UINT32 ulXOffset, UINT32 ulYOffset, UINT8 *pucBuffer)
{
	UINT32 ulXloop = 0x00;
	UINT32 ulYloop = 0x00;
    UINT32 i = 0;
	UINT32 ulXEnd = (ulXStart + ulXOffset -1);
	UINT32 ulYEnd = (ulYStart + ulYOffset -1);
	/*Input parameter validity check*/
	if (NULL == pucBuffer)
	{
	    return ERROR;
	}
	if ((ulXEnd >= LCD_WIDTH) || (ulYEnd >= LCD_HIGHT))
	{
		hitft_error("tftRefresh para error!! ");
		return ERROR;
	}
	
	/*Avoid the app brushing the screen white, check the dot in the upper left corner of the screen, and do not brush it if it is white*/
    if(pucBuffer[0] == 0xFF && pucBuffer[1] == 0xFF)
    {
    	hitft_error("XXXXX APP want Refresh LCD to all white XXXXX!! ");
    	return OK;
    }
	
	/*Mutex semaphore protection is required*/
	down(&g_ulOledSemMux);
    
	
    /*Set the column address, the high position is in front*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*Set the line address, the high position is in front*/
	emiCmdOut8(0x2B);
    emiDataOut8((ulYStart>>8)&0xff);
    emiDataOut8(ulYStart&0xff);
    emiDataOut8((ulYEnd>>8)&0xff);
    emiDataOut8(ulYEnd&0xff);
    
	/*output follows RAM content*/
	emiCmdOut8(0x2C); 
	
    for (i = 0; i < 2*(ulXEnd - ulXStart + 1)*(ulYEnd - ulYStart + 1); i++)
    {
        buffer_t[LCD_HIGHT*2*ulYStart + 2*ulXStart + i] = *(pucBuffer + i);        
    }
	
	for (ulYloop = ulYStart; ulYloop <= ulYEnd; ulYloop++)
	{
		for (ulXloop = ulXStart; ulXloop <= ulXEnd; ulXloop++)
		{
			emiDataOut8(*pucBuffer++);
			emiDataOut8(*pucBuffer++);
		}
	}

    
	/* disable the EMI clock gating */
	/* Clock gating disable(clock gating emi); */

	up(&g_ulOledSemMux);

	return OK;   
}
EXPORT_SYMBOL(tftRefresh);

/* BEIGN ,Added by xuchao,2012/2/14 */
/*************************************************
*Function: tftRefreshByBit
*Description: Displays the bitwise stored image, with the high bit in front, and one byte stores eight points
*Called By:
*Input:
*BSP_U32 ulXStart, BSP_U32 ulYStart---Start coordinates
*BSP_U32 ulXOffset, BSP_U32 ulYOffset---horizontal vertical offset
*BSP_U8 *pucBuffer--Display data, one byte represents eight dots, 1 is white, 0 is black

*Output:
*BSP_FALSE: The parameter exceeds the screen boundary or is empty
*BSP_OK: Normal display
*Return:
*N/A
*************************************************/
BSP_S32 tftRefreshByBit(UINT32 ulXStart, UINT32 ulYStart, UINT32 ulXOffset, UINT32 ulYOffset, UINT8 *pucBuffer)
{
	UINT32 ulXloop = 0x00;
	UINT32 ulYloop = 0x00;

#if defined(FEATURE_LCD_BOTTOM)
	ulYStart = ulYStart + 26;
	if(102 < ulYOffset)
	{
	    ulYOffset = 102;
	}
#endif
	
	UINT32 ulXEnd = (ulXStart + ulXOffset -1);
	UINT32 ulYEnd = (ulYStart + ulYOffset -1);

	UINT32 bitNum = 0; //Used when looping in each byte

	UINT8 *buffer = pucBuffer;
	
	/*Input parameter validity check*/
	if (NULL == pucBuffer)
	{
	    return ERROR;
	}
	
	if ((ulXEnd >= LCD_WIDTH) || (ulYEnd >= LCD_HIGHT))
	{
		return ERROR;
	}
    /*When a bad board is detected in factory mode, x is displayed. At this time, other workstations are prohibited from flushing the screen.*/
    if(BSP_TRUE == is_ftm_bad_board)
    {
        return OK;
    }
	/*Mutex semaphore protection is required*/
	down(&g_ulOledSemMux);
	
    /*Set the column address, the high position is in front*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*Set the line address, the high position is in front*/
	emiCmdOut8(0x2B);
    emiDataOut8((ulYStart>>8)&0xff);
    emiDataOut8(ulYStart&0xff);
    emiDataOut8((ulYEnd>>8)&0xff);
    emiDataOut8(ulYEnd&0xff);
    
	/*output follows RAM content*/
	emiCmdOut8(0x2C); 
	
	for (ulYloop = ulYStart; ulYloop <= ulYEnd; ulYloop++)
	{
		for (ulXloop = ulXStart; ulXloop <= ulXEnd; ulXloop++)
		{
			if( bitNum ==8 )
			{//When one byte output is finished, the pointer is moved backward and the bit flag is cleared

				buffer++;
				bitNum=0;
			}
			if( (((*buffer)>>(7-bitNum)) & 0x01) == 0x01)
			{//Start to judge from a high position

				emiDataOut8(0xFF);
				emiDataOut8(0xFF);
			}
			else
			{
				emiDataOut8(0x00);
				emiDataOut8(0x00);
			}
			bitNum++;
		}
	}
	up(&g_ulOledSemMux);

	return OK;   
}
EXPORT_SYMBOL(tftRefreshByBit);
/* END ,Added by xuchao,2012/2/14 */

/****************************************************
*set x/y position for write image
*off = x_position + y_position *LCD_WIDTH
****************************************************/
static loff_t balong_tft_lseek(struct file *file, loff_t off, BSP_S32 whence)
{
	/* for pclint */
	if (NULL == file && 0 == whence)
	{
		;
	}
	
	x_position = (UINT32)off % LCD_WIDTH;
	y_position = (UINT32)off / LCD_WIDTH;
	
	hitft_error("%s off=%d  x_position=%d  y_position=%d\n", __func__, (BSP_S32)off, x_position, y_position);

	return 0;
}
/* BEIGN ,Motified by xuchao,2012/2/14 */
unsigned char tft_buff[LCD_WIDTH * LCD_HIGHT * 2];
/* END   ,Motified by xuchao,2012/2/14 */
static ssize_t balong_tft_write(struct file *file, const char __user *buf, size_t count,
					loff_t *ppos)
{
	UINT8 *image = NULL;
	BSP_S32 ret = 0;
	/* BEIGN ,Added by xuchao,2012/2/14 */
	BSP_S32 bufCount =LCD_WIDTH * LCD_HIGHT * 2;
	
#if (FEATURE_E5 == FEATURE_ON)
	if(BSP_CHGC_SplyStGet())
	{
		return 0;
	}
	else
#endif		
	{
		/* END   ,Added by xuchao,2012/2/14 */
		/* for pclint */
		if (NULL == file && NULL == ppos)
		{
			;
		}
	#if ( FEATURE_MMI_TEST == FEATURE_ON )			
		if ( true == mmi_test_mode )
		{
		    /*If in mmi test mode, it will not be processed*/
		    return 0;
		}
	#endif	
	    /* BEIGN ,Motified by xuchao,2012/2/14 */
		if (count > (BSP_U32)bufCount) {
	    /* END   ,Motified by xuchao,2012/2/14 */
			hitft_error("balong_tft_write too many bytes!");
			return -EFAULT;
		}
	
	image = &tft_buff[0];
	hitft_trace(1,"count=%d    x_position=%d   y_position=%d", count, x_position, y_position);
	if (copy_from_user((BSP_VOID*)image, buf, count))
	{
		return -EFAULT;
	}

	ret = tftRefresh(x_position, y_position,  LCD_WIDTH - x_position, LCD_HIGHT - y_position, image);
	if (0 > ret)
	{
		printk("tftRefresh failed.\n");
	}

		return (ssize_t)count;		
	}
}

static BSP_S32 balong_tft_open(struct inode *inode, struct file *file)
{
	hitft_trace(1," ------------------ \n");
	/* for pclint */
	if (NULL != inode && NULL != file)
	{
		;
	}
	return 0;
}

static BSP_S32 balong_tft_release(struct inode *inode, struct file *file)
{
	hitft_trace(1," ------------------ \n");
	/* for pclint */
	if (NULL != inode && NULL != file)
	{
		;
	}
	return 0;
}

static void balong_tft_exit_low_power_mode(void)
{
	if (LCD_PWR_ON_STATE != g_ulOledPwrOnState)
	{
		return;
	}

	PWRCTRL_EMI_LowPowerExit();
    emiCmdOut8(0x11);
	msleep(120);
    /*display on*/
    emiCmdOut8(0x29);
	msleep(120);

}

/*************************************************
* Function:  tftIoctrl
* Description: tft ioctrl *
* Called By:AP
* Table Accessed:
* Table Updated:
* Input:
*         int cmd --command ID
*         arg--para

* Output:
*        N/A
* Return:
*        N/A
*************************************************/
BSP_S32 balong_tft_ioctl(struct file *file, BSP_U32 cmd, unsigned long arg)
{
	unsigned char value = 0;
	tft_write_data image_data;
	BSP_S32 ret = 0;
	unsigned char* buffer_kernel = NULL;
    signed long image_arg = 0;

	printk(KERN_DEBUG "balong_tft_ioctl: cmd=%u  arg=%lu\n",  cmd, arg);

	/* for pclint */
	if (NULL == file)
	{
		;
	}
#if ( FEATURE_MMI_TEST == FEATURE_ON )			
	else if ( true == mmi_test_mode )
	{
	    /*If in mmi test mode, it will not be processed*/
	    return 0;
	}

#endif	

#if ( FEATURE_E5 == FEATURE_ON )			
	if(BSP_CHGC_SplyStGet())
	{		
		switch (cmd)
		{		
			case OLED_BATS_PASS:
				tftClearWholeScreen();
				tftUpdateDisplay(TRUE);
				break;
			case OLED_BATS_FAIL:
				balong_tft_exit_low_power_mode();
				tftClearWholeScreen();
				tftUpdateDisplay(FALSE);
				break;
			default:
				break;
		}
		return 0;
	}
#endif

	/* Enable the EMI clock gating */
	/* Clock gating enable(clock gating emi); */

	switch (cmd)
	{
		case OLED_LIGHT: /*Light up the LCD backlight*/     
			/* Pwrctrl lcd lowpower exit(); */
			tftLight();
			break;
		case OLED_SLEEP: /*Turn off LCD backlight*/
			tftSleep();
			/* Pwrctrl lcd lowpower enter(); */
			break;
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
        case OLED_DIM:  /*Dim lcd mode*/
            tftDim();
            break;
#endif
		case OLED_RESET:/*Reset lcd*/
			/*Calling the TFT reset interface alone will cause the screen to be white and cannot be restored. If you do not turn on the backlight, you need to refresh the screen before turning on the backlight.*/
			tftPwrOff();
			msleep(100);
			tftPwrOn();
			break;
		case OLED_GET_FRAME_BUFFER:/*Get image data*/
			tft_get_frame_buffer((unsigned char*)arg);
			break; 			
		case OLED_UPDATE:/*Upgrade software, not implemented here*/
			/*Tft update();*/
			break;            
		case OLED_CONTRAST:/*Adjust the contrast*/
			value = (unsigned char)arg;
			hitft_trace(1,"LCD_CONTRAST --------- value=0x%x\n", value);
			emiCmdOut8(0x81);	//set contrast control.

			emiCmdOut8(value);//second byte.

			break;
		case OLED_BRIGHTNESS:/*Adjust brightness*/
			/* not support now */
			/*
			emiCmdOut8(0x82);
			emiCmdOut8(value);
			*/
			break;
		case OLED_POWER_ON:/*Power on Lcd*/
			/* Pwrctrl lcd lowpower exit(); */
#if defined(FEATURE_SHORT_ON_OFF)
            /* When the false shutdown function is turned on, the screen enters sleep in mode and does not turn off LDO13, so there is no need to re-initialize it to speed up the screen turning on. */
            g_ulOledPwrOnState = LCD_PWR_ON_STATE;
#else
			tftPwrOn();
#endif
			break;
		case OLED_POWER_OFF:/*Lcd power down*/
			/* Pwrctrl lcd lowpower enter(); */
			tftPwrOff();
			break;
		case OLED_DEL_TIMER:
			/* BEGIN PN: Added by xuchao x00202188, 2012/2/18 */
			if( 1 == tft_poweroff_init_flag)
			{//If the shutdown charging screen is activated, the full battery screen is closed and displayed

				tft_poweroff_init_flag = 0;
				ret = cancel_delayed_work_sync(&tft_pwroff_chg_work);
				if (0 > ret)
				{
					hitft_error("cancel_delayed_work_sync failed.");
				}
			
				/*Add to display full charge picture*/
				tftRefreshByBit(0, 0, LCD_X_OFFSET_128, LCD_Y_OFFSET_128, (UINT8 *)g_chgImage[MAX_OFF_CHARGING_ANI -1]);
			}	
			/* END   PN: Added by xuchao x00202188, 2012/2/18 */
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
            ret = cancel_delayed_work_sync(&tft_pwron_ani_work);
            if (0 > ret)
            {
                hitft_error("cancel_delayed_work_sync tft_pwron_ani_work failed.");
            }
#endif
			break;
		case OLED_SYS_POWER_DOWN:/*Power off the system*/
			/* Pwrctrl lcd lowpower enter(); */
			tftPwrOff();
			break;
		case OLED_INIT_IMAGE:/*Lcd initialization*/
			tftInitImage();
			break;
		case OLED_POWER_OFF_CHARGING_START:/*Start shutdown charging animation*/
            /*if power off charging already started,do nothing*/
			if (0 == tft_poweroff_init_flag)
			{
			    g_ulPicCount = 0;
				ret = tftPwrOffChgAniInstall();
				if (0 > ret)
				{
					hitft_error("install power off charge failed.");
					return ERROR;
				}
				tft_poweroff_init_flag = 1;			
			    /*If lcd has been initialized, wake up the charging animation display task*/
			    if(BSP_TRUE == g_bTftInit)
			    {
				    up(&g_ulOledSemSyn);			
			    }
			}
			break;
		case OLED_POWER_OFF_CHARGING_STOP:/*Turn off the shutdown charging animation*/
			/* stop power off charging picture */
			tft_poweroff_init_flag = 0;
			ret = cancel_delayed_work_sync(&tft_pwroff_chg_work);
			if (0 > ret)
			{
				hitft_error("cancel_delayed_work_sync failed.");
			}
			
			/*Add to display full charge picture*/
            image_arg = (signed long)arg;
			if (image_arg >= MAX_OFF_CHARGING_ANI)
			{
				image_arg = MAX_OFF_CHARGING_ANI -1;
			}
            else if(image_arg < 0)
            {
                image_arg = 0;
            }
            else
            {
            }
			/* BEIGN ,Modified by xuchao,2012/2/14 */
			ret = tftRefreshByBit(0, 0, LCD_X_OFFSET_128, LCD_Y_OFFSET_128, (UINT8 *)g_chgImage[image_arg]);
			/* END   ,Modified by xuchao,2012/2/14 */
			if (0 > ret)
			{
				hitft_error("tftRefresh failed.");
			}
			break;
            
		case OLED_WRITE_IMAGE:/*Write to the image*/
            if(file)
            {
    			if (copy_from_user(&image_data, (BSP_VOID __user *)arg, sizeof(tft_write_data)))
    			{
    			    printk(KERN_ERR"struct copy error!\n");
    				return -EFAULT;
    			}
    			if(!image_data.image || copy_from_user(buffer_t, (BSP_VOID __user *)image_data.image,
    			    image_data.x_offset*image_data.y_offset))
    			{
    			    printk(KERN_ERR"image copy error!\n");
    			    return -EFAULT;
    			}
    			ret = tftRefresh(image_data.x_start, image_data.y_start,  image_data.x_offset, image_data.y_offset, buffer_kernel);  		            
    		}
    		else
    		{
    		    /*ioctl called from kernel space*/
    		    memcpy(&image_data, (void*)arg, sizeof(tft_write_data));

				ret = tftRefresh(image_data.x_start, image_data.y_start,  image_data.x_offset, image_data.y_offset, image_data.image);

    		}
			if (0 > ret)
			{
				hitft_error("tftRefresh failed.");
			}
            
			break;
        case OLED_ALL_WHITE:/*Write to full screen, all white */
		    /* BEIGN ,Modified by xuchao,2012/2/14 */
			tftWhite(0, 0, LCD_WIDTH, LCD_HIGHT);
			/* END   ,Modified by xuchao,2012/2/14 */
			break;
        case OLED_ALL_BLACK:/*All black screen */
		    /* BEIGN ,Modified by xuchao,2012/2/14 */
			tftClear(0, 0, LCD_WIDTH, LCD_HIGHT);
			/* END   ,Modified by xuchao,2012/2/14 */
			break;				
        /*The bad plate was detected in the filament version, and the "X" is displayed*/
        case OLED_BATS_FAIL:/*Show "X"*/    
            if(FTM_BAD_BOARD == arg)
            {
                if(BSP_TRUE == g_bTftInit)
                {
                    tftClearWholeScreen();
                    tftUpdateDisplay(FALSE);
                    is_ftm_bad_board = BSP_TRUE;
                    printk(KERN_ERR "tftRefresh is forbidden.\n");
                }
                else
                {
                    /*The burned-out version detects a bad board and displays "X". You need to wait for tft to be initialized before refreshing.*/
                    refresh_tft_when_inited = BSP_TRUE; 
                }
            }
            else
            {
                tftClearWholeScreen();
                tftUpdateDisplay(FALSE);
            }
            break;
		default:
		    return -1;
	/*BEGIN:y00206456 2012-04-26 Modified for pclint e527*/
    /*END:y00206456 2012-04-26 Modified for pclint e527*/
	}

	/* disable the EMI clock gating */
	/* Clock gating disable(clock gating emi); */

	return 0;
}
EXPORT_SYMBOL(balong_tft_ioctl);


/*****************************************************************************
  Function    : tftCharDisplay
  Description : tftCharDisplay
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : OK              ERROR      
*****************************************************************************/
BSP_VOID tftCharDisplay(UINT8 ucX, UINT8 ucY, UINT8 *pucChar)
{
	BSP_S32 ret = 0;
	UINT8 *pucStr = NULL;

	if ((*pucChar >= 'A' )&& (*pucChar <= 'Z' ))
	{
		pucStr = (UINT8 *)(g_aucStr_A_Z_8x16 + ((*pucChar -'A')*16));
	}
	else if ((*pucChar >= 'a' ) && (*pucChar <= 'z'))
	{
		pucStr = (UINT8 *)(g_aucStr_a_z_8x16 + ((*pucChar-'a')*16));
	}
	else if ((*pucChar >= '0' )&& (*pucChar <= '9'))
	{
		pucStr = (UINT8 *)(g_aucNum_0_9_8x16 + ((*pucChar-'0')*16));
	}
	else if (*pucChar == ' ')
	{
		pucStr = (UINT8 *) g_aucSpace;
	}
	else if (*pucChar == '!')
	{
		pucStr = (UINT8 *)g_aucExclamatory;
	}
	else
	{
	}
    /* BEIGN ,Modified by xuchao,2012/2/14 */
	ret = tftRefreshByBit(ucX, ucY, 8, 16, pucStr);
	/* END   ,Modified by xuchao,2012/2/14 */
	if (0 > ret)
	{
		hitft_error("tftRefresh failed.");
	}

	return;
}
EXPORT_SYMBOL(tftCharDisplay);

/*****************************************************************************
  Function    : tftStringDisplay
  Description : tftStringDisplay
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : OK              ERROR      
*****************************************************************************/
BSP_VOID tftStringDisplay(UINT8 ucX, UINT8 ucY, UINT8 *pucStr)
{
	while ('\0' != *pucStr)
	{
		tftCharDisplay(ucX, ucY, pucStr++);
		ucX += 8;
		if (ucX > 127)
		{
			ucX = 0;
			ucY += 16;
		}
	}
}
EXPORT_SYMBOL(tftStringDisplay);

/*****************************************************************************
  Function: tftPwrOffChgAniIsr
  Description: Shut down and charge interrupt ISR
  Calls: 
  Called By: 
  Input : NULL
  Output : NULL
  Return : OK ERROR      
*****************************************************************************/
static BSP_VOID tftPwrOffChgAniIsr(struct work_struct *work)
{
	BSP_S32 ret = 0;

	if (NULL == work)
	{
		hitft_error("work is NULL and return.");
		return;
	}

	ret = queue_delayed_work(tft_queue, &tft_pwroff_chg_work, msecs_to_jiffies(1000));
	if (0 == ret)
	{
		hitft_error("queue_delayed_work failed.");
	}

	up(&g_ulOledSemSyn);
}
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
/*****************************************************************************
 Prototype    : tftPwrOnAniIsr
 Description  : 开机动画显示
 Input        : struct work_struct *work  
 Output       : None
 Return Value : static BSP_VOID
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/12
    Author       : jujianbo
    Modification : Created function

*****************************************************************************/
static BSP_VOID tftPwrOnAniIsr(struct work_struct *work)
{
    BSP_S32 ret = 0;

    if (NULL == work)
    {
        hitft_error("work is NULL and return.");
        return;
    }

    ret = queue_delayed_work(tft_queue, &tft_pwron_ani_work, msecs_to_jiffies(550));
    if (0 == ret)
    {
        hitft_error("queue_delayed_work failed.");
    }

    up(&g_ulOledPowerOnAniSemSyn);
}
#endif

/*****************************************************************************
  Function    : tftChgTaskFunc
  Description : tftChgTaskFunc
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : OK              ERROR      
*****************************************************************************/
static BSP_S32 tftChgTaskFunc(BSP_VOID *arg)
{
	BSP_S32 ret = 0;

	/* for pclint */
	if (NULL == arg)
	{
		;
	}

	for(;;)
	{
		down(&g_ulOledSemSyn);

		if (g_ulPicCount >= MAX_OFF_CHARGING_ANI)
		{
			g_ulPicCount = 0;
		}
        /* BEIGN ,Modified by xuchao,2012/2/14 */
		ret = tftRefreshByBit(0, 0, LCD_WIDTH, LCD_HIGHT, g_chgImage[g_ulPicCount]);
		/* END   ,Modified by xuchao,2012/2/14 */
		if (0 > ret)
		{
			hitft_error("tftRefresh failed.");
			return ret;
		}

		/*Only move when the screen is on, and still move when the screen is off*/
		if( BSP_TRUE == g_bIsLight )
		{
			g_ulPicCount++;
		}
	}

	return 0;
}

#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
/*****************************************************************************
 Prototype: tftPowerOnAniTaskFunc
 Description: Start the animation processing task
 Input: BSP_VOID *arg  
 Output : None
 Return Value: static BSP_S32
 Calls: 
 Called By: 
 
  History:
  1.Date: 2013/3/12
    Author: jujianbo
    Modification : Created function

*****************************************************************************/
static BSP_S32 tftPowerOnAniTaskFunc(BSP_VOID *arg)
{
    BSP_S32 ret = 0;

    /* for pclint */
    if (NULL == arg)
    {
        ;
    }
   
    for(;;)
    {
        down(&g_ulOledPowerOnAniSemSyn);

        if (g_ulPowerOnAniPicCount > 5)
        {
            g_ulPowerOnAniPicCount = 0;
        }

        /*Animation in sequence*/
        ret = tftRefreshByBit(0, 0, LCD_WIDTH, LCD_HIGHT, g_power_on_display_image[g_ulPowerOnAniPicCount]);
        if (0 > ret)
        {
            hitft_error("tftRefresh failed.");
            return ret;
        }
        g_ulPowerOnAniPicCount++;
    }

    return 0;
}


/*****************************************************************************
  Function: tftPwrOnChgAniInstall
  Description: Shut down and charge animation registration
  Calls: 
  Called By: 
  Input : NULL
  Output : NULL
  Return : OK ERROR      
*****************************************************************************/
BSP_S32 tftPwrOnAniInstall(BSP_VOID)
{
    BSP_S32 ret = 0;


    if (1 == tft_pwron_init_flag)
    {
        return ret;
    }

    if(NULL == power_on_ani_task)
    {    
         printk("Balong_tft_init:start power_on_ani_task \n");
         power_on_ani_task = kthread_run(tftPowerOnAniTaskFunc, NULL, "icc_poweron_ani");
          if (IS_ERR(power_on_ani_task))
          {
               hitft_error("create kthread power_on_ani_task failed!");
               return -1;
           }
     }

        /*Start the startup animation*/


     ret = queue_delayed_work(tft_queue, &tft_pwron_ani_work, msecs_to_jiffies(500));
     if (0 == ret)
     {
         hitft_error("queue delayed work failed.");
             
         return ret;
     }

   
    
    tft_pwron_init_flag = 1;
 
    return OK;
}

EXPORT_SYMBOL(tftPwrOnAniInstall);
#endif
/*****************************************************************************
  Function: tftPwrOffChgAniInstall
  Description: Shut down and charge animation registration
  Calls: 
  Called By: 
  Input : NULL
  Output : NULL
  Return : OK ERROR      
*****************************************************************************/
BSP_S32 tftPwrOffChgAniInstall(BSP_VOID)
{
	BSP_S32 ret = 0;

	/* BEIGN ,Added by xuchao,2012/2/14 */
	/*Cannot be called repeatedly when the shutdown charging screen is being displayed*/
	if (1 == tft_poweroff_init_flag)
	{
		return ret;
	}
	/* END   ,Added by xuchao,2012/2/14 */
	/*Create a task cycle display charging animation. If the LCD is not initialized, the shutdown charging animation display task will not be started.*/	
	if((NULL == charge_task)&&((BSP_TRUE == g_bTftInit)))	
	{		
		printk("tftPwrOffChgAniInstall:start power off charging \n");
	    charge_task = kthread_run(tftChgTaskFunc, NULL, "icc_charge");
	    if (IS_ERR(charge_task))
	    {
		    hitft_error("create kthread charge_task failed!");
		    return -1;
	    }
	}
	
	if(BSP_TRUE == g_bTftInit)
	{
		/*Start shutdown charging animation*/
		ret = queue_delayed_work(tft_queue, &tft_pwroff_chg_work, msecs_to_jiffies(1000));
		if (0 == ret)
		{
			hitft_error("queue delayed work failed.");
			return ret;
		}
	}
	
	tft_poweroff_init_flag = 1;
	return OK;
}
EXPORT_SYMBOL(tftPwrOffChgAniInstall);

#ifdef BALONG_LCD_SYSFS
static ssize_t balong_tft_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	BSP_CHAR *command = NULL;
	UINT32 val = 0;
	UINT8 value = 0;
	UINT32 x_from_user = 0;
	UINT32 y_from_user = 0;
	BSP_S32 ret = 0;

	/* for pclint */
	if (NULL == dev && NULL == attr)
	{
		;
	}

	command = kzalloc(64, GFP_KERNEL);
	if (NULL == command)
	{
		hitft_error("malloc command failed.");
		return (ssize_t)count;
	}

	sscanf(buf, "%s %d %d %d", command, &val , &x_from_user , &y_from_user);
	hitft_trace(1,"command=%s val=%x", command, val);

	if (!strncmp(command, "tftLight", strlen("tftLight")))
	{
		hitft_trace(1,"begin to test: tftLight");
		tftLight();
	}
	else if (!strncmp(command, "tftSleep", strlen("tftSleep")))
	{
		hitft_trace(1,"begin to test: tftSleep");
		tftSleep();
	}
	else if (!strncmp(command, "tftClearWholeScreen", strlen("tftClearWholeScreen")))
	{
		hitft_trace(1,"begin to test: tftClearWholeScreen");
		tftClearWholeScreen();
	}
	else if (!strncmp(command, "tftDim", strlen("tftDim")))
	{
		hitft_trace(1,"begin to test: tftDim");
		//Tft dim();

	}
	else if (!strncmp(command, "tftInitImage", strlen("tftInitImage")))
	{
		hitft_trace(1,"begin to test: tftInitImage");
		tftInitImage();
	}
	else if (!strncmp(command, "tftPwrOnAniInstall", strlen("tftPwrOnAniInstall")))
	{
		hitft_trace(1,"begin to test: tftPwrOnAniInstall");
		//Tft pwr on ani install();

	}
	else if (!strncmp(command, "tftPwrOffChgAniInstall", strlen("tftPwrOffChgAniInstall")))
	{
		hitft_trace(1,"begin to test: tftPwrOffChgAniInstall");
		ret = tftPwrOffChgAniInstall();
		if (0 > ret)
		{
			printk("tftPwrOffChgAniInstall failed.");
		}
	}
	else if (!strncmp(command, "tftPwrOn", strlen("tftPwrOn")))
	{
		hitft_trace(1,"begin to test: tftPwrOn");
		tftPwrOn();
	}
	else if (!strncmp(command, "tftPwrOff", strlen("tftPwrOff")))
	{
		hitft_trace(1,"begin to test: tftPwrOff");
		tftPwrOff();
	}
	else if (!strncmp(command, "tftWhite", strlen("tftWhite")))
	{
		hitft_trace(1,"begin to test: tftWhite");
		ret = tftWhite(0, 0, LCD_WIDTH, LCD_HIGHT);
		if (0 > ret)
		{
			hitft_error("tftWhite failed.");
		}
	}
	else if (!strncmp(command, "cancel_poweron_work", strlen("cancel_poweron_work")))
	{
		hitft_trace(1,"begin to test: cancel_poweron_work");
		//ret = cancel_delayed_work_sync(&tft_poweron_work);

		if (0 > ret)
		{
			hitft_error("cancel_delayed_work_sync failed.");
		}
		#if 0
		    memcpy(buffer_s,buffer_t,BUFFER_SIZE);
		    tftClear(0, 0, 128, 64);
		    msleep(1000);
		    tftRefresh(0, 0, LCD_X_OFFSET_128, 64, buffer_s);

		#endif
	}
	else if (!strncmp(command, "cancel_poweroff_work", strlen("cancel_poweroff_work")))
	{
		hitft_trace(1,"begin to test: cancel_poweroff_work");
		ret  = cancel_delayed_work_sync(&tft_pwroff_chg_work);
		if (0 > ret)
		{
			hitft_error("cancel_delayed_work_sync failed.");
		}
	}
	else if (!strncmp(command, "set_contrast", strlen("set_contrast")))
	{
		value = (UINT8)val;
		hitft_trace(1,"begin to test: set_contrast  value=%u", value);
		emiCmdOut8(0x81);
		emiCmdOut8(value);
	}
	else if (!strncmp(command, "tftStringDisplay", strlen("tftStringDisplay")))
	{
		hitft_trace(1,"begin to test: tftStringDisplay");
		/*pclint 734*/
		tftStringDisplay((UINT8)x_from_user, (UINT8)y_from_user, (UINT8*)"HUAWEI");
	}
	else if (!strncmp(command, "tftCharDisplay", strlen("tftCharDisplay")))
	{
		hitft_trace(1,"begin to test: tftCharDisplay");
		tftCharDisplay(16, 24, (UINT8*)"H");
	}
	else if (!strncmp(command, "tftAllWhite", strlen("tftAllWhite")))
	{
		hitft_trace(1,"begin to test: tftAllWhite");
		tftWhite(0, 0, 128, 64);
	}
	else if (!strncmp(command, "tftAllBlack", strlen("tftAllBlack")))
	{
		hitft_trace(1,"begin to test: tftAllBlack");
		tftClear(0, 0, 128, 64);
	}	
	else if (!strncmp(command, "tftTestEmiAndDc", strlen("tftTestEmiAndDc")))
	{
		hitft_trace(1,"begin to test: tftTestEmiAndDc");
		emiCmdOut8(0x55);
	}	
	else if (!strncmp(command, "tftTestDc", strlen("tftTestDc")))
	{
		hitft_trace(1,"begin to test: tftTestDc");
		emiDataOut8(0x55);
	}	
	kfree(command);

	return (ssize_t)count;
}
static DEVICE_ATTR(balong_tft, 0666, NULL, balong_tft_store);

static struct attribute *balong_tft_attributes[] = {
        &dev_attr_balong_tft.attr,
	NULL
};

static const struct attribute_group balong_tft_group = {
	.attrs = balong_tft_attributes,
};
#endif

static const struct file_operations balong_tft_fops = {
	.owner		= THIS_MODULE,
	.llseek		= balong_tft_lseek,
	.write		= balong_tft_write,
	.unlocked_ioctl	= balong_tft_ioctl,/*lint !e64*/
	.open		= balong_tft_open,
	.release		= balong_tft_release,
};

static struct miscdevice tft_miscdev = {
	.minor	= TFT_DYNAMIC_MINOR,
	.name	= "balong_tft_dev",
	.fops		= &balong_tft_fops
};

static BSP_S32 __init balong_tft_init(BSP_VOID)
{
	BSP_S32 ret = 0;
	
	hitft_trace(1,"enter tft driver: balong_tft_init");
	buffer_t = kzalloc(SCREEN_BUFFER_SIZE, GFP_KERNEL);
	if (buffer_t== NULL) {
		ret = -ENOMEM;
		goto fail1;
	}
    memset(buffer_t,0x0,SCREEN_BUFFER_SIZE);

	sema_init(&g_ulOledSemSyn, SEM_FULL);
	sema_init(&g_ulOledSemMux, SEM_FULL);
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
    sema_init(&g_ulOledPowerOnAniSemSyn, SEM_FULL);
#endif

	tft_queue = create_workqueue("balong_tft_queue");
	if (NULL == tft_queue) {
		hitft_error("failed to create workqueue tft_queue");
		ret = -1;
		goto fail2;
	}

	/* init delayed work for power off charge animation */
	INIT_DELAYED_WORK(&tft_pwroff_chg_work, tftPwrOffChgAniIsr);
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
    INIT_DELAYED_WORK(&tft_pwron_ani_work, tftPwrOnAniIsr);
#endif
#ifdef BALONG_LCD_SYSFS
	tft_dev = platform_device_register_simple("balong_tft_test", -1, NULL, 0);
	if (NULL == tft_dev)
	{
		hitft_error("register platform device balong_tft failed.");
		destroy_workqueue(tft_queue);
		return -1;
	}
	
	/* create sys filesystem node for testing */
	ret = sysfs_create_group(&tft_dev->dev.kobj, &balong_tft_group);
	if (0 != ret)
	{
		hitft_error("create tft sys filesystem node failed.");
		platform_device_unregister(tft_dev);
		destroy_workqueue(tft_queue);
		return -1;
	}
#endif
	/* register char dev for user space app */
	ret = misc_register(&tft_miscdev);
	if (0 > ret)
	{
		hitft_error("-------cannot register device: balong_tft");
#ifdef BALONG_LCD_SYSFS		
		sysfs_remove_group(&tft_dev->dev.kobj, &balong_tft_group);
		platform_device_unregister(tft_dev);
#endif		
		destroy_workqueue(tft_queue);
		return ret;
	}
	/* BEIGN ,Deleted by xuchao,2012/2/14 */
    //LcdInit();
	/* END   ,Deleted by xuchao,2012/2/14 */
    tftPwrOn();
	/*BEGIN: HanJiuping 00122021 2012-02-15 removed*/
    /*Tft clear whole screen();*/
	/*END: HanJiuping 00122021 2012-02-15 removed*/
	
	/*In the case of trickle charging, if the M-core starts shutdown charging animation earlier than the LCD initialization,
	LCDStart the shutdown charging display task during initialization*/	
	if(1 == tft_poweroff_init_flag)
	{
		if(NULL == charge_task)
		{	
			printk("Balong_tft_init:start power off charging \n");
			charge_task = kthread_run(tftChgTaskFunc, NULL, "icc_charge");
			if (IS_ERR(charge_task)){
				hitft_error("create kthread charge_task failed!");
				return -1;
			}
		}

		/*Start shutdown charging animation*/
		ret = queue_delayed_work(tft_queue, &tft_pwroff_chg_work, msecs_to_jiffies(1000));
		if (0 == ret)
		{
			hitft_error("queue delayed work failed.");
			return ret;
		}
		
		/*Start charging animation*/
		up(&g_ulOledSemSyn);			
	}

	g_bTftInit = BSP_TRUE;
	printk("balong_tft_init init success\n");
    if(BSP_TRUE == refresh_tft_when_inited)
    {
        refresh_tft_when_inited = BSP_FALSE;

        tftClearWholeScreen();
        tftUpdateDisplay(FALSE);
        is_ftm_bad_board = BSP_TRUE;
        printk(KERN_ERR "tftRefresh is forbidden.\n");
    }

	return ret;
    
fail2:
    kfree(buffer_t);
fail1:
    return ret;
}

static BSP_VOID __exit balong_tft_exit(BSP_VOID)
{
	BSP_S32 ret = 0;

	hitft_trace(1,"exit tft driver: balong_tft_cleanup");
    tftPwrOff();
	ret = misc_deregister(&tft_miscdev);
	if (0 > ret)
	{
		hitft_error("misc_deregister failed.");
	}
#ifdef BALONG_LCD_SYSFS
	sysfs_remove_group(&tft_dev->dev.kobj, &balong_tft_group);

	platform_device_unregister(tft_dev);
#endif
	destroy_workqueue(tft_queue);
}

module_init(balong_tft_init);
module_exit(balong_tft_exit);

MODULE_AUTHOR("Hisilicon Drive Group");
MODULE_DESCRIPTION("Balong LCD Driver");
MODULE_LICENSE("GPL");
