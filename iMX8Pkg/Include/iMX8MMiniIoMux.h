/** @file
*
*  Header defining IMX8M Mini Muxing
*
*  Copyright (c), Microsoft Corporation. All rights reserved.
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
#ifndef _IMX8MMini_IOMUX_H_
#define _IMX8MMini_IOMUX_H_

//
// IOMux common definition
//

#include <iMXIoMux.h>

//
// GPIO common definition
//

#include <iMXGpio.h>

//
// Pad control settings
//

typedef enum {
	IMX_PE_0_Pull_Resistors_Disabled,
	IMX_PE_1_Pull_Resistors_Enabled,
} IMX_PE;

typedef enum {
    IMX_HYS_DISABLED,
    IMX_HYS_ENABLED,
} IMX_HYS;

typedef enum {
    IMX_PUE_0_Pull_Up_Disabled, // select pull-down resistors
    IMX_PUE_1_Pull_Up_Enabled,  // select pull-up resistors
} IMX_PUE;

typedef enum {
    IMX_ODE_0_Open_Drain_Disabled,
    IMX_ODE_1_Open_Drain_Enabled,
} IMX_ODE;

typedef enum {
    IMX_FSEL_0_SLOW,
    IMX_FSEL_2_FAST = 2,
} IMX_FSEL;

typedef enum {
    IMX_DSE_0_X1,
    IMX_DSE_2_X2 = 2,
    IMX_DSE_4_X4 = 4,
    IMX_DSE_6_X6 = 6,
} IMX_DSE;

typedef enum {
    IMX_SION_DISABLED,
    IMX_SION_ENABLED,
} IMX_IOMUXC_CTL_SION;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 DSE : 3;
        UINT32 FSEL : 2;
        UINT32 ODE : 1;
        UINT32 PUE : 1;
        UINT32 HYS : 1;
        UINT32 PE : 1;
        UINT32 reserved : 23;
    } Fields;
} IMX_IOMUXC_PAD_CTL;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 MUX_MODE : 3;
        UINT32 reserved1 : 1;
        UINT32 SION : 1;
        UINT32 reserved2 : 27;
    } Fields;
} IMX_IOMUXC_MUX_CTL;

typedef struct {
    UINT32 DAISY : 3;
    UINT32 reserved : 29;
} IMX_IOMUXC_SEL_INP_CTL;

#define _IMX_SEL_INP_VALUE(InpSel) \
          (((InpSel) >> 8) & 0x07)

#define _IMX_SEL_INP_REGISTER(InpSel) \
          ((((InpSel) & 0xFF) * 4) + IOMUXC_SELECT_INPUT_BASE_ADDRESS)

// 11 bits
#define _IMX_MAKE_INP_SEL(InpSelReg, InpSelVal) \
          (((((InpSelReg) - IOMUXC_SELECT_INPUT_BASE_ADDRESS) / 4) & 0xFF) | \
          (((InpSelVal) & 0x7) << 8))

// 5 bits
#define _IMX_MAKE_MUX_CTL(Sion, MuxAlt) \
          (((MuxAlt) & 0x7) | \
          (((Sion) & 0x1) << 4))

// 9 bits
#define _IMX_MAKE_PAD_CTL(Dse, Fsel, Ode, Pue, Hys, Pe) \
          (((Dse) & 0x7) | \
          (((Fsel) & 0x3) << 3) | \
          (((Ode) & 0x1) << 5) | \
          (((Pue) & 0x1) << 6) | \
          (((Hys) & 0x1) << 7) | \
          (((Pe) & 0x1) << 8))

/**
  Define a configuration for a pad, including drive settings,
  MUX setting and Select Input setting and offset.

  Dse - IMX_DSE - Drive strength (Bit 0-2)
  Fsel - IMX_FSEL - Slew Rate setting (Bit 3,4)
  Ode - IMX_ODE - Open Drain Enable (bit 5)
  Pue - IMX_PUE - Pull Up/Down Select (bit 6)
  Hys - IMX_HYS - Hysteresis enable (Bit 7)
  Pe  - IMX_PE  - Pull Resistor enable (Bit 8)

  Sion - Software Input on Field (Bit 4)
  MuxAlt- Alternate function number (Bit 0-2)

  SelInpReg - select input register offset div 4
  SelInpVal - select input value (Bit 0-2)

  Pack format
  <Select Input 14-24><Mux CTL Bit 9-13><Pad CTL Bit 0-8>

**/

#define IMX_PAD_CTL_PACK_OFFSET         0
#define IMX_MUX_CTL_PACK_OFFSET         9
#define IMX_SELECT_INPUT_PACK_OFFSET    14

#define _IMX_MAKE_PADCFG_INPSEL(Dse, Fsel, Ode, Pue, Hys, Pe, Sion, MuxAlt, SelInpReg, SelInpValue) \
          (_IMX_MAKE_PAD_CTL(Dse, Fsel, Ode, Pue, Hys, Pe) | \
          (_IMX_MAKE_MUX_CTL(Sion, MuxAlt) << IMX_MUX_CTL_PACK_OFFSET) | \
          (_IMX_MAKE_INP_SEL(SelInpReg, SelInpValue) << IMX_SELECT_INPUT_PACK_OFFSET))

#define _IMX_MAKE_PADCFG(Dse, Fsel, Ode, Pue, Hys, Pe, Sion, MuxAlt) \
          (_IMX_MAKE_PAD_CTL(Dse, Fsel, Ode, Pue, Hys, Pe) | \
           _IMX_MAKE_MUX_CTL(Sion, MuxAlt) << IMX_MUX_CTL_PACK_OFFSET)

#define _IMX_PADCFG_PAD_CTL(PadCfg) ((PadCfg) & 0x000001FF)
#define _IMX_PADCFG_MUX_CTL(PadCfg) (((PadCfg) >> IMX_MUX_CTL_PACK_OFFSET) & 0x00000017)
#define _IMX_PADCFG_SEL_INP(PadCfg) (((PadCfg) >> IMX_SELECT_INPUT_PACK_OFFSET) & 0x000007FF)

typedef UINT32 IMX_PADCFG;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 MuxCtlRegOffset : 16;
        UINT32 PadCtlRegOffset : 16;
    } Fields;
} IMX_PAD_BITS;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 DSE : 3;
        UINT32 FSEL : 2;
        UINT32 ODE : 1;
        UINT32 PUE : 1;
        UINT32 HYS : 1;
        UINT32 PE : 1;
        UINT32 MuxAlt : 3;
        UINT32 Reserved0 : 1;
        UINT32 Sion : 1;
        UINT32 InpSelReg : 8;
        UINT32 InpSel : 3;
        UINT32 Reserved1 : 9;
    } Fields;
} IMX_PADCFG_BITS;

