/** @file
*
*  Header defining the iMX8 constants (Base addresses, sizes, flags)
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

#ifndef __IMX8_H__
#define __IMX8_H__

//
// Memory mapping
//

// TODO: probably need to add references to all other hardware blocks,
// unlike imx6/7, AARCH64 uses the MMU and if it's not mapped you get an exception
#if defined(CPU_IMX8MM)
#define ARM_PERIPHERALS_REGISTERS_PHYSICAL  0x31000000
#define ARM_PERIPHERALS_REGISTERS_LENGTH    0x00400000

#define ARM_IP_BUS_REGISTERS_PHYSICAL       0x30000000
#define ARM_IP_BUS_REGISTERS_LENGTH         0x01000000

#define PCIE_REG_REGISTER_PHYSICAL          0x33800000
#define PCIE_REG_REGISTER_LENGTH            0x00400000

#define ARM_USB_REG_REGISTER_PHYSICAL       0x32E40000
#define ARM_USB_REG_REGISTER_LENGTH         0x00020000

#define ARM_GIC_REG_REGISTER_PHYSICAL       0x38800000
#define ARM_GIC_REG_REGISTER_LENGTH         0x00100000

#else // IMX8MQ

#define ARM_PERIPHERALS_REGISTERS_PHYSICAL  0x31000000
#define ARM_PERIPHERALS_REGISTERS_LENGTH    0x00400000

#define ARM_IP_BUS_REGISTERS_PHYSICAL       0x30000000
#define ARM_IP_BUS_REGISTERS_LENGTH         0x01000000

#define PCIE_REG_REGISTER_PHYSICAL          0x33800000
#define PCIE_REG_REGISTER_LENGTH            0x00020000

#define ARM_USB_REG_REGISTER_PHYSICAL       0x38100000
#define ARM_USB_REG_REGISTER_LENGTH         0x00200000

#define ARM_GIC_REG_REGISTER_PHYSICAL       0x38800000
#define ARM_GIC_REG_REGISTER_LENGTH         0x00100000
#endif

//
// Interrupts
//
#define TOTAL_PRIVATE_INTERRUPT     32
#define DEFINE_IMX8_INTERRUPT(a)    (a + TOTAL_PRIVATE_INTERRUPT)

//
// Serial debug port
//
#define UART1_BASE_ADDRESS   0x30860000
#define UART2_BASE_ADDRESS   0x30890000
#define UART3_BASE_ADDRESS   0x30880000
#define UART4_BASE_ADDRESS   0x30A60000

#define UART_ADDRESS_SIZE    0x00010000 // 64KB

#define IMX_SERIAL_DBG_PORT_SUBTYPE     0x000C

#define SERIAL_DEBUG_PORT_INIT_MSG "\r\nUEFI Debug Serial Port Init\r\n"

//
// Clock Source
//
#if defined(CPU_IMX8MM)
#define SOC_OSC_FREQUENCY_REF_HZ  24000000  // Oscillator frequency 24Mhz
#else
#define SOC_OSC_FREQUENCY_REF_HZ  25000000  // Oscillator frequency 25Mhz
#endif

// General Purpose Timers

#define GPT1_BASE_ADDRESS   0x302D0000
#define GPT2_BASE_ADDRESS   0x302E0000
#define GPT3_BASE_ADDRESS   0x302F0000

#define GPT4_BASE_ADDRESS   0x30700000
#define GPT5_BASE_ADDRESS   0x306F0000
#define GPT6_BASE_ADDRESS   0x306E0000

#define GPT1_IRQ    DEFINE_IMX8_INTERRUPT(55)
#define GPT2_IRQ    DEFINE_IMX8_INTERRUPT(54)
#define GPT3_IRQ    DEFINE_IMX8_INTERRUPT(53)

// Clock Source Select definitions

#define GPT_CLOCK_NONE          0x00
#define GPT_CLOCK_PERIPHERAL    0x01
#define GPT_CLOCK_HIGH_FREQ     0x02
#define GPT_CLOCK_EXTERNAL      0x03
#define GPT_CLOCK_LOW_FREQ      0x04
#define GPT_CLOCK_24M           0x05

//
// TODO : Figure out what is the system bus clock value.
// For now use the prescaler to ensure the clock frequency less than half of
// System Bus Clock. Set PRESCALER24M to 12 which yield 2Mhz.
//
#define GPT_TIMER_DEFAULT_PRESCALER24M      (12 - 1)

//
// Default prescaler is 2 to set frequency to 1MHz with
// a 24Mhz reference clock divided by 12.
//
#define GPT_TIMER_DEFAULT_PRESCALER         (2 - 1)

// SDMA (Smart DMA) controllers
#define SDMA1_BASE_ADDRESS      0x30BD0000
#define SDMA1_IRQ               DEFINE_IMX8_INTERRUPT(2)
#define SDMA2_BASE_ADDRESS      0x302C0000
#define SDMA2_IRQ               DEFINE_IMX8_INTERRUPT(103)
#if defined(CPU_IMX8MM)
#define SDMA3_BASE_ADDRESS      0x302B0000
#define SDMA3_IRQ               DEFINE_IMX8_INTERRUPT(34)
#endif

#pragma pack(push, 1)
typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 EN : 1;
        UINT32 ENMOD : 1;
        UINT32 DBGEN : 1;
        UINT32 WAITEN : 1;
        UINT32 DOZEEN : 1;
        UINT32 STOPEN : 1;
        UINT32 CLKSRC : 3;
        UINT32 FRR : 1;
        UINT32 EN_24M : 1;
        UINT32 Reserved : 4;
        UINT32 SWR : 1;
        UINT32 IM1 : 2;
        UINT32 IM2 : 2;
        UINT32 OM1 : 3;
        UINT32 OM2 : 3;
        UINT32 OM3 : 3;
        UINT32 FO1 : 1;
        UINT32 FO2 : 1;
        UINT32 FO3 : 1;
    };
} GPT_CR;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 PRESCALER : 12;
        UINT32 PRESCALER24M : 4;
        UINT32 Reserved : 16;
    };
} GPT_PR;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 OF1IE : 1;
        UINT32 OF2IE : 1;
        UINT32 OF3IE : 1;
        UINT32 IF1IE : 1;
        UINT32 IF2IE : 1;
        UINT32 ROVIE : 1;
        UINT32 Reserved : 26;
    };
} GPT_IR, GPT_SR;

typedef struct {
    GPT_CR CR;
    GPT_PR PR;
    GPT_SR SR;
    GPT_IR IR;
    UINT32 OCR1;
    UINT32 OCR2;
    UINT32 OCR3;
    UINT32 ICR1;
    UINT32 ICR2;
    UINT32 CNT;
} GPT_REGS;
#pragma pack(pop)

//
// Secure Non-Volatile Storage (SNVS)
//

#define SNVS_HP_BASE_ADDRESS    0x30370000
#define SNVS_HP_ADDRESS_SIZE    64 * 1024
#define SNVS_LP_OFFSET          0x034

#pragma pack(push, 1)
typedef struct {
    UINT32 LPLR;
    UINT32 LPCR;
    UINT32 LPMKCR;
    UINT32 LPSVCR;
    UINT32 LPTGFCR;
    UINT32 LPTDCR;
    UINT32 LPSR;
    UINT32 LPSRCMR;
    UINT32 LPSRCLR;
    UINT32 LPTAR;
    UINT32 LPSMCMR;
    UINT32 LPSMCLR;
    UINT32 LPPGDR;
    UINT32 LPPGDR0;
}SVNS_LP_REGS;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 SRTC_ENV : 1;        // 0 Secure Real Time Counter Enabled and Valid
        UINT32 LPTA_EN : 1;         // 1 LP Time Alarm Enable
        UINT32 MC_ENV : 1;          // 2 Monotonic Counter Enabled and Valid
        UINT32 LPWUI_EN : 1;        // 3 LP Wake-Up Interrupt Enable
        UINT32 SRTC_INV_EN : 1;     // 4 If this bit is 1, in the case of a security violation the SRTC stops counting and the SRTC is invalidated
        UINT32 DP_EN : 1;           // 5 Dumb PMIC Enabled
        UINT32 TOP : 1;             // 6 Turn off System Power
        UINT32 LPCALB_EN : 1;       // 7 Power Glitch Enable
        UINT32 PWR_GLITCH_EN : 1;   // 8 LP Calibration Enable
        UINT32 Reserved0 : 1;       // 9 ???
        UINT32 LPCALB_VAL : 5;      // 10-14 LP Calibration Value
        UINT32 Reserved1 : 1;       // 15 ???
        UINT32 BTN_PRESS_TIME : 2;  // 16-17 This field configures the button press time out values for the PMIC Logic
        UINT32 DEBOUNCE : 2;        // 18-19 This field configures the amount of debounce time for the BTN input signal.
        UINT32 ON_TIME : 2;         // 20-21
        UINT32 PK_EN : 1;           // 22 PMIC On Request Enable
        UINT32 PK_OVERRIDE : 1;     // 23 PMIC On Request Override
        UINT32 GPR_Z_DIS : 1;       // 24 General Purpose Registers Zeroization Disable.
        UINT32 Reserved2 : 7;       // 25-31 ???
    };
} SNVS_LPCR_REG;
#pragma pack(pop)

//
// Watchdog
//

#define WATCHDOG_1_BASE_ADDRESS     0x30280000
#define WATCHDOG_2_BASE_ADDRESS     0x30290000
#define WATCHDOG_3_BASE_ADDRESS     0x302A0000
#define WATCHDOG_ADDRESS_SIZE     64 * 1024

#pragma pack(push, 1)
typedef struct {
    UINT16 WCR;                      // 0x0 Watchdog Control Register (WDOGx_WCR)
    UINT16 WSR;                      // 0x2 Watchdog Service Register (WDOGx_WSR)
    UINT16 WRSR;                     // 0x4 Watchdog Reset Status Register (WDOGx_WRSR)
    UINT16 WICR;                     // 0x6 Watchdog Interrupt Control Register (WDOGx_WICR)
    UINT16 WMCR;                     // 0x8 Watchdog Miscellaneous Control Register (WDOGx_WMCR)
} WDOG_REGS;

typedef union {
    UINT16 AsUint16;
    struct {
        UINT16 WDZST : 1;            // 0 Watchdog Low Power
        UINT16 WDBG : 1;             // 1 Watchdog DEBUG Enable
        UINT16 WDE : 1;              // 2 Watchdog Enable
        UINT16 WDT : 1;              // 3 WDOG_B Time-out assertion.
        UINT16 SRS : 1;              // 4 Software Reset Signal
        UINT16 WDA : 1;              // 5 WDOG_B assertion
        UINT16 SRE : 1;              // 6 software reset extension, an option way to generate software reset
        UINT16 WDW : 1;              // 7 Watchdog Disable for Wait
        UINT16 WT : 8;               // 8-15 Watchdog Time-out Field
    };
} WDOG_WCR_REG;
#pragma pack(pop)

//
// GPIO
//
#define IMX_GPIO_BASE_ADDRESS       0x30200000
#define IMX_GPIO_BASE               IMX_GPIO_BASE_ADDRESS

//
// IOMux:
// IMX8M specific
//

#if defined(CPU_IMX8MM)
#define IOMUXC_GPR_BASE_ADDRESS             0x30340000

#define IOMUXC_SW_MUX_PAD_BASE_ADDRESS      0x30330000
#define IOMUXC_SW_MUX_BASE_ADDRESS          0x30330014
#define IOMUXC_SW_PAD_BASE_ADDRESS          0x30330254
#define IOMUXC_SELECT_INPUT_BASE_ADDRESS    0x303304B8 // IOMUXC_CCM_PMIC_READY_SELECT_INPUT - 0x04

//
// Select Input
//
typedef enum {
    IOMUXC_CCM_PMIC_READY_SELECT_INPUT = 0x303304BC,
    IOMUXC_ENET1_MDIO_SELECT_INPUT = 0x303304C0,
    IOMUXC_SAI1_RX_SYNC_SELECT_INPUT = 0x303304C4,
    IOMUXC_SAI1_TX_BCLK_SELECT_INPUT = 0x303304C8,
    IOMUXC_SAI1_TX_SYNC_SELECT_INPUT = 0x303304CC,
    IOMUXC_SAI5_RX_BCLK_SELECT_INPUT = 0x303304D0,
    IOMUXC_SAI5_RX_DATA_SELECT_INPUT_0 = 0x303304D4,
    IOMUXC_SAI5_RX_DATA_SELECT_INPUT_1 = 0x303304D8,
    IOMUXC_SAI5_RX_DATA_SELECT_INPUT_2 = 0x303304DC,
    IOMUXC_SAI5_RX_DATA_SELECT_INPUT_3 = 0x303304E0,
    IOMUXC_SAI5_RX_SYNC_SELECT_INPUT = 0x303304E4,
    IOMUXC_SAI5_TX_BCLK_SELECT_INPUT = 0x303304E8,
    IOMUXC_SAI5_TX_SYNC_SELECT_INPUT = 0x303304EC,
    IOMUXC_UART1_RTS_B_SELECT_INPUT = 0x303304F0,
    IOMUXC_UART1_RXD_SELECT_INPUT = 0x303304F4,
    IOMUXC_UART2_RTS_B_SELECT_INPUT = 0x303304F8,
    IOMUXC_UART2_RXD_SELECT_INPUT = 0x303304FC,
    IOMUXC_UART3_RTS_B_SELECT_INPUT = 0x30330500,
    IOMUXC_UART3_RXD_SELECT_INPUT = 0x30330504,
    IOMUXC_UART4_RTS_B_SELECT_INPUT = 0x30330508,
    IOMUXC_UART4_RXD_SELECT_INPUT = 0x3033050C,
    IOMUXC_SAI6_RX_BCLK_SELECT_INPUT = 0x30330510,
    IOMUXC_SAI6_RX_DATA_SELECT_INPUT_0 = 0x30330514,
    IOMUXC_SAI6_RX_SYNC_SELECT_INPUT = 0x30330518,
    IOMUXC_SAI6_TX_BCLK_SELECT_INPUT = 0x3033051C,
    IOMUXC_SAI6_TX_SYNC_SELECT_INPUT = 0x30330520,
    IOMUXC_PCIE1_CLKREQ_B_SELECT_INPUT = 0x30330524,
    IOMUXC_SAI5_MCLK_SELECT_INPUT = 0x3033052C,
    IOMUXC_SAI6_MCLK_SELECT_INPUT = 0x30330530,
    IOMUXC_PDM_BIT_STREAM_SELECT_INPUT_0 = 0x30330534,
    IOMUXC_PDM_BIT_STREAM_SELECT_INPUT_1 = 0x30330538,
    IOMUXC_PDM_BIT_STREAM_SELECT_INPUT_2 = 0x3033053C,
    IOMUXC_PDM_BIT_STREAM_SELECT_INPUT_3 = 0x30330540,
    IOMUXC_USDHC3_CD_B_SELECT_INPUT = 0x30330544,
    IOMUXC_USDHC3_WP_SELECT_INPUT = 0x30330548,
    IOMUXC_SELECT_INPUT_UPPER_BOUND = IOMUXC_USDHC3_WP_SELECT_INPUT,
} IMX_INPUT_SELECT;

#else // CPU_IMX8MQ

#define IOMUXC_GPR_BASE_ADDRESS             0x30340000

#define IOMUXC_SW_MUX_PAD_BASE_ADDRESS      0x30330000
#define IOMUXC_SW_MUX_BASE_ADDRESS          0x30330014
#define IOMUXC_SW_PAD_BASE_ADDRESS          0x30330254
#define IOMUXC_SELECT_INPUT_BASE_ADDRESS    0x303304B8 // IOMUXC_CCM_PMIC_READY_SELECT_INPUT - 0x04

//
// Select Input
//
typedef enum {
    IOMUXC_CCM_PMIC_READY_SELECT_INPUT = 0x303304BC,
    IOMUXC_ENET1_MDIO_SELECT_INPUT = 0x303304C0,
    IOMUXC_SAI1_RX_SYNC_SELECT_INPUT = 0x303304C4,
    IOMUXC_SAI1_TX_BCLK_SELECT_INPUT = 0x303304C8,
    IOMUXC_SAI1_TX_SYNC_SELECT_INPUT = 0x303304CC,
    IOMUXC_SAI5_RX_BCLK_SELECT_INPUT = 0x303304D0,
    IOMUXC_SAI5_RXD0_SELECT_INPUT = 0x303304D4,
    IOMUXC_SAI5_RXD1_SELECT_INPUT = 0x303304D8,
    IOMUXC_SAI5_RXD2_SELECT_INPUT = 0x303304DC,
    IOMUXC_SAI5_RXD3_SELECT_INPUT = 0x303304E0,
    IOMUXC_SAI5_RX_SYNC_SELECT_INPUT = 0x303304E4,
    IOMUXC_SAI5_TX_BCLK_SELECT_INPUT = 0x303304E8,
    IOMUXC_SAI5_TX_SYNC_SELECT_INPUT = 0x303304EC,
    IOMUXC_UART1_RTS_B_SELECT_INPUT = 0x303304F0,
    IOMUXC_UART1_RXD_SELECT_INPUT = 0x303304F4,
    IOMUXC_UART2_RTS_B_SELECT_INPUT = 0x303304F8,
    IOMUXC_UART2_RXD_SELECT_INPUT = 0x303304FC,
    IOMUXC_UART3_RTS_B_SELECT_INPUT = 0x30330500,
    IOMUXC_UART3_RXD_SELECT_INPUT = 0x30330504,
    IOMUXC_UART4_RTS_B_SELECT_INPUT = 0x30330508,
    IOMUXC_UART4_RXD_SELECT_INPUT = 0x3033050C,
    IOMUXC_SAI6_RX_BCLK_SELECT_INPUT = 0x30330510,
    IOMUXC_SAI6_RXD0_SELECT_INPUT = 0x30330514,
    IOMUXC_SAI6_RX_SYNC_SELECT_INPUT = 0x30330518,
    IOMUXC_SAI6_TX_BCLK_SELECT_INPUT = 0x3033051C,
    IOMUXC_SAI6_TX_SYNC_SELECT_INPUT = 0x30330520,
    IOMUXC_PCIE1_CLKREQ_B_SELECT_INPUT = 0x30330524,
    IOMUXC_PCIE2_CLKREQ_B_SELECT_INPUT = 0x30330528,
    IOMUXC_SAI5_MCLK_SELECT_INPUT = 0x3033052C,
    IOMUXC_SAI6_MCLK_SELECT_INPUT = 0x30330530,
    IOMUXC_SELECT_INPUT_UPPER_BOUND = IOMUXC_SAI6_MCLK_SELECT_INPUT,
} IMX_INPUT_SELECT;
#endif // CPU_IMX8MM

#endif // __IMX8_PLATFORM_H__
