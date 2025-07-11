#ifndef __ASM_ARCH_BOARD_PV500_H
#define __ASM_ARCH_BOARD_PV500_H

#include <asm/sizes.h>
#include <mach/hardware.h>
/*I2C*/
#define I2C_BASE        0x90019000
#define I2C_SIZE        SZ_4K

#define BC_CTRL_BASE    0x9000c000
#define BC_CTRL_SIZE    SZ_4K

/*串口*/
#define UART0_BASE      0x90007000      /* UART 0 */
#define UART1_BASE      0x90016000      /* UART 1 */
#define UART2_BASE      0x90017000      /* UART 2 */
#define UART3_BASE      0x90018000      /* UART 3 */
#define UART_REGS_SIZE  SZ_4K


#define PV500V1_NANDC_BASE      0xA0100000
#define PV500V1_NANDC_REGS_SIZE SZ_64K

/*系统控制器*/
#define V3R2_SC_BASE            0x90000000
#define V3R2_SC_SIZE                    SZ_4K
#define V3R2_SC_VA_BASE      IO_ADDRESS(V3R2_SC_BASE)

/*SRAM1*/
#define V3R2_SRAM1_BASE                 0x2FFE0000
#define V3R2_SRAM1_SIZE                 SZ_128K
#define ISRAM1_VA_BASE          IO_ADDRESS(V3R2_SRAM1_BASE)

#define PBXA9_GIC_DIST_CONTROL_VA       IO_ADDRESS(PBXA9_GIC_DIST_CONTROL)


/*NAND FLASH*/
#define NANDF_BUFFER_ADDR               0xA0000000
#define NANDF_BUFFER_SIZE               SZ_256M

/*timer 8~15*/
#define TIMER_REGBASE_8TO15             0x90003000
#define TIMER_8                         8
#define TIMER_OFFSET(x)                 0x14 * ((x) - TIMER_8)
#define TIMER8TO15_REGS_SIZE            SZ_4K

#define TIMER14_BASE                    (TIMER_REGBASE_8TO15 + TIMER_OFFSET(14))

#define PBXA9_PERIPHBASE                (0x4000000)
#define PBXA9_PERIPHBASE_SIZE           SZ_4K

#define INTEGRATOR_SC_BASE              (V3R2_SC_BASE)

/*
 * Generic Interrupt Controller
 * Note: FIQ is not handled within VxWorks so this is just IRQ
 */
#define PBXA9_GIC_CPU_BASE                  (PBXA9_PERIPHBASE)
#define PBXA9_GIC_CPU_CONTROL               (PBXA9_GIC_CPU_BASE + 0x0100)
#define PBXA9_GIC_CPU_PRIORITY              (PBXA9_GIC_CPU_BASE + 0x0104)
#define PBXA9_GIC_CPU_POINT                 (PBXA9_GIC_CPU_BASE + 0x0108)
#define PBXA9_GIC_CPU_ACK                   (PBXA9_GIC_CPU_BASE + 0x010C)
#define PBXA9_GIC_CPU_END_INTR              (PBXA9_GIC_CPU_BASE + 0x0110)
#define PBXA9_GIC_CPU_RUNNING               (PBXA9_GIC_CPU_BASE + 0x0114)
#define PBXA9_GIC_CPU_PENDING               (PBXA9_GIC_CPU_BASE + 0x0118)

/*AXI*/
#define AXI_MEM_BASE_ADDR                    0x2ffc0000
#define AXI_MEM_ADDR                                    IO_ADDRESS(AXI_MEM_BASE_ADDR)

#define AXI_MEM_SIZE                    0x40000

#define PBXA9_DRAM_SHARE_PHYS           (0x34000000)
#define VX_PBXA9_DRAM_SHARE_PHYS     (0x30000000)


/* 96 is the maximum interrupt number. It covers SGI, PPI and SPI */
#define SYS_INT_LEVELS_MAX          (96 + 32)

/* interrupt distributor */
#define PBXA9_GIC_DIST_CONTROL      (PBXA9_PERIPHBASE + 0x1000)
#define PBXA9_GIC_DIST_SIZE         SZ_4K

/* interrupt levels */
#define A9_GLOBAL_INTS              32
#define INT_LVL_MAX                 128

#define INT_LVL_WDT                 (A9_GLOBAL_INTS + 0)
/* 1保留 */
/* 2-3为ARM11中断,BSP暂不定义 */
#define INT_LVL_TIMER_0             (A9_GLOBAL_INTS + 4)
#define INT_LVL_TIMER_1             (A9_GLOBAL_INTS + 5)
#define INT_LVL_TIMER_23            (A9_GLOBAL_INTS + 6)
#define INT_LVL_TIMER_4567          (A9_GLOBAL_INTS + 7)
#define INT_LVL_TIMER_14            91