/**
  Select Input definitions
**/
#if 0
   GPIO1_IO05_ALT5 = 0x0 // Selecting ALT5 of pad GPIO1_IO05 for CCM_PMIC_READY

   GPIO1_IO07_ALT1 = 0x0 // Selecting ALT1 of pad GPIO1_IO07 for ENET1_MDIO

   GPIO1_IO11_ALT5 = 0x1 // Selecting ALT5 of pad GPIO1_IO11 for CCM_PMIC_READY

   GPIO1_IO14_ALT4 = 0x2 // Selecting ALT4 of pad GPIO1_IO14 for USDHC3_CD_B

   GPIO1_IO15_ALT4 = 0x2 // Selecting ALT4 of pad GPIO1_IO15 for USDHC3_WP

   ENET_MDIO_ALT0 = 0x1 // Selecting ALT0 of pad ENET_MDIO for ENET1_MDIO

   NAND_DATA02_ALT2 = 0x0 // Selecting ALT2 of pad NAND_DATA02 for USDHC3_CD_B

   NAND_DATA03_ALT2 = 0x0 // Selecting ALT2 of pad NAND_DATA03 for USDHC3_WP

   SAI5_RXFS_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_RXFS for SAI5_RX_SYNC

   SAI5_RXC_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_RXC for SAI5_RX_BCLK

   SAI5_RXD0_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_RXD0 for SAI5_RX_DATA_0
   SAI5_RXD0_ALT4 = 0x0 // Selecting ALT4 of pad SAI5_RXD0 for PDM_BIT_STREAM_0

   SAI5_RXD1_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_RXD1 for SAI5_RX_DATA_1
   SAI5_RXD1_ALT2 = 0x0 // Selecting ALT2 of pad SAI5_RXD1 for SAI1_TX_SYNC
   SAI5_RXD1_ALT3 = 0x0 // Selecting ALT3 of pad SAI5_RXD1 for SAI5_TX_SYNC
   SAI5_RXD1_ALT4 = 0x0 // Selecting ALT4 of pad SAI5_RXD1 for PDM_BIT_STREAM_1

   SAI5_RXD2_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_RXD2 for SAI5_RX_DATA_2
   SAI5_RXD2_ALT2 = 0x1 // Selecting ALT2 of pad SAI5_RXD2 for SAI1_TX_SYNC
   SAI5_RXD2_ALT3 = 0x0 // Selecting ALT3 of pad SAI5_RXD2 for SAI5_TX_BCLK
   SAI5_RXD2_ALT4 = 0x0 // Selecting ALT4 of pad SAI5_RXD2 for PDM_BIT_STREAM_2

   SAI5_RXD3_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_RXD3 for SAI5_RX_DATA_3
   SAI5_RXD3_ALT2 = 0x2 // Selecting ALT2 of pad SAI5_RXD3 for SAI1_TX_SYNC
   SAI5_RXD3_ALT4 = 0x0 // Selecting ALT4 of pad SAI5_RXD3 for PDM_BIT_STREAM_3

   SAI5_MCLK_ALT0 = 0x0 // Selecting ALT0 of pad SAI5_MCLK for SAI5_MCLK
   SAI5_MCLK_ALT1 = 0x0 // Selecting ALT1 of pad SAI5_MCLK for SAI1_TX_BCLK

   SAI1_RXFS_ALT0 = 0x0 // Selecting ALT0 of pad SAI1_RXFS for SAI1_RX_SYNC
   SAI1_RXFS_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_RXFS for SAI5_RX_SYNC

   SAI1_RXC_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_RXC for SAI5_RX_BCLK

   SAI1_RXD0_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_RXD0 for SAI5_RX_DATA_0
   SAI1_RXD0_ALT3 = 0x1 // Selecting ALT3 of pad SAI1_RXD0 for PDM_BIT_STREAM_0

   SAI1_RXD1_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_RXD1 for SAI5_RX_DATA_1
   SAI1_RXD1_ALT3 = 0x1 // Selecting ALT3 of pad SAI1_RXD1 for PDM_BIT_STREAM_1

   SAI1_RXD2_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_RXD2 for SAI5_RX_DATA_2
   SAI1_RXD2_ALT3 = 0x1 // Selecting ALT3 of pad SAI1_RXD2 for PDM_BIT_STREAM_2

   SAI1_RXD3_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_RXD3 for SAI5_RX_DATA_3
   SAI1_RXD3_ALT3 = 0x1 // Selecting ALT3 of pad SAI1_RXD3 for PDM_BIT_STREAM_3

   SAI1_RXD4_ALT1 = 0x0 // Selecting ALT1 of pad SAI1_RXD4 for SAI6_TX_BCLK
   SAI1_RXD4_ALT2 = 0x0 // Selecting ALT2 of pad SAI1_RXD4 for SAI6_RX_BCLK

   SAI1_RXD5_ALT2 = 0x0 // Selecting ALT2 of pad SAI1_RXD5 for SAI6_RX_DATA_0
   SAI1_RXD5_ALT3 = 0x1 // Selecting ALT3 of pad SAI1_RXD5 for SAI1_RX_SYNC

   SAI1_RXD6_ALT1 = 0x0 // Selecting ALT1 of pad SAI1_RXD6 for SAI6_TX_SYNC
   SAI1_RXD6_ALT2 = 0x0 // Selecting ALT2 of pad SAI1_RXD6 for SAI6_RX_SYNC

   SAI1_RXD7_ALT1 = 0x0 // Selecting ALT1 of pad SAI1_RXD7 for SAI6_MCLK
   SAI1_RXD7_ALT2 = 0x4 // Selecting ALT2 of pad SAI1_RXD7 for SAI1_TX_SYNC

   SAI1_TXFS_ALT0 = 0x3 // Selecting ALT0 of pad SAI1_TXFS for SAI1_TX_SYNC
   SAI1_TXFS_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_TXFS for SAI5_TX_SYNC

   SAI1_TXC_ALT0 = 0x1 // Selecting ALT0 of pad SAI1_TXC for SAI1_TX_BCLK
   SAI1_TXC_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_TXC for SAI5_TX_BCLK

   SAI1_TXD4_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_TXD4 for SAI6_RX_BCLK
   SAI1_TXD4_ALT2 = 0x1 // Selecting ALT2 of pad SAI1_TXD4 for SAI6_TX_BCLK

   SAI1_TXD5_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_TXD5 for SAI6_RX_DATA_0

   SAI1_TXD6_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_TXD6 for SAI6_RX_SYNC
   SAI1_TXD6_ALT2 = 0x1 // Selecting ALT2 of pad SAI1_TXD6 for SAI6_TX_SYNC

   SAI1_TXD7_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_TXD7 for SAI6_MCLK

   SAI1_MCLK_ALT1 = 0x1 // Selecting ALT1 of pad SAI1_MCLK for SAI5_MCLK
   SAI1_MCLK_ALT2 = 0x2 // Selecting ALT2 of pad SAI1_MCLK for SAI1_TX_BCLK

   SAI2_RXFS_ALT1 = 0x2 // Selecting ALT1 of pad SAI2_RXFS for SAI5_TX_SYNC
   SAI2_RXFS_ALT4 = 0x2 // Selecting ALT4 of pad SAI2_RXFS for UART1_RXD

   SAI2_RXC_ALT1 = 0x2 // Selecting ALT1 of pad SAI2_RXC for SAI5_TX_BCLK
   SAI2_RXC_ALT4 = 0x3 // Selecting ALT4 of pad SAI2_RXC for UART1_RXD

   SAI2_RXD0_ALT4 = 0x2 // Selecting ALT4 of pad SAI2_RXD0 for UART1_RTS_B

   SAI2_TXFS_ALT4 = 0x3 // Selecting ALT4 of pad SAI2_TXFS for UART1_RTS_B

   SAI2_MCLK_ALT1 = 0x2 // Selecting ALT1 of pad SAI2_MCLK for SAI5_MCLK

   SAI3_RXFS_ALT2 = 0x2 // Selecting ALT2 of pad SAI3_RXFS for SAI5_RX_SYNC

   SAI3_RXC_ALT2 = 0x2 // Selecting ALT2 of pad SAI3_RXC for SAI5_RX_BCLK
   SAI3_RXC_ALT4 = 0x2 // Selecting ALT4 of pad SAI3_RXC for UART2_RTS_B

   SAI3_RXD_ALT2 = 0x2 // Selecting ALT2 of pad SAI3_RXD for SAI5_RX_DATA_0
   SAI3_RXD_ALT4 = 0x3 // Selecting ALT4 of pad SAI3_RXD for UART2_RTS_B

   SAI3_TXFS_ALT1 = 0x2 // Selecting ALT1 of pad SAI3_TXFS for SAI5_RX_DATA_1
   SAI3_TXFS_ALT4 = 0x2 // Selecting ALT4 of pad SAI3_TXFS for UART2_RXD

   SAI3_TXC_ALT2 = 0x2 // Selecting ALT2 of pad SAI3_TXC for SAI5_RX_DATA_2
   SAI3_TXC_ALT4 = 0x3 // Selecting ALT4 of pad SAI3_TXC for UART2_RXD

   SAI3_TXD_ALT2 = 0x2 // Selecting ALT2 of pad SAI3_TXD for SAI5_RX_DATA_3

   SAI3_MCLK_ALT2 = 0x3 // Selecting ALT2 of pad SAI3_MCLK for SAI5_MCLK

   ECSPI1_SCLK_ALT1 = 0x0 // Selecting ALT1 of pad ECSPI1_SCLK for UART3_RXD

   ECSPI1_MOSI_ALT1 = 0x1 // Selecting ALT1 of pad ECSPI1_MOSI for UART3_RXD

   ECSPI1_MISO_ALT1 = 0x0 // Selecting ALT1 of pad ECSPI1_MISO for UART3_RTS_B

   ECSPI1_SS0_ALT1 = 0x1 // Selecting ALT1 of pad ECSPI1_SS0 for UART3_RTS_B

   ECSPI2_SCLK_ALT1 = 0x0 // Selecting ALT1 of pad ECSPI2_SCLK for UART4_RXD

   ECSPI2_MOSI_ALT1 = 0x1 // Selecting ALT1 of pad ECSPI2_MOSI for UART4_RXD

   ECSPI2_MISO_ALT1 = 0x0 // Selecting ALT1 of pad ECSPI2_MISO for UART4_RTS_B

   ECSPI2_SS0_ALT1 = 0x1 // Selecting ALT1 of pad ECSPI2_SS0 for UART4_RTS_B

   I2C1_SDA_ALT1 = 0x2 // Selecting ALT1 of pad I2C1_SDA for ENET1_MDIO

   I2C2_SCL_ALT2 = 0x1 // Selecting ALT2 of pad I2C2_SCL for USDHC3_CD_B

   I2C2_SDA_ALT2 = 0x1 // Selecting ALT2 of pad I2C2_SDA for USDHC3_WP

   I2C4_SCL_ALT2 = 0x0 // Selecting ALT2 of pad I2C4_SCL for PCIE1_CLKREQ_B

   UART1_RXD_ALT0 = 0x0 // Selecting ALT0 of pad UART1_RXD for UART1_RXD

   UART1_TXD_ALT0 = 0x1 // Selecting ALT0 of pad UART1_TXD for UART1_RXD

   UART2_RXD_ALT0 = 0x0 // Selecting ALT0 of pad UART2_RXD for UART2_RXD

   UART2_TXD_ALT0 = 0x1 // Selecting ALT0 of pad UART2_TXD for UART2_RXD

   UART3_RXD_ALT0 = 0x2 // Selecting ALT0 of pad UART3_RXD for UART3_RXD
   UART3_RXD_ALT1 = 0x0 // Selecting ALT1 of pad UART3_RXD for UART1_RTS_B

   UART3_TXD_ALT0 = 0x3 // Selecting ALT0 of pad UART3_TXD for UART3_RXD
   UART3_TXD_ALT1 = 0x1 // Selecting ALT1 of pad UART3_TXD for UART1_RTS_B

   UART4_RXD_ALT0 = 0x2 // Selecting ALT0 of pad UART4_RXD for UART4_RXD
   UART4_RXD_ALT1 = 0x0 // Selecting ALT1 of pad UART4_RXD for UART2_RTS_B
   UART4_RXD_ALT2 = 0x1 // Selecting ALT2 of pad UART4_RXD for PCIE1_CLKREQ_B

   UART4_TXD_ALT0 = 0x3 // Selecting ALT0 of pad UART4_TXD for UART4_RXD
   UART4_TXD_ALT1 = 0x1 // Selecting ALT1 of pad UART4_TXD for UART2_RTS_B
#endif

/*
    IOMux configuration dump created from iMX8 documentation
 */

