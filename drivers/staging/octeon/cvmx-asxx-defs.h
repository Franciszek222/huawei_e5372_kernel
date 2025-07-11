/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

#ifndef __CVMX_ASXX_DEFS_H__
#define __CVMX_ASXX_DEFS_H__

#define CVMX_ASXX_GMII_RX_CLK_SET(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000180ull + (((block_id) & 0) * 0x8000000ull))
#define CVMX_ASXX_GMII_RX_DAT_SET(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000188ull + (((block_id) & 0) * 0x8000000ull))
#define CVMX_ASXX_INT_EN(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000018ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_INT_REG(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000010ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_MII_RX_DAT_SET(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000190ull + (((block_id) & 0) * 0x8000000ull))
#define CVMX_ASXX_PRT_LOOP(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000040ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_BYPASS(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000248ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_BYPASS_SETTING(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000250ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_COMP(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000220ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_DATA_DRV(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000218ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_FCRAM_MODE(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000210ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_NCTL_STRONG(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000230ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_NCTL_WEAK(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000240ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_PCTL_STRONG(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000228ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_PCTL_WEAK(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000238ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RLD_SETTING(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000258ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RX_CLK_SETX(offset, block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000020ull + (((offset) & 3) * 8) + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RX_PRT_EN(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000000ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RX_WOL(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000100ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RX_WOL_MSK(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000108ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RX_WOL_POWOK(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000118ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_RX_WOL_SIG(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000110ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_TX_CLK_SETX(offset, block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000048ull + (((offset) & 3) * 8) + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_TX_COMP_BYP(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000068ull + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_TX_HI_WATERX(offset, block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000080ull + (((offset) & 3) * 8) + (((block_id) & 1) * 0x8000000ull))
#define CVMX_ASXX_TX_PRT_EN(block_id) \
	CVMX_ADD_IO_SEG(0x00011800B0000008ull + (((block_id) & 1) * 0x8000000ull))

