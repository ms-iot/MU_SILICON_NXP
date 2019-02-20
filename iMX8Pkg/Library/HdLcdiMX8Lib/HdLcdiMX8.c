/** @file

  Copyright (c) 2013-2018, ARM Ltd. All rights reserved.
  Copyright (c) Microsoft Corporation. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>


static SCAN_TIMINGS timings[] = {
    {HD720_H_RES_PIXELS, HD720_H_SYNC, HD720_H_BACK_PORCH, HD720_H_FRONT_PORCH},
    {HD720_V_RES_PIXELS, HD720_V_SYNC, HD720_V_BACK_PORCH, HD720_V_FRONT_PORCH}
};

/** HDLCD platform specific initialization function.

  @param[in] Handle              Handle to the LCD device instance.

  @retval EFI_SUCCESS            Plaform library initialized successfully.
  @retval EFI_UNSUPPORTED        PcdGopPixelFormat must be
                                 PixelRedGreenBlueReserved8BitPerColor OR
                                 PixelBlueGreenRedReserved8BitPerColor
                                 any other format is not supported.
  @retval !(EFI_SUCCESS)         Other errors.
**/
EFI_STATUS
LcdPlatformInitializeDisplay (
  IN  EFI_HANDLE  Handle
  )
{
  (VOID)Handle;

  return EFI_SUCCESS;
}

/** Allocate VRAM memory in DRAM for the framebuffer
  (unless it is reserved already).

  The allocated address can be used to set the framebuffer.

  @param[out] VramBaseAddress     A pointer to the framebuffer address.
  @param[out] VramSize            A pointer to the size of the framebuffer
                                  in bytes

  @retval EFI_SUCCESS             Framebuffer memory allocated successfully.
  @retval !(EFI_SUCCESS)          Other errors.
**/
EFI_STATUS
LcdPlatformGetVram (
  OUT EFI_PHYSICAL_ADDRESS  * VramBaseAddress,
  OUT UINTN                 * VramSize
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;

  ASSERT (VramBaseAddress != NULL);
  ASSERT (VramSize != NULL);

  // Set the VRAM size.
  *VramSize = HD720_H_RES_PIXELS * HD720_V_RES_PIXELS * 4;
  ASSERT (*VramSize <= FixedPcdGet64 (PcdArmLcdDdrFrameBufferSize));

  // TODO: Pull frame buffer from global memory block
  *VramBaseAddress =
     (EFI_PHYSICAL_ADDRESS)FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase);

  return Status;
}

/** Return total number of modes supported.

  Note: Valid mode numbers are 0 to MaxMode - 1
  See Section 12.9 of the UEFI Specification 2.7

  @retval UINT32             Mode Number.
**/
UINT32
LcdPlatformGetMaxMode (VOID)
{
  return 1;
}

/** Set the requested display mode.

  @param[in] ModeNumber             Mode Number.

  @retval EFI_SUCCESS              Mode set successfully.
  @retval EFI_NOT_FOUND            Clock protocol instance not found.
  @retval EFI_DEVICE_ERROR         SCMI error.
  @retval EFI_INVALID_PARAMETER    Requested mode not found.
  @retval !(EFI_SUCCESS)           Other errors.
*/
EFI_STATUS
LcdPlatformSetMode (
  IN  UINT32  ModeNumber
  )
{
  if (ModeNumber >= 1) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/** Return information for the requested mode number.

  @param[in]  ModeNumber          Mode Number.

  @param[out] Info                Pointer for returned mode information
                                  (on success).

  @retval EFI_SUCCESS             Mode information for the requested mode
                                  returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformQueryMode (
  IN  UINT32                                  ModeNumber,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  * Info
  )
{
  if (ModeNumber >= 1 ){
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Info != NULL);

  Info->Version = 0;

#if defined(CPU_IMX8MM)
  Info->HorizontalResolution = HD_H_RES_PIXELS;
  Info->VerticalResolution = HD_V_RES_PIXELS;
  Info->PixelsPerScanLine = HD_H_RES_PIXELS;
#elif defined(CPU_IMX8MQ)
  Info->HorizontalResolution = HD720_H_RES_PIXELS;
  Info->VerticalResolution = HD720_V_RES_PIXELS;
  Info->PixelsPerScanLine = HD720_H_RES_PIXELS;
#else
#error "Unknown CPU family"
#endif

  Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;

  return EFI_SUCCESS;
}

/** Return display timing information for the requested mode number.

  @param[in]  ModeNumber          Mode Number.

  @param[out] Horizontal          Pointer to horizontal timing parameters.
                                  (Resolution, Sync, Back porch, Front porch)
  @param[out] Vertical            Pointer to vertical timing parameters.
                                  (Resolution, Sync, Back porch, Front porch)

  @retval EFI_SUCCESS             Display timing information for the requested
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformGetTimings (
  IN  UINT32           ModeNumber,
  OUT SCAN_TIMINGS  ** Horizontal,
  OUT SCAN_TIMINGS  ** Vertical
  )
{
  if (ModeNumber >= 1 ){
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Horizontal != NULL);
  ASSERT (Vertical != NULL);

  *Horizontal = &timings[0];
  *Vertical   = &timings[1];

  return EFI_SUCCESS;
}

/** Return bits per pixel information for a mode number.

  @param[in]  ModeNumber          Mode Number.

  @param[out] Bpp                 Pointer to bits per pixel information.

  @retval EFI_SUCCESS             Bits per pixel information for the requested
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformGetBpp (
  IN  UINT32     ModeNumber,
  OUT LCD_BPP  * Bpp
  )
{
  if (ModeNumber >= 1) {
    // Check valid ModeNumber and Bpp.
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Bpp != NULL);

  *Bpp = LCD_BITS_PER_PIXEL_24;

  return EFI_SUCCESS;
}