typedef enum {
    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO00)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO00)
    //
    IMX_PAD_GPIO1_IO00 = IMX_IOMUX_PAD_DEFINE(0x290, 0x28), // GPIO1_IO00

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO01)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO01)
    //
    IMX_PAD_GPIO1_IO01 = IMX_IOMUX_PAD_DEFINE(0x294, 0x2C), // GPIO1_IO01

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO02)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO02)
    //
    IMX_PAD_GPIO1_IO02 = IMX_IOMUX_PAD_DEFINE(0x298, 0x30), // GPIO1_IO02

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03)
    //
    IMX_PAD_GPIO1_IO03 = IMX_IOMUX_PAD_DEFINE(0x29C, 0x34), // GPIO1_IO03

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO04)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04)
    //
    IMX_PAD_GPIO1_IO04 = IMX_IOMUX_PAD_DEFINE(0x2A0, 0x38), // GPIO1_IO04

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO05)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05)
    //
    IMX_PAD_GPIO1_IO05 = IMX_IOMUX_PAD_DEFINE(0x2A4, 0x3C), // GPIO1_IO05

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO06)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO06)
    //
    IMX_PAD_GPIO1_IO06 = IMX_IOMUX_PAD_DEFINE(0x2A8, 0x40), // GPIO1_IO06

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO07)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO07)
    //
    IMX_PAD_GPIO1_IO07 = IMX_IOMUX_PAD_DEFINE(0x2AC, 0x44), // GPIO1_IO07

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO08)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO08)
    //
    IMX_PAD_GPIO1_IO08 = IMX_IOMUX_PAD_DEFINE(0x2B0, 0x48), // GPIO1_IO08

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO09)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO09)
    //
    IMX_PAD_GPIO1_IO09 = IMX_IOMUX_PAD_DEFINE(0x2B4, 0x4C), // GPIO1_IO09

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO10)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO10)
    //
    IMX_PAD_GPIO1_IO10 = IMX_IOMUX_PAD_DEFINE(0x2B8, 0x50), // GPIO1_IO10

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO11)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO11)
    //
    IMX_PAD_GPIO1_IO11 = IMX_IOMUX_PAD_DEFINE(0x2BC, 0x54), // GPIO1_IO11

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO12)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO12)
    //
    IMX_PAD_GPIO1_IO12 = IMX_IOMUX_PAD_DEFINE(0x2C0, 0x58), // GPIO1_IO12

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO13)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO13)
    //
    IMX_PAD_GPIO1_IO13 = IMX_IOMUX_PAD_DEFINE(0x2C4, 0x5C), // GPIO1_IO13

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO14)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO14)
    //
    IMX_PAD_GPIO1_IO14 = IMX_IOMUX_PAD_DEFINE(0x2C8, 0x60), // GPIO1_IO14

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO15)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO15)
    //
    IMX_PAD_GPIO1_IO15 = IMX_IOMUX_PAD_DEFINE(0x2CC, 0x64), // GPIO1_IO15

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_MDC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_MDC)
    //
    IMX_PAD_ENET_MDC = IMX_IOMUX_PAD_DEFINE(0x2D0, 0x68), // ENET1_MDC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_MDIO)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_MDIO)
    //
    IMX_PAD_ENET_MDIO = IMX_IOMUX_PAD_DEFINE(0x2D4, 0x6C), // ENET1_MDIO

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TD3)
    //
    IMX_PAD_ENET_TD3 = IMX_IOMUX_PAD_DEFINE(0x2D8, 0x70), // ENET1_RGMII_TD3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TD2)
    //
    IMX_PAD_ENET_TD2 = IMX_IOMUX_PAD_DEFINE(0x2DC, 0x74), // ENET1_RGMII_TD2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TD1)
    //
    IMX_PAD_ENET_TD1 = IMX_IOMUX_PAD_DEFINE(0x2E0, 0x78), // ENET1_RGMII_TD1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TD0)
    //
    IMX_PAD_ENET_TD0 = IMX_IOMUX_PAD_DEFINE(0x2E4, 0x7C), // ENET1_RGMII_TD0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TX_CTL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TX_CTL)
    //
    IMX_PAD_ENET_TX_CTL = IMX_IOMUX_PAD_DEFINE(0x2E8, 0x80), // ENET1_RGMII_TX_CTL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TXC)
    //
    IMX_PAD_ENET_TXC = IMX_IOMUX_PAD_DEFINE(0x2EC, 0x84), // ENET1_RGMII_TXC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RX_CTL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RX_CTL)
    //
    IMX_PAD_ENET_RX_CTL = IMX_IOMUX_PAD_DEFINE(0x2F0, 0x88), // ENET1_RGMII_RX_CTL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RXC)
    //
    IMX_PAD_ENET_RXC = IMX_IOMUX_PAD_DEFINE(0x2F4, 0x8C), // ENET1_RGMII_RXC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RD0)
    //
    IMX_PAD_ENET_RD0 = IMX_IOMUX_PAD_DEFINE(0x2F8, 0x90), // ENET1_RGMII_RD0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RD1)
    //
    IMX_PAD_ENET_RD1 = IMX_IOMUX_PAD_DEFINE(0x2FC, 0x94), // ENET1_RGMII_RD1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RD2)
    //
    IMX_PAD_ENET_RD2 = IMX_IOMUX_PAD_DEFINE(0x300, 0x98), // ENET1_RGMII_RD2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RD3)
    //
    IMX_PAD_ENET_RD3 = IMX_IOMUX_PAD_DEFINE(0x304, 0x9C), // ENET1_RGMII_RD3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_CLK)
    //
    IMX_PAD_SD1_CLK = IMX_IOMUX_PAD_DEFINE(0x308, 0xA0), // USDHC1_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_CMD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_CMD)
    //
    IMX_PAD_SD1_CMD = IMX_IOMUX_PAD_DEFINE(0x30C, 0xA4), // USDHC1_CMD

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA0)
    //
    IMX_PAD_SD1_DATA0 = IMX_IOMUX_PAD_DEFINE(0x310, 0xA8), // USDHC1_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA1)
    //
    IMX_PAD_SD1_DATA1 = IMX_IOMUX_PAD_DEFINE(0x314, 0xAC), // USDHC1_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA2)
    //
    IMX_PAD_SD1_DATA2 = IMX_IOMUX_PAD_DEFINE(0x318, 0xB0), // USDHC1_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA3)
    //
    IMX_PAD_SD1_DATA3 = IMX_IOMUX_PAD_DEFINE(0x31C, 0xB4), // USDHC1_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA4)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA4)
    //
    IMX_PAD_SD1_DATA4 = IMX_IOMUX_PAD_DEFINE(0x320, 0xB8), // USDHC1_DATA4

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA5)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA5)
    //
    IMX_PAD_SD1_DATA5 = IMX_IOMUX_PAD_DEFINE(0x324, 0xBC), // USDHC1_DATA5

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA6)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA6)
    //
    IMX_PAD_SD1_DATA6 = IMX_IOMUX_PAD_DEFINE(0x328, 0xC0), // USDHC1_DATA6

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA7)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA7)
    //
    IMX_PAD_SD1_DATA7 = IMX_IOMUX_PAD_DEFINE(0x32C, 0xC4), // USDHC1_DATA7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_RESET_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_RESET_B)
    //
    IMX_PAD_SD1_RESET_B = IMX_IOMUX_PAD_DEFINE(0x330, 0xC8), // USDHC1_RESET_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_STROBE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_STROBE)
    //
    IMX_PAD_SD1_STROBE = IMX_IOMUX_PAD_DEFINE(0x334, 0xCC), // USDHC1_STROBE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_CD_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_CD_B)
    //
    IMX_PAD_SD2_CD_B = IMX_IOMUX_PAD_DEFINE(0x338, 0xD0), // USDHC2_CD_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_CLK)
    //
    IMX_PAD_SD2_CLK = IMX_IOMUX_PAD_DEFINE(0x33C, 0xD4), // USDHC2_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_CMD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_CMD)
    //
    IMX_PAD_SD2_CMD = IMX_IOMUX_PAD_DEFINE(0x340, 0xD8), // USDHC2_CMD

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA0)
    //
    IMX_PAD_SD2_DATA0 = IMX_IOMUX_PAD_DEFINE(0x344, 0xDC), // USDHC2_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA1)
    //
    IMX_PAD_SD2_DATA1 = IMX_IOMUX_PAD_DEFINE(0x348, 0xE0), // USDHC2_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA2)
    //
    IMX_PAD_SD2_DATA2 = IMX_IOMUX_PAD_DEFINE(0x34C, 0xE4), // USDHC2_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA3)
    //
    IMX_PAD_SD2_DATA3 = IMX_IOMUX_PAD_DEFINE(0x350, 0xE8), // USDHC2_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_RESET_B)
    //
    IMX_PAD_SD2_RESET_B = IMX_IOMUX_PAD_DEFINE(0x354, 0xEC), // USDHC2_RESET_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_WP)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_WP)
    //
    IMX_PAD_SD2_WP = IMX_IOMUX_PAD_DEFINE(0x358, 0xF0), // USDHC2_WP

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_ALE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_ALE)
    //
    IMX_PAD_NAND_ALE = IMX_IOMUX_PAD_DEFINE(0x35C, 0xF4), // RAWNAND_ALE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_CE0_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_CE0_B)
    //
    IMX_PAD_NAND_CE0_B = IMX_IOMUX_PAD_DEFINE(0x360, 0xF8), // RAWNAND_CE0_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_CE1_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B)
    //
    IMX_PAD_NAND_CE1_B = IMX_IOMUX_PAD_DEFINE(0x364, 0xFC), // RAWNAND_CE1_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_CE2_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_CE2_B)
    //
    IMX_PAD_NAND_CE2_B = IMX_IOMUX_PAD_DEFINE(0x368, 0x100), // RAWNAND_CE2_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_CE3_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_CE3_B)
    //
    IMX_PAD_NAND_CE3_B = IMX_IOMUX_PAD_DEFINE(0x36C, 0x104), // RAWNAND_CE3_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_CLE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_CLE)
    //
    IMX_PAD_NAND_CLE = IMX_IOMUX_PAD_DEFINE(0x370, 0x108), // RAWNAND_CLE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA00)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA00)
    //
    IMX_PAD_NAND_DATA00 = IMX_IOMUX_PAD_DEFINE(0x374, 0x10C), // RAWNAND_DATA00

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA01)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA01)
    //
    IMX_PAD_NAND_DATA01 = IMX_IOMUX_PAD_DEFINE(0x378, 0x110), // RAWNAND_DATA01

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA02)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA02)
    //
    IMX_PAD_NAND_DATA02 = IMX_IOMUX_PAD_DEFINE(0x37C, 0x114), // RAWNAND_DATA02

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA03)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA03)
    //
    IMX_PAD_NAND_DATA03 = IMX_IOMUX_PAD_DEFINE(0x380, 0x118), // RAWNAND_DATA03

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA04)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA04)
    //
    IMX_PAD_NAND_DATA04 = IMX_IOMUX_PAD_DEFINE(0x384, 0x11C), // RAWNAND_DATA04

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA05)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA05)
    //
    IMX_PAD_NAND_DATA05 = IMX_IOMUX_PAD_DEFINE(0x388, 0x120), // RAWNAND_DATA05

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA06)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA06)
    //
    IMX_PAD_NAND_DATA06 = IMX_IOMUX_PAD_DEFINE(0x38C, 0x124), // RAWNAND_DATA06

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DATA07)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DATA07)
    //
    IMX_PAD_NAND_DATA07 = IMX_IOMUX_PAD_DEFINE(0x390, 0x128), // RAWNAND_DATA07

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_DQS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_DQS)
    //
    IMX_PAD_NAND_DQS = IMX_IOMUX_PAD_DEFINE(0x394, 0x12C), // RAWNAND_DQS

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_RE_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_RE_B)
    //
    IMX_PAD_NAND_RE_B = IMX_IOMUX_PAD_DEFINE(0x398, 0x130), // RAWNAND_RE_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_READY_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_READY_B)
    //
    IMX_PAD_NAND_READY_B = IMX_IOMUX_PAD_DEFINE(0x39C, 0x134), // RAWNAND_READY_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_WE_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_WE_B)
    //
    IMX_PAD_NAND_WE_B = IMX_IOMUX_PAD_DEFINE(0x3A0, 0x138), // RAWNAND_WE_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_NAND_WP_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_NAND_WP_B)
    //
    IMX_PAD_NAND_WP_B = IMX_IOMUX_PAD_DEFINE(0x3A4, 0x13C), // RAWNAND_WP_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_RXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_RXFS)
    //
    IMX_PAD_SAI5_RXFS = IMX_IOMUX_PAD_DEFINE(0x3A8, 0x140), // SAI5_RX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_RXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_RXC)
    //
    IMX_PAD_SAI5_RXC = IMX_IOMUX_PAD_DEFINE(0x3AC, 0x144), // SAI5_RX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_RXD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD0)
    //
    IMX_PAD_SAI5_RXD0 = IMX_IOMUX_PAD_DEFINE(0x3B0, 0x148), // SAI5_RX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_RXD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD1)
    //
    IMX_PAD_SAI5_RXD1 = IMX_IOMUX_PAD_DEFINE(0x3B4, 0x14C), // SAI5_RX_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_RXD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD2)
    //
    IMX_PAD_SAI5_RXD2 = IMX_IOMUX_PAD_DEFINE(0x3B8, 0x150), // SAI5_RX_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_RXD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD3)
    //
    IMX_PAD_SAI5_RXD3 = IMX_IOMUX_PAD_DEFINE(0x3BC, 0x154), // SAI5_RX_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI5_MCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI5_MCLK)
    //
    IMX_PAD_SAI5_MCLK = IMX_IOMUX_PAD_DEFINE(0x3C0, 0x158), // SAI5_MCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXFS)
    //
    IMX_PAD_SAI1_RXFS = IMX_IOMUX_PAD_DEFINE(0x3C4, 0x15C), // SAI1_RX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXC)
    //
    IMX_PAD_SAI1_RXC = IMX_IOMUX_PAD_DEFINE(0x3C8, 0x160), // SAI1_RX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD0)
    //
    IMX_PAD_SAI1_RXD0 = IMX_IOMUX_PAD_DEFINE(0x3CC, 0x164), // SAI1_RX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD1)
    //
    IMX_PAD_SAI1_RXD1 = IMX_IOMUX_PAD_DEFINE(0x3D0, 0x168), // SAI1_RX_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD2)
    //
    IMX_PAD_SAI1_RXD2 = IMX_IOMUX_PAD_DEFINE(0x3D4, 0x16C), // SAI1_RX_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD3)
    //
    IMX_PAD_SAI1_RXD3 = IMX_IOMUX_PAD_DEFINE(0x3D8, 0x170), // SAI1_RX_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD4)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD4)
    //
    IMX_PAD_SAI1_RXD4 = IMX_IOMUX_PAD_DEFINE(0x3DC, 0x174), // SAI1_RX_DATA4

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD5)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD5)
    //
    IMX_PAD_SAI1_RXD5 = IMX_IOMUX_PAD_DEFINE(0x3E0, 0x178), // SAI1_RX_DATA5

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD6)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD6)
    //
    IMX_PAD_SAI1_RXD6 = IMX_IOMUX_PAD_DEFINE(0x3E4, 0x17C), // SAI1_RX_DATA6

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RXD7)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD7)
    //
    IMX_PAD_SAI1_RXD7 = IMX_IOMUX_PAD_DEFINE(0x3E8, 0x180), // SAI1_RX_DATA7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXFS)
    //
    IMX_PAD_SAI1_TXFS = IMX_IOMUX_PAD_DEFINE(0x3EC, 0x184), // SAI1_TX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXC)
    //
    IMX_PAD_SAI1_TXC = IMX_IOMUX_PAD_DEFINE(0x3F0, 0x188), // SAI1_TX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD0)
    //
    IMX_PAD_SAI1_TXD0 = IMX_IOMUX_PAD_DEFINE(0x3F4, 0x18C), // SAI1_TX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD1)
    //
    IMX_PAD_SAI1_TXD1 = IMX_IOMUX_PAD_DEFINE(0x3F8, 0x190), // SAI1_TX_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD2)
    //
    IMX_PAD_SAI1_TXD2 = IMX_IOMUX_PAD_DEFINE(0x3FC, 0x194), // SAI1_TX_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD3)
    //
    IMX_PAD_SAI1_TXD3 = IMX_IOMUX_PAD_DEFINE(0x400, 0x198), // SAI1_TX_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD4)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD4)
    //
    IMX_PAD_SAI1_TXD4 = IMX_IOMUX_PAD_DEFINE(0x404, 0x19C), // SAI1_TX_DATA4

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD5)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD5)
    //
    IMX_PAD_SAI1_TXD5 = IMX_IOMUX_PAD_DEFINE(0x408, 0x1A0), // SAI1_TX_DATA5

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD6)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD6)
    //
    IMX_PAD_SAI1_TXD6 = IMX_IOMUX_PAD_DEFINE(0x40C, 0x1A4), // SAI1_TX_DATA6

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TXD7)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD7)
    //
    IMX_PAD_SAI1_TXD7 = IMX_IOMUX_PAD_DEFINE(0x410, 0x1A8), // SAI1_TX_DATA7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_MCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_MCLK)
    //
    IMX_PAD_SAI1_MCLK = IMX_IOMUX_PAD_DEFINE(0x414, 0x1AC), // SAI1_MCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_RXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_RXFS)
    //
    IMX_PAD_SAI2_RXFS = IMX_IOMUX_PAD_DEFINE(0x418, 0x1B0), // SAI2_RX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_RXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_RXC)
    //
    IMX_PAD_SAI2_RXC = IMX_IOMUX_PAD_DEFINE(0x41C, 0x1B4), // SAI2_RX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_RXD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_RXD0)
    //
    IMX_PAD_SAI2_RXD0 = IMX_IOMUX_PAD_DEFINE(0x420, 0x1B8), // SAI2_RX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_TXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_TXFS)
    //
    IMX_PAD_SAI2_TXFS = IMX_IOMUX_PAD_DEFINE(0x424, 0x1BC), // SAI2_TX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_TXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_TXC)
    //
    IMX_PAD_SAI2_TXC = IMX_IOMUX_PAD_DEFINE(0x428, 0x1C0), // SAI2_TX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_TXD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_TXD0)
    //
    IMX_PAD_SAI2_TXD0 = IMX_IOMUX_PAD_DEFINE(0x42C, 0x1C4), // SAI2_TX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_MCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_MCLK)
    //
    IMX_PAD_SAI2_MCLK = IMX_IOMUX_PAD_DEFINE(0x430, 0x1C8), // SAI2_MCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_RXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_RXFS)
    //
    IMX_PAD_SAI3_RXFS = IMX_IOMUX_PAD_DEFINE(0x434, 0x1CC), // SAI3_RX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_RXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_RXC)
    //
    IMX_PAD_SAI3_RXC = IMX_IOMUX_PAD_DEFINE(0x438, 0x1D0), // SAI3_RX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_RXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_RXD)
    //
    IMX_PAD_SAI3_RXD = IMX_IOMUX_PAD_DEFINE(0x43C, 0x1D4), // SAI3_RX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_TXFS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_TXFS)
    //
    IMX_PAD_SAI3_TXFS = IMX_IOMUX_PAD_DEFINE(0x440, 0x1D8), // SAI3_TX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_TXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_TXC)
    //
    IMX_PAD_SAI3_TXC = IMX_IOMUX_PAD_DEFINE(0x444, 0x1DC), // SAI3_TX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_TXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_TXD)
    //
    IMX_PAD_SAI3_TXD = IMX_IOMUX_PAD_DEFINE(0x448, 0x1E0), // SAI3_TX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI3_MCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI3_MCLK)
    //
    IMX_PAD_SAI3_MCLK = IMX_IOMUX_PAD_DEFINE(0x44C, 0x1E4), // SAI3_MCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SPDIF_TX)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SPDIF_TX)
    //
    IMX_PAD_SPDIF_TX = IMX_IOMUX_PAD_DEFINE(0x450, 0x1E8), // SPDIF1_OUT

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SPDIF_RX)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SPDIF_RX)
    //
    IMX_PAD_SPDIF_RX = IMX_IOMUX_PAD_DEFINE(0x454, 0x1EC), // SPDIF1_IN

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SPDIF_EXT_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SPDIF_EXT_CLK)
    //
    IMX_PAD_SPDIF_EXT_CLK = IMX_IOMUX_PAD_DEFINE(0x458, 0x1F0), // SPDIF1_EXT_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_SCLK)
    //
    IMX_PAD_ECSPI1_SCLK = IMX_IOMUX_PAD_DEFINE(0x45C, 0x1F4), // ECSPI1_SCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_MOSI)
    //
    IMX_PAD_ECSPI1_MOSI = IMX_IOMUX_PAD_DEFINE(0x460, 0x1F8), // ECSPI1_MOSI

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MISO)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_MISO)
    //
    IMX_PAD_ECSPI1_MISO = IMX_IOMUX_PAD_DEFINE(0x464, 0x1FC), // ECSPI1_MISO

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_SS0)
    //
    IMX_PAD_ECSPI1_SS0 = IMX_IOMUX_PAD_DEFINE(0x468, 0x200), // ECSPI1_SS0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_SCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SCLK)
    //
    IMX_PAD_ECSPI2_SCLK = IMX_IOMUX_PAD_DEFINE(0x46C, 0x204), // ECSPI2_SCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MOSI)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MOSI)
    //
    IMX_PAD_ECSPI2_MOSI = IMX_IOMUX_PAD_DEFINE(0x470, 0x208), // ECSPI2_MOSI

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MISO)
    //
    IMX_PAD_ECSPI2_MISO = IMX_IOMUX_PAD_DEFINE(0x474, 0x20C), // ECSPI2_MISO

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_SS0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SS0)
    //
    IMX_PAD_ECSPI2_SS0 = IMX_IOMUX_PAD_DEFINE(0x478, 0x210), // ECSPI2_SS0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C1_SCL)
    //
    IMX_PAD_I2C1_SCL = IMX_IOMUX_PAD_DEFINE(0x47C, 0x214), // I2C1_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C1_SDA)
    //
    IMX_PAD_I2C1_SDA = IMX_IOMUX_PAD_DEFINE(0x480, 0x218), // I2C1_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C2_SCL)
    //
    IMX_PAD_I2C2_SCL = IMX_IOMUX_PAD_DEFINE(0x484, 0x21C), // I2C2_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C2_SDA)
    //
    IMX_PAD_I2C2_SDA = IMX_IOMUX_PAD_DEFINE(0x488, 0x220), // I2C2_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C3_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C3_SCL)
    //
    IMX_PAD_I2C3_SCL = IMX_IOMUX_PAD_DEFINE(0x48C, 0x224), // I2C3_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C3_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C3_SDA)
    //
    IMX_PAD_I2C3_SDA = IMX_IOMUX_PAD_DEFINE(0x490, 0x228), // I2C3_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C4_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C4_SCL)
    //
    IMX_PAD_I2C4_SCL = IMX_IOMUX_PAD_DEFINE(0x494, 0x22C), // I2C4_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C4_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C4_SDA)
    //
    IMX_PAD_I2C4_SDA = IMX_IOMUX_PAD_DEFINE(0x498, 0x230), // I2C4_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART1_RXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART1_RXD)
    //
    IMX_PAD_UART1_RXD = IMX_IOMUX_PAD_DEFINE(0x49C, 0x234), // UART1_RX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART1_TXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART1_TXD)
    //
    IMX_PAD_UART1_TXD = IMX_IOMUX_PAD_DEFINE(0x4A0, 0x238), // UART1_TX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART2_RXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART2_RXD)
    //
    IMX_PAD_UART2_RXD = IMX_IOMUX_PAD_DEFINE(0x4A4, 0x23C), // UART2_RX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART2_TXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART2_TXD)
    //
    IMX_PAD_UART2_TXD = IMX_IOMUX_PAD_DEFINE(0x4A8, 0x240), // UART2_TX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART3_RXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART3_RXD)
    //
    IMX_PAD_UART3_RXD = IMX_IOMUX_PAD_DEFINE(0x4AC, 0x244), // UART3_RX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART3_TXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART3_TXD)
    //
    IMX_PAD_UART3_TXD = IMX_IOMUX_PAD_DEFINE(0x4B0, 0x248), // UART3_TX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART4_RXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART4_RXD)
    //
    IMX_PAD_UART4_RXD = IMX_IOMUX_PAD_DEFINE(0x4B4, 0x24C), // UART4_RX

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART4_TXD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART4_TXD)
    //
    IMX_PAD_UART4_TXD = IMX_IOMUX_PAD_DEFINE(0x4B8, 0x250), // UART4_TX

} IMX_PAD;

