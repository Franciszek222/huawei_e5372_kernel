/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  balong_tft_drv.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  TFT����ʾ,ת��ͼƬʱʹ��16λ���ɫ/128*128/��λ��ǰ/ˮƽɨ��
*
*   �޸ļ�¼ :  2011��12��16��  v1.00  wuzechun  ����
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
#include "font.h"
#include "image.h"

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
/
#define TFT_LCD_PWR_LDO     13
#define TFT_BKLIG_PMU_DR_NR 1
#endif
#endif
    
static BSP_U32 emi_ctrl_base_addr_v = 0;
static BSP_U32 emi_buf_base_addr_v = 0; /*lint !e551 */

/* ��ʼ����ʶ*/
static BSP_BOOL g_bEmiInit = BSP_FALSE;

static BSP_U32 g_bTftInit = BSP_FALSE;

/* ��ʼ����ʶ*/
BSP_BOOL g_bLcdInit = BSP_FALSE;

/*��ǰ�Ƿ�������ʶ��TRUE����  FALSE����*/
static BSP_BOOL g_bIsLight = BSP_TRUE;

/* ����ʱ���ȫ�ֱ���*/
LCD_TYPE_E g_bLcdType = LCD_TYPE_STN;
LCD_IF_E   g_bLcdIf   = LCD_IF_EMI;
BSP_U32	rstCnt = 0;

/* ����ʱ���ȫ�ֱ���*/
static EMI_SCDUL_MODE_E g_bEmiScdul = EMI_TFT_8BIT;

/* �����ӿ�λ����ȫ�ֱ���*/
static EMI_IF_WDTH_E g_bEmiIfWdth = EMI_WDTH_8BIT;

static BSP_U8 u8E0Data[LCD_TFT_DATA_LGTH] = {0x3F,0x1C, 0x18,0x25,0x1E,0x0C,0x42,0xD8,\
    0x2B,0x13,0x12,0x08,0x10,0x03,0x00};
static BSP_U8 u8E1Data[LCD_TFT_DATA_LGTH] = {0x00,0x23,0x27,0x0A,0x11,0x13,0x3D,0x72,\
    0x54,0x0C,0x1D,0x27,0x2F,0x3C,0x3F};

/* x/y position for user to refresh image */
static UINT32 x_position = 0;
static UINT32 y_position = 0;

#define SCREEN_BUFFER_SIZE (LCD_HIGHT*LCD_WIDTH*2)
/*��⵽��������״̬��־,��ʱ�������¶ȼ�����������麸*/
static BSP_BOOL is_ftm_bad_board = BSP_FALSE;  
/*��⵽��������,ˢ������������*/
#define FTM_BAD_BOARD       0x00212897 

static BSP_BOOL refresh_tft_when_inited = BSP_FALSE; 

static unsigned char *buffer_t = NULL;
static volatile UINT32 g_ulPicCount = 0x00;  /*ͼƬ����*/

static volatile UINT32 g_ulOledPwrOnState = LCD_PWR_OFF_STATE;  /*tft���µ�״̬*/

static UINT32	tft_poweroff_init_flag = 0;

/* ͬ���ź���*/
static struct semaphore	g_ulOledSemSyn;

/*����ˢ�������ź���*/
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
static volatile UINT32 g_ulPowerOnAniPicCount = 0x00;  /*ͼƬ����*/
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


/******************************************************************
*
*
*                                      �ػ����ͼƬ
*
*
*******************************************************************/


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
    /*�˺�������LCD����ͶƱ��ʱ�ӷ��ü��ؽӿڣ�Ŀǰֻ����ͶƱ����*/
    BSP_PWRCTRL_SleepVoteUnLock(PWRCTRL_SLEEP_LCD);
#endif
	return BSP_OK;
}

int PWRCTRL_EMI_LowPowerExit(void)
{
	PWRCTRL_EMI_SoftClkEn();
#if defined (FEATURE_LCD_ON)
    /*�˺�������LCD����ͶƱ��ʱ�ӷ��ü��ؽӿڣ�Ŀǰֻ����ͶƱ����*/
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
    emi_buf_base_addr_v = (uint32_t)ioremap(EMI_BUF_BASE_ADDR, 0x1000); /* �Դ�buff */

#if  (FEATURE_E5 == FEATURE_ON) && (FEATURE_E5_UDP != FEATURE_ON)
    /* ��LDO13 */
    BSP_PMU_LDOCtrl(TFT_LCD_PWR_LDO, BSP_TRUE);
    /* ����ԴΪ9mA */
    BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR, 18);
#endif

   /*lint -e778*/
    /*��λEMI*/
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, 1); //
    
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, CRG_RST_DIS);
   
    /*��ʱ��*/
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, CRG_CLK_OFFSET, 1, CRG_CLK_VALID);
 
    /* I8080��ʹ���ź�*/
    /* I8080дʹ���ź�*/
    /* �ӿ�����/����ѡ���ź�*/
    CRG_REG_SETBITS(IOS_CTRL71_OFFSET, 10, 1, 1); /*ʹ��Ϊ�����Ĭ�ϼ������*/
	/*lint +e778*/

