/** @file
*
*  Copyright (c) 2013-2015, ARM Limited. All rights reserved.
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

#include "iMX8DxeInternal.h"

#include <IndustryStandard/Pci.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>

#include <Guid/EventGroup.h>
#include <Guid/GlobalVariable.h>

#include <Library/ArmShellCmdLib.h>
#include <Library/AcpiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/PrintLib.h>

// 7E374E25-8E01-4FEE-87F2-390C23C606CD is the global GUID for the AcpiTable.  
STATIC CONST EFI_GUID mImx8AcpiTableFile = { 0x7E374E25, 0x8E01, 0x4FEE, {0x87, 0xF2, 0x39, 0x0C, 0x23, 0xC6, 0x06, 0xCD} };

EFI_STATUS
EFIAPI
iMX8EntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  HypBase;

  //
  // Register the XHCI controllers as non-coherent
  // non-discoverable devices.
  //
  Status = RegisterNonDiscoverableMmioDevice (
             NonDiscoverableDeviceTypeXhci,
             NonDiscoverableDeviceDmaTypeNonCoherent,
             NULL,
             NULL,
             1,
             FixedPcdGet64 (PcdUsb1XhciBaseAddress),
             SIZE_1MB
             );
  ASSERT_EFI_ERROR (Status);

  Status = RegisterNonDiscoverableMmioDevice (
             NonDiscoverableDeviceTypeXhci,
             NonDiscoverableDeviceDmaTypeNonCoherent,
             NULL,
             NULL,
             1,
             FixedPcdGet64 (PcdUsb2XhciBaseAddress),
             SIZE_1MB
             );
  ASSERT_EFI_ERROR (Status);

  //
  // If a hypervisor has been declared then we need to make sure its region is protected at runtime
  //
  // Note: This code is only a workaround for our dummy hypervisor (ArmPkg/Extra/AArch64ToAArch32Shim/)
  //       that does not set up (yet) the stage 2 translation table to hide its own memory to EL1.
  //
  if (FixedPcdGet32 (PcdHypFvSize) != 0) {
    // Ensure the hypervisor region is strictly contained into a EFI_PAGE_SIZE-aligned region.
    // The memory must be a multiple of EFI_PAGE_SIZE to ensure we do not reserve more memory than the hypervisor itself.
    // A UEFI Runtime region size granularity cannot be smaller than EFI_PAGE_SIZE. If the hypervisor size is not rounded
    // to this size then there is a risk some non-runtime memory could be visible to the OS view.
    if (((FixedPcdGet32 (PcdHypFvSize) & EFI_PAGE_MASK) == 0) && ((FixedPcdGet32 (PcdHypFvBaseAddress) & EFI_PAGE_MASK) == 0)) {
      // The memory needs to be declared because the DXE core marked it as reserved and removed it from the memory space
      // as it contains the Firmware.
      Status = gDS->AddMemorySpace (
          EfiGcdMemoryTypeSystemMemory,
          FixedPcdGet32 (PcdHypFvBaseAddress), FixedPcdGet32 (PcdHypFvSize),
          EFI_MEMORY_WB | EFI_MEMORY_RUNTIME
          );
      if (!EFI_ERROR (Status)) {
        // We allocate the memory to ensure it is marked as runtime memory
        HypBase = FixedPcdGet32 (PcdHypFvBaseAddress);
        Status = gBS->AllocatePages (AllocateAddress, EfiRuntimeServicesCode,
                                     EFI_SIZE_TO_PAGES (FixedPcdGet32 (PcdHypFvSize)), &HypBase);
      }
    } else {
      // The hypervisor must be contained into a EFI_PAGE_SIZE-aligned region and its size must also be aligned
      // on a EFI_PAGE_SIZE boundary (ie: 4KB).
      Status = EFI_UNSUPPORTED;
      ASSERT_EFI_ERROR (Status);
    }

    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  // Install dynamic Shell command to run baremetal binaries.
  Status = ShellDynCmdRunAxfInstall (ImageHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "iMX8Dxe: Failed to install ShellDynCmdRunAxf\n"));
  }

  return Status;
}