// Alternate function numbers
typedef enum {
   IMX_IOMUXC_GPIO1_IO00_ALT0_GPIO1_IO00 = 0,
   IMX_IOMUXC_GPIO1_IO00_ALT1_CCMSRCGPCMIX_ENET_PHY_REF_CLK_ROOT = 1,
   IMX_IOMUXC_GPIO1_IO00_ALT5_ANAMIX_REF_CLK_32K = 5,
   IMX_IOMUXC_GPIO1_IO00_ALT6_CCMSRCGPCMIX_EXT_CLK1 = 6,
} IMX_IOMUXC_GPIO1_IO00_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO01_ALT0_GPIO1_IO01 = 0,
   IMX_IOMUXC_GPIO1_IO01_ALT1_PWM1_OUT = 1,
   IMX_IOMUXC_GPIO1_IO01_ALT5_ANAMIX_REF_CLK_24M = 5,
   IMX_IOMUXC_GPIO1_IO01_ALT6_CCMSRCGPCMIX_EXT_CLK2 = 6,
} IMX_IOMUXC_GPIO1_IO01_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO02_ALT0_GPIO1_IO02 = 0,
   IMX_IOMUXC_GPIO1_IO02_ALT1_WDOG1_WDOG_B = 1,
   IMX_IOMUXC_GPIO1_IO02_ALT5_WDOG1_WDOG_ANY = 5,
   IMX_IOMUXC_GPIO1_IO02_ALT7_SJC_DE_B = 7,
} IMX_IOMUXC_GPIO1_IO02_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO03_ALT0_GPIO1_IO03 = 0,
   IMX_IOMUXC_GPIO1_IO03_ALT1_USDHC1_VSELECT = 1,
   IMX_IOMUXC_GPIO1_IO03_ALT5_SDMA1_EXT_EVENT0 = 5,
} IMX_IOMUXC_GPIO1_IO03_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO04_ALT0_GPIO1_IO04 = 0,
   IMX_IOMUXC_GPIO1_IO04_ALT1_USDHC2_VSELECT = 1,
   IMX_IOMUXC_GPIO1_IO04_ALT5_SDMA1_EXT_EVENT1 = 5,
} IMX_IOMUXC_GPIO1_IO04_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO05_ALT0_GPIO1_IO05 = 0,
   IMX_IOMUXC_GPIO1_IO05_ALT1_M4_NMI = 1,
   IMX_IOMUXC_GPIO1_IO05_ALT5_CCM_PMIC_READY = 5,
} IMX_IOMUXC_GPIO1_IO05_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO06_ALT0_GPIO1_IO06 = 0,
   IMX_IOMUXC_GPIO1_IO06_ALT1_ENET1_MDC = 1,
   IMX_IOMUXC_GPIO1_IO06_ALT5_USDHC1_CD_B = 5,
   IMX_IOMUXC_GPIO1_IO06_ALT6_CCMSRCGPCMIX_EXT_CLK3 = 6,
} IMX_IOMUXC_GPIO1_IO06_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO07_ALT0_GPIO1_IO07 = 0,
   IMX_IOMUXC_GPIO1_IO07_ALT1_ENET1_MDIO = 1,
   IMX_IOMUXC_GPIO1_IO07_ALT5_USDHC1_WP = 5,
   IMX_IOMUXC_GPIO1_IO07_ALT6_CCMSRCGPCMIX_EXT_CLK4 = 6,
} IMX_IOMUXC_GPIO1_IO07_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO08_ALT0_GPIO1_IO08 = 0,
   IMX_IOMUXC_GPIO1_IO08_ALT1_ENET1_1588_EVENT0_IN = 1,
   IMX_IOMUXC_GPIO1_IO08_ALT5_USDHC2_RESET_B = 5,
} IMX_IOMUXC_GPIO1_IO08_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO09_ALT0_GPIO1_IO09 = 0,
   IMX_IOMUXC_GPIO1_IO09_ALT1_ENET1_1588_EVENT0_OUT = 1,
   IMX_IOMUXC_GPIO1_IO09_ALT4_USDHC3_RESET_B = 4,
   IMX_IOMUXC_GPIO1_IO09_ALT5_SDMA2_EXT_EVENT0 = 5,
} IMX_IOMUXC_GPIO1_IO09_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO10_ALT0_GPIO1_IO10 = 0,
   IMX_IOMUXC_GPIO1_IO10_ALT1_USB1_OTG_ID = 1,
} IMX_IOMUXC_GPIO1_IO10_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO11_ALT0_GPIO1_IO11 = 0,
   IMX_IOMUXC_GPIO1_IO11_ALT1_USB2_OTG_ID = 1,
   IMX_IOMUXC_GPIO1_IO11_ALT4_USDHC3_VSELECT = 4,
   IMX_IOMUXC_GPIO1_IO11_ALT5_CCM_PMIC_READY = 5,
} IMX_IOMUXC_GPIO1_IO11_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO12_ALT0_GPIO1_IO12 = 0,
   IMX_IOMUXC_GPIO1_IO12_ALT1_USB1_OTG_PWR = 1,
   IMX_IOMUXC_GPIO1_IO12_ALT5_SDMA2_EXT_EVENT1 = 5,
} IMX_IOMUXC_GPIO1_IO12_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO13_ALT0_GPIO1_IO13 = 0,
   IMX_IOMUXC_GPIO1_IO13_ALT1_USB1_OTG_OC = 1,
   IMX_IOMUXC_GPIO1_IO13_ALT5_PWM2_OUT = 5,
} IMX_IOMUXC_GPIO1_IO13_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO14_ALT0_GPIO1_IO14 = 0,
   IMX_IOMUXC_GPIO1_IO14_ALT1_USB2_OTG_PWR = 1,
   IMX_IOMUXC_GPIO1_IO14_ALT4_USDHC3_CD_B = 4,
   IMX_IOMUXC_GPIO1_IO14_ALT5_PWM3_OUT = 5,
   IMX_IOMUXC_GPIO1_IO14_ALT6_CCMSRCGPCMIX_CLKO1 = 6,
} IMX_IOMUXC_GPIO1_IO14_ALT;

typedef enum {
   IMX_IOMUXC_GPIO1_IO15_ALT0_GPIO1_IO15 = 0,
   IMX_IOMUXC_GPIO1_IO15_ALT1_USB2_OTG_OC = 1,
   IMX_IOMUXC_GPIO1_IO15_ALT4_USDHC3_WP = 4,
   IMX_IOMUXC_GPIO1_IO15_ALT5_PWM4_OUT = 5,
   IMX_IOMUXC_GPIO1_IO15_ALT6_CCMSRCGPCMIX_CLKO2 = 6,
} IMX_IOMUXC_GPIO1_IO15_ALT;

typedef enum {
   IMX_IOMUXC_ENET_MDC_ALT0_ENET1_MDC = 0,
   IMX_IOMUXC_ENET_MDC_ALT5_GPIO1_IO16 = 5,
} IMX_IOMUXC_ENET_MDC_ALT;

typedef enum {
   IMX_IOMUXC_ENET_MDIO_ALT0_ENET1_MDIO = 0,
   IMX_IOMUXC_ENET_MDIO_ALT5_GPIO1_IO17 = 5,
} IMX_IOMUXC_ENET_MDIO_ALT;

typedef enum {
   IMX_IOMUXC_ENET_TD3_ALT0_ENET1_RGMII_TD3 = 0,
   IMX_IOMUXC_ENET_TD3_ALT5_GPIO1_IO18 = 5,
} IMX_IOMUXC_ENET_TD3_ALT;

typedef enum {
   IMX_IOMUXC_ENET_TD2_ALT0_ENET1_RGMII_TD2 = 0,
   IMX_IOMUXC_ENET_TD2_ALT1_ENET1_TX_CLK = 1,
   IMX_IOMUXC_ENET_TD2_ALT5_GPIO1_IO19 = 5,
} IMX_IOMUXC_ENET_TD2_ALT;

typedef enum {
   IMX_IOMUXC_ENET_TD1_ALT0_ENET1_RGMII_TD1 = 0,
   IMX_IOMUXC_ENET_TD1_ALT5_GPIO1_IO20 = 5,
} IMX_IOMUXC_ENET_TD1_ALT;

typedef enum {
   IMX_IOMUXC_ENET_TD0_ALT0_ENET1_RGMII_TD0 = 0,
   IMX_IOMUXC_ENET_TD0_ALT5_GPIO1_IO21 = 5,
} IMX_IOMUXC_ENET_TD0_ALT;

typedef enum {
   IMX_IOMUXC_ENET_TX_CTL_ALT0_ENET1_RGMII_TX_CTL = 0,
   IMX_IOMUXC_ENET_TX_CTL_ALT5_GPIO1_IO22 = 5,
} IMX_IOMUXC_ENET_TX_CTL_ALT;

typedef enum {
   IMX_IOMUXC_ENET_TXC_ALT0_ENET1_RGMII_TXC = 0,
   IMX_IOMUXC_ENET_TXC_ALT1_ENET1_TX_ER = 1,
   IMX_IOMUXC_ENET_TXC_ALT5_GPIO1_IO23 = 5,
} IMX_IOMUXC_ENET_TXC_ALT;

typedef enum {
   IMX_IOMUXC_ENET_RX_CTL_ALT0_ENET1_RGMII_RX_CTL = 0,
   IMX_IOMUXC_ENET_RX_CTL_ALT5_GPIO1_IO24 = 5,
} IMX_IOMUXC_ENET_RX_CTL_ALT;

typedef enum {
   IMX_IOMUXC_ENET_RXC_ALT0_ENET1_RGMII_RXC = 0,
   IMX_IOMUXC_ENET_RXC_ALT1_ENET1_RX_ER = 1,
   IMX_IOMUXC_ENET_RXC_ALT5_GPIO1_IO25 = 5,
} IMX_IOMUXC_ENET_RXC_ALT;

typedef enum {
   IMX_IOMUXC_ENET_RD0_ALT0_ENET1_RGMII_RD0 = 0,
   IMX_IOMUXC_ENET_RD0_ALT5_GPIO1_IO26 = 5,
} IMX_IOMUXC_ENET_RD0_ALT;

typedef enum {
   IMX_IOMUXC_ENET_RD1_ALT0_ENET1_RGMII_RD1 = 0,
   IMX_IOMUXC_ENET_RD1_ALT5_GPIO1_IO27 = 5,
} IMX_IOMUXC_ENET_RD1_ALT;

typedef enum {
   IMX_IOMUXC_ENET_RD2_ALT0_ENET1_RGMII_RD2 = 0,
   IMX_IOMUXC_ENET_RD2_ALT5_GPIO1_IO28 = 5,
} IMX_IOMUXC_ENET_RD2_ALT;

typedef enum {
   IMX_IOMUXC_ENET_RD3_ALT0_ENET1_RGMII_RD3 = 0,
   IMX_IOMUXC_ENET_RD3_ALT5_GPIO1_IO29 = 5,
} IMX_IOMUXC_ENET_RD3_ALT;

typedef enum {
   IMX_IOMUXC_SD1_CLK_ALT0_USDHC1_CLK = 0,
   IMX_IOMUXC_SD1_CLK_ALT5_GPIO2_IO00 = 5,
} IMX_IOMUXC_SD1_CLK_ALT;

typedef enum {
   IMX_IOMUXC_SD1_CMD_ALT0_USDHC1_CMD = 0,
   IMX_IOMUXC_SD1_CMD_ALT5_GPIO2_IO01 = 5,
} IMX_IOMUXC_SD1_CMD_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA0_ALT0_USDHC1_DATA0 = 0,
   IMX_IOMUXC_SD1_DATA0_ALT5_GPIO2_IO02 = 5,
} IMX_IOMUXC_SD1_DATA0_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA1_ALT0_USDHC1_DATA1 = 0,
   IMX_IOMUXC_SD1_DATA1_ALT5_GPIO2_IO03 = 5,
} IMX_IOMUXC_SD1_DATA1_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA2_ALT0_USDHC1_DATA2 = 0,
   IMX_IOMUXC_SD1_DATA2_ALT5_GPIO2_IO04 = 5,
} IMX_IOMUXC_SD1_DATA2_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA3_ALT0_USDHC1_DATA3 = 0,
   IMX_IOMUXC_SD1_DATA3_ALT5_GPIO2_IO05 = 5,
} IMX_IOMUXC_SD1_DATA3_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA4_ALT0_USDHC1_DATA4 = 0,
   IMX_IOMUXC_SD1_DATA4_ALT5_GPIO2_IO06 = 5,
} IMX_IOMUXC_SD1_DATA4_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA5_ALT0_USDHC1_DATA5 = 0,
   IMX_IOMUXC_SD1_DATA5_ALT5_GPIO2_IO07 = 5,
} IMX_IOMUXC_SD1_DATA5_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA6_ALT0_USDHC1_DATA6 = 0,
   IMX_IOMUXC_SD1_DATA6_ALT5_GPIO2_IO08 = 5,
} IMX_IOMUXC_SD1_DATA6_ALT;

typedef enum {
   IMX_IOMUXC_SD1_DATA7_ALT0_USDHC1_DATA7 = 0,
   IMX_IOMUXC_SD1_DATA7_ALT5_GPIO2_IO09 = 5,
} IMX_IOMUXC_SD1_DATA7_ALT;

typedef enum {
   IMX_IOMUXC_SD1_RESET_B_ALT0_USDHC1_RESET_B = 0,
   IMX_IOMUXC_SD1_RESET_B_ALT5_GPIO2_IO10 = 5,
} IMX_IOMUXC_SD1_RESET_B_ALT;

typedef enum {
   IMX_IOMUXC_SD1_STROBE_ALT0_USDHC1_STROBE = 0,
   IMX_IOMUXC_SD1_STROBE_ALT5_GPIO2_IO11 = 5,
} IMX_IOMUXC_SD1_STROBE_ALT;

typedef enum {
   IMX_IOMUXC_SD2_CD_B_ALT0_USDHC2_CD_B = 0,
   IMX_IOMUXC_SD2_CD_B_ALT5_GPIO2_IO12 = 5,
} IMX_IOMUXC_SD2_CD_B_ALT;

typedef enum {
   IMX_IOMUXC_SD2_CLK_ALT0_USDHC2_CLK = 0,
   IMX_IOMUXC_SD2_CLK_ALT5_GPIO2_IO13 = 5,
} IMX_IOMUXC_SD2_CLK_ALT;

typedef enum {
   IMX_IOMUXC_SD2_CMD_ALT0_USDHC2_CMD = 0,
   IMX_IOMUXC_SD2_CMD_ALT5_GPIO2_IO14 = 5,
} IMX_IOMUXC_SD2_CMD_ALT;

typedef enum {
   IMX_IOMUXC_SD2_DATA0_ALT0_USDHC2_DATA0 = 0,
   IMX_IOMUXC_SD2_DATA0_ALT5_GPIO2_IO15 = 5,
} IMX_IOMUXC_SD2_DATA0_ALT;

typedef enum {
   IMX_IOMUXC_SD2_DATA1_ALT0_USDHC2_DATA1 = 0,
   IMX_IOMUXC_SD2_DATA1_ALT5_GPIO2_IO16 = 5,
} IMX_IOMUXC_SD2_DATA1_ALT;

typedef enum {
   IMX_IOMUXC_SD2_DATA2_ALT0_USDHC2_DATA2 = 0,
   IMX_IOMUXC_SD2_DATA2_ALT5_GPIO2_IO17 = 5,
} IMX_IOMUXC_SD2_DATA2_ALT;

typedef enum {
   IMX_IOMUXC_SD2_DATA3_ALT0_USDHC2_DATA3 = 0,
   IMX_IOMUXC_SD2_DATA3_ALT5_GPIO2_IO18 = 5,
} IMX_IOMUXC_SD2_DATA3_ALT;

typedef enum {
   IMX_IOMUXC_SD2_RESET_B_ALT0_USDHC2_RESET_B = 0,
   IMX_IOMUXC_SD2_RESET_B_ALT5_GPIO2_IO19 = 5,
} IMX_IOMUXC_SD2_RESET_B_ALT;

typedef enum {
   IMX_IOMUXC_SD2_WP_ALT0_USDHC2_WP = 0,
   IMX_IOMUXC_SD2_WP_ALT5_GPIO2_IO20 = 5,
} IMX_IOMUXC_SD2_WP_ALT;

typedef enum {
   IMX_IOMUXC_NAND_ALE_ALT0_RAWNAND_ALE = 0,
   IMX_IOMUXC_NAND_ALE_ALT1_QSPI_A_SCLK = 1,
   IMX_IOMUXC_NAND_ALE_ALT5_GPIO3_IO00 = 5,
} IMX_IOMUXC_NAND_ALE_ALT;

typedef enum {
   IMX_IOMUXC_NAND_CE0_B_ALT0_RAWNAND_CE0_B = 0,
   IMX_IOMUXC_NAND_CE0_B_ALT1_QSPI_A_SS0_B = 1,
   IMX_IOMUXC_NAND_CE0_B_ALT5_GPIO3_IO01 = 5,
} IMX_IOMUXC_NAND_CE0_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_CE1_B_ALT0_RAWNAND_CE1_B = 0,
   IMX_IOMUXC_NAND_CE1_B_ALT1_QSPI_A_SS1_B = 1,
   IMX_IOMUXC_NAND_CE1_B_ALT2_USDHC3_STROBE = 2,
   IMX_IOMUXC_NAND_CE1_B_ALT5_GPIO3_IO02 = 5,
} IMX_IOMUXC_NAND_CE1_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_CE2_B_ALT0_RAWNAND_CE2_B = 0,
   IMX_IOMUXC_NAND_CE2_B_ALT1_QSPI_B_SS0_B = 1,
   IMX_IOMUXC_NAND_CE2_B_ALT2_USDHC3_DATA5 = 2,
   IMX_IOMUXC_NAND_CE2_B_ALT5_GPIO3_IO03 = 5,
} IMX_IOMUXC_NAND_CE2_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_CE3_B_ALT0_RAWNAND_CE3_B = 0,
   IMX_IOMUXC_NAND_CE3_B_ALT1_QSPI_B_SS1_B = 1,
   IMX_IOMUXC_NAND_CE3_B_ALT2_USDHC3_DATA6 = 2,
   IMX_IOMUXC_NAND_CE3_B_ALT5_GPIO3_IO04 = 5,
} IMX_IOMUXC_NAND_CE3_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_CLE_ALT0_RAWNAND_CLE = 0,
   IMX_IOMUXC_NAND_CLE_ALT1_QSPI_B_SCLK = 1,
   IMX_IOMUXC_NAND_CLE_ALT2_USDHC3_DATA7 = 2,
   IMX_IOMUXC_NAND_CLE_ALT5_GPIO3_IO05 = 5,
} IMX_IOMUXC_NAND_CLE_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA00_ALT0_RAWNAND_DATA00 = 0,
   IMX_IOMUXC_NAND_DATA00_ALT1_QSPI_A_DATA0 = 1,
   IMX_IOMUXC_NAND_DATA00_ALT5_GPIO3_IO06 = 5,
} IMX_IOMUXC_NAND_DATA00_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA01_ALT0_RAWNAND_DATA01 = 0,
   IMX_IOMUXC_NAND_DATA01_ALT1_QSPI_A_DATA1 = 1,
   IMX_IOMUXC_NAND_DATA01_ALT5_GPIO3_IO07 = 5,
} IMX_IOMUXC_NAND_DATA01_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA02_ALT0_RAWNAND_DATA02 = 0,
   IMX_IOMUXC_NAND_DATA02_ALT1_QSPI_A_DATA2 = 1,
   IMX_IOMUXC_NAND_DATA02_ALT2_USDHC3_CD_B = 2,
   IMX_IOMUXC_NAND_DATA02_ALT5_GPIO3_IO08 = 5,
} IMX_IOMUXC_NAND_DATA02_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA03_ALT0_RAWNAND_DATA03 = 0,
   IMX_IOMUXC_NAND_DATA03_ALT1_QSPI_A_DATA3 = 1,
   IMX_IOMUXC_NAND_DATA03_ALT2_USDHC3_WP = 2,
   IMX_IOMUXC_NAND_DATA03_ALT5_GPIO3_IO09 = 5,
} IMX_IOMUXC_NAND_DATA03_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA04_ALT0_RAWNAND_DATA04 = 0,
   IMX_IOMUXC_NAND_DATA04_ALT1_QSPI_B_DATA0 = 1,
   IMX_IOMUXC_NAND_DATA04_ALT2_USDHC3_DATA0 = 2,
   IMX_IOMUXC_NAND_DATA04_ALT5_GPIO3_IO10 = 5,
} IMX_IOMUXC_NAND_DATA04_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA05_ALT0_RAWNAND_DATA05 = 0,
   IMX_IOMUXC_NAND_DATA05_ALT1_QSPI_B_DATA1 = 1,
   IMX_IOMUXC_NAND_DATA05_ALT2_USDHC3_DATA1 = 2,
   IMX_IOMUXC_NAND_DATA05_ALT5_GPIO3_IO11 = 5,
} IMX_IOMUXC_NAND_DATA05_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA06_ALT0_RAWNAND_DATA06 = 0,
   IMX_IOMUXC_NAND_DATA06_ALT1_QSPI_B_DATA2 = 1,
   IMX_IOMUXC_NAND_DATA06_ALT2_USDHC3_DATA2 = 2,
   IMX_IOMUXC_NAND_DATA06_ALT5_GPIO3_IO12 = 5,
} IMX_IOMUXC_NAND_DATA06_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DATA07_ALT0_RAWNAND_DATA07 = 0,
   IMX_IOMUXC_NAND_DATA07_ALT1_QSPI_B_DATA3 = 1,
   IMX_IOMUXC_NAND_DATA07_ALT2_USDHC3_DATA3 = 2,
   IMX_IOMUXC_NAND_DATA07_ALT5_GPIO3_IO13 = 5,
} IMX_IOMUXC_NAND_DATA07_ALT;

