/*******************************************************************************
* Copyright (C), 2010-2011, HUAWEI Tech. Co., Ltd
*
* File name:
*                pwcSocReg.h
* Description:
*                Soc reg define
*
* Author:        ������
* Date:          2011-09-20
* Version:       1.0
*
*
*
* History:
* Author:		������
* Date:			2011-09-20
* Description:	Initial version
*
*******************************************************************************/

#ifndef PWRCTRL_ACPU_SOC_REG
#define PWRCTRL_ACPU_SOC_REG
/*V3R2 CS f00164371*/
#include "generated/FeatureConfigDRV.h"

#if (FEATURE_DEEPSLEEP == FEATURE_ON) || (defined(CHIP_BB_6920ES) || defined (CHIP_BB_6920CS))
/**********************************************************/
/*physical addr */
/**********************************************************/
#define PWRC_REG_SCU_BASE                   (0x04000000)   /*SCU ����ַ*/

#define ACPU_GIC_PHY_BASE                           (0x04001000)
#define PWR_SC_PHY_BASE                         (0x90000000)
#if (defined(BOARD_ASIC_BIGPACK) && defined(VERSION_V3R2))  \
	|| (defined(BOARD_SFT) && defined(VERSION_V7R1)) \
	|| ((defined(CHIP_BB_6920ES) || defined (CHIP_BB_6920CS)) \
	&& (defined(BOARD_ASIC) || defined (BOARD_ASIC_BIGPACK) || defined (BOARD_SFT)))

#define PWRCTRL_SOC_PHY_ADDR_TIMER0                     (0x90002000)
#define PWRCTRL_SOC_PHY_ADDR_TIMER1                     (0x90003000)
#define PWRCTRL_SOC_PHY_ADDR_TIMER2                     (0x90004000)

#elif (defined (BOARD_SFT) && defined (VERSION_V3R2))

#define PWRCTRL_SOC_PHY_ADDR_TIMER0                     (0x90026000)
#define PWRCTRL_SOC_PHY_ADDR_TIMER1                     (0x90002000)
#define PWRCTRL_SOC_PHY_ADDR_TIMER2                     (0x90003000)
#endif


#define PWRCTRL_DDRC_BASE                   (0x90180000)
#define ACPU_IPC_INT_PHY_BASE               (0x900A5410)

/* sram1 */
#define SRAM1_BASE                                      (0x2FFE0000)
#define ISRAM1_SLEEP                        (SRAM1_BASE)
#define DSRAM1_STORE_BASE                       (SRAM1_BASE + 0x2000)
#define DSRAM1_STORE_ARM_REG_BASE               (SRAM1_BASE + 0x2150)
#define DSRAM1_STORE_GIC_BASE               (SRAM1_BASE + 0x2200)
#define DSRAM1_STORE_CTRL_REG_BASE          (SRAM1_BASE + 0x2400)
#define SRAM1_SIZE                                      (0x4000)

/* uart */
#define UART0_BASE                          (0x90020000)


/*gic*/
#define ACPU_GIC_ICCICR     (PBXA9_GIC_CPU_CONTROL + 0x000)
#define ACPU_GIC_ICCPMR     (PBXA9_GIC_CPU_CONTROL + 0x004)
#define ACPU_GIC_ICCBPR     (PBXA9_GIC_CPU_CONTROL + 0x008)
#define ACPU_GIC_ICCEOIR    (PBXA9_GIC_CPU_CONTROL + 0x010)
#define ACPU_GIC_ICABPR     (PBXA9_GIC_CPU_CONTROL + 0x01C)

#define ACPU_GIC_ICDDCR     (PBXA9_GIC_DIST_CONTROL + 0x000)
#define ACPU_GIC_ICDISR     (PBXA9_GIC_DIST_CONTROL + 0x080)
#define ACPU_GIC_ICDISER    (PBXA9_GIC_DIST_CONTROL + 0x100)
#define ACPU_GIC_ICDICER    (PBXA9_GIC_DIST_CONTROL + 0x180)
#define ACPU_GIC_ICDISPR    (PBXA9_GIC_DIST_CONTROL + 0x200)
#define ACPU_GIC_ICDICPR    (PBXA9_GIC_DIST_CONTROL + 0x280)
#define ACPU_GIC_ICDIPR     (PBXA9_GIC_DIST_CONTROL + 0x400)
#define ACPU_GIC_ICDICFR    (PBXA9_GIC_DIST_CONTROL + 0xC00)
#define ACPU_GIC_ICDSGIR    (PBXA9_GIC_DIST_CONTROL + 0xF00)



/*********************************************************/
/*virtual addr*/
/*********************************************************/
#if (defined(BOARD_ASIC_BIGPACK) && defined(VERSION_V3R2))  \
	|| (defined(BOARD_SFT) && defined(VERSION_V7R1))  \
	|| ((defined(CHIP_BB_6920ES) || defined (CHIP_BB_6920CS)) \
	&& (defined(BOARD_ASIC) || defined (BOARD_ASIC_BIGPACK) || defined (BOARD_SFT)))
#define PWRCTRL_SOC_ADDR_TIMER0_0               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 0))
#define PWRCTRL_SOC_ADDR_TIMER0_1               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 1))
#define PWRCTRL_SOC_ADDR_TIMER0_2               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 2))
#define PWRCTRL_SOC_ADDR_TIMER0_3               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 3))
#define PWRCTRL_SOC_ADDR_TIMER0_4               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 4))
#define PWRCTRL_SOC_ADDR_TIMER0_5               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 5))
#define PWRCTRL_SOC_ADDR_TIMER0_6               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 6))
#define PWRCTRL_SOC_ADDR_TIMER0_7               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 7))