#if defined(CHIP_BB_6920ES) /* CSͳһ���� */
    /* ��λ��Ƭѡ�ź�PAD ����*/
    CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 20, 1, 1); /* �����������㣬�˴���Ĭ��*/
    CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 23, 1, 0); /* �����������㣬Ĭ��*/
    CRG_REG_SETBITS(IOS_CTRL99_OFFSET, 17, 1, 0); /* �����������㣬Ĭ��*/
#endif

    /* �����ź�*/
    /* ��������*/
    
    /* ����Ƭѡ�źŵ���Ч*/
    EMI_REG_SETBITS(EMI_CR0_OFFSET,1,1,0);
    
#if defined(CHIP_BB_6920ES) && (FEATURE_E5_UDP == FEATURE_ON)
    /* �򿪱����*/
    /* ����GPIO3_1��pad*/
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
    /* ����GPIO_1_19/20ΪGPIO */    
	CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 18,1, 0);
	CRG_REG_SETBITS(IOS_CTRL98_OFFSET, 19,1, 0);
#endif
	if(!is_init)
	{
		if(gpio_request(LCD_ID0, "LCD_ID0"))
		{
			hitft_error("Gpio is busy!!! \n");
		}
		/* ����GPIOΪ���� */
		gpio_direction_input(LCD_ID0);	
		
		if(gpio_request(LCD_ID1, "LCD_ID1"))
		{
			hitft_error("Gpio is busy!!! \n");
		}
		
		is_init = BSP_TRUE;
	}
	
 	gpio_direction_input(LCD_ID1); 

    /* ��ȡ����ֵ */
	u32Data1 = gpio_get_value(LCD_ID0); /*lint !e732 */
	u32Data2 = gpio_get_value(LCD_ID1); /*lint !e732 */

	printk("LCD_ID0 %x,LCD_ID1 %x\n",u32Data1,u32Data2);
    /* ��ȡ LCD ����ID*/
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
* �� �� ��  : tftCMInit
*
* ��������  : CHIMEI TFT LCD ��ʼ��
*
* �������  : 
*
* �������  : ��
*
* �� �� ֵ  : 
*****************************************************************************/
BSP_VOID tftCMInit(BSP_VOID)
{
	BSP_U32 i = 0;
    BSP_U16 ucId = 0;
    /* ����LCD */
    emiCmdOut8(0x11);
    
    /* �ӳ�120ms */
    msleep(120);

    /* �������ظ�ʽ*/
    emiCmdOut8(0x3A);
    emiDataOut8(0x55);
    
    /* ����GVDD �� VCI1 */
    emiCmdOut8(0xC0);
    emiDataOut8(0x0C); /* GVDD 4.20V */
    emiDataOut8(0x05); /* VCI1 2.50V */

    msleep(10);

    /* ����voltage supply */
    emiCmdOut8(0xC1);
    emiDataOut8(0x06); /* û��06����?? by wuzechun */

    msleep(10);

    /* ���÷Ŵ�������ֵ,Medium High*/
    emiCmdOut8(0xC2);
    emiDataOut8(0x04);
    
    msleep(10);

    /* ����VCOM1, VCOMH:3.525V, VCOML:-1.225V */
    emiCmdOut8(0xC5);
    emiDataOut8(0x29);
    emiDataOut8(0x35);

    msleep(10);

    /* ����VCOM ƫ��*/
    emiCmdOut8(0xC7);
    emiDataOut8(0xD5);

    /* Gamma adjustment E0��E1ʹ��*/
    emiCmdOut8(0xF2);
    emiDataOut8(0x01);

    /* ����E0*/
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
	
    /* ����ɨ�跽��*/
	emiCmdOut8(0x36);
#if defined(FEATURE_LCD_TURN_RIGHT)
    /*��Ļ��ʾ˳ʱ����ת��ʮ��*/
	emiDataOut8(0x68);	
#else
    emiDataOut8(0x08);
#endif
	
    /* ����֡��*/
    emiCmdOut8(0xB1);
    emiDataOut8(0x10);
    emiDataOut8(0x0A);

    /* ����ʾ��*/
    emiCmdOut8(0x29);
    
    msleep(10);

    /*Memory Write, no need? by wuzechun*/
    emiCmdOut8(0x2C);
    
    msleep(120);
}