typedef enum {
   IMX_IOMUXC_NAND_DQS_ALT0_RAWNAND_DQS = 0,
   IMX_IOMUXC_NAND_DQS_ALT1_QSPI_A_DQS = 1,
   IMX_IOMUXC_NAND_DQS_ALT5_GPIO3_IO14 = 5,
} IMX_IOMUXC_NAND_DQS_ALT;

typedef enum {
   IMX_IOMUXC_NAND_RE_B_ALT0_RAWNAND_RE_B = 0,
   IMX_IOMUXC_NAND_RE_B_ALT1_QSPI_B_DQS = 1,
   IMX_IOMUXC_NAND_RE_B_ALT2_USDHC3_DATA4 = 2,
   IMX_IOMUXC_NAND_RE_B_ALT5_GPIO3_IO15 = 5,
} IMX_IOMUXC_NAND_RE_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_READY_B_ALT0_RAWNAND_READY_B = 0,
   IMX_IOMUXC_NAND_READY_B_ALT2_USDHC3_RESET_B = 2,
   IMX_IOMUXC_NAND_READY_B_ALT5_GPIO3_IO16 = 5,
} IMX_IOMUXC_NAND_READY_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_WE_B_ALT0_RAWNAND_WE_B = 0,
   IMX_IOMUXC_NAND_WE_B_ALT2_USDHC3_CLK = 2,
   IMX_IOMUXC_NAND_WE_B_ALT5_GPIO3_IO17 = 5,
} IMX_IOMUXC_NAND_WE_B_ALT;

typedef enum {
   IMX_IOMUXC_NAND_WP_B_ALT0_RAWNAND_WP_B = 0,
   IMX_IOMUXC_NAND_WP_B_ALT2_USDHC3_CMD = 2,
   IMX_IOMUXC_NAND_WP_B_ALT5_GPIO3_IO18 = 5,
} IMX_IOMUXC_NAND_WP_B_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_RXFS_ALT0_SAI5_RX_SYNC = 0,
   IMX_IOMUXC_SAI5_RXFS_ALT1_SAI1_TX_DATA0 = 1,
   IMX_IOMUXC_SAI5_RXFS_ALT5_GPIO3_IO19 = 5,
} IMX_IOMUXC_SAI5_RXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_RXC_ALT0_SAI5_RX_BCLK = 0,
   IMX_IOMUXC_SAI5_RXC_ALT1_SAI1_TX_DATA1 = 1,
   IMX_IOMUXC_SAI5_RXC_ALT4_PDM_CLK = 4,
   IMX_IOMUXC_SAI5_RXC_ALT5_GPIO3_IO20 = 5,
} IMX_IOMUXC_SAI5_RXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_RXD0_ALT0_SAI5_RX_DATA0 = 0,
   IMX_IOMUXC_SAI5_RXD0_ALT1_SAI1_TX_DATA2 = 1,
   IMX_IOMUXC_SAI5_RXD0_ALT4_PDM_BIT_STREAM0 = 4,
   IMX_IOMUXC_SAI5_RXD0_ALT5_GPIO3_IO21 = 5,
} IMX_IOMUXC_SAI5_RXD0_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_RXD1_ALT0_SAI5_RX_DATA1 = 0,
   IMX_IOMUXC_SAI5_RXD1_ALT1_SAI1_TX_DATA3 = 1,
   IMX_IOMUXC_SAI5_RXD1_ALT2_SAI1_TX_SYNC = 2,
   IMX_IOMUXC_SAI5_RXD1_ALT3_SAI5_TX_SYNC = 3,
   IMX_IOMUXC_SAI5_RXD1_ALT4_PDM_BIT_STREAM1 = 4,
   IMX_IOMUXC_SAI5_RXD1_ALT5_GPIO3_IO22 = 5,
} IMX_IOMUXC_SAI5_RXD1_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_RXD2_ALT0_SAI5_RX_DATA2 = 0,
   IMX_IOMUXC_SAI5_RXD2_ALT1_SAI1_TX_DATA4 = 1,
   IMX_IOMUXC_SAI5_RXD2_ALT2_SAI1_TX_SYNC = 2,
   IMX_IOMUXC_SAI5_RXD2_ALT3_SAI5_TX_BCLK = 3,
   IMX_IOMUXC_SAI5_RXD2_ALT4_PDM_BIT_STREAM2 = 4,
   IMX_IOMUXC_SAI5_RXD2_ALT5_GPIO3_IO23 = 5,
} IMX_IOMUXC_SAI5_RXD2_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_RXD3_ALT0_SAI5_RX_DATA3 = 0,
   IMX_IOMUXC_SAI5_RXD3_ALT1_SAI1_TX_DATA5 = 1,
   IMX_IOMUXC_SAI5_RXD3_ALT2_SAI1_TX_SYNC = 2,
   IMX_IOMUXC_SAI5_RXD3_ALT3_SAI5_TX_DATA0 = 3,
   IMX_IOMUXC_SAI5_RXD3_ALT4_PDM_BIT_STREAM3 = 4,
   IMX_IOMUXC_SAI5_RXD3_ALT5_GPIO3_IO24 = 5,
} IMX_IOMUXC_SAI5_RXD3_ALT;

