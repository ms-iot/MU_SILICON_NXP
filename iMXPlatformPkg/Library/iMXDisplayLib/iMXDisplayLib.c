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

#include <Library/DebugLib.h>

#include <iMXDisplay.h>

/**
  Convert detailed timing descriptor to display timing format

  @param[in]    DTDPtr            Pointer to detailed timing descriptor.
  @param[out]   DisplayTimingPtr  Pointer to display timing structure.

  @retval   EFI_SUCCESS   Detailed timing descriptor data was converted.

**/
EFI_STATUS
ConvertDTDToDisplayTiming (
  IN DETAILED_TIMING_DESCRIPTOR   *DTDPtr,
  OUT DISPLAY_TIMING              *DisplayTimingPtr
  )
{
  UINT32  edidPixelClock;

  DEBUG ((DEBUG_INFO, "++ConvertDTDToDisplayTiming()\r\n"));
  // Refer to 3.10.2 VESA EDID spec
  edidPixelClock = (DTDPtr->PixelClock[0] | (DTDPtr->PixelClock[1] << 8));
  DisplayTimingPtr->PixelClock = edidPixelClock * 10000;
  DisplayTimingPtr->HActive = (DTDPtr->HActiveBlank & 0xF0);
  DisplayTimingPtr->HActive = (DisplayTimingPtr->HActive << 4) | DTDPtr->HActive;
  DisplayTimingPtr->HBlank = (DTDPtr->HActiveBlank & 0x0F);
  DisplayTimingPtr->HBlank = (DisplayTimingPtr->HBlank << 8) | DTDPtr->HBlank;
  DisplayTimingPtr->VActive = (DTDPtr->VActiveBlank & 0xF0);
  DisplayTimingPtr->VActive = (DisplayTimingPtr->VActive << 4) | DTDPtr->VActive;
  DisplayTimingPtr->VBlank = (DTDPtr->VActiveBlank & 0x0F);
  DisplayTimingPtr->VBlank = (DisplayTimingPtr->VBlank << 8) | DTDPtr->VBlank;
  DisplayTimingPtr->HSyncOffset = (DTDPtr->HVOffsetWidth & 0xC0);
  DisplayTimingPtr->HSyncOffset = (DisplayTimingPtr->HSyncOffset << 2) |
                                  DTDPtr->HSyncOffset;
  DisplayTimingPtr->VSyncOffset = (DTDPtr->HVOffsetWidth & 0x0C);
  DisplayTimingPtr->VSyncOffset = (DisplayTimingPtr->VSyncOffset << 2) |
                                  ((DTDPtr->VSyncOffsetWidth & 0xF0) >> 4);
  DisplayTimingPtr->HSync = (DTDPtr->HVOffsetWidth & 0x30);
  DisplayTimingPtr->HSync = (DisplayTimingPtr->HSync << 4) | DTDPtr->HSyncWidth;
  DisplayTimingPtr->VSync = (DTDPtr->HVOffsetWidth & 0x03);
  DisplayTimingPtr->VSync = (DisplayTimingPtr->VSync << 4) |
                            (DTDPtr->VSyncOffsetWidth & 0x0F);
  DisplayTimingPtr->HImageSize = ((DTDPtr->HVImageSize & 0xF0) << 4) |
                                 DTDPtr->HImageSize;
  DisplayTimingPtr->VImageSize = ((DTDPtr->HVImageSize & 0x0F) << 8) |
                                 DTDPtr->VImageSize;
  DisplayTimingPtr->HBorder = DTDPtr->HBorder;
  DisplayTimingPtr->VBorder = DTDPtr->VBorder;
  DisplayTimingPtr->EdidFlags = DTDPtr->EdidFlags;
  DisplayTimingPtr->Flags = 0;

  DEBUG ((DEBUG_INFO, "--ConvertDTDToDisplayTiming()=ok\r\n"));
  return EFI_SUCCESS;
}

