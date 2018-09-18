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

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMXDisplay.h>

#include "Display.h"
#include "Edid.h"
#include "Ddc.h"

EFI_STATUS
ReadEdid (
  IN  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN  DISPLAY_INTERFACE_TYPE   DisplayInterface,
  IN  UINT8               *EdidDataPtr,
  OUT UINT32              *EdidDataSizePtr
  )
{
  EFI_STATUS  Status;

  Status = Imx6DdcRead (
             DisplayContextPtr,
             DisplayInterface,
             EDID_I2C_ADDRESS,
             0,
             EDID_MIN_SIZE,
             EdidDataPtr
           );
  if (Status != EFI_SUCCESS) {
    goto Exit;
  }

  Status = ValidateEdidData (
             EdidDataPtr
           );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "%a: Invalid EDID data\n", __FUNCTION__));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "%a: EDID initialized\n", __FUNCTION__));

  *EdidDataSizePtr = EDID_MIN_SIZE;

Exit:
  return Status;
}

EFI_STATUS
GetEdidPreferredTiming (
  IN  UINT8           *EdidDataPtr,
  IN  UINT32          EdidDataSizePtr,
  OUT DISPLAY_TIMING  *PreferredTiming
  )
{
  DETAILED_TIMING_DESCRIPTOR  *pEdidPreferredTiming;
  EFI_STATUS                  Status;

  if (EdidDataSizePtr < EDID_MIN_SIZE) {
    DEBUG ((DEBUG_WARN, "%a: Insufficient EDID data\n", __FUNCTION__));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  pEdidPreferredTiming = (DETAILED_TIMING_DESCRIPTOR *)&EdidDataPtr[EDID_DTD_1_OFFSET];
  Status = ConvertDTDToDisplayTiming (pEdidPreferredTiming, PreferredTiming);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: Conversion to display timing failed\n",
      __FUNCTION__));
    goto Exit;
  }

Exit:

  return Status;
}
