/** @file
 *DeviceBootManager  - Ms Device specific extensions to BdsDxe.

Copyright (c) 2017, Microsoft Corporation

All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/
#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Guid/ZeroGuid.h>

#define gEndEntire \
  { \
    END_DEVICE_PATH_TYPE, \
    END_ENTIRE_DEVICE_PATH_SUBTYPE, \
    { \
      END_DEVICE_PATH_LENGTH, \
      0 \
    } \
  }

typedef struct {
    VENDOR_DEVICE_PATH             VendorDevicePath;
    EFI_DEVICE_PATH_PROTOCOL       End;
} GRAPHICS_DEVICE_PATH;

static GRAPHICS_DEVICE_PATH gPlatformGraphicsDevice = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    ZERO_GUID
  },
  gEndEntire
};


/**
  OnDemandConInCOnnect
 */
EFI_DEVICE_PATH_PROTOCOL **
EFIAPI
DeviceBootManagerOnDemandConInConnect (
  VOID
  )
{
    return NULL;
}

/**
  Do the device specific action at start of BDS
**/
VOID
EFIAPI
DeviceBootManagerBdsEntry (
  VOID
  )
{
   // PlatformBdsInit();
}

/**
  Do the device specific action before the console is connected.

  Such as:
      Initialize the platform boot order
      Supply Console information
**/
EFI_HANDLE
EFIAPI
DeviceBootManagerBeforeConsole (
  EFI_DEVICE_PATH_PROTOCOL    **DevicePath,
  BDS_CONSOLE_CONNECT_ENTRY   **PlatformConsoles
  )
{

  EFI_STATUS               Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                    HandleCount;
  UINTN                    Index;
  UINTN                    MyPathSize;
  EFI_HANDLE               Handle = NULL;

  *DevicePath = NULL;
  HandleBuffer = NULL;
  HandleCount = 0;
  MyPathSize = GetDevicePathSize((EFI_DEVICE_PATH_PROTOCOL *) &gPlatformGraphicsDevice);

  //
  // Get the list of available DevicePaths handles
  //
  Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiDevicePathProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Handles with gEfiDevicePathProtocolGuid not found. Status = %r\n", Status));
    goto Done;
  }

  //
  // For each device path handle, get the full device path and check to see if it
  // matches the one we are looking for.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                        HandleBuffer[Index],
                        &gEfiDevicePathProtocolGuid,
                        (VOID **) DevicePath
                        );
    if (EFI_ERROR (Status)) {
      *DevicePath = NULL;
      DEBUG((DEBUG_ERROR, "Device Path on handle of Hyper-V video device not found.  Status = %r\n", Status));
      continue;
    }

    //
    // Correct DevicePath will match the one we are looking for exactly
    //
    if (CompareMem(*DevicePath, &gPlatformGraphicsDevice, MyPathSize) == 0) {
      Handle = HandleBuffer[Index];

      //
      // Have to duplicate the device path because BDS will try to free it.
      //
      *DevicePath = DuplicateDevicePath(*DevicePath);
      if (*DevicePath == NULL) {
        Handle = NULL;
      }
      break;
    }
    *DevicePath = NULL;
  }

Done:
  DEBUG((DEBUG_ERROR, "%a - exiting - %r!\n", __FUNCTION__, Status));
  return Handle;
}

/**
  Do the device specific action after the console is connected.

  Such as: Nothing right now.
**/
EFI_DEVICE_PATH_PROTOCOL **
EFIAPI
DeviceBootManagerAfterConsole (
  VOID
  )
{
  return NULL;
}

/**
ProcessBootCompletion
*/
VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (
  IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOption
  )
{
  return;
}

/**
 * Check for HardKeys during boot.  If the hard keys are pressed, builds
 * a boot option for the specific hard key setting.
 *
 *
 * @param BootOption   - Boot Option filled in based on which hard key is pressed
 *
 * @return EFI_STATUS  - EFI_NOT_FOUND - no hard key pressed, no BootOption
 *                       EFI_SUCCESS   - BootOption is valid
 *                       other error   - Unable to build BootOption
 */
EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (
  EFI_BOOT_MANAGER_LOAD_OPTION   *BootOption
  )
{
    return EFI_NOT_FOUND;
}

/**
 This is called from BDS right before going into front page 
 when no bootable devices/options found
*/
VOID
EFIAPI
DeviceBootManagerUnableToBoot (
  VOID
  )
{

}

EFI_STATUS
EFIAPI
DeviceBootManagerConstructor (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
  ) {
  return EFI_SUCCESS;
}