/** @file
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
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

#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>

#include <iMX8.h>
#if defined(CPU_IMX8MM)
#include <iMX8MMiniIoMux.h>
#else
#include <iMX8MIoMux.h>
#endif

//
// Muxing functions
//

VOID
ImxPadConfig (
  IMX_PAD Pad,
  IMX_PADCFG PadConfig
  )
{
  _Static_assert(
    (IOMUXC_SELECT_INPUT_UPPER_BOUND - IOMUXC_SELECT_INPUT_BASE_ADDRESS) < (0xff * 4),
    "Too many SELECT_INPUT registers values to encode in IMX_PADCFG");

  //
  // Configure Mux Control
  //
  MmioWrite32 (
    IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_MUX_OFFSET(Pad),
    _IMX_PADCFG_MUX_CTL(PadConfig));

  //
  // Configure Select Input Control
  //
  if (_IMX_PADCFG_SEL_INP(PadConfig) != 0) {
    DEBUG ((DEBUG_INFO, "Setting INPUT_SELECT %x value %x\n",
      _IMX_SEL_INP_REGISTER(_IMX_PADCFG_SEL_INP(PadConfig)),
      _IMX_SEL_INP_VALUE(_IMX_PADCFG_SEL_INP(PadConfig))));

    MmioWrite32 (
      _IMX_SEL_INP_REGISTER(_IMX_PADCFG_SEL_INP(PadConfig)),
      _IMX_SEL_INP_VALUE(_IMX_PADCFG_SEL_INP(PadConfig)));
  }

  //
  // Configure Pad Control
  //
  MmioWrite32 (
    IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_CTL_OFFSET(Pad),
    _IMX_PADCFG_PAD_CTL(PadConfig));
}

VOID
ImxPadDumpConfig (
  char *SignalFriendlyName,
  IMX_PAD Pad
  )
{
  IMX_IOMUXC_MUX_CTL muxCtl;
  muxCtl.AsUint32 = MmioRead32 (
    IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_MUX_OFFSET(Pad));

  DEBUG ((
    DEBUG_INIT,
    "- %a MUX_CTL(0x%p)=0x%08x: MUX_MODE:%d SION:%d | ",
    SignalFriendlyName,
    IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_MUX_OFFSET(Pad),
    muxCtl.AsUint32,
    muxCtl.Fields.MUX_MODE,
    muxCtl.Fields.SION));

  IMX_IOMUXC_PAD_CTL padCtl;
  padCtl.AsUint32 = MmioRead32 (
    IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_CTL_OFFSET(Pad));

#if defined(CPU_IMX8MM)
  DEBUG ((
	DEBUG_INIT,
	"PAD_CTL(0x%p)=0x%08x: DSE:%d FSEL:%d ODE:%d PUE:%d HYS:%d PE:%d\n",
	IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_CTL_OFFSET(Pad),
	padCtl.AsUint32,
	padCtl.Fields.DSE,
	padCtl.Fields.FSEL,
	padCtl.Fields.ODE,
	padCtl.Fields.PUE,
	padCtl.Fields.HYS,
	padCtl.Fields.PE));
#else
  DEBUG ((
    DEBUG_INIT,
    "PAD_CTL(0x%p)=0x%08x: DSE:%d SRE:%d ODE:%d PUE:%d HYS:%d LVTTL:%d VSEL:%d\n",
    IOMUXC_SW_MUX_PAD_BASE_ADDRESS + IMX_IOMUX_PAD_CTL_OFFSET(Pad),
    padCtl.AsUint32,
    padCtl.Fields.DSE,
    padCtl.Fields.SRE,
    padCtl.Fields.ODE,
    padCtl.Fields.PUE,
    padCtl.Fields.HYS,
    padCtl.Fields.LVTTL,
    padCtl.Fields.VSEL));
#endif
}

//
// GPIO functions
//

VOID
ImxGpioDirection (
  IMX_GPIO_BANK Bank,
  UINT32 IoNumber,
  IMX_GPIO_DIR Direction
  )
{
  volatile IMX_GPIO_REGISTERS *gpioRegisters =
      (IMX_GPIO_REGISTERS *) IMX_GPIO_BASE;

  ASSERT (IoNumber < 32);

  if (Direction == IMX_GPIO_DIR_INPUT) {
    MmioAnd32 ((UINTN) &gpioRegisters->Banks[Bank - 1].GDIR, ~(1 << IoNumber));
  } else {
    MmioOr32 ((UINTN) &gpioRegisters->Banks[Bank - 1].GDIR, 1 << IoNumber);
  }
}

VOID
ImxGpioWrite (
  IMX_GPIO_BANK Bank,
  UINT32 IoNumber,
  IMX_GPIO_VALUE Value
  )
{
  volatile IMX_GPIO_REGISTERS *gpioRegisters =
      (IMX_GPIO_REGISTERS *) IMX_GPIO_BASE;

  ASSERT (IoNumber < 32);

  if (Value == IMX_GPIO_LOW) {
    MmioAnd32 ((UINTN) &gpioRegisters->Banks[Bank - 1].DR, ~(1 << IoNumber));
  } else {
    MmioOr32 ((UINTN) &gpioRegisters->Banks[Bank - 1].DR, 1 << IoNumber);
  }
}

IMX_GPIO_VALUE
ImxGpioRead (
  IMX_GPIO_BANK Bank,
  UINT32 IoNumber
  )
{
  volatile IMX_GPIO_REGISTERS *gpioRegisters =
      (IMX_GPIO_REGISTERS *) IMX_GPIO_BASE;

  ASSERT (IoNumber < 32);

  UINT32 Mask = (1 << IoNumber);
  UINT32 Psr = MmioRead32 ((UINTN) &gpioRegisters->Banks[Bank - 1].PSR);

  if (Psr & Mask) {
    return IMX_GPIO_HIGH;
  } else {
    return IMX_GPIO_LOW;
  }
}