#define PWRCTRL_SOC_ADDR_TIMER1_0               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 0))
#define PWRCTRL_SOC_ADDR_TIMER1_1               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 1))
#define PWRCTRL_SOC_ADDR_TIMER1_2               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 2))
#define PWRCTRL_SOC_ADDR_TIMER1_3               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 3))
#define PWRCTRL_SOC_ADDR_TIMER1_4               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 4))
#define PWRCTRL_SOC_ADDR_TIMER1_5               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 5))
#define PWRCTRL_SOC_ADDR_TIMER1_6               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 6))
#define PWRCTRL_SOC_ADDR_TIMER1_7               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 7))

#define PWRCTRL_SOC_ADDR_TIMER2_0               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 0))
#define PWRCTRL_SOC_ADDR_TIMER2_1               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 1))
#define PWRCTRL_SOC_ADDR_TIMER2_2               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 2))
#define PWRCTRL_SOC_ADDR_TIMER2_3               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 3))
#define PWRCTRL_SOC_ADDR_TIMER2_4               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 4))
#define PWRCTRL_SOC_ADDR_TIMER2_5               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 5))
#define PWRCTRL_SOC_ADDR_TIMER2_6               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 6))
#define PWRCTRL_SOC_ADDR_TIMER2_7               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 7))


#elif (defined (BOARD_SFT) && defined (VERSION_V3R2))

#define PWRCTRL_SOC_ADDR_TIMER0_0               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 0))
#define PWRCTRL_SOC_ADDR_TIMER0_1               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 1))
#define PWRCTRL_SOC_ADDR_TIMER0_2               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 2))
#define PWRCTRL_SOC_ADDR_TIMER0_3               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 3))
#define PWRCTRL_SOC_ADDR_TIMER0_4               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 4))
#define PWRCTRL_SOC_ADDR_TIMER0_5               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 5))
#define PWRCTRL_SOC_ADDR_TIMER0_6               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 6))
#define PWRCTRL_SOC_ADDR_TIMER0_7               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER0 + 5 * 4 * 7))

#define PWRCTRL_SOC_ADDR_TIMER1_0               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 0))
#define PWRCTRL_SOC_ADDR_TIMER1_1               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 1))
#define PWRCTRL_SOC_ADDR_TIMER1_2               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 2))
#define PWRCTRL_SOC_ADDR_TIMER1_3               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 3))
#define PWRCTRL_SOC_ADDR_TIMER1_4               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 4))
#define PWRCTRL_SOC_ADDR_TIMER1_5               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 5))
#define PWRCTRL_SOC_ADDR_TIMER1_6               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 6))
#define PWRCTRL_SOC_ADDR_TIMER1_7               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER1 + 5 * 4 * 7))

#define PWRCTRL_SOC_ADDR_TIMER2_0               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 0))
#define PWRCTRL_SOC_ADDR_TIMER2_1               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 1))
#define PWRCTRL_SOC_ADDR_TIMER2_2               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 2))
#define PWRCTRL_SOC_ADDR_TIMER2_3               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 3))
#define PWRCTRL_SOC_ADDR_TIMER2_4               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 4))
#define PWRCTRL_SOC_ADDR_TIMER2_5               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 5))
#define PWRCTRL_SOC_ADDR_TIMER2_6               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 6))
#define PWRCTRL_SOC_ADDR_TIMER2_7               (IO_ADDRESS(PWRCTRL_SOC_PHY_ADDR_TIMER2 + 5 * 4 * 7))

#elif (defined (BOARD_FPGA) && defined (VERSION_V3R2)) \
	|| (defined (BOARD_FPGA_P500) && (defined(CHIP_BB_6920ES) || defined (CHIP_BB_6920CS)))
#define PWRCTRL_SOC_ADDR_TIMER0_0               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_1               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_2               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_3               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_4               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_5               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_6               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER0_7               (IO_ADDRESS(0x20003000))

#define PWRCTRL_SOC_ADDR_TIMER1_0               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_1               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_2               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_3               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_4               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_5               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_6               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER1_7               (IO_ADDRESS(0x20003000))

#define PWRCTRL_SOC_ADDR_TIMER2_0               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_1               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_2               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_3               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_4               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_5               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_6               (IO_ADDRESS(0x20003000))
#define PWRCTRL_SOC_ADDR_TIMER2_7               (IO_ADDRESS(0x20003000))
#endif





/*sysctrl*/
#define PWR_SC_PERIPH_STAT0                 (IO_ADDRESS(PWR_SC_PHY_BASE) + 0x108)
#define PWR_SC_AARM_WKUP_INT_EN             (IO_ADDRESS(PWR_SC_PHY_BASE) + 0x0B4)
#define PWR_SC_PERIPH_CLKSTAT3              (IO_ADDRESS(PWR_SC_PHY_BASE) + 0x080)
#define PWR_SC_PERIPH_CLKDIS3               (IO_ADDRESS(PWR_SC_PHY_BASE) + 0x07C)


/* uart */

#define PWRCTRL_UART0                       (IO_ADDRESS(UART0_BASE))

/* sram1 */

#define ISRAM1_VA_SLEEP                     (IO_ADDRESS(SRAM1_BASE) + 0x400)


/*********************************************************/

#endif

#endif
