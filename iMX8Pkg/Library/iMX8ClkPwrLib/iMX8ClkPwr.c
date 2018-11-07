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

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>

#include <iMX8.h>
#include <iMX8ClkPwrReg.h>

//
// List of Low Power Clock Gates to enable that have not yet been enabled
//
UINT32
ImxActiveClock[] = {
//  0, // Dvfs      0x4000
//  1, // Anamix    0x4010
//  2, // Cpu       0x4020
//  3, // Csu       0x4030
//  4, // debug     0x4040
//  5, // Dram1     0x4050
//  6, // reverved  0x4060
//  7, // Ecspi1    0x4070
//  8, // Ecspi2    0x4080
//  9, // Ecspi3    0x4090
//  10, // Enet1    0x40A0
//  11, // Gpio1    0x40B0
//  12, // Gpio2    0x40C0
//  13, // Gpio3    0x40D0
//  14, // Gpio4    0x40E0
//  15, // Gpio5    0x40F0
//  16, // Gpt1     0x4100
//  17, // Gpt2     0x4110
//  18, // Gpt3     0x4120
//  19, // Gpt4     0x4130
//  20, // Gpt5     0x4140
//  21, // Gpt6     0x4150
//  22, // Hs       0x4160
//  23, // I2c1     0x4170
//  24, // I2c2     0x4180
//  25, // I2c3     0x4190
//  26, // I2c4     0x41A0
//  27, // Iomux    0x41B0
//  28, // Iomux1   0x41C0
//  29, // Iomux2   0x41D0
//  30, // Iomux3   0x41E0
//  31, // Iomux4   0x41F0
//  32, // M4       0x4200
//  33, // Mu       0x4210
//  34, // Ocotp    0x4220
//  35, // Ocram    0x4230
//  36, // Ocram_s  0x4240
//  37, // Pcie     0x4250
//  38, // Perfmon1 0x4260
//  39, // Perfmon2 0x4270
//  40, // Pwm1     0x4280
//  41, // Pwm2     0x4290
//  42, // Pwm3     0x42A0
//  43, // Pwm4     0x42B0
//  44, // Qos      0x42C0
//  45, // Dismix   0x42D0
//  46, // Megamix  0x42E0
//  47, // Qspi     0x42F0
//  48, // Rawnand  0x4300
//  49, // Rdc      0x4310
//  50, // Rom      0x4320
//  51, // Sai1     0x4330
  52, // Sai2     0x4340
//  53, // Sai3     0x4350
//  54, // Sai4     0x4360
//  55, // Sai5     0x4370
//  56, // Sai6     0x4380
//  57, // Sctr     0x4390
  58, // Sdma1    0x43A0
  59, // Sdma2    0x43B0
//  60, // Sec_debug    0x43C0
//  61, // Sema1    0x43D0
//  62, // Sema2    0x43E0
//  63, // Sim_display    0x43F0
//  64, // Sim_enet 0x4400
//  65, // Sim_m    0x4410
//  66, // Sim_main 0x4420
//  67, // Sim_s    0x4430
//  68, // Sim_wakeup    0x4440
//  69, // Sim_usb  0x4450
//  70, // Sim_vpu  0x4460
//  71, // Snvs     0x4470
//  72, // Trace    0x4480
//  73, // Uart1    0x4490
//  74, // Uart2    0x44A0
//  75, // Uart3    0x44B0
//  76, // Uart4    0x44C0
//  77, // Usb_ctrl1    0x44D0
//  78, // Usb_ctrl2    0x44E0
//  79, // Usb_phy1 0x44F0
//  80, // Usb_phy2 0x4500
//  81, // Usdhc1   0x4510
//  82, // Usdhc2   0x4520
//  83, // Wdog1    0x4530
//  84, // Wdog2    0x4540
//  85, // Wdog3    0x4550
//  86, // Va53     0x4560
//  87, // Gpu      0x4570
//  88, // Hevc     0x4580
//  89, // Avc      0x4590
//  90, // Vp9      0x45A0
//  91, // Hevc_inter    0x45B0
//  92, // Gic      0x45C0
//  93, // Display    0x45D0
//  94, // Hdmi     0x45E0
//  95, // Hdmi_phy    0x45F0
//  96, // Xtal    0x4600
//  97, // Pll     0x4610
//  98, // Tsensor    0x4620
//  99, // Vpu_dec    0x4630
//  100, // Pcie2   0x4640
//  101, // Mipi_csi1    0x4650
//  102, // Mipi_csi2    0x4660
};