typedef enum {
   IMX_IOMUXC_SAI5_MCLK_ALT0_SAI5_MCLK = 0,
   IMX_IOMUXC_SAI5_MCLK_ALT1_SAI1_TX_BCLK = 1,
   IMX_IOMUXC_SAI5_MCLK_ALT5_GPIO3_IO25 = 5,
} IMX_IOMUXC_SAI5_MCLK_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXFS_ALT0_SAI1_RX_SYNC = 0,
   IMX_IOMUXC_SAI1_RXFS_ALT1_SAI5_RX_SYNC = 1,
   IMX_IOMUXC_SAI1_RXFS_ALT4_CORESIGHT_TRACE_CLK = 4,
   IMX_IOMUXC_SAI1_RXFS_ALT5_GPIO4_IO00 = 5,
} IMX_IOMUXC_SAI1_RXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXC_ALT0_SAI1_RX_BCLK = 0,
   IMX_IOMUXC_SAI1_RXC_ALT1_SAI5_RX_BCLK = 1,
   IMX_IOMUXC_SAI1_RXC_ALT4_CORESIGHT_TRACE_CTL = 4,
   IMX_IOMUXC_SAI1_RXC_ALT5_GPIO4_IO01 = 5,
} IMX_IOMUXC_SAI1_RXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD0_ALT0_SAI1_RX_DATA0 = 0,
   IMX_IOMUXC_SAI1_RXD0_ALT1_SAI5_RX_DATA0 = 1,
   IMX_IOMUXC_SAI1_RXD0_ALT2_SAI1_TX_DATA1 = 2,
   IMX_IOMUXC_SAI1_RXD0_ALT3_PDM_BIT_STREAM0 = 3,
   IMX_IOMUXC_SAI1_RXD0_ALT4_CORESIGHT_TRACE0 = 4,
   IMX_IOMUXC_SAI1_RXD0_ALT5_GPIO4_IO02 = 5,
   IMX_IOMUXC_SAI1_RXD0_ALT6_CCMSRCGPCMIX_BOOT_CFG0 = 6,
} IMX_IOMUXC_SAI1_RXD0_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD1_ALT0_SAI1_RX_DATA1 = 0,
   IMX_IOMUXC_SAI1_RXD1_ALT1_SAI5_RX_DATA1 = 1,
   IMX_IOMUXC_SAI1_RXD1_ALT3_PDM_BIT_STREAM1 = 3,
   IMX_IOMUXC_SAI1_RXD1_ALT4_CORESIGHT_TRACE1 = 4,
   IMX_IOMUXC_SAI1_RXD1_ALT5_GPIO4_IO03 = 5,
   IMX_IOMUXC_SAI1_RXD1_ALT6_CCMSRCGPCMIX_BOOT_CFG1 = 6,
} IMX_IOMUXC_SAI1_RXD1_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD2_ALT0_SAI1_RX_DATA2 = 0,
   IMX_IOMUXC_SAI1_RXD2_ALT1_SAI5_RX_DATA2 = 1,
   IMX_IOMUXC_SAI1_RXD2_ALT3_PDM_BIT_STREAM2 = 3,
   IMX_IOMUXC_SAI1_RXD2_ALT4_CORESIGHT_TRACE2 = 4,
   IMX_IOMUXC_SAI1_RXD2_ALT5_GPIO4_IO04 = 5,
   IMX_IOMUXC_SAI1_RXD2_ALT6_CCMSRCGPCMIX_BOOT_CFG2 = 6,
} IMX_IOMUXC_SAI1_RXD2_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD3_ALT0_SAI1_RX_DATA3 = 0,
   IMX_IOMUXC_SAI1_RXD3_ALT1_SAI5_RX_DATA3 = 1,
   IMX_IOMUXC_SAI1_RXD3_ALT3_PDM_BIT_STREAM3 = 3,
   IMX_IOMUXC_SAI1_RXD3_ALT4_CORESIGHT_TRACE3 = 4,
   IMX_IOMUXC_SAI1_RXD3_ALT5_GPIO4_IO05 = 5,
   IMX_IOMUXC_SAI1_RXD3_ALT6_CCMSRCGPCMIX_BOOT_CFG3 = 6,
} IMX_IOMUXC_SAI1_RXD3_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD4_ALT0_SAI1_RX_DATA4 = 0,
   IMX_IOMUXC_SAI1_RXD4_ALT1_SAI6_TX_BCLK = 1,
   IMX_IOMUXC_SAI1_RXD4_ALT2_SAI6_RX_BCLK = 2,
   IMX_IOMUXC_SAI1_RXD4_ALT4_CORESIGHT_TRACE4 = 4,
   IMX_IOMUXC_SAI1_RXD4_ALT5_GPIO4_IO06 = 5,
   IMX_IOMUXC_SAI1_RXD4_ALT6_CCMSRCGPCMIX_BOOT_CFG4 = 6,
} IMX_IOMUXC_SAI1_RXD4_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD5_ALT0_SAI1_RX_DATA5 = 0,
   IMX_IOMUXC_SAI1_RXD5_ALT1_SAI6_TX_DATA0 = 1,
   IMX_IOMUXC_SAI1_RXD5_ALT2_SAI6_RX_DATA0 = 2,
   IMX_IOMUXC_SAI1_RXD5_ALT3_SAI1_RX_SYNC = 3,
   IMX_IOMUXC_SAI1_RXD5_ALT4_CORESIGHT_TRACE5 = 4,
   IMX_IOMUXC_SAI1_RXD5_ALT5_GPIO4_IO07 = 5,
   IMX_IOMUXC_SAI1_RXD5_ALT6_CCMSRCGPCMIX_BOOT_CFG5 = 6,
} IMX_IOMUXC_SAI1_RXD5_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD6_ALT0_SAI1_RX_DATA6 = 0,
   IMX_IOMUXC_SAI1_RXD6_ALT1_SAI6_TX_SYNC = 1,
   IMX_IOMUXC_SAI1_RXD6_ALT2_SAI6_RX_SYNC = 2,
   IMX_IOMUXC_SAI1_RXD6_ALT4_CORESIGHT_TRACE6 = 4,
   IMX_IOMUXC_SAI1_RXD6_ALT5_GPIO4_IO08 = 5,
   IMX_IOMUXC_SAI1_RXD6_ALT6_CCMSRCGPCMIX_BOOT_CFG6 = 6,
} IMX_IOMUXC_SAI1_RXD6_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_RXD7_ALT0_SAI1_RX_DATA7 = 0,
   IMX_IOMUXC_SAI1_RXD7_ALT1_SAI6_MCLK = 1,
   IMX_IOMUXC_SAI1_RXD7_ALT2_SAI1_TX_SYNC = 2,
   IMX_IOMUXC_SAI1_RXD7_ALT3_SAI1_TX_DATA4 = 3,
   IMX_IOMUXC_SAI1_RXD7_ALT4_CORESIGHT_TRACE7 = 4,
   IMX_IOMUXC_SAI1_RXD7_ALT5_GPIO4_IO09 = 5,
   IMX_IOMUXC_SAI1_RXD7_ALT6_CCMSRCGPCMIX_BOOT_CFG7 = 6,
} IMX_IOMUXC_SAI1_RXD7_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXFS_ALT0_SAI1_TX_SYNC = 0,
   IMX_IOMUXC_SAI1_TXFS_ALT1_SAI5_TX_SYNC = 1,
   IMX_IOMUXC_SAI1_TXFS_ALT4_CORESIGHT_EVENTO = 4,
   IMX_IOMUXC_SAI1_TXFS_ALT5_GPIO4_IO10 = 5,
} IMX_IOMUXC_SAI1_TXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXC_ALT0_SAI1_TX_BCLK = 0,
   IMX_IOMUXC_SAI1_TXC_ALT1_SAI5_TX_BCLK = 1,
   IMX_IOMUXC_SAI1_TXC_ALT4_CORESIGHT_EVENTI = 4,
   IMX_IOMUXC_SAI1_TXC_ALT5_GPIO4_IO11 = 5,
} IMX_IOMUXC_SAI1_TXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD0_ALT0_SAI1_TX_DATA0 = 0,
   IMX_IOMUXC_SAI1_TXD0_ALT1_SAI5_TX_DATA0 = 1,
   IMX_IOMUXC_SAI1_TXD0_ALT4_CORESIGHT_TRACE8 = 4,
   IMX_IOMUXC_SAI1_TXD0_ALT5_GPIO4_IO12 = 5,
   IMX_IOMUXC_SAI1_TXD0_ALT6_CCMSRCGPCMIX_BOOT_CFG8 = 6,
} IMX_IOMUXC_SAI1_TXD0_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD1_ALT0_SAI1_TX_DATA1 = 0,
   IMX_IOMUXC_SAI1_TXD1_ALT1_SAI5_TX_DATA1 = 1,
   IMX_IOMUXC_SAI1_TXD1_ALT4_CORESIGHT_TRACE9 = 4,
   IMX_IOMUXC_SAI1_TXD1_ALT5_GPIO4_IO13 = 5,
   IMX_IOMUXC_SAI1_TXD1_ALT6_CCMSRCGPCMIX_BOOT_CFG9 = 6,
} IMX_IOMUXC_SAI1_TXD1_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD2_ALT0_SAI1_TX_DATA2 = 0,
   IMX_IOMUXC_SAI1_TXD2_ALT1_SAI5_TX_DATA2 = 1,
   IMX_IOMUXC_SAI1_TXD2_ALT4_CORESIGHT_TRACE10 = 4,
   IMX_IOMUXC_SAI1_TXD2_ALT5_GPIO4_IO14 = 5,
   IMX_IOMUXC_SAI1_TXD2_ALT6_CCMSRCGPCMIX_BOOT_CFG10 = 6,
} IMX_IOMUXC_SAI1_TXD2_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD3_ALT0_SAI1_TX_DATA3 = 0,
   IMX_IOMUXC_SAI1_TXD3_ALT1_SAI5_TX_DATA3 = 1,
   IMX_IOMUXC_SAI1_TXD3_ALT4_CORESIGHT_TRACE11 = 4,
   IMX_IOMUXC_SAI1_TXD3_ALT5_GPIO4_IO15 = 5,
   IMX_IOMUXC_SAI1_TXD3_ALT6_CCMSRCGPCMIX_BOOT_CFG11 = 6,
} IMX_IOMUXC_SAI1_TXD3_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD4_ALT0_SAI1_TX_DATA4 = 0,
   IMX_IOMUXC_SAI1_TXD4_ALT1_SAI6_RX_BCLK = 1,
   IMX_IOMUXC_SAI1_TXD4_ALT2_SAI6_TX_BCLK = 2,
   IMX_IOMUXC_SAI1_TXD4_ALT4_CORESIGHT_TRACE12 = 4,
   IMX_IOMUXC_SAI1_TXD4_ALT5_GPIO4_IO16 = 5,
   IMX_IOMUXC_SAI1_TXD4_ALT6_CCMSRCGPCMIX_BOOT_CFG12 = 6,
} IMX_IOMUXC_SAI1_TXD4_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD5_ALT0_SAI1_TX_DATA5 = 0,
   IMX_IOMUXC_SAI1_TXD5_ALT1_SAI6_RX_DATA0 = 1,
   IMX_IOMUXC_SAI1_TXD5_ALT2_SAI6_TX_DATA0 = 2,
   IMX_IOMUXC_SAI1_TXD5_ALT4_CORESIGHT_TRACE13 = 4,
   IMX_IOMUXC_SAI1_TXD5_ALT5_GPIO4_IO17 = 5,
   IMX_IOMUXC_SAI1_TXD5_ALT6_CCMSRCGPCMIX_BOOT_CFG13 = 6,
} IMX_IOMUXC_SAI1_TXD5_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD6_ALT0_SAI1_TX_DATA6 = 0,
   IMX_IOMUXC_SAI1_TXD6_ALT1_SAI6_RX_SYNC = 1,
   IMX_IOMUXC_SAI1_TXD6_ALT2_SAI6_TX_SYNC = 2,
   IMX_IOMUXC_SAI1_TXD6_ALT4_CORESIGHT_TRACE14 = 4,
   IMX_IOMUXC_SAI1_TXD6_ALT5_GPIO4_IO18 = 5,
   IMX_IOMUXC_SAI1_TXD6_ALT6_CCMSRCGPCMIX_BOOT_CFG14 = 6,
} IMX_IOMUXC_SAI1_TXD6_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_TXD7_ALT0_SAI1_TX_DATA7 = 0,
   IMX_IOMUXC_SAI1_TXD7_ALT1_SAI6_MCLK = 1,
   IMX_IOMUXC_SAI1_TXD7_ALT3_PDM_CLK = 3,
   IMX_IOMUXC_SAI1_TXD7_ALT4_CORESIGHT_TRACE15 = 4,
   IMX_IOMUXC_SAI1_TXD7_ALT5_GPIO4_IO19 = 5,
   IMX_IOMUXC_SAI1_TXD7_ALT6_CCMSRCGPCMIX_BOOT_CFG15 = 6,
} IMX_IOMUXC_SAI1_TXD7_ALT;

typedef enum {
   IMX_IOMUXC_SAI1_MCLK_ALT0_SAI1_MCLK = 0,
   IMX_IOMUXC_SAI1_MCLK_ALT1_SAI5_MCLK = 1,
   IMX_IOMUXC_SAI1_MCLK_ALT2_SAI1_TX_BCLK = 2,
   IMX_IOMUXC_SAI1_MCLK_ALT3_PDM_CLK = 3,
   IMX_IOMUXC_SAI1_MCLK_ALT5_GPIO4_IO20 = 5,
} IMX_IOMUXC_SAI1_MCLK_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_RXFS_ALT0_SAI2_RX_SYNC = 0,
   IMX_IOMUXC_SAI2_RXFS_ALT1_SAI5_TX_SYNC = 1,
   IMX_IOMUXC_SAI2_RXFS_ALT2_SAI5_TX_DATA1 = 2,
   IMX_IOMUXC_SAI2_RXFS_ALT3_SAI2_RX_DATA1 = 3,
   IMX_IOMUXC_SAI2_RXFS_ALT4_UART1_TX = 4,
   IMX_IOMUXC_SAI2_RXFS_ALT5_GPIO4_IO21 = 5,
} IMX_IOMUXC_SAI2_RXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_RXC_ALT0_SAI2_RX_BCLK = 0,
   IMX_IOMUXC_SAI2_RXC_ALT1_SAI5_TX_BCLK = 1,
   IMX_IOMUXC_SAI2_RXC_ALT4_UART1_RX = 4,
   IMX_IOMUXC_SAI2_RXC_ALT5_GPIO4_IO22 = 5,
} IMX_IOMUXC_SAI2_RXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_RXD0_ALT0_SAI2_RX_DATA0 = 0,
   IMX_IOMUXC_SAI2_RXD0_ALT1_SAI5_TX_DATA0 = 1,
   IMX_IOMUXC_SAI2_RXD0_ALT4_UART1_RTS_B = 4,
   IMX_IOMUXC_SAI2_RXD0_ALT5_GPIO4_IO23 = 5,
} IMX_IOMUXC_SAI2_RXD0_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_TXFS_ALT0_SAI2_TX_SYNC = 0,
   IMX_IOMUXC_SAI2_TXFS_ALT1_SAI5_TX_DATA1 = 1,
   IMX_IOMUXC_SAI2_TXFS_ALT3_SAI2_TX_DATA1 = 3,
   IMX_IOMUXC_SAI2_TXFS_ALT4_UART1_CTS_B = 4,
   IMX_IOMUXC_SAI2_TXFS_ALT5_GPIO4_IO24 = 5,
} IMX_IOMUXC_SAI2_TXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_TXC_ALT0_SAI2_TX_BCLK = 0,
   IMX_IOMUXC_SAI2_TXC_ALT1_SAI5_TX_DATA2 = 1,
   IMX_IOMUXC_SAI2_TXC_ALT5_GPIO4_IO25 = 5,
} IMX_IOMUXC_SAI2_TXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_TXD0_ALT0_SAI2_TX_DATA0 = 0,
   IMX_IOMUXC_SAI2_TXD0_ALT1_SAI5_TX_DATA3 = 1,
   IMX_IOMUXC_SAI2_TXD0_ALT5_GPIO4_IO26 = 5,
} IMX_IOMUXC_SAI2_TXD0_ALT;

typedef enum {
   IMX_IOMUXC_SAI2_MCLK_ALT0_SAI2_MCLK = 0,
   IMX_IOMUXC_SAI2_MCLK_ALT1_SAI5_MCLK = 1,
   IMX_IOMUXC_SAI2_MCLK_ALT5_GPIO4_IO27 = 5,
} IMX_IOMUXC_SAI2_MCLK_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_RXFS_ALT0_SAI3_RX_SYNC = 0,
   IMX_IOMUXC_SAI3_RXFS_ALT1_GPT1_CAPTURE1 = 1,
   IMX_IOMUXC_SAI3_RXFS_ALT2_SAI5_RX_SYNC = 2,
   IMX_IOMUXC_SAI3_RXFS_ALT3_SAI3_RX_DATA1 = 3,
   IMX_IOMUXC_SAI3_RXFS_ALT5_GPIO4_IO28 = 5,
} IMX_IOMUXC_SAI3_RXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_RXC_ALT0_SAI3_RX_BCLK = 0,
   IMX_IOMUXC_SAI3_RXC_ALT1_GPT1_CLK = 1,
   IMX_IOMUXC_SAI3_RXC_ALT2_SAI5_RX_BCLK = 2,
   IMX_IOMUXC_SAI3_RXC_ALT4_UART2_CTS_B = 4,
   IMX_IOMUXC_SAI3_RXC_ALT5_GPIO4_IO29 = 5,
} IMX_IOMUXC_SAI3_RXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_RXD_ALT0_SAI3_RX_DATA0 = 0,
   IMX_IOMUXC_SAI3_RXD_ALT1_GPT1_COMPARE1 = 1,
   IMX_IOMUXC_SAI3_RXD_ALT2_SAI5_RX_DATA0 = 2,
   IMX_IOMUXC_SAI3_RXD_ALT4_UART2_RTS_B = 4,
   IMX_IOMUXC_SAI3_RXD_ALT5_GPIO4_IO30 = 5,
} IMX_IOMUXC_SAI3_RXD_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_TXFS_ALT0_SAI3_TX_SYNC = 0,
   IMX_IOMUXC_SAI3_TXFS_ALT1_GPT1_CAPTURE2 = 1,
   IMX_IOMUXC_SAI3_TXFS_ALT2_SAI5_RX_DATA1 = 2,
   IMX_IOMUXC_SAI3_TXFS_ALT3_SAI3_TX_DATA1 = 3,
   IMX_IOMUXC_SAI3_TXFS_ALT4_UART2_RX = 4,
   IMX_IOMUXC_SAI3_TXFS_ALT5_GPIO4_IO31 = 5,
} IMX_IOMUXC_SAI3_TXFS_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_TXC_ALT0_SAI3_TX_BCLK = 0,
   IMX_IOMUXC_SAI3_TXC_ALT1_GPT1_COMPARE2 = 1,
   IMX_IOMUXC_SAI3_TXC_ALT2_SAI5_RX_DATA2 = 2,
   IMX_IOMUXC_SAI3_TXC_ALT4_UART2_TX = 4,
   IMX_IOMUXC_SAI3_TXC_ALT5_GPIO5_IO00 = 5,
} IMX_IOMUXC_SAI3_TXC_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_TXD_ALT0_SAI3_TX_DATA0 = 0,
   IMX_IOMUXC_SAI3_TXD_ALT1_GPT1_COMPARE3 = 1,
   IMX_IOMUXC_SAI3_TXD_ALT2_SAI5_RX_DATA3 = 2,
   IMX_IOMUXC_SAI3_TXD_ALT5_GPIO5_IO01 = 5,
} IMX_IOMUXC_SAI3_TXD_ALT;