/*****************************************************************************
* �� �� ��  : tftTMInit
*
* ��������  : ILITEK TFT LCD ��ʼ��
*
* �������  : 
*
* �������  : ��
*
* �� �� ֵ  : 
*****************************************************************************/
BSP_VOID tftTMInit(void)
{
	int i = 0;

	BSP_U8 u8E0Data[LCD_TFT_DATA_LGTH] = {0x3F,0x1C, 0x18,0x25,0x1E,0x0C,0x42,0xD8,\
		0x2B,0x13,0x12,0x08,0x10,0x03,0x00};
	BSP_U8 u8E1Data[LCD_TFT_DATA_LGTH] = {0x00,0x23,0x27,0x0A,0x11,0x13,0x3D,0x72,\
		0x54,0x0C,0x1D,0x27,0x2F,0x3C,0x3F};

    /* ����LCD */
    emiCmdOut8(0x11);
	
    /* �ӳ�120ms */
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
    /*��Ļ��ʾ˳ʱ����ת��ʮ��*/
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
* �� �� ��  : tftlcdInit
*
* ��������  : TFT LCD ��ʼ��
*
* �������  : 
*
* �������  : ��
*
* �� �� ֵ  : 
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
* �� �� ��  : emiSetScdul
*
* ��������  : ����ʱ��
*
* �������  : eSchdulMode��LCD���ͼ��ӿ�ģʽ
                  pScdulPara: ʱ�����ֵ
*
* �������  : ��
*
* �� �� ֵ  : ���óɹ�����ʧ��
*****************************************************************************/
BSP_S32 emiSetScdul(EMI_SCDUL_MODE_E eSchdulMode, EMI_SCDUL_PARA_S *pScdulPara)
{
    g_bEmiScdul = eSchdulMode;
    
    /* ����ʱ�� */
    if(eSchdulMode < EMI_SCHEDULING_BUTT)
    {
        // I8080 ��M6800 ����
        if(EMI_STN_8BIT_M6800 == eSchdulMode)
        {
            emi_reg_set_bit(CS_CTRL3_OFFSET, 1); /*����ΪM6800ģʽ*/
        }
        else
        {
            emi_reg_clear_bit(CS_CTRL3_OFFSET, 1);
        }

        // 8-bit ��16-bit ����
        if(EMI_TFT_9BIT == eSchdulMode)
        {
            EMI_REG_SETBITS(EMI_CR0_OFFSET,3,2,1);
            EMI_REG_SETBITS(EMI_CR0_OFFSET,5,1,1);
			/*lint -e778*/
            CRG_REG_SETBITS(IOS_CTRL98_OFFSET,2,1,1); /* Ĭ��pad bit8-bit15*/
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
                /* ���ö�����ʱ��*/
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,0,10,pScdulPara->u32Wst1); /* ���߶����ʵȴ�ʱ��*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,16,8,pScdulPara->u32Wst3); /* ���ζ�д����֮����*/
                EMI_REG_SETBITS(EMI_WSTOENR0_OFFSET,0,4,pScdulPara->u32OEn); /* CS��Ч�����ź���Ч�ӳ�*/
                EMI_REG_SETBITS(EMI_WSTOENR0_OFFSET,5,2,pScdulPara->u32OEnDelay); /* ���ź���Ч��Ƭѡ��Чʱ���ӳ�*/

                /* ����д����ʱ��*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,0,10,pScdulPara->u32Wst2); /* ����д���ʵȴ�ʱ��*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,16,8,pScdulPara->u32Wst3); /* ���ζ�д����֮����*/
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,0,4,pScdulPara->u32WEn); /* CS��Ч��д�ź���Ч�ӳ�*/
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,4,2,pScdulPara->u32WEnDelay); /* д�ź���Ч��Ƭѡ��Чʱ���ӳ�*/
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

                /* ������� */
                u32UpDlyrSave   = pScdulPara->u32UpDlyr;
                u32DwnDlyrSave  = pScdulPara->u32DdnDlyr;
                u32UpDlywSave   = pScdulPara->u32UpDlyw;
                u32DwnDlywSave  = pScdulPara->u32DdnDlyw;
                
                /* ���ö�����ʱ��*/
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,0,10,pScdulPara->u32Wst1); /* ���߶����ʵȴ�ʱ��*/
                u32UpDlyrTmp = (u32UpDlyrSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,4,7,u32UpDlyrTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��7λ*/
                u32UpDlyrTmp = ((u32UpDlyrSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,10,3,u32UpDlyrTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��3λ*/

                u32DwnDlyrTmp = (u32DwnDlyrSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,11,7,u32DwnDlyrTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��7λ*/
                u32DwnDlyrTmp = ((u32DwnDlyrSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,13,3,u32DwnDlyrTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��3λ*/

                /* ���ö�����ʱ��*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,0,10,pScdulPara->u32Wst2); /* ����д���ʵȴ�ʱ��*/
                EMI_REG_SETBITS(EMI_WST2R0_OFFSET,16,8,pScdulPara->u32Wst3); /* ���߶�д���ʵȴ�ʱ��*/
                
                u32UpDlywTmp = (u32UpDlywSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,18,7,u32UpDlywTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��7λ*/
                u32UpDlywTmp = ((u32UpDlywSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,16,3,u32UpDlywTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��3λ*/

                u32DwnDlywTmp = (u32DwnDlywSave & 0x007f);
                EMI_REG_SETBITS(EMI_IDCYR0_OFFSET,25,7,u32DwnDlywTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��7λ*/
                u32DwnDlywTmp = ((u32DwnDlywSave >> 7) & 0x0007);
                EMI_REG_SETBITS(EMI_WST1R0_OFFSET,19,3,u32DwnDlywTmp); /* PREB�ź��������CS��Ч�źŵ��ӳ�ʱ�䣬��3λ*/
                
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,0,4,pScdulPara->u32WEn); /* CS��Ч��д�ź���Ч�ӳ�*/
                EMI_REG_SETBITS(EMI_WSTWENR0_OFFSET,5,2,pScdulPara->u32WEnDelay); /* д�ź���Ч��Ƭѡ��Чʱ���ӳ�*/
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
* �� �� ��  : emiSetBackLight
*
* ��������  : LCD��������
*
* �������  : BSP_TRUE:������   BSP_FALSE:�ر���
*
* �������  : ��
*
* �� �� ֵ  : ���óɹ�����ʧ��
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
* �� �� ��  : stnlcdInit
*
* ��������  : STN LCD ��ʼ��
*
* �������  : 
*
* �������  : ��
*
* �� �� ֵ  : 
*****************************************************************************/
BSP_VOID stnlcdInit(BSP_VOID)
{
    // internal reset
    emiCmdOut8(0xe2);
    
    /* �ӳ�10ms */
    msleep(10);

    emiCmdOut8(0xc0);

    emiCmdOut8(0xa2);
    
    /* �ӳ�10ms */
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
* �� �� ��  : LcdInit
*
* ��������  : LCD��ʼ��
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ�����ʧ��
*****************************************************************************/
BSP_S32 LcdInit(BSP_VOID)
{
    EMI_SCDUL_PARA_S sScdulPara;
    EMI_SCDUL_MODE_E eSdulMode;

    memset(&sScdulPara, 0, sizeof(EMI_SCDUL_PARA_S));
    eSdulMode = g_bEmiScdul;
    (BSP_VOID)emiInit();
	/* BEIGN ,Deleted by xuchao,2012/2/14 */
    //(BSP_VOID)tftReset(); �򿪺���Ļ����˸һ��
    /* END ,Deleted by xuchao,2012/2/14 */
    switch(eSdulMode)
    {
        case EMI_TFT_8BIT:
        case EMI_TFT_9BIT:
        {
            // read 
            sScdulPara.u32Wst3 = 1; //20ns
            sScdulPara.u32Wst1 = 24;
            sScdulPara.u32OEnDelay = 2; 
            sScdulPara.u32OEn= 2;

            // write 
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
            // read 
            sScdulPara.u32Wst1 = 10;
            sScdulPara.u32Wst3 = 3; //100ns
            sScdulPara.u32OEnDelay = 0; 
            sScdulPara.u32OEn  = 0;
            sScdulPara.u32UpDlyr = 1;
            sScdulPara.u32DdnDlyr= 9;

            // write 
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
    /* ��LDO13 */
  //  BSP_PMU_LDOCtrl(TFT_LCD_PWR_LDO, BSP_TRUE);
    /* ����ԴΪ9mA */
  //  BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR, 6);

    /*��λEMI*/
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, 1); //
    
    CRG_REG_SETBITS(CRG_CTRL14_OFFSET, CRG_RST_OFFSET, 1, CRG_RST_DIS);
    /*��ʱ��*/
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, CRG_CLK_OFFSET, 1, CRG_CLK_VALID);
    CRG_REG_SETBITS(CRG_CTRL0_OFFSET, 17, 1, CRG_CLK_VALID);
    /* ����Ƭѡ�źŵ���Ч*/
    EMI_REG_SETBITS(EMI_CR0_OFFSET,1,1,0);
	/* BEIGN ,Deleted by xuchao,2012/2/14 */
    //(BSP_VOID)tftReset(); �򿪺���Ļ����˸һ��
    /* END ,Deleted by xuchao,2012/2/14 */
    switch(eSdulMode)
    {
        case EMI_TFT_8BIT:
        case EMI_TFT_9BIT:
        {
            // read 
            sScdulPara.u32Wst3 = 1; //20ns
            sScdulPara.u32Wst1 = 24;
            sScdulPara.u32OEnDelay = 2; 
            sScdulPara.u32OEn= 2;

            // write 
            sScdulPara.u32Wst2= 3; //20ns
            sScdulPara.u32Wst3= 1;
            sScdulPara.u32WEnDelay = 0; 
            sScdulPara.u32WEn   = 1;

            (BSP_VOID)emiSetScdul(eSdulMode, &sScdulPara); /*lint !e718*/
           // (BSP_VOID)tftlcdInit();
        }
            break;
        case EMI_STN_8BIT_M6800:
        case EMI_STN_8BIT_I8080:
        {
            // read 
            sScdulPara.u32Wst1 = 10;
            sScdulPara.u32Wst3 = 3; //100ns
            sScdulPara.u32OEnDelay = 0; 
            sScdulPara.u32OEn  = 0;
            sScdulPara.u32UpDlyr = 1;
            sScdulPara.u32DdnDlyr= 9;

            // write 
            sScdulPara.u32Wst2 = 10; //100ns
            sScdulPara.u32Wst3 = 3;
            sScdulPara.u32WEnDelay = 0; 
            sScdulPara.u32WEn  = 0;
            sScdulPara.u32UpDlyw = 2;
            sScdulPara.u32DdnDlyw= 7;
            emiSetScdul(eSdulMode, &sScdulPara); 

          // stnlcdInit();
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
    CRG_REG_SETBITS(CS_CTRL3_OFFSET, 0, 1, 0); /* ��λ*/
    msleep(10);

    CRG_REG_SETBITS(CS_CTRL3_OFFSET, 0, 1, 1); /* �⸴λ*/
    msleep(50);
	/*lint +e778*/
	rstCnt++;
}

BSP_U32 tftRstCntGet(void)
{
	return rstCnt;
}

BSP_VOID tftPwrOn(BSP_VOID)
{
    PWRCTRL_EMI_LowPowerExit();
	if (LCD_PWR_ON_STATE != g_ulOledPwrOnState)
	{
		LcdInit();//�ȳ�ʼ���󿪱��⣬�쳬 2012-02-14
		g_ulOledPwrOnState = LCD_PWR_ON_STATE;
	}
	else
	{
		hitft_trace(5, "tft has been powered on.\n");
	}
	
	/*�ճ�ʼ������Ļ�ǰ׵ģ����������򿪱��⣬��Ҫ���ô˺�����ˢ����Ļ���ٿ�����*/
	//emiSetBackLight(BSP_TRUE);
	
}
EXPORT_SYMBOL(tftPwrOn);


BSP_VOID tftPwrOff(BSP_VOID)
{
	/*�����µ�״̬�����������µ�*/
	if (LCD_PWR_OFF_STATE == g_ulOledPwrOnState)
	{
		hitft_trace(5, "tft has been powered off.\n");
		return;
	}

	/*�رձ���*/
	emiSetBackLight(BSP_FALSE);
	
	/* tft enter sleep mode  */
	tftSleep();
	
#if  (FEATURE_E5 == FEATURE_ON) && (FEATURE_E5_UDP != FEATURE_ON)
    /* �ر�LDO13 */
#if defined(FEATURE_SHORT_ON_OFF)
    /* �ٹػ����ܿ���ʱ��Ļ����sleep inģʽ�����ر�LDO13 */
    tftClearWholeScreen();
#else
    BSP_PMU_LDOCtrl(TFT_LCD_PWR_LDO, BSP_FALSE);
#endif
#endif
	g_ulOledPwrOnState = LCD_PWR_OFF_STATE;
	PWRCTRL_EMI_LowPowerEnter();
}
EXPORT_SYMBOL(tftPwrOff);


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

    /*�򿪱���*/
  
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



BSP_VOID tftDim(BSP_VOID)
{

    BSP_PMU_DrCurrentSet(TFT_BKLIG_PMU_DR_NR,Dim_light);
}
EXPORT_SYMBOL(tftDim);

#endif


BSP_VOID tftSleep(BSP_VOID)
{
	BSP_S32 ret = 0;
	down(&g_ulOledSemMux);
    /*�رձ���*/
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
  Function    : tftInitImage
  Description : ��ʾ������ʼ������
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : NULL     
  Date:         2012-02-10
*****************************************************************************/
BSP_VOID tftInitImage(BSP_VOID)
{
	BSP_S32 ret = 0;
    tftClearWholeScreen();
	ret = tftRefreshByBit(0, 0,  LCD_WIDTH, LCD_HIGHT, (UINT8 *)gImage_init);
	if (0 > ret)
	{
		hitft_error("tftRefresh failed.");
	}	
}
EXPORT_SYMBOL(tftInitImage);

/*****************************************************************************
  Function    : tftUpdateDisplay
  Description : tft��ʾ�����ɹ�ͼƬ
  Calls       : 
  Called By   : 
  Input       : UpdateStatus:TRUE:�����ɹ� FALSE:����ʧ��
  Output      : NULL
  Return      : NULL      
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
*  Function:  tftClear
*  Description: tft clear *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*          UINT32 ulXStart, UINT32 ulYStart������ʼ����
*          UINT32 ulXEnd, UINT32 ulYEnd������������ƫ����
*  Output:
*         N/A
*  Return:
*         N/A
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
    /*����ģʽ�¼�⵽�����������ʾX����ʱ��ֹ������λˢ��*/
    if(BSP_TRUE == is_ftm_bad_board)
    {
        return OK;
    }
	
	/*�����ӻ����ź�������*/
	down(&g_ulOledSemMux);
	
    /*�����е�ַ,��λ��ǰ*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*�����е�ַ,��λ��ǰ*/
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
	/* clock_gating_disable(CLOCK_GATING_EMI); */
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
    /*����ģʽ�¼�⵽�����������ʾX����ʱ��ֹ������λˢ��*/
    if(BSP_TRUE == is_ftm_bad_board)
    {
        return OK;
    }
	
	/*�����ӻ����ź�������*/
	down(&g_ulOledSemMux);
	
    /*�����е�ַ,��λ��ǰ*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*�����е�ַ,��λ��ǰ*/
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
	/* clock_gating_disable(CLOCK_GATING_EMI); */
	memset(buffer_t ,0xFF ,SCREEN_BUFFER_SIZE);
	up(&g_ulOledSemMux);
	
	return OK;   
}
EXPORT_SYMBOL(tftWhite);


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
*  Function:  tftRefresh
*  Description: tft refresh *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*          UINT32 ulXStart, UINT32 ulYStart������ʼ����
*          UINT32 ulXOffset, UINT32 ulYOffset������������ƫ����
*          UINT8 *pucBuffer������ʾ����

*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
BSP_S32 tftRefresh(UINT32 ulXStart, UINT32 ulYStart, UINT32 ulXOffset, UINT32 ulYOffset, UINT8 *pucBuffer)
{
	UINT32 ulXloop = 0x00;
	UINT32 ulYloop = 0x00;
    UINT32 i = 0;
	UINT32 ulXEnd = (ulXStart + ulXOffset -1);
	UINT32 ulYEnd = (ulYStart + ulYOffset -1);
	/*���������Ч�Լ��*/
	if (NULL == pucBuffer)
	{
	    return ERROR;
	}
	if ((ulXEnd >= LCD_WIDTH) || (ulYEnd >= LCD_HIGHT))
	{
		hitft_error("tftRefresh para error!! ");
		return ERROR;
	}
	
	/*����APP����Ļˢ�ף������Ļ���Ͻǵĵ㣬����ǰ׵ľͲ�ˢ*/
    if(pucBuffer[0] == 0xFF && pucBuffer[1] == 0xFF)
    {
    	hitft_error("XXXXX APP want Refresh LCD to all white XXXXX!! ");
    	return OK;
    }
	
	/*�����ӻ����ź�������*/
	down(&g_ulOledSemMux);
    
	
    /*�����е�ַ,��λ��ǰ*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*�����е�ַ,��λ��ǰ*/
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
	/* clock_gating_disable(CLOCK_GATING_EMI); */

	up(&g_ulOledSemMux);

	return OK;   
}
EXPORT_SYMBOL(tftRefresh);

/*************************************************
*  Function:  tftRefreshByBit
*  Description: ��ʾ��λ�洢��ͼ�񣬸�λ��ǰ��һ���ֽڴ洢�˸���
*  Called By:
*  Input:
*          BSP_U32 ulXStart, BSP_U32 ulYStart������ʼ����
*          BSP_U32 ulXOffset, BSP_U32 ulYOffset������������ƫ����
*          BSP_U8 *pucBuffer������ʾ����,һ���ֽڱ�ʾ�˸��㣬1Ϊ��ɫ0Ϊ��ɫ

*  Output:
*         BSP_FALSE:����������Ļ�߽����Ϊ��
*         BSP_OK:   ������ʾ
*  Return:
*         N/A
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

	UINT32 bitNum = 0; //ÿ���ֽ��и�λѭ��ʱʹ��
	UINT8 *buffer = pucBuffer;
	
	/*���������Ч�Լ��*/
	if (NULL == pucBuffer)
	{
	    return ERROR;
	}
	
	if ((ulXEnd >= LCD_WIDTH) || (ulYEnd >= LCD_HIGHT))
	{
		return ERROR;
	}
    /*����ģʽ�¼�⵽�����������ʾX����ʱ��ֹ������λˢ��*/
    if(BSP_TRUE == is_ftm_bad_board)
    {
        return OK;
    }
	/*�����ӻ����ź�������*/
	down(&g_ulOledSemMux);
	
    /*�����е�ַ,��λ��ǰ*/
    emiCmdOut8(0x2A);
    emiDataOut8((ulXStart>>8)&0xff);
    emiDataOut8(ulXStart&0xff);
    emiDataOut8((ulXEnd>>8)&0xff);
    emiDataOut8(ulXEnd&0xff);

    /*�����е�ַ,��λ��ǰ*/
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
			{//һ���ֽ�������ʱָ����ƣ�λ��־����
				buffer++;
				bitNum=0;
			}
			if( (((*buffer)>>(7-bitNum)) & 0x01) == 0x01)
			{//�Ӹ�λ��ʼ�ж�
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

/****************************************************
* set x/y position for write image
* off = x_position + y_position * LCD_WIDTH
****************************************************/
static loff_t balong_tft_lseek(struct file *file, loff_t off, BSP_S32 whence)
{

	
	x_position = (UINT32)off % LCD_WIDTH;
	y_position = (UINT32)off / LCD_WIDTH;
	
	hitft_error("%s off=%d  x_position=%d  y_position=%d\n", __func__, (BSP_S32)off, x_position, y_position);

	return 0;
}
unsigned char tft_buff[LCD_WIDTH * LCD_HIGHT * 2];
static ssize_t balong_tft_write(struct file *file, const char __user *buf, size_t count,
					loff_t *ppos)
{
	UINT8 *image = NULL;
	BSP_S32 ret = 0;
	BSP_S32 bufCount =LCD_WIDTH * LCD_HIGHT * 2;
	
#if (FEATURE_E5 == FEATURE_ON)
	if(BSP_CHGC_SplyStGet())
	{
		return 0;
	}
	else
#endif		
	{

	#if ( FEATURE_MMI_TEST == FEATURE_ON )			
		if ( true == mmi_test_mode )
		{
		    /*�����mmi����ģʽ���򲻴���*/
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

	return 0;
}

static BSP_S32 balong_tft_release(struct inode *inode, struct file *file)
{
	hitft_trace(1," ------------------ \n");

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
*  Function:  tftIoctrl
*  Description: tft ioctrl *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*          int cmd --command ID
*          arg--para

*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
BSP_S32 balong_tft_ioctl(struct file *file, BSP_U32 cmd, unsigned long arg)
{
	unsigned char value = 0;
	tft_write_data image_data;
	BSP_S32 ret = 0;
	unsigned char* buffer_kernel = NULL;
    signed long image_arg = 0;

	printk(KERN_DEBUG "balong_tft_ioctl: cmd=%u  arg=%lu\n",  cmd, arg);


#if ( FEATURE_MMI_TEST == FEATURE_ON )			
	else if ( true == mmi_test_mode )
	{
	    /*�����mmi����ģʽ���򲻴���*/
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
	/* clock_gating_enable(CLOCK_GATING_EMI); */

	switch (cmd)
	{
		case OLED_LIGHT: /*����LCD����*/     
			/* pwrctrl_lcd_lowpower_exit(); */
			tftLight();
			break;
		case OLED_SLEEP: /*�ر�LCD����*/
			tftSleep();
			/* pwrctrl_lcd_lowpower_enter(); */
			break;
#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
        case OLED_DIM:  /*dimLCDģʽ*/
            tftDim();
            break;
#endif
		case OLED_RESET:/*��λLCD*/
			/*��������tftReset�ӿڻ�������޷��ָ����˴�����������ˢ����Ļ���ٿ�����*/
			tftPwrOff();
			msleep(100);
			tftPwrOn();
			break;
		case OLED_GET_FRAME_BUFFER:/*��ȡͼ������*/
			tft_get_frame_buffer((unsigned char*)arg);
			break; 			
		case OLED_UPDATE:/*��������,�˴���ʵ��*/
			/*tft_update();*/
			break;            
		case OLED_CONTRAST:/*�����Աȶ�*/
			value = (unsigned char)arg;
			hitft_trace(1,"LCD_CONTRAST --------- value=0x%x\n", value);
			emiCmdOut8(0x81);	//set contrast control.
			emiCmdOut8(value);//second byte.
			break;
		case OLED_BRIGHTNESS:/*��������*/
			/* not support now */
			/*
			emiCmdOut8(0x82);
			emiCmdOut8(value);
			*/
			break;
		case OLED_POWER_ON:/*LCD�ϵ�*/
			/* pwrctrl_lcd_lowpower_exit(); */
#if defined(FEATURE_SHORT_ON_OFF)
            /* �ٹػ����ܿ���ʱ��Ļ����sleep inģʽ�����ر�LDO13, ����������³�ʼ��,�Ա�ӿ쿪�������ٶ� */
            g_ulOledPwrOnState = LCD_PWR_ON_STATE;
#else
			tftPwrOn();
#endif
			break;
		case OLED_POWER_OFF:/*LCD�µ�*/
			/* pwrctrl_lcd_lowpower_enter(); */
			tftPwrOff();
			break;
		case OLED_DEL_TIMER:
			/* BEGIN PN: Added by xuchao x00202188, 2012/2/18 */
			if( 1 == tft_poweroff_init_flag)
			{//����ػ���续�汻��������رղ���ʾ���续��
				tft_poweroff_init_flag = 0;
				ret = cancel_delayed_work_sync(&tft_pwroff_chg_work);
				if (0 > ret)
				{
					hitft_error("cancel_delayed_work_sync failed.");
				}
			
				/*������ʾ�����ͼƬ*/
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
		case OLED_SYS_POWER_DOWN:/*ϵͳ�µ�*/
			/* pwrctrl_lcd_lowpower_enter(); */
			tftPwrOff();
			break;
		case OLED_INIT_IMAGE:/*LCD��ʼ��*/
			tftInitImage();
			break;
		case OLED_POWER_OFF_CHARGING_START:/*�����ػ���綯��*/
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
			    /*���LCD�Ѿ���ʼ�������ѳ�綯����ʾ����*/
			    if(BSP_TRUE == g_bTftInit)
			    {
				    up(&g_ulOledSemSyn);			
			    }
			}
			break;
		case OLED_POWER_OFF_CHARGING_STOP:/*�رչػ���綯��*/
			/* stop power off charging picture */
			tft_poweroff_init_flag = 0;
			ret = cancel_delayed_work_sync(&tft_pwroff_chg_work);
			if (0 > ret)
			{
				hitft_error("cancel_delayed_work_sync failed.");
			}
			
			/*������ʾ�����ͼƬ*/
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
            
		case OLED_WRITE_IMAGE:/*д��ͼ��*/
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
        case OLED_ALL_WHITE:/*д��ȫ����ȫ�� */
		    /* BEIGN ,Modified by xuchao,2012/2/14 */
			tftWhite(0, 0, LCD_WIDTH, LCD_HIGHT);
			/* END   ,Modified by xuchao,2012/2/14 */
			break;
        case OLED_ALL_BLACK:/*ȫ���� */
		    /* BEIGN ,Modified by xuchao,2012/2/14 */
			tftClear(0, 0, LCD_WIDTH, LCD_HIGHT);
			/* END   ,Modified by xuchao,2012/2/14 */
			break;				
        /*��Ƭ�汾��⵽�������壬��ʾ�� X ��*/
        case OLED_BATS_FAIL:/*��ʾ" X "*/    
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
                    /*��Ƭ�汾��⵽�������壬��ʾ�� X ������Ҫ��tft��ʼ����ˢ��*/
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

	}

	/* disable the EMI clock gating */
	/* clock_gating_disable(CLOCK_GATING_EMI); */

	return 0;
}
EXPORT_SYMBOL(balong_tft_ioctl);


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
	ret = tftRefreshByBit(ucX, ucY, 8, 16, pucStr);
	if (0 > ret)
	{
		hitft_error("tftRefresh failed.");
	}

	return;
}
EXPORT_SYMBOL(tftCharDisplay);


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
  Function    : tftPwrOffChgAniIsr
  Description : �ػ�����ж�ISR
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : OK              ERROR      
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
 Description  : ����������ʾ
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



	for(;;)
	{
		down(&g_ulOledSemSyn);

		if (g_ulPicCount >= MAX_OFF_CHARGING_ANI)
		{
			g_ulPicCount = 0;
		}
		ret = tftRefreshByBit(0, 0, LCD_WIDTH, LCD_HIGHT, g_chgImage[g_ulPicCount]);
		if (0 > ret)
		{
			hitft_error("tftRefresh failed.");
			return ret;
		}

		/*ֻ������ʱ�Ŷ�������ʱ����*/
		if( BSP_TRUE == g_bIsLight )
		{
			g_ulPicCount++;
		}
	}

	return 0;
}

#if(FEATURE_ON == FEATURE_HUAWEI_VDF)
/*****************************************************************************
 Prototype    : tftPowerOnAniTaskFunc
 Description  : ����������������
 Input        : BSP_VOID *arg  
 Output       : None
 Return Value : static BSP_S32
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/12
    Author       : jujianbo
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

        /*˳���Զ���*/
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
  Function    : tftPwrOnChgAniInstall
  Description : �ػ���綯��ע��
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : OK              ERROR      
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

        /*������������*/


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
  Function    : tftPwrOffChgAniInstall
  Description : �ػ���綯��ע��
  Calls       : 
  Called By   : 
  Input       : NULL
  Output      : NULL
  Return      : OK              ERROR      
*****************************************************************************/
BSP_S32 tftPwrOffChgAniInstall(BSP_VOID)
{
	BSP_S32 ret = 0;

	/* BEIGN ,Added by xuchao,2012/2/14 */
	/*������ʾ�ػ���续��ʱ�����ظ�����*/
	if (1 == tft_poweroff_init_flag)
	{
		return ret;
	}
	/* END   ,Added by xuchao,2012/2/14 */
	/*��������ѭ����ʾ��綯��, ���LCDû�г�ʼ��,�������ػ���綯����ʾ����*/	
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
		/*�����ػ���綯��*/
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
		//tftDim();
	}
	else if (!strncmp(command, "tftInitImage", strlen("tftInitImage")))
	{
		hitft_trace(1,"begin to test: tftInitImage");
		tftInitImage();
	}
	else if (!strncmp(command, "tftPwrOnAniInstall", strlen("tftPwrOnAniInstall")))
	{
		hitft_trace(1,"begin to test: tftPwrOnAniInstall");
		//tftPwrOnAniInstall();
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
	.unlocked_ioctl	= balong_tft_ioctl,
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
    /*tftClearWholeScreen();*/
	/*END: HanJiuping 00122021 2012-02-15 removed*/
	
	/*丳�����£����M���𶯹ػ���綯����LCD��ʼ���磬
	��LCD��ʼ��ʱ�����ػ������ʾ����*/	
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

		/*�����ػ���綯��*/
		ret = queue_delayed_work(tft_queue, &tft_pwroff_chg_work, msecs_to_jiffies(1000));
		if (0 == ret)
		{
			hitft_error("queue delayed work failed.");
			return ret;
		}
		
		/*������綯��*/
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