VOID
ImxUngateActiveClock ( 
  VOID
  )
{
  UINT32 index;

  // TODO: determine correct clock domain settings
  // ungate the clocks across all RDC domains
  UINT32 val =
           IMX_CCM_CCGR_GATE_ALL_THE_TIME |
          (IMX_CCM_CCGR_GATE_ALL_THE_TIME << 4) |
          (IMX_CCM_CCGR_GATE_ALL_THE_TIME << 8) |
          (IMX_CCM_CCGR_GATE_ALL_THE_TIME << 12);

  for (index = 0; index < ARRAYSIZE(ImxActiveClock); ++index) {
    DEBUG ((EFI_D_INFO, "Ungating %08x %04x\n",
      IMX_CCM_CCGR(ImxActiveClock[index]),
      val));

    MmioWrite32 (
      IMX_CCM_CCGR(ImxActiveClock[index]),
      val);

    DEBUG ((EFI_D_INFO, "value after ungating %08x\n",
      MmioRead32 (IMX_CCM_CCGR(ImxActiveClock[index]))));
  }
}

VOID
IMXSetAudioPllClockRate (
  UINT32 TargetClockRate,
  UINT32 PreDividerAudioVal,
  UINT32 PostDividerAudioVal
  )
{
  IMX_CCM_PLL_AUDIO_CFG0_REG pllAudioCfg0Reg;
  IMX_CCM_PLL_AUDIO_CFG1_REG pllAudioCfg1Reg;
  IMX_CCM_TARGET_ROOT_REG ccmTargetRootAudioMclkReg;
  UINT32 outdiv;
  UINT32 refclkdiv;
  INT32 count;

  // check conditions for using 25MHz refclk and refclk_div of 2;
  ASSERT (IMX_REF_CLK_25M_FREQ >= PLL_AUDIO_REF_MIN_FREQ);
  ASSERT (IMX_REF_CLK_25M_FREQ <= PLL_AUDIO_REF_MAX_FREQ);

  ASSERT (IMX_REF_CLK_25M_FREQ/2 >= PLL_AUDIO_REF_MIN_FREQ);
  ASSERT (IMX_REF_CLK_25M_FREQ/2 <= PLL_AUDIO_REF_MAX_FREQ);

  // check that output pll value is in acceptable range
  if ((TargetClockRate < PLL_AUDIO_OUT_MIN_FREQ) ||
      (TargetClockRate > PLL_AUDIO_OUT_MAX_FREQ)) {
    DEBUG ((DEBUG_ERROR, "TargetClockRate is out of range\n"));
    return;
  }

  outdiv = 2;
  refclkdiv = 5;

  // Gate the SAI2 AUDIO device
  MmioWrite32 (IMX_CCM_CCGR(52), IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED);

  // Disable SAI2_CLK_ROOT
  ccmTargetRootAudioMclkReg.AsUint32 = 0;
  ccmTargetRootAudioMclkReg.ENABLE = 0;
  MmioWrite32 (
    IMX_CCM_TARGET_ROOT(76) + IMX_CCM_CLR_OFFSET,
    ccmTargetRootAudioMclkReg.AsUint32);

  // powerdown AUDIO PLL1
  pllAudioCfg0Reg.AsUint32 = 0;
  pllAudioCfg1Reg.AsUint32 = 0;

  pllAudioCfg0Reg.REFCLK_DIV_VAL = refclkdiv - 1;
  pllAudioCfg0Reg.OUTPUT_DIV_VAL = (outdiv / 2) - 1;
  pllAudioCfg0Reg.PD = 1;

  MmioWrite32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0, pllAudioCfg0Reg.AsUint32);

  ASSERT (TargetClockRate >= PLL_AUDIO_OUT_MIN_FREQ);
  ASSERT (TargetClockRate <= PLL_AUDIO_OUT_MAX_FREQ);

  // PLL output frequency = (Reference Freq/REFCLK_DIV) * 8 * (1 + INT_DIV + (FRAC_DIV / 2^24)) / OUTDIV
  // and INT_DIV >= 1 so
  // 1 + INT_DIV + (FRAC_DIV / 2^24) = PLL output freq * REFCLK_DIV * OUTDIV / ( 8 * Reference Freq ) >= 2
  {
    UINT32 denom = IMX_REF_CLK_25M_FREQ * 8;
    UINT32 intDiv = ((TargetClockRate * refclkdiv * outdiv) / denom) - 1;
    UINT64 fracDiv = ((((UINT64)TargetClockRate * refclkdiv * outdiv) % denom) << 24) / denom;

    DEBUG ((DEBUG_INFO, "refclk = %u refclkdiv = %u outDiv = %u intDiv = %u fracDiv = %lu denom = %u\n",
            IMX_REF_CLK_25M_FREQ, refclkdiv, outdiv, intDiv, fracDiv, denom));
    ASSERT (intDiv >= 1);

    // write the new integer and fractional divisors
    pllAudioCfg1Reg.INT_DIV_CTL = intDiv - 1;
    pllAudioCfg1Reg.FRAC_DIV_CTL = (UINT32)fracDiv;
    MmioWrite32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG1, pllAudioCfg1Reg.AsUint32);

    // Signal new values and deassert PD or the the divider won't acknowledge
    pllAudioCfg0Reg.NEWDIV_VAL = 1;
    pllAudioCfg0Reg.PD = 0;
    MmioWrite32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0, pllAudioCfg0Reg.AsUint32);

    // wait for ack
    for (count = 0 ; count < 10; ++count) {
      pllAudioCfg0Reg.AsUint32 = MmioRead32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0);
      if (pllAudioCfg0Reg.NEWDIV_ACK) {
        break;
      }
      MicroSecondDelay (10);
    }

    if (pllAudioCfg0Reg.NEWDIV_ACK != 1) {
      DEBUG ((DEBUG_ERROR, "Time out waiting for PLL divisors to load\n"));
      ASSERT (pllAudioCfg0Reg.NEWDIV_ACK == 1);
    }

    // clear int/frac div load flag
    pllAudioCfg0Reg.NEWDIV_VAL = 0;
    MmioWrite32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0, pllAudioCfg0Reg.AsUint32);
  }

  DEBUG ((DEBUG_INFO, "Waiting for Audio PLL to lock\n"));

  // wait for PLL to lock
  for (count = 0 ; count < 1000; ++count) {
    pllAudioCfg0Reg.AsUint32 = MmioRead32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0);

    if (pllAudioCfg0Reg.LOCK == 1) {
      break;
    }
    MicroSecondDelay(10);
  }

  if (pllAudioCfg0Reg.LOCK != 1) {
    DEBUG ((DEBUG_ERROR, "Time out waiting for PLL to lock\n"));
    ASSERT (pllAudioCfg0Reg.LOCK == 1);
  }

  DEBUG ((DEBUG_INFO, "Audio PLL locked\n"));

  // Enable the PLL output
  pllAudioCfg0Reg.CLKE = 1;
  MmioWrite32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0, pllAudioCfg0Reg.AsUint32);

  // Set muxing, pre and post divider for SAI2_CLK_ROOT
  // Freq = (clock source freq)/(pre_div+1)/(post_div+1)
  ccmTargetRootAudioMclkReg.AsUint32 = MmioRead32 (IMX_CCM_TARGET_ROOT(76));

  ccmTargetRootAudioMclkReg.PRE_PODF = (PreDividerAudioVal - 1);
  ccmTargetRootAudioMclkReg.POST_PODF = (PostDividerAudioVal - 1);
  ccmTargetRootAudioMclkReg.MUX = 0x01; // 001b - Audio PLL
  ccmTargetRootAudioMclkReg.ENABLE = 0x01;

  MmioWrite32 (IMX_CCM_TARGET_ROOT(76), ccmTargetRootAudioMclkReg.AsUint32);

  // Ungate the SAI2 AUDIO device clocks
  MmioWrite32 (IMX_CCM_CCGR(52), IMX_CCM_CCGR_GATE_RUN_WAIT);

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INIT, "IMX_CCM_ANALOG_AUDIO_PLL1_CFG0 0x%08x\n", MmioRead32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG0)));
  DEBUG ((DEBUG_INIT, "IMX_CCM_ANALOG_AUDIO_PLL1_CFG1 0x%08x\n", MmioRead32 (IMX_CCM_ANALOG_AUDIO_PLL1_CFG1)));

  DEBUG ((DEBUG_INIT, "IMX_CCM_PLL_CTRL(35) 0x%08x : 0x%08x\n", IMX_CCM_PLL_CTRL(35), MmioRead32 (IMX_CCM_PLL_CTRL(35))));
  DEBUG ((DEBUG_INIT, "IMX_CCM_CCGR(52)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(52), MmioRead32 (IMX_CCM_CCGR(52))));
  DEBUG_CODE_END ();
}


EFI_STATUS
ImxSetSAI2ClockRate (
  UINT32 ClockRate
  )
{
  UINT32 targetFreq;
  UINT32 preDivSelect;
  UINT32 postDivSelect;

  preDivSelect = 8;
  postDivSelect = 4;
  targetFreq = ClockRate * preDivSelect * postDivSelect;

  DEBUG ((
    DEBUG_INFO,
    "Audio PLL setting (%d) Target Freq (%d) PreDiv %d PostDiv %d\n",
    ClockRate,
    targetFreq,
    preDivSelect,
    postDivSelect
    ));

  IMXSetAudioPllClockRate (
    targetFreq,
    preDivSelect,
    postDivSelect);

  return EFI_SUCCESS;
}