typedef enum {
   IMX_IOMUXC_SAI3_MCLK_ALT0_SAI3_MCLK = 0,
   IMX_IOMUXC_SAI3_MCLK_ALT1_PWM4_OUT = 1,
   IMX_IOMUXC_SAI3_MCLK_ALT2_SAI5_MCLK = 2,
   IMX_IOMUXC_SAI3_MCLK_ALT5_GPIO5_IO02 = 5,
} IMX_IOMUXC_SAI3_MCLK_ALT;

typedef enum {
   IMX_IOMUXC_SPDIF_TX_ALT0_SPDIF1_OUT = 0,
   IMX_IOMUXC_SPDIF_TX_ALT1_PWM3_OUT = 1,
   IMX_IOMUXC_SPDIF_TX_ALT5_GPIO5_IO03 = 5,
} IMX_IOMUXC_SPDIF_TX_ALT;

typedef enum {
   IMX_IOMUXC_SPDIF_RX_ALT0_SPDIF1_IN = 0,
   IMX_IOMUXC_SPDIF_RX_ALT1_PWM2_OUT = 1,
   IMX_IOMUXC_SPDIF_RX_ALT5_GPIO5_IO04 = 5,
} IMX_IOMUXC_SPDIF_RX_ALT;

typedef enum {
   IMX_IOMUXC_SPDIF_EXT_CLK_ALT0_SPDIF1_EXT_CLK = 0,
   IMX_IOMUXC_SPDIF_EXT_CLK_ALT1_PWM1_OUT = 1,
   IMX_IOMUXC_SPDIF_EXT_CLK_ALT5_GPIO5_IO05 = 5,
} IMX_IOMUXC_SPDIF_EXT_CLK_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI1_SCLK_ALT0_ECSPI1_SCLK = 0,
   IMX_IOMUXC_ECSPI1_SCLK_ALT1_UART3_RX = 1,
   IMX_IOMUXC_ECSPI1_SCLK_ALT5_GPIO5_IO06 = 5,
} IMX_IOMUXC_ECSPI1_SCLK_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI1_MOSI_ALT0_ECSPI1_MOSI = 0,
   IMX_IOMUXC_ECSPI1_MOSI_ALT1_UART3_TX = 1,
   IMX_IOMUXC_ECSPI1_MOSI_ALT5_GPIO5_IO07 = 5,
} IMX_IOMUXC_ECSPI1_MOSI_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI1_MISO_ALT0_ECSPI1_MISO = 0,
   IMX_IOMUXC_ECSPI1_MISO_ALT1_UART3_CTS_B = 1,
   IMX_IOMUXC_ECSPI1_MISO_ALT5_GPIO5_IO08 = 5,
} IMX_IOMUXC_ECSPI1_MISO_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI1_SS0_ALT0_ECSPI1_SS0 = 0,
   IMX_IOMUXC_ECSPI1_SS0_ALT1_UART3_RTS_B = 1,
   IMX_IOMUXC_ECSPI1_SS0_ALT5_GPIO5_IO09 = 5,
} IMX_IOMUXC_ECSPI1_SS0_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI2_SCLK_ALT0_ECSPI2_SCLK = 0,
   IMX_IOMUXC_ECSPI2_SCLK_ALT1_UART4_RX = 1,
   IMX_IOMUXC_ECSPI2_SCLK_ALT5_GPIO5_IO10 = 5,
} IMX_IOMUXC_ECSPI2_SCLK_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI2_MOSI_ALT0_ECSPI2_MOSI = 0,
   IMX_IOMUXC_ECSPI2_MOSI_ALT1_UART4_TX = 1,
   IMX_IOMUXC_ECSPI2_MOSI_ALT5_GPIO5_IO11 = 5,
} IMX_IOMUXC_ECSPI2_MOSI_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI2_MISO_ALT0_ECSPI2_MISO = 0,
   IMX_IOMUXC_ECSPI2_MISO_ALT1_UART4_CTS_B = 1,
   IMX_IOMUXC_ECSPI2_MISO_ALT5_GPIO5_IO12 = 5,
} IMX_IOMUXC_ECSPI2_MISO_ALT;

typedef enum {
   IMX_IOMUXC_ECSPI2_SS0_ALT0_ECSPI2_SS0 = 0,
   IMX_IOMUXC_ECSPI2_SS0_ALT1_UART4_RTS_B = 1,
   IMX_IOMUXC_ECSPI2_SS0_ALT5_GPIO5_IO13 = 5,
} IMX_IOMUXC_ECSPI2_SS0_ALT;

typedef enum {
   IMX_IOMUXC_I2C1_SCL_ALT0_I2C1_SCL = 0,
   IMX_IOMUXC_I2C1_SCL_ALT1_ENET1_MDC = 1,
   IMX_IOMUXC_I2C1_SCL_ALT5_GPIO5_IO14 = 5,
} IMX_IOMUXC_I2C1_SCL_ALT;

typedef enum {
   IMX_IOMUXC_I2C1_SDA_ALT0_I2C1_SDA = 0,
   IMX_IOMUXC_I2C1_SDA_ALT1_ENET1_MDIO = 1,
   IMX_IOMUXC_I2C1_SDA_ALT5_GPIO5_IO15 = 5,
} IMX_IOMUXC_I2C1_SDA_ALT;

typedef enum {
   IMX_IOMUXC_I2C2_SCL_ALT0_I2C2_SCL = 0,
   IMX_IOMUXC_I2C2_SCL_ALT1_ENET1_1588_EVENT1_IN = 1,
   IMX_IOMUXC_I2C2_SCL_ALT2_USDHC3_CD_B = 2,
   IMX_IOMUXC_I2C2_SCL_ALT5_GPIO5_IO16 = 5,
} IMX_IOMUXC_I2C2_SCL_ALT;

typedef enum {
   IMX_IOMUXC_I2C2_SDA_ALT0_I2C2_SDA = 0,
   IMX_IOMUXC_I2C2_SDA_ALT1_ENET1_1588_EVENT1_OUT = 1,
   IMX_IOMUXC_I2C2_SDA_ALT2_USDHC3_WP = 2,
   IMX_IOMUXC_I2C2_SDA_ALT5_GPIO5_IO17 = 5,
} IMX_IOMUXC_I2C2_SDA_ALT;

typedef enum {
   IMX_IOMUXC_I2C3_SCL_ALT0_I2C3_SCL = 0,
   IMX_IOMUXC_I2C3_SCL_ALT1_PWM4_OUT = 1,
   IMX_IOMUXC_I2C3_SCL_ALT2_GPT2_CLK = 2,
   IMX_IOMUXC_I2C3_SCL_ALT5_GPIO5_IO18 = 5,
} IMX_IOMUXC_I2C3_SCL_ALT;

typedef enum {
   IMX_IOMUXC_I2C3_SDA_ALT0_I2C3_SDA = 0,
   IMX_IOMUXC_I2C3_SDA_ALT1_PWM3_OUT = 1,
   IMX_IOMUXC_I2C3_SDA_ALT2_GPT3_CLK = 2,
   IMX_IOMUXC_I2C3_SDA_ALT5_GPIO5_IO19 = 5,
} IMX_IOMUXC_I2C3_SDA_ALT;

typedef enum {
   IMX_IOMUXC_I2C4_SCL_ALT0_I2C4_SCL = 0,
   IMX_IOMUXC_I2C4_SCL_ALT1_PWM2_OUT = 1,
   IMX_IOMUXC_I2C4_SCL_ALT2_PCIE1_CLKREQ_B = 2,
   IMX_IOMUXC_I2C4_SCL_ALT5_GPIO5_IO20 = 5,
} IMX_IOMUXC_I2C4_SCL_ALT;

typedef enum {
   IMX_IOMUXC_I2C4_SDA_ALT0_I2C4_SDA = 0,
   IMX_IOMUXC_I2C4_SDA_ALT1_PWM1_OUT = 1,
   IMX_IOMUXC_I2C4_SDA_ALT5_GPIO5_IO21 = 5,
} IMX_IOMUXC_I2C4_SDA_ALT;

typedef enum {
   IMX_IOMUXC_UART1_RXD_ALT0_UART1_RX = 0,
   IMX_IOMUXC_UART1_RXD_ALT1_ECSPI3_SCLK = 1,
   IMX_IOMUXC_UART1_RXD_ALT5_GPIO5_IO22 = 5,
} IMX_IOMUXC_UART1_RXD_ALT;

typedef enum {
   IMX_IOMUXC_UART1_TXD_ALT0_UART1_TX = 0,
   IMX_IOMUXC_UART1_TXD_ALT1_ECSPI3_MOSI = 1,
   IMX_IOMUXC_UART1_TXD_ALT5_GPIO5_IO23 = 5,
} IMX_IOMUXC_UART1_TXD_ALT;

typedef enum {
   IMX_IOMUXC_UART2_RXD_ALT0_UART2_RX = 0,
   IMX_IOMUXC_UART2_RXD_ALT1_ECSPI3_MISO = 1,
   IMX_IOMUXC_UART2_RXD_ALT5_GPIO5_IO24 = 5,
} IMX_IOMUXC_UART2_RXD_ALT;

typedef enum {
   IMX_IOMUXC_UART2_TXD_ALT0_UART2_TX = 0,
   IMX_IOMUXC_UART2_TXD_ALT1_ECSPI3_SS0 = 1,
   IMX_IOMUXC_UART2_TXD_ALT5_GPIO5_IO25 = 5,
} IMX_IOMUXC_UART2_TXD_ALT;

typedef enum {
   IMX_IOMUXC_UART3_RXD_ALT0_UART3_RX = 0,
   IMX_IOMUXC_UART3_RXD_ALT1_UART1_CTS_B = 1,
   IMX_IOMUXC_UART3_RXD_ALT2_USDHC3_RESET_B = 2,
   IMX_IOMUXC_UART3_RXD_ALT5_GPIO5_IO26 = 5,
} IMX_IOMUXC_UART3_RXD_ALT;

typedef enum {
   IMX_IOMUXC_UART3_TXD_ALT0_UART3_TX = 0,
   IMX_IOMUXC_UART3_TXD_ALT1_UART1_RTS_B = 1,
   IMX_IOMUXC_UART3_TXD_ALT2_USDHC3_VSELECT = 2,
   IMX_IOMUXC_UART3_TXD_ALT5_GPIO5_IO27 = 5,
} IMX_IOMUXC_UART3_TXD_ALT;

typedef enum {
   IMX_IOMUXC_UART4_RXD_ALT0_UART4_RX = 0,
   IMX_IOMUXC_UART4_RXD_ALT1_UART2_CTS_B = 1,
   IMX_IOMUXC_UART4_RXD_ALT2_PCIE1_CLKREQ_B = 2,
   IMX_IOMUXC_UART4_RXD_ALT5_GPIO5_IO28 = 5,
} IMX_IOMUXC_UART4_RXD_ALT;

typedef enum {
   IMX_IOMUXC_UART4_TXD_ALT0_UART4_TX = 0,
   IMX_IOMUXC_UART4_TXD_ALT1_UART2_RTS_B = 1,
   IMX_IOMUXC_UART4_TXD_ALT5_GPIO5_IO29 = 5,
} IMX_IOMUXC_UART4_TXD_ALT;

/**
    Put a pad in the specified configuration.

    For example, to configure GPIO0 as CCM_CLK01 output:
    ImxPadConfig (IMX_PAD_GPIO_0, IMX_PAD_GPIO_0_CCM_CLKO1);

**/
VOID ImxPadConfig (IMX_PAD Pad, IMX_PADCFG PadConfig);

/**

    Dumps to console the specified PAD mux/control configuration.

**/
VOID ImxPadDumpConfig (char *PadName, IMX_PAD Pad);

#endif // _IMX8MMini_IOMUX_H_
