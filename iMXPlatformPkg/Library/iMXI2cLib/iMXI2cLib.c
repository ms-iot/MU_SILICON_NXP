/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
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

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

#include <iMXI2cLib.h>

IMX_I2C_DIVIDER DividerValue[] = {
  { 22, 0x20, },
  { 24, 0x21, },
  { 26, 0x22, },
  { 28, 0x23, },
  { 30, 0x00, },
  { 32, 0x24, },
  { 36, 0x25, },
  { 40, 0x26, },
  { 42, 0x03, },
  { 44, 0x27, },
  { 48, 0x28, },
  { 52, 0x05, },
  { 56, 0x29, },
  { 60, 0x06, },
  { 64, 0x2A, },
  { 72, 0x2B, },
  { 80, 0x2C, },
  { 88, 0x09, },
  { 96, 0x2D, },
  { 104, 0x0A, },
  { 112, 0x2E, },
  { 128, 0x2F, },
  { 144, 0x0C, },
  { 160, 0x30, },
  { 192, 0x31, },
  { 224, 0x32, },
  { 240, 0x0F, },
  { 256, 0x33, },
  { 288, 0x10, },
  { 320, 0x34, },
  { 384, 0x35, },
  { 448, 0x36, },
  { 480, 0x13, },
  { 512, 0x37, },
  { 576, 0x14, },
  { 640, 0x38, },
  { 768, 0x39, },
  { 896, 0x3A, },
  { 960, 0x17, },
  { 1024, 0x3B, },
  { 1152, 0x18, },
  { 1280, 0x3C, },
  { 1536, 0x3D, },
  { 1792, 0x3E, },
  { 1920, 0x1B, },
  { 2048, 0x3F, },
  { 2304, 0x1C, },
  { 2560, 0x1D, },
  { 3072, 0x1E, },
  { 3840, 0x1F, },
};

#define DIVIDER_VALUE_TOTAL (sizeof(DividerValue) / sizeof(DividerValue[0]))

BOOLEAN
iMXI2cWaitStatusSet (
  IN  IMX_I2C_CONFIG   *I2cConfigPtr,
  IN  UINT16           StatusBits
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  UINT32            Counter;
  IMX_I2C_I2SR_REG  I2srReg;

  Counter = I2cConfigPtr->TimeoutInUs;
  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;
  while (Counter) {
    I2srReg = (IMX_I2C_I2SR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2SR);
    if ((I2srReg.AsUint16 & StatusBits) == StatusBits) {
      return TRUE;
    }
    MicroSecondDelay (1);
    --Counter;
  }

  return FALSE;
}

BOOLEAN
iMXI2cWaitStatusUnSet (
  IN  IMX_I2C_CONFIG   *I2cConfigPtr,
  IN  UINT16           StatusBits
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  UINT32            Counter;
  IMX_I2C_I2SR_REG  I2srReg;

  Counter = I2cConfigPtr->TimeoutInUs;
  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;
  while (Counter) {
    I2srReg = (IMX_I2C_I2SR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2SR);
    if ((I2srReg.AsUint16 & StatusBits) == 0) {
      return TRUE;
    }
    MicroSecondDelay (1);
    --Counter;
  }

  return FALSE;
}

BOOLEAN
iMXI2cSendByte (
  IN  IMX_I2C_CONFIG  *I2cConfigPtr,
  IN  UINT8           Data
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  UINT32            Counter;
  IMX_I2C_I2SR_REG  I2srReg;
  UINT16            SendData;

  SendData = Data;
  Counter = I2cConfigPtr->TimeoutInUs;
  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;

  // Clear status and transfer byte
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2SR, 0);
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2DR, SendData);

  while (Counter) {
    I2srReg = (IMX_I2C_I2SR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2SR);
    if (I2srReg.IIF == 1) {
      return TRUE;
    } else if (I2srReg.IAL == 1) {
      DEBUG ((DEBUG_ERROR, "iMXI2cSendByte: fail 0x%04x\n", I2srReg.AsUint16));
      return FALSE;
    }
    MicroSecondDelay (1);
    --Counter;
  }

  DEBUG ((DEBUG_ERROR, "iMXI2cSendByte: Fail timeout\n"));
  return FALSE;
}