union cvmx_asxx_gmii_rx_clk_set {
	uint64_t				u64;
	struct cvmx_asxx_gmii_rx_clk_set_s {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_gmii_rx_clk_set_s	cn30xx;
	struct cvmx_asxx_gmii_rx_clk_set_s	cn31xx;
	struct cvmx_asxx_gmii_rx_clk_set_s	cn50xx;
};

union cvmx_asxx_gmii_rx_dat_set {
	uint64_t				u64;
	struct cvmx_asxx_gmii_rx_dat_set_s {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_gmii_rx_dat_set_s	cn30xx;
	struct cvmx_asxx_gmii_rx_dat_set_s	cn31xx;
	struct cvmx_asxx_gmii_rx_dat_set_s	cn50xx;
};

union cvmx_asxx_int_en {
	uint64_t u64;
	struct cvmx_asxx_int_en_s {
		uint64_t	reserved_12_63:52;
		uint64_t	txpsh:4;
		uint64_t	txpop:4;
		uint64_t	ovrflw:4;
	} s;
	struct cvmx_asxx_int_en_cn30xx {
		uint64_t	reserved_11_63:53;
		uint64_t	txpsh:3;
		uint64_t	reserved_7_7:1;
		uint64_t	txpop:3;
		uint64_t	reserved_3_3:1;
		uint64_t	ovrflw:3;
	} cn30xx;
	struct cvmx_asxx_int_en_cn30xx	cn31xx;
	struct cvmx_asxx_int_en_s	cn38xx;
	struct cvmx_asxx_int_en_s	cn38xxp2;
	struct cvmx_asxx_int_en_cn30xx	cn50xx;
	struct cvmx_asxx_int_en_s	cn58xx;
	struct cvmx_asxx_int_en_s	cn58xxp1;
};

union cvmx_asxx_int_reg {
	uint64_t u64;
	struct cvmx_asxx_int_reg_s {
		uint64_t	reserved_12_63:52;
		uint64_t	txpsh:4;
		uint64_t	txpop:4;
		uint64_t	ovrflw:4;
	} s;
	struct cvmx_asxx_int_reg_cn30xx {
		uint64_t	reserved_11_63:53;
		uint64_t	txpsh:3;
		uint64_t	reserved_7_7:1;
		uint64_t	txpop:3;
		uint64_t	reserved_3_3:1;
		uint64_t	ovrflw:3;
	} cn30xx;
	struct cvmx_asxx_int_reg_cn30xx cn31xx;
	struct cvmx_asxx_int_reg_s	cn38xx;
	struct cvmx_asxx_int_reg_s	cn38xxp2;
	struct cvmx_asxx_int_reg_cn30xx cn50xx;
	struct cvmx_asxx_int_reg_s	cn58xx;
	struct cvmx_asxx_int_reg_s	cn58xxp1;
};

union cvmx_asxx_mii_rx_dat_set {
	uint64_t				u64;
	struct cvmx_asxx_mii_rx_dat_set_s {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_mii_rx_dat_set_s	cn30xx;
	struct cvmx_asxx_mii_rx_dat_set_s	cn50xx;
};

union cvmx_asxx_prt_loop {
	uint64_t u64;
	struct cvmx_asxx_prt_loop_s {
		uint64_t	reserved_8_63:56;
		uint64_t	ext_loop:4;
		uint64_t	int_loop:4;
	} s;
	struct cvmx_asxx_prt_loop_cn30xx {
		uint64_t	reserved_7_63:57;
		uint64_t	ext_loop:3;
		uint64_t	reserved_3_3:1;
		uint64_t	int_loop:3;
	} cn30xx;
	struct cvmx_asxx_prt_loop_cn30xx	cn31xx;
	struct cvmx_asxx_prt_loop_s		cn38xx;
	struct cvmx_asxx_prt_loop_s		cn38xxp2;
	struct cvmx_asxx_prt_loop_cn30xx	cn50xx;
	struct cvmx_asxx_prt_loop_s		cn58xx;
	struct cvmx_asxx_prt_loop_s		cn58xxp1;
};

union cvmx_asxx_rld_bypass {
	uint64_t			u64;
	struct cvmx_asxx_rld_bypass_s {
		uint64_t	reserved_1_63:63;
		uint64_t	bypass:1;
	} s;
	struct cvmx_asxx_rld_bypass_s	cn38xx;
	struct cvmx_asxx_rld_bypass_s	cn38xxp2;
	struct cvmx_asxx_rld_bypass_s	cn58xx;
	struct cvmx_asxx_rld_bypass_s	cn58xxp1;
};

union cvmx_asxx_rld_bypass_setting {
	uint64_t				u64;
	struct cvmx_asxx_rld_bypass_setting_s {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_rld_bypass_setting_s	cn38xx;
	struct cvmx_asxx_rld_bypass_setting_s	cn38xxp2;
	struct cvmx_asxx_rld_bypass_setting_s	cn58xx;
	struct cvmx_asxx_rld_bypass_setting_s	cn58xxp1;
};

union cvmx_asxx_rld_comp {
	uint64_t u64;
	struct cvmx_asxx_rld_comp_s {
		uint64_t	reserved_9_63:55;
		uint64_t	pctl:5;
		uint64_t	nctl:4;
	} s;
	struct cvmx_asxx_rld_comp_cn38xx {
		uint64_t	reserved_8_63:56;
		uint64_t	pctl:4;
		uint64_t	nctl:4;
	} cn38xx;
	struct cvmx_asxx_rld_comp_cn38xx	cn38xxp2;
	struct cvmx_asxx_rld_comp_s		cn58xx;
	struct cvmx_asxx_rld_comp_s		cn58xxp1;
};

union cvmx_asxx_rld_data_drv {
	uint64_t			u64;
	struct cvmx_asxx_rld_data_drv_s {
		uint64_t	reserved_8_63:56;
		uint64_t	pctl:4;
		uint64_t	nctl:4;
	} s;
	struct cvmx_asxx_rld_data_drv_s cn38xx;
	struct cvmx_asxx_rld_data_drv_s cn38xxp2;
	struct cvmx_asxx_rld_data_drv_s cn58xx;
	struct cvmx_asxx_rld_data_drv_s cn58xxp1;
};

union cvmx_asxx_rld_fcram_mode {
	uint64_t				u64;
	struct cvmx_asxx_rld_fcram_mode_s {
		uint64_t	reserved_1_63:63;
		uint64_t	mode:1;
	} s;
	struct cvmx_asxx_rld_fcram_mode_s	cn38xx;
	struct cvmx_asxx_rld_fcram_mode_s	cn38xxp2;
};

union cvmx_asxx_rld_nctl_strong {
	uint64_t				u64;
	struct cvmx_asxx_rld_nctl_strong_s {
		uint64_t	reserved_5_63:59;
		uint64_t	nctl:5;
	} s;
	struct cvmx_asxx_rld_nctl_strong_s	cn38xx;
	struct cvmx_asxx_rld_nctl_strong_s	cn38xxp2;
	struct cvmx_asxx_rld_nctl_strong_s	cn58xx;
	struct cvmx_asxx_rld_nctl_strong_s	cn58xxp1;
};

union cvmx_asxx_rld_nctl_weak {
	uint64_t				u64;
	struct cvmx_asxx_rld_nctl_weak_s {
		uint64_t	reserved_5_63:59;
		uint64_t	nctl:5;
	} s;
	struct cvmx_asxx_rld_nctl_weak_s	cn38xx;
	struct cvmx_asxx_rld_nctl_weak_s	cn38xxp2;
	struct cvmx_asxx_rld_nctl_weak_s	cn58xx;
	struct cvmx_asxx_rld_nctl_weak_s	cn58xxp1;
};

union cvmx_asxx_rld_pctl_strong {
	uint64_t				u64;
	struct cvmx_asxx_rld_pctl_strong_s {
		uint64_t	reserved_5_63:59;
		uint64_t	pctl:5;
	} s;
	struct cvmx_asxx_rld_pctl_strong_s	cn38xx;
	struct cvmx_asxx_rld_pctl_strong_s	cn38xxp2;
	struct cvmx_asxx_rld_pctl_strong_s	cn58xx;
	struct cvmx_asxx_rld_pctl_strong_s	cn58xxp1;
};

union cvmx_asxx_rld_pctl_weak {
	uint64_t				u64;
	struct cvmx_asxx_rld_pctl_weak_s {
		uint64_t	reserved_5_63:59;
		uint64_t	pctl:5;
	} s;
	struct cvmx_asxx_rld_pctl_weak_s	cn38xx;
	struct cvmx_asxx_rld_pctl_weak_s	cn38xxp2;
	struct cvmx_asxx_rld_pctl_weak_s	cn58xx;
	struct cvmx_asxx_rld_pctl_weak_s	cn58xxp1;
};

union cvmx_asxx_rld_setting {
	uint64_t u64;
	struct cvmx_asxx_rld_setting_s {
		uint64_t	reserved_13_63:51;
		uint64_t	dfaset:5;
		uint64_t	dfalag:1;
		uint64_t	dfalead:1;
		uint64_t	dfalock:1;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_rld_setting_cn38xx {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} cn38xx;
	struct cvmx_asxx_rld_setting_cn38xx	cn38xxp2;
	struct cvmx_asxx_rld_setting_s		cn58xx;
	struct cvmx_asxx_rld_setting_s		cn58xxp1;
};

union cvmx_asxx_rx_clk_setx {
	uint64_t			u64;
	struct cvmx_asxx_rx_clk_setx_s {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_rx_clk_setx_s	cn30xx;
	struct cvmx_asxx_rx_clk_setx_s	cn31xx;
	struct cvmx_asxx_rx_clk_setx_s	cn38xx;
	struct cvmx_asxx_rx_clk_setx_s	cn38xxp2;
	struct cvmx_asxx_rx_clk_setx_s	cn50xx;
	struct cvmx_asxx_rx_clk_setx_s	cn58xx;
	struct cvmx_asxx_rx_clk_setx_s	cn58xxp1;
};

union cvmx_asxx_rx_prt_en {
	uint64_t u64;
	struct cvmx_asxx_rx_prt_en_s {
		uint64_t	reserved_4_63:60;
		uint64_t	prt_en:4;
	} s;
	struct cvmx_asxx_rx_prt_en_cn30xx {
		uint64_t	reserved_3_63:61;
		uint64_t	prt_en:3;
	} cn30xx;
	struct cvmx_asxx_rx_prt_en_cn30xx	cn31xx;
	struct cvmx_asxx_rx_prt_en_s		cn38xx;
	struct cvmx_asxx_rx_prt_en_s		cn38xxp2;
	struct cvmx_asxx_rx_prt_en_cn30xx	cn50xx;
	struct cvmx_asxx_rx_prt_en_s		cn58xx;
	struct cvmx_asxx_rx_prt_en_s		cn58xxp1;
};

union cvmx_asxx_rx_wol {
	uint64_t			u64;
	struct cvmx_asxx_rx_wol_s {
		uint64_t	reserved_2_63:62;
		uint64_t	status:1;
		uint64_t	enable:1;
	} s;
	struct cvmx_asxx_rx_wol_s	cn38xx;
	struct cvmx_asxx_rx_wol_s	cn38xxp2;
};

union cvmx_asxx_rx_wol_msk {
	uint64_t			u64;
	struct cvmx_asxx_rx_wol_msk_s {
		uint64_t msk:64;
	} s;
	struct cvmx_asxx_rx_wol_msk_s	cn38xx;
	struct cvmx_asxx_rx_wol_msk_s	cn38xxp2;
};

union cvmx_asxx_rx_wol_powok {
	uint64_t			u64;
	struct cvmx_asxx_rx_wol_powok_s {
		uint64_t	reserved_1_63:63;
		uint64_t	powerok:1;
	} s;
	struct cvmx_asxx_rx_wol_powok_s cn38xx;
	struct cvmx_asxx_rx_wol_powok_s cn38xxp2;
};

union cvmx_asxx_rx_wol_sig {
	uint64_t			u64;
	struct cvmx_asxx_rx_wol_sig_s {
		uint64_t	reserved_32_63:32;
		uint64_t	sig:32;
	} s;
	struct cvmx_asxx_rx_wol_sig_s	cn38xx;
	struct cvmx_asxx_rx_wol_sig_s	cn38xxp2;
};

union cvmx_asxx_tx_clk_setx {
	uint64_t			u64;
	struct cvmx_asxx_tx_clk_setx_s {
		uint64_t	reserved_5_63:59;
		uint64_t	setting:5;
	} s;
	struct cvmx_asxx_tx_clk_setx_s	cn30xx;
	struct cvmx_asxx_tx_clk_setx_s	cn31xx;
	struct cvmx_asxx_tx_clk_setx_s	cn38xx;
	struct cvmx_asxx_tx_clk_setx_s	cn38xxp2;
	struct cvmx_asxx_tx_clk_setx_s	cn50xx;
	struct cvmx_asxx_tx_clk_setx_s	cn58xx;
	struct cvmx_asxx_tx_clk_setx_s	cn58xxp1;
};

union cvmx_asxx_tx_comp_byp {
	uint64_t u64;
	struct cvmx_asxx_tx_comp_byp_s {
		uint64_t reserved_0_63:64;
	} s;
	struct cvmx_asxx_tx_comp_byp_cn30xx {
		uint64_t	reserved_9_63:55;
		uint64_t	bypass:1;
		uint64_t	pctl:4;
		uint64_t	nctl:4;
	} cn30xx;
	struct cvmx_asxx_tx_comp_byp_cn30xx	cn31xx;
	struct cvmx_asxx_tx_comp_byp_cn38xx {
		uint64_t	reserved_8_63:56;
		uint64_t	pctl:4;
		uint64_t	nctl:4;
	} cn38xx;
	struct cvmx_asxx_tx_comp_byp_cn38xx	cn38xxp2;
	struct cvmx_asxx_tx_comp_byp_cn50xx {
		uint64_t	reserved_17_63:47;
		uint64_t	bypass:1;
		uint64_t	reserved_13_15:3;
		uint64_t	pctl:5;
		uint64_t	reserved_5_7:3;
		uint64_t	nctl:5;
	} cn50xx;
	struct cvmx_asxx_tx_comp_byp_cn58xx {
		uint64_t	reserved_13_63:51;
		uint64_t	pctl:5;
		uint64_t	reserved_5_7:3;
		uint64_t	nctl:5;
	} cn58xx;
	struct cvmx_asxx_tx_comp_byp_cn58xx cn58xxp1;
};

union cvmx_asxx_tx_hi_waterx {
	uint64_t u64;
	struct cvmx_asxx_tx_hi_waterx_s {
		uint64_t	reserved_4_63:60;
		uint64_t	mark:4;
	} s;
	struct cvmx_asxx_tx_hi_waterx_cn30xx {
		uint64_t	reserved_3_63:61;
		uint64_t	mark:3;
	} cn30xx;
	struct cvmx_asxx_tx_hi_waterx_cn30xx	cn31xx;
	struct cvmx_asxx_tx_hi_waterx_s		cn38xx;
	struct cvmx_asxx_tx_hi_waterx_s		cn38xxp2;
	struct cvmx_asxx_tx_hi_waterx_cn30xx	cn50xx;
	struct cvmx_asxx_tx_hi_waterx_s		cn58xx;
	struct cvmx_asxx_tx_hi_waterx_s		cn58xxp1;
};

union cvmx_asxx_tx_prt_en {
	uint64_t u64;
	struct cvmx_asxx_tx_prt_en_s {
		uint64_t	reserved_4_63:60;
		uint64_t	prt_en:4;
	} s;
	struct cvmx_asxx_tx_prt_en_cn30xx {
		uint64_t	reserved_3_63:61;
		uint64_t	prt_en:3;
	} cn30xx;
	struct cvmx_asxx_tx_prt_en_cn30xx	cn31xx;
	struct cvmx_asxx_tx_prt_en_s		cn38xx;
	struct cvmx_asxx_tx_prt_en_s		cn38xxp2;
	struct cvmx_asxx_tx_prt_en_cn30xx	cn50xx;
	struct cvmx_asxx_tx_prt_en_s		cn58xx;
	struct cvmx_asxx_tx_prt_en_s		cn58xxp1;
};

#endif
