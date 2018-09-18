/** @file
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _IMX8_CLK_PWR_REG_H_
#define _IMX8_CLK_PWR_REG_H_

// Array sizes helper macro
#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

// 25MHz clock from XTAL
#define IMX_REF_CLK_25M_FREQ 25000000

// Generic reg, set, clear and toggle offset for CCM
#define IMX_CCM_REG_OFFSET       0x00
#define IMX_CCM_SET_OFFSET       0x04
#define IMX_CCM_CLR_OFFSET       0x08
#define IMX_CCM_TOGGLE_OFFSET    0x0C

#define IMX_CCM_BASE 0x30380000

#define IMX_CCM_PLL_CTRL(a)                 (a < 38) ? IMX_CCM_BASE + 0x0800 + (0x10 * a) : IMX_CCM_BASE + 0x0A60
#define IMX_CCM_CCGR(a)                     IMX_CCM_BASE + 0x4000 + (0x10 * a)
#define IMX_CCM_TARGET_ROOT(a)              IMX_CCM_BASE + 0x8000 + (0x80 * a)

typedef enum {
  IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED = 0x00,
  IMX_CCM_CCGR_GATE_RUN = 0x01,
  IMX_CCM_CCGR_GATE_RUN_WAIT = 0x02,
  IMX_CCM_CCGR_GATE_ALL_THE_TIME = 0x03,
} IMX_CCM_CCGR_GATE_SETTING;

#define IMX_CCM_ANALOG_BASE 0x30360000

#define IMX_CCM_ANALOG_AUDIO_PLL1_CFG0_OFFSET     0x00
#define IMX_CCM_ANALOG_AUDIO_PLL1_CFG1_OFFSET     0x04
#define IMX_CCM_ANALOG_AUDIO_PLL2_CFG0_OFFSET     0x08
#define IMX_CCM_ANALOG_AUDIO_PLL2_CFG1_OFFSET     0x0C
#define IMX_CCM_ANALOG_VIDEO_PLL1_CFG0_OFFSET     0x10
#define IMX_CCM_ANALOG_VIDEO_PLL1_CFG1_OFFSET     0x14
#define IMX_CCM_ANALOG_GPU_PLL_CFG0_OFFSET        0x18
#define IMX_CCM_ANALOG_GPU_PLL_CFG1_OFFSET        0x1C
#define IMX_CCM_ANALOG_VPU_PLL_CFG0_OFFSET        0x20
#define IMX_CCM_ANALOG_VPU_PLL_CFG1_OFFSET        0x24
#define IMX_CCM_ANALOG_ARM_PLL_CFG0_OFFSET        0x28
#define IMX_CCM_ANALOG_ARM_PLL_CFG1_OFFSET        0x2C
#define IMX_CCM_ANALOG_SYS_PLL1_CFG0_OFFSET       0x30
#define IMX_CCM_ANALOG_SYS_PLL1_CFG1_OFFSET       0x34
#define IMX_CCM_ANALOG_SYS_PLL1_CFG2_OFFSET       0x38
#define IMX_CCM_ANALOG_SYS_PLL2_CFG0_OFFSET       0x3C
#define IMX_CCM_ANALOG_SYS_PLL2_CFG1_OFFSET       0x40
#define IMX_CCM_ANALOG_SYS_PLL2_CFG2_OFFSET       0x44
#define IMX_CCM_ANALOG_SYS_PLL3_CFG0_OFFSET       0x48
#define IMX_CCM_ANALOG_SYS_PLL3_CFG1_OFFSET       0x4C
#define IMX_CCM_ANALOG_SYS_PLL3_CFG2_OFFSET       0x50
#define IMX_CCM_ANALOG_VIDEO_PLL2_CFG0_OFFSET     0x54
#define IMX_CCM_ANALOG_VIDEO_PLL2_CFG1_OFFSET     0x58
#define IMX_CCM_ANALOG_VIDEO_PLL2_CFG2_OFFSET     0x5C
#define IMX_CCM_ANALOG_DRAM_PLL_CFG0_OFFSET       0x60
#define IMX_CCM_ANALOG_DRAM_PLL_CFG1_OFFSET       0x64
#define IMX_CCM_ANALOG_DRAM_PLL_CFG2_OFFSET       0x68

#define IMX_CCM_ANALOG_AUDIO_PLL1_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_AUDIO_PLL1_CFG0_OFFSET
#define IMX_CCM_ANALOG_AUDIO_PLL1_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_AUDIO_PLL1_CFG1_OFFSET
#define IMX_CCM_ANALOG_AUDIO_PLL2_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_AUDIO_PLL2_CFG0_OFFSET
#define IMX_CCM_ANALOG_AUDIO_PLL2_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_AUDIO_PLL2_CFG1_OFFSET
#define IMX_CCM_ANALOG_VIDEO_PLL1_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VIDEO_PLL1_CFG0_OFFSET
#define IMX_CCM_ANALOG_VIDEO_PLL1_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VIDEO_PLL1_CFG1_OFFSET
#define IMX_CCM_ANALOG_GPU_PLL_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_GPU_PLL_CFG0_OFFSET
#define IMX_CCM_ANALOG_GPU_PLL_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_GPU_PLL_CFG1_OFFSET
#define IMX_CCM_ANALOG_VPU_PLL_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VPU_PLL_CFG0_OFFSET
#define IMX_CCM_ANALOG_VPU_PLL_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VPU_PLL_CFG1_OFFSET
#define IMX_CCM_ANALOG_ARM_PLL_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_ARM_PLL_CFG0_OFFSET
#define IMX_CCM_ANALOG_ARM_PLL_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_ARM_PLL_CFG1_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL1_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL1_CFG0_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL1_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL1_CFG1_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL1_CFG2    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL1_CFG2_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL2_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL2_CFG0_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL2_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL2_CFG1_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL2_CFG2    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL2_CFG2_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL3_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL3_CFG0_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL3_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL3_CFG1_OFFSET
#define IMX_CCM_ANALOG_SYS_PLL3_CFG2    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_SYS_PLL3_CFG2_OFFSET
#define IMX_CCM_ANALOG_VIDEO_PLL2_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VIDEO_PLL2_CFG0_OFFSET
#define IMX_CCM_ANALOG_VIDEO_PLL2_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VIDEO_PLL2_CFG1_OFFSET
#define IMX_CCM_ANALOG_VIDEO_PLL2_CFG2    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_VIDEO_PLL2_CFG2_OFFSET
#define IMX_CCM_ANALOG_DRAM_PLL_CFG0    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_DRAM_PLL_CFG0_OFFSET
#define IMX_CCM_ANALOG_DRAM_PLL_CFG1    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_DRAM_PLL_CFG1_OFFSET
#define IMX_CCM_ANALOG_DRAM_PLL_CFG2    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_DRAM_PLL_CFG2_OFFSET

// Audio PLL limits
// Range of valid clock frequencies for both REF and post_divide REF 10MHz - 300Mhz
#define PLL_AUDIO_REF_MIN_FREQ 10000000
#define PLL_AUDIO_REF_MAX_FREQ 300000000

// Range of valid clock frequencies for output clock 30MHz - 2000Mhz
#define PLL_AUDIO_OUT_MIN_FREQ 30000000
#define PLL_AUDIO_OUT_MAX_FREQ 2000000000

// Range of valid PLL Integer and fractional divider
#define PLL_AUDIO_INT_DIV_MAX 32
#define PLL_AUDIO_FRAC_DIV_MAX 224

// Range of post and pre 
#define PLL_PRE_PODF_MAX 8
#define PLL_POST_PODF_MAX 64

typedef union {
  UINT32 AsUint32;
  struct {
    // LSB
    UINT32 OUTPUT_DIV_VAL : 5 ;     // 0-4 PLL Output divide value. 2-64 (value+1)^2
    UINT32 REFCLK_DIV_VAL : 6 ;     // 5-10 PLL reference clock divice value. 1-64 = value+1 Both REF and post_DIV must be between 10 and 300 MHz. 
    UINT32 NEWDIV_ACK : 1 ;         // 11 new fraction divide handshake
    UINT32 NEWDIV_VAL : 1 ;         // 12 new fraction divide input control
    UINT32 COUNTCLK_SEL : 1 ;       // 13 PLL maximum lock time counter clock select 0 = 25MHz, 1 = 27MHz
    UINT32 BYPASS : 1 ;             // 14 Bypass the PLL
    UINT32 LOCK_SEL : 1 ;           // 15 PLL Lock signal select 0 = PLL lock output, 1 = max lock time counter output
    UINT32 REFCLK_SEL : 2 ;         // 16-17 Reference clock select 00 = 25MHz, 01 = 27MHz, 10 = HDMI_PHY, 11 CLK_P_N
    UINT32 PD_OVERRIDE : 1 ;        // 18 The OVERRIDE bit allows the clock control module to automatically override portions of the register.
    UINT32 PD : 1 ;                 // 19 Powers down PLL
    UINT32 CLKE_OVERRIDE : 1 ;      // 20 The OVERRIDE bit allows the clock control module to automatically override portions of the register.
    UINT32 CLKE : 1 ;               // 21 Enable PLLL output
    UINT32 Reserved : 9 ;           // 22-30 Always set to zero
    UINT32 LOCK : 1 ;               // 31 PLL is/not currently locked
    // MSB
  };
} IMX_CCM_PLL_AUDIO_CFG0_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    // LSB
    UINT32 INT_DIV_CTL : 7 ;        // 0-6 PLL integer divide control
    UINT32 FRAC_DIV_CTL : 24 ;      // 7-30 PLL fractional divide control
    UINT32 Reserved : 1 ;           // 31 Always set to zero 
    // MSB
  };
} IMX_CCM_PLL_AUDIO_CFG1_REG;

typedef enum {
  IMX_25M_REF_CLK = 0x00,
  IMX_27M_REF_CLK = 0x01,
  IMX_HDMI_PHY_27M_CLK = 0x02,
  IMX_CLK_P_N = 0x03,
} IMX_PLL_REF_CLK_SEL;

typedef enum {
  IMX_POST_DIV_SEL_DIVIDE_2 = 0x01,
  IMX_POST_DIV_SEL_DIVIDE_1 = 0x02,
  IMX_POST_DIV_SEL_DIVIDE_4 = 0x03,
} IMX_CCM_PLL_VIDEO_CTRL_POST_DIV_SEL;

typedef enum {
  IMX_TEST_DIV_SEL_DIVIDE_4 = 0x00,
  IMX_TEST_DIV_SEL_DIVIDE_2 = 0x01,
  IMX_TEST_DIV_SEL_DIVIDE_1 = 0x02,
} IMX_CCM_PLL_VIDEO_CTRL_TEST_DIV_SEL;

typedef union {
  UINT32 AsUint32;
  struct {
    // LSB
    UINT32 POST_PODF : 6;           // 0-5 Post divider divide number
    UINT32 Reservedd0 : 10;         // 6-15
    UINT32 PRE_PODF : 3;            // 16-18 Pre divider divide the number.
    UINT32 Reserved2 : 5;           // 19-23
    UINT32 MUX : 3;                 // 24-26 Selection of clock sources
    UINT32 Reserved3 : 1;           // 27
    UINT32 ENABLE : 1;              // 28 Enable PLL output
    UINT32 Reserved4 : 3;           // 29-31 Always set to zero
    // MSB
  };
} IMX_CCM_TARGET_ROOT_REG;

#endif