/**
  Debug dump of Display Timing structure

  @param[in]    DisplayTimingNamePtr  Name of display timing structure.
  @param[in]    DisplayTimingPtr      Pointer to display timing structure.
**/
VOID
PrintDisplayTiming (
  IN CHAR8            *DisplayTimingNamePtr,
  IN DISPLAY_TIMING   *DisplayTimingPtr
  )
{
  DEBUG ((DEBUG_INFO, "**********************\n"));
  DEBUG ((DEBUG_INFO, "%a\n", DisplayTimingNamePtr));
  DEBUG ((DEBUG_INFO, "**********************\n"));
  DEBUG ((DEBUG_INFO, "PixelClock %d\n", DisplayTimingPtr->PixelClock));
  DEBUG ((DEBUG_INFO, "HActive %d\n", DisplayTimingPtr->HActive));
  DEBUG ((DEBUG_INFO, "HBlank %d\n", DisplayTimingPtr->HBlank));
  DEBUG ((DEBUG_INFO, "VActive %d\n", DisplayTimingPtr->VActive));
  DEBUG ((DEBUG_INFO, "VBlank %d\n", DisplayTimingPtr->VBlank));
  DEBUG ((DEBUG_INFO, "HSync %d\n", DisplayTimingPtr->HSync));
  DEBUG ((DEBUG_INFO, "VSync %d\n", DisplayTimingPtr->VSync));
  DEBUG ((DEBUG_INFO, "HSyncOffset %d\n", DisplayTimingPtr->HSyncOffset));
  DEBUG ((DEBUG_INFO, "VSyncOffset %d\n", DisplayTimingPtr->VSyncOffset));
  DEBUG ((DEBUG_INFO, "HBorder %d\n", DisplayTimingPtr->HBorder));
  DEBUG ((DEBUG_INFO, "VBorder %d\n", DisplayTimingPtr->VBorder));
  DEBUG ((DEBUG_INFO, "EdidFlags %d\n", DisplayTimingPtr->EdidFlags));
  DEBUG ((DEBUG_INFO, "Flags %d\n", DisplayTimingPtr->Flags));
  DEBUG ((DEBUG_INFO, "PixelRepetition %d\n", DisplayTimingPtr->PixelRepetition));
  DEBUG ((DEBUG_INFO, "BPP %d\n", DisplayTimingPtr->Bpp));
  DEBUG ((DEBUG_INFO, "PixelFormat %d\n", DisplayTimingPtr->PixelFormat));
  DEBUG ((DEBUG_INFO, "**********************\n"));
}

/**
  Check if EDID is valid

  @param[in]    EdidDataPtr  Pointer to EDID data.

  @retval   EFI_SUCCESS             EDID data is a valid EDID.
  @retval   EFI_INVALID_PARAMETER   EDID data is invalid.

**/
EFI_STATUS
ValidateEdidData (
  IN UINT8 *EdidDataPtr
  )
{
  UINT8   Checksum;
  UINT8   Index;

  DEBUG ((DEBUG_INFO, "++ValidateEdidData()\r\n"));

  if (EdidDataPtr[0] != 0x00 ||
      EdidDataPtr[1] != 0xFF ||
      EdidDataPtr[2] != 0xFF ||
      EdidDataPtr[3] != 0xFF ||
      EdidDataPtr[4] != 0xFF ||
      EdidDataPtr[5] != 0xFF ||
      EdidDataPtr[6] != 0xFF ||
      EdidDataPtr[7] != 0x00) {
    DEBUG ((DEBUG_ERROR, "Invalid EDID header\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Validate EDID checksum
  Checksum = 0;
  for (Index = 0; Index < EDID_MIN_SIZE; Index++) {
    Checksum += EdidDataPtr[Index];
  }

  if (Checksum != 0) {
    DEBUG ((DEBUG_ERROR, "Invalid EDID checksum\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "--ValidateEdidData()=ok\r\n"));
  return EFI_SUCCESS;
}