//TODO:#define INT_LVL_TIMER_2_3     (37) /* 2 or 3 Timers */
//TODO:#define INT_LVL_TIMER_0_1     (36) /* 0 or 1 Timers */
/* 8保留 */
#define INT_LVL_SCI                 (A9_GLOBAL_INTS + 9)
#define INT_LVL_DMA                 (A9_GLOBAL_INTS + 10)
#define INT_LVL_CIPHER              (A9_GLOBAL_INTS + 11)
#define INT_LVL_USB                 (A9_GLOBAL_INTS + 12)
#define INT_LVL_UART_0              102 /* UART0 */
#define INT_LVL_UART_1              103 /* UART1 */
#define INT_LVL_UART_2              104 /* UART2 */
#define INT_LVL_UART_3              105 /* UART3 */
#define INT_LVL_RTC                 (A9_GLOBAL_INTS + 29)
#define INT_LVL_NANDC               (A9_GLOBAL_INTS + 30)
#define INT_LVL_SD_MMC              (A9_GLOBAL_INTS + 31)
#define INT_LVL_I2C                 (A9_GLOBAL_INTS + 32)
#define INT_LVL_GMAC                (A9_GLOBAL_INTS + 33)
#define INT_LVL_GPIO_0              (A9_GLOBAL_INTS + 34)
#define INT_LVL_DESRSASHA           (A9_GLOBAL_INTS + 35)
#define INT_LVL_IPCM2ARM            (A9_GLOBAL_INTS + 36)
/* 37-40为CEVA中断,BSP不定义 */
#define INT_LVL_PMU                 (A9_GLOBAL_INTS + 41)       /* PMU组合中断*/
#define INT_LVL_AXIMONITOR          (A9_GLOBAL_INTS + 42)       /* PMU组合中断*/
/* 43-44为CEVA中断,BSP不定义 */
#define INT_LVL_GPIO_1              (A9_GLOBAL_INTS + 45)
/* 46-47保留 */
/* 48-63为PAD_INTX,BSP暂不定义 */
/* 64-85保留 */
#ifdef BSP_CORE_MODEM
#define INT_LVL_SUBVIC              (A9_GLOBAL_INTS + 54)
#else
#define INT_LVL_SUBVIC              (A9_GLOBAL_INTS + 55)
#endif
#define INT_LVL_FPGA                INT_LVL_SUBVIC      /* FPGA_AXI中断*/
/* 88-95保留 */

#define INT_LVL_IPF                 45  /*IPF*/

#define INT_VEC_SY_CONTROL0         132

#define SYS_TIMER_INT_LVL           (INT_LVL_TIMER_14)
#define AUX_TIMER_INT_LVL           (INT_LVL_TIMER_14)
#define STAMP_TIMER_INT_LVL         (INT_LVL_TIMER_14)
#define USR_TIMER_INT_LVL           (INT_LVL_TIMER_14)

/*****************************************************
* 时钟区
*****************************************************/
/* signals generated from various clock generators */
#define PBXA9_TIMERS_CLK        (48000000)              /* Real time clock */
#define PBXA9_UART_CLK          (48000000)              /* UART clock */
/* frequency of counter/timers */
#define SYS_TIMER_CLK           (PBXA9_TIMERS_CLK)
//#define SYS_CLK_RATE_MIN        (10)
//#define SYS_CLK_RATE_MAX        (8000)
#define AUX_TIMER_CLK           (PBXA9_TIMERS_CLK)
#define STAMP_TIMER_CLK         (PBXA9_TIMERS_CLK)

#define STAMP_CLK_RATE_MIN      10
#define STAMP_CLK_RATE_MAX      8000

/*address transfer */
#define IO_TYPE(var) ((ICC_SUB_CHAN_S *)IO_ADDRESS((unsigned int)(var)))
#define PHY_TO_VIRT(var)  ((BSP_U8 *)IO_ADDRESS((unsigned int)(var)))
#define VIRT_TO_PHY(var)  (((var) - AXI_MEM_ADDR) + AXI_MEM_BASE_ADDR)

/* gpio3 */
#define PV500V1_GPIO3_BASE      0x90014000
#define PV500V1_GPIO_REGS_SIZE  SZ_4K

/* emi */
#define EMI_REG_BASE_ADDR       0x900A2000
#define EMI_REG_SIZE            0x1000
#define EMI_MEM_BASE_ADDR       0x900A3000
#define EMI_MEM_SIZE            0x1000

#endif