RETURN_STATUS
iMXI2cSetupController (
  IN  IMX_I2C_CONFIG *I2cConfigPtr
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  UINT32            Divider;
  UINT32            DividerCount;
  UINT32            IfdrDiv;
  IMX_I2C_I2CR_REG  I2crReg;

  I2cRegsPtr = (IMX_I2C_REGS *)I2cConfigPtr->ControllerAddress;

  // Disable controller and clear any pending interrupt
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, 0);
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2SR, 0);

  // Setup Divider if reference freq is provided. If no, use value setup by
  // 1st boot loader
  if (I2cConfigPtr->ReferenceFreq != 0) {
    IfdrDiv = 0;
    Divider = I2cConfigPtr->ReferenceFreq / I2cConfigPtr->TargetFreq;

    for (DividerCount = 0; DividerCount < DIVIDER_VALUE_TOTAL; ++DividerCount) {
      if (DividerValue[DividerCount].Divider >= Divider) {
        DEBUG ((
                 DEBUG_INFO,
                 "iMXI2cSetupController: Divider %d IC 0x%02x\n",
                 DividerValue[DividerCount].Divider,
                 DividerValue[DividerCount].IC));
        IfdrDiv = DividerValue[DividerCount].IC;
        break;
      }
    }

    if (IfdrDiv == 0) {
      DEBUG ((
               DEBUG_ERROR,
               "iMXI2cSetupController: could not find Divider for %d\n",
               Divider));
      return RETURN_INVALID_PARAMETER;
    }

    MmioWrite16 ((UINTN)&I2cRegsPtr->IFDR, IfdrDiv);
  }

  // Setup slave address
  MmioWrite16 ((UINTN)&I2cRegsPtr->IADR,
               (I2cConfigPtr->ControllerSlaveAddress << 1));

  // Enable controller and set to master mode.
  I2crReg = (IMX_I2C_I2CR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2CR);

  // This bit must be set before any other I2C_I2CR bits have an effect
  I2crReg.IEN = 1;
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);
  MicroSecondDelay (100);

  MmioWrite16 ((UINTN)&I2cRegsPtr->I2SR, 0);

  // Wait for bus to be idle
  if (iMXI2cWaitStatusUnSet (I2cConfigPtr, IMX_I2C_I2SR_IBB) == FALSE) {
    DEBUG ((DEBUG_ERROR, "iMXI2cGenerateStart: Controller remains busy\n"));
    return RETURN_DEVICE_ERROR;
  }

  I2crReg.MSTA = 1;
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);

  // Now wait for bus to be busy
  if (iMXI2cWaitStatusSet (I2cConfigPtr, IMX_I2C_I2SR_IBB) == FALSE) {
    DEBUG ((DEBUG_ERROR, "iMXI2cGenerateStart: Controller remains idle\n"));
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

RETURN_STATUS
iMXI2cGenerateStart (
  IN  IMX_I2C_CONFIG  *I2cConfigPtr,
  IN  UINT8           RegisterAddress
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  IMX_I2C_I2CR_REG  I2crReg;
  BOOLEAN           Result;
  RETURN_STATUS     Status;

  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;
  Status = iMXI2cSetupController (I2cConfigPtr);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "iMXI2cGenerateStart: Fail to setup controller %r\n",
            Status));
    return Status;
  }

  // Set controller to transmit mode
  I2crReg = (IMX_I2C_I2CR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2CR);
  I2crReg.MTX = 1;
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);

  Result = iMXI2cSendByte (I2cConfigPtr, (I2cConfigPtr->SlaveAddress << 1));
  if (Result == FALSE) {
    DEBUG ((
             DEBUG_ERROR,
             "iMXI2cGenerateStart: Slave address transfer fail 0x%04x\n",
             MmioRead16 ((UINTN)&I2cRegsPtr->I2SR)));
    return RETURN_DEVICE_ERROR;
  }

  // Send slave register address
  Result = iMXI2cSendByte (I2cConfigPtr, RegisterAddress);
  if (Result == FALSE) {
    DEBUG ((
             DEBUG_ERROR,
             "iMXI2cGenerateStart: Slave register address transfer fail 0x%04x\n",
             MmioRead16 ((UINTN)&I2cRegsPtr->I2SR)));
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

RETURN_STATUS
iMXI2cGenerateStop (
  IN  IMX_I2C_CONFIG *I2cConfigPtr
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  IMX_I2C_I2CR_REG  I2crReg;

  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;
  I2crReg = (IMX_I2C_I2CR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2CR);
  I2crReg.MSTA = 0;
  I2crReg.MTX = 0;
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);

  // Bus should go idle
  if (iMXI2cWaitStatusUnSet (I2cConfigPtr, IMX_I2C_I2SR_IBB) == FALSE) {
    DEBUG ((DEBUG_ERROR, "iMXI2cGenerateStop: Controller remains busy\n"));
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

/**
  Perform I2C read operation.

  The iMXI2cRead perform I2C read operation by programming the I2C controller.
  The caller is responsible to provide I2C controller configuration.

  @param[in]    IMX_I2C_CONFIG    Pointer to structure containing the targeted
                                  I2C controller to be used for I2C operation.
  @param[in]    RegisterAddress   Targeted device register address to start read.
  @param[out]   ReadBufferPtr     Caller supplied buffer that would be written
                                  into with data from the read operation.
  @param[in]    ReadBufferSize    Size of caller supplied buffer.

  @retval   RETURN_SUCCESS        I2C Read operation succeeded.
  @retval   RETURN_DEVICE_ERROR   The I2C device is not functioning correctly.

**/
RETURN_STATUS
iMXI2cRead (
  IN IMX_I2C_CONFIG  *I2cConfigPtr,
  IN UINT8            RegisterAddress,
  OUT UINT8          *ReadBufferPtr,
  IN UINT32           ReadBufferSize
  )
{
  IMX_I2C_REGS      *I2cRegsPtr;
  IMX_I2C_I2CR_REG  I2crReg;
  BOOLEAN           Result;
  RETURN_STATUS     Status;

  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;
  Status = iMXI2cGenerateStart (I2cConfigPtr, RegisterAddress);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "iMXI2cRead: iMXI2cGenerateStart failed %r\n", Status));
    goto Exit;
  }

  // Send slave address again to begin read
  I2crReg = (IMX_I2C_I2CR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2CR);
  I2crReg.RSTA = 1; // Repeated start
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);
  Result = iMXI2cSendByte (
              I2cConfigPtr,
              (I2cConfigPtr->SlaveAddress << 1 | 1));
  if (Result == FALSE) {
    DEBUG ((
              DEBUG_ERROR,
              "iMXI2cRead: 2nd Slave address transfer failed 0x%04x\n",
              MmioRead16 ((UINTN)&I2cRegsPtr->I2SR)));
    Status = RETURN_DEVICE_ERROR;
    goto Exit;
  }

  // Disable master mode
  I2crReg = (IMX_I2C_I2CR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2CR);

  // NXP application note AN4481 - Only one byte so do not send acknowledge
  if (ReadBufferSize == 1) {
    I2crReg.TXAK = 1;
  } else {
    I2crReg.TXAK = 0;
  }

  I2crReg.MTX = 0;
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);

  // A data transfer can now be initiated by a read from I2C_I2DR in Slave
  // Receive mode.
  MmioWrite16 ((UINTN)&I2cRegsPtr->I2SR, 0);
  MmioRead16 ((UINTN)&I2cRegsPtr->I2DR);

  do {
    // Wait for transfer to complete
    if (iMXI2cWaitStatusSet (I2cConfigPtr, IMX_I2C_I2SR_IIF) == FALSE) {
      DEBUG ((DEBUG_ERROR, "iMXI2cRead: waiting for read fail\n"));
      Status = RETURN_DEVICE_ERROR;
      goto Exit;
    }

    if (iMXI2cWaitStatusSet (I2cConfigPtr, IMX_I2C_I2SR_ICF) == FALSE) {
      DEBUG ((DEBUG_ERROR, "iMXI2cRead: waiting for read fail\n"));
      Status = RETURN_DEVICE_ERROR;
      goto Exit;
    }

    // Before the last byte is read, a Stop signal must be generated
    if (ReadBufferSize == 1) {
      Status = iMXI2cGenerateStop (
                 I2cConfigPtr);
      if (RETURN_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "iMXI2cRead: iMXI2cGenerateStop fail %r\n", Status));
        goto Exit;
      }
    }

    if (ReadBufferSize == 2) {
      I2crReg = (IMX_I2C_I2CR_REG)MmioRead16 ((UINTN)&I2cRegsPtr->I2CR);
      I2crReg.TXAK = 1;
      MmioWrite16 ((UINTN)&I2cRegsPtr->I2CR, I2crReg.AsUint16);
    }

    MmioWrite16 ((UINTN)&I2cRegsPtr->I2SR, 0);

    *ReadBufferPtr = MmioRead8 ((UINTN)&I2cRegsPtr->I2DR);
    ++ReadBufferPtr;
    --ReadBufferSize;
  } while (ReadBufferSize > 0);

Exit:
  Status = iMXI2cGenerateStop (I2cConfigPtr);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "iMXI2cRead: Final iMXI2cGenerateStop fail %r\n", Status));
  }

  return Status;
}

/**
  Perform I2C write operation.

  The iMXI2cWrite perform I2C write operation by programming the I2C
  controller. The caller is responsible to provide I2C controller
  configuration.

  @param[in]    IMX_I2C_CONFIG    Pointer to structure containing the targeted
                                  I2C controller to be used for I2C operation.
  @param[in]    RegisterAddress   Targeted device register address to start write.
  @param[out]   WriteBufferPtr    Caller supplied buffer that contained data that
                                  would be read from for I2C write operation.
  @param[in]    WriteBufferSize   Size of caller supplied buffer.

  @retval   RETURN_SUCCESS        I2C Write operation succeeded.
  @retval   RETURN_DEVICE_ERROR   The I2C device is not functioning correctly.

**/
RETURN_STATUS
iMXI2cWrite (
  IN IMX_I2C_CONFIG  *I2cConfigPtr,
  IN UINT8            RegisterAddress,
  IN UINT8           *WriteBufferPtr,
  IN UINT32           WriteBufferSize
  )
{
  IMX_I2C_REGS    *I2cRegsPtr;
  BOOLEAN         Result;
  RETURN_STATUS   Status;

  I2cRegsPtr = (IMX_I2C_REGS*)I2cConfigPtr->ControllerAddress;
  Status = iMXI2cGenerateStart (I2cConfigPtr, RegisterAddress);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "iMXI2cWrite: iMXI2cGenerateStart fail %r\n", Status));
    goto Exit;
  }

  while (WriteBufferSize > 0) {
    Result = iMXI2cSendByte (I2cConfigPtr, *WriteBufferPtr);
    if (Result == FALSE) {
      DEBUG ((
               DEBUG_ERROR,
               "iMXI2cWrite: Slave address transfer fail 0x%04x\n",
               MmioRead16 ((UINTN)&I2cRegsPtr->I2SR)));
      Status = RETURN_DEVICE_ERROR;
      goto Exit;
    }

    ++WriteBufferPtr;
    --WriteBufferSize;
  }

Exit:
  Status = iMXI2cGenerateStop (I2cConfigPtr);
  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "iMXI2cWrite: iMXI2cGenerateStop fail %r\n", Status));
  }

  return Status;
}
