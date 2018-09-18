//
// Copyright (C) Microsoft. All rights reserved
//
/** @file

  Copyright (c) 2008-2009, Apple Inc. All rights reserved.

  All rights reserved. This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PciEmulation.h"


#define HOST_CONTROLLER_OPERATION_REG_SIZE  0x44

typedef struct {
  ACPI_HID_DEVICE_PATH      AcpiDevicePath;
  PCI_DEVICE_PATH           PciDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  EndDevicePath;
} EFI_PCI_IO_DEVICE_PATH;

typedef struct {
  UINT32                  Signature;
  EFI_PCI_IO_DEVICE_PATH  DevicePath;
  EFI_PCI_IO_PROTOCOL     PciIoProtocol;
  PCI_TYPE00              *ConfigSpace;
  PCI_ROOT_BRIDGE         RootBridge;
  UINTN                   Segment;
  UINT32                  CommonBuffer;
  UINT32                  CommonBufferPages;
  BOOLEAN                 *CommonBufferMap;
} EFI_PCI_IO_PRIVATE_DATA;

#define EFI_PCI_IO_PRIVATE_DATA_SIGNATURE     SIGNATURE_32('p', 'c', 'i', 'o')
#define EFI_PCI_IO_PRIVATE_DATA_FROM_THIS(a)  CR(a, EFI_PCI_IO_PRIVATE_DATA, PciIoProtocol, EFI_PCI_IO_PRIVATE_DATA_SIGNATURE)

EFI_PCI_IO_DEVICE_PATH PciIoDevicePathTemplate =
{
  {
    { 
        ACPI_DEVICE_PATH, 
        ACPI_DP,
        {
            sizeof (ACPI_HID_DEVICE_PATH), 
            0
        }
    },
    EISA_PNP_ID(0x0A03),  // HID
    0                     // UID
  },
  {
    { 
        HARDWARE_DEVICE_PATH, 
        HW_PCI_DP, 
        {
            sizeof (PCI_DEVICE_PATH), 
            0
        }
    },
    0,
    0
  },
  { 
    END_DEVICE_PATH_TYPE, 
    END_ENTIRE_DEVICE_PATH_SUBTYPE, 
    {
        sizeof (EFI_DEVICE_PATH_PROTOCOL), 
        0
    }
  }
};

EFI_STATUS
PciIoAllocateBuffer (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_ALLOCATE_TYPE            Type,
  IN  EFI_MEMORY_TYPE              MemoryType,
  IN  UINTN                        Pages,
  OUT VOID                         **HostAddress,
  IN  UINT64                       Attributes
  );

EFI_STATUS
PciIoFreeBuffer (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  UINTN                        Pages,
  IN  VOID                         *HostAddress
  );


//
// USB Host controller registers definitions
//
#define USB_HC_USBCMD_REG   0x140
    #define USB_HC_USBCMD_RST   0x00000002
#define USB_HC_USBMOD_REG   0x1A8
    #define USB_HC_USBMOD_HOST  0x00000003

// Period of 1mSec gBS->Stall(uSec)
#define STALL_1_MSEC                (1000)

// USB host controller reset timeout [mSec]
#define USB_HC_RESET_TIMEOUT_MSEC       (500*STALL_1_MSEC)

// USB host status delay after rest [mSec]
#define USB_HC_STATUS_UPDATE_DELAY_MSEC (100*STALL_1_MSEC)

// USB host controller polling interval [mSec]
#define USB_HC_POLL_MSEC                (10*STALL_1_MSEC)

// Bit set test
#define USB_IS_BIT_SET(Data, Bit)   ((BOOLEAN)(((Data) & (Bit)) == (Bit)))


// Post write memory procedure type
typedef VOID (*POST_MEM_WRITE_PROC)(
                  IN     UINT64 Offset,
                  IN     UINTN  Count,
                  IN OUT VOID   *Buffer
                  );


// Post write memory procedure descriptor
typedef struct
{
    // If descriptor is active
    BOOLEAN                     IsActive;

    // Target address
    UINT64                      Address;    
    
    // Data width 
    EFI_PCI_IO_PROTOCOL_WIDTH   DataWidth;

    // Handler
    POST_MEM_WRITE_PROC         Handler;

} POST_MEM_WRITE_PROC_DESC;

VOID
UsbSetRegBit32 (
  IN UINT64 Address,
  IN UINT32 Bit
  )
{
  UINT32 data;

  data  = MmioRead32 ((UINT32)Address);
  data |= Bit;
  MmioWrite32 ((UINT32)Address, data);
}

VOID
UsbClearRegBit32 (
  IN UINT64 Address,
  IN UINT32 Bit
  )
{
  UINT32 data;

  data  = MmioRead32 ((UINT32)Address);
  data &= ~(Bit);
  MmioWrite32 ((UINT32)Address, data);
}

EFI_STATUS
UsbWaitRegBit32 (
  IN UINT64     Address,
  IN UINT32     Bit,
  IN BOOLEAN    IsWaitForSet,
  IN UINT32     TimeoutMs
  )
{
    UINT32 retry; 

    for (retry = 0; retry < TimeoutMs/USB_HC_POLL_MSEC; ++retry) {
        
        if (USB_IS_BIT_SET(MmioRead32((UINT32)Address), Bit) == IsWaitForSet) {
            return EFI_SUCCESS;
        }

        gBS->Stall (USB_HC_POLL_MSEC);
    }

    return EFI_TIMEOUT;
}

// 
// 'Memory Write' post processing handlers
//

VOID
UsbConfigureOtgPort(
  IN     UINT64 Offset,
  IN     UINTN  Count,
  IN OUT VOID   *Buffer
  )
{
    UINT32 otgHcBase;
    UINT32 usbCmdReg;

    // Make sure it is a single register access
    if (Count != 1) {
        return;
    }

    otgHcBase = FixedPcdGet32(PcdUSBOTGBase);
    usbCmdReg = *((UINT32*)Buffer);

    // A reset command ? 
    if (USB_IS_BIT_SET(usbCmdReg, USB_HC_USBCMD_RST)) {
        UINT32 status; 

        DEBUG((EFI_D_INFO, "UsbOtgSetHostMode: A USB_OTG reset command\n",Count));

        // Wait for reset to complete
        status = UsbWaitRegBit32(Offset, USB_HC_USBCMD_RST, FALSE, USB_HC_RESET_TIMEOUT_MSEC);
        if (status == EFI_SUCCESS) {

            //
            // Read boot mode or other configuration parameters to decide
            // how to configure the OTG port:
            // - In normal mode, it is configured to host mode.
            // - In Flash mode, it can be configured as device mode and expose
            //   a USB mass storage device that enables updating the on-board MMC. 
            //

            // Reset is done, configure OTG port to host mode
            UsbSetRegBit32(otgHcBase + USB_HC_USBMOD_REG, USB_HC_USBMOD_HOST);

            // Apply delay to allow the port changed bit to be reflected
            gBS->Stall (USB_HC_STATUS_UPDATE_DELAY_MSEC);
            return;
        }

        DEBUG((EFI_D_ERROR, "UsbOtgSetHostMode: Info: A USB_OTG reset timeout!\n",Count));

    } // A reset command
}

// 
// 'Memory Write' post processing sniffer
//

void
PciIoMemWritePostProcessing (
  IN     UINT64                     Offset,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
  IN     UINTN                      Count,
  IN OUT VOID                       *Buffer
  )
{
    // Post memory write descriptors
    static POST_MEM_WRITE_PROC_DESC postMemWriteDescs[] = {

        // OTG reset -> set configure OTG port (host/device)
        {FixedPcdGetBool(PcdIsUsbPortOTG), FixedPcdGet32(PcdUSBOTGBase) + USB_HC_USBCMD_REG, EfiPciWidthUint32, UsbConfigureOtgPort}
    };
    int descInx;

    // Sniff the target access, and see if this is something we need to do 
    // post processing on
    for (descInx = 0; descInx < ARRAYSIZE(postMemWriteDescs); ++descInx) {
        POST_MEM_WRITE_PROC_DESC* descPtr = &postMemWriteDescs[descInx];

        if (!descPtr->IsActive) {
            continue;
        }

        if ((Offset != descPtr->Address) ||
            (Width != descPtr->DataWidth))
        {
            continue;
        }

        ASSERT (descPtr->Handler != NULL);

        (*descPtr->Handler)(Offset, Count, Buffer);
    }
}

// 
// PCI IO handlers
//

EFI_STATUS
PciIoPollMem (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
PciIoPollIo (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
PciIoMemRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  EFI_PCI_IO_PRIVATE_DATA *Private = EFI_PCI_IO_PRIVATE_DATA_FROM_THIS(This);
  EFI_STATUS Status;

  Status = PciRootBridgeIoMemRead (&Private->RootBridge.Io,
                                (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH) Width,
                                Private->ConfigSpace->Device.Bar[BarIndex] + Offset,
                                Count,
                                Buffer
                                );
   return Status;
}

EFI_STATUS
PciIoMemWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  EFI_PCI_IO_PRIVATE_DATA *Private = EFI_PCI_IO_PRIVATE_DATA_FROM_THIS(This);
  EFI_STATUS Status;

  Status = PciRootBridgeIoMemWrite (&Private->RootBridge.Io,
                                 (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH) Width,
                                 Private->ConfigSpace->Device.Bar[BarIndex] + Offset,
                                 Count,
                                 Buffer
                                 );
  if (Status == EFI_SUCCESS) {
    // Primarily for setting configuring USB OTG port to host mode
    PciIoMemWritePostProcessing (
        Private->ConfigSpace->Device.Bar[BarIndex] + Offset,
        Width,
        Count,
        Buffer
        );
  }

  return Status;
}

EFI_STATUS
PciIoIoRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
PciIoIoWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
PciIoPciRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PTR Destination;
  EFI_PCI_IO_PRIVATE_DATA *Private = EFI_PCI_IO_PRIVATE_DATA_FROM_THIS(This);
  PTR Source;

  Destination.ui = (UINTN)Buffer;
  Source.ui = (UINTN)(((UINT8 *)Private->ConfigSpace) + Offset);

  return PciRootBridgeIoMemRW ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH)Width,
                               Count,
                               TRUE,
                               Destination,
                               TRUE,
                               Source
                              );
}

EFI_STATUS
PciIoPciWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PTR Destination;
  EFI_PCI_IO_PRIVATE_DATA *Private = EFI_PCI_IO_PRIVATE_DATA_FROM_THIS(This);
  PTR Source;

  Destination.ui = (UINTN)(((UINT8 *)Private->ConfigSpace) + Offset);
  Source.ui = (UINTN)Buffer;

  return PciRootBridgeIoMemRW ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH) Width,
                               Count,
                               TRUE,
                               Destination,
                               TRUE,
                               Source
                               );
}

EFI_STATUS
PciIoCopyMem (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        DestBarIndex,
  IN     UINT64                       DestOffset,
  IN     UINT8                        SrcBarIndex,
  IN     UINT64                       SrcOffset,
  IN     UINTN                        Count
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
PciIoMap (
  IN EFI_PCI_IO_PROTOCOL                *This,
  IN     EFI_PCI_IO_PROTOCOL_OPERATION  Operation,
  IN     VOID                           *HostAddress,
  IN OUT UINTN                          *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS           *DeviceAddress,
  OUT    VOID                           **Mapping
  )
{
	DMA_MAP_OPERATION	DmaOperation;
	
	if (Operation == EfiPciIoOperationBusMasterRead) {
	  DmaOperation = MapOperationBusMasterRead;
	} else if (Operation == EfiPciIoOperationBusMasterWrite) {
	  DmaOperation = MapOperationBusMasterWrite;
	} else if (Operation == EfiPciIoOperationBusMasterCommonBuffer) {
	  DmaOperation = MapOperationBusMasterCommonBuffer;
	} else {
	  return EFI_INVALID_PARAMETER;
	}
	return DmaMap (DmaOperation, HostAddress, NumberOfBytes, DeviceAddress, Mapping);
}

EFI_STATUS
PciIoUnmap (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  VOID                         *Mapping
  )
{
	return DmaUnmap (Mapping);

}

EFI_STATUS
PciIoAllocateBuffer (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_ALLOCATE_TYPE            Type,
  IN  EFI_MEMORY_TYPE              MemoryType,
  IN  UINTN                        Pages,
  OUT VOID                         **HostAddress,
  IN  UINT64                       Attributes
  )
{
	if (Attributes & EFI_PCI_ATTRIBUTE_INVALID_FOR_ALLOCATE_BUFFER) {
	  // Check this
	  return EFI_UNSUPPORTED;
	}
	
	return DmaAllocateBuffer (MemoryType, Pages, HostAddress);
}

EFI_STATUS
PciIoFreeBuffer (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  UINTN                        Pages,
  IN  VOID                         *HostAddress
  )
{
	return DmaFreeBuffer (Pages, HostAddress);
}


EFI_STATUS
PciIoFlush (
  IN EFI_PCI_IO_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
PciIoGetLocation (
  IN EFI_PCI_IO_PROTOCOL          *This,
  OUT UINTN                       *SegmentNumber,
  OUT UINTN                       *BusNumber,
  OUT UINTN                       *DeviceNumber,
  OUT UINTN                       *FunctionNumber
  )
{
  EFI_PCI_IO_PRIVATE_DATA *Private = EFI_PCI_IO_PRIVATE_DATA_FROM_THIS(This);

  if (SegmentNumber != NULL) {
    *SegmentNumber = Private->Segment;
  }

  if (BusNumber != NULL) {
    *BusNumber = 0xff;
  }

  if (DeviceNumber != NULL) {
    *DeviceNumber = 0;
  }

  if (FunctionNumber != NULL) {
    *FunctionNumber = 0;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PciIoAttributes (
  IN EFI_PCI_IO_PROTOCOL                       *This,
  IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
  IN  UINT64                                   Attributes,
  OUT UINT64                                   *Result OPTIONAL
  )
{
  switch (Operation) {
  case EfiPciIoAttributeOperationGet:
  case EfiPciIoAttributeOperationSupported:
    if (Result == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    // We are not a real PCI device so just say things we kind of do
    *Result = EFI_PCI_IO_ATTRIBUTE_MEMORY | EFI_PCI_IO_ATTRIBUTE_BUS_MASTER | EFI_PCI_DEVICE_ENABLE;
    break;

  case EfiPciIoAttributeOperationSet:
  case EfiPciIoAttributeOperationEnable:
  case EfiPciIoAttributeOperationDisable:
    // Since we are not a real PCI device no enable/set or disable operations exist.
    return EFI_SUCCESS;
    break;

  default:
  ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  };
  return EFI_SUCCESS;
}

EFI_STATUS
PciIoGetBarAttributes (
  IN EFI_PCI_IO_PROTOCOL             *This,
  IN  UINT8                          BarIndex,
  OUT UINT64                         *Supports, OPTIONAL
  OUT VOID                           **Resources OPTIONAL
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
PciIoSetBarAttributes (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     UINT64                       Attributes,
  IN     UINT8                        BarIndex,
  IN OUT UINT64                       *Offset,
  IN OUT UINT64                       *Length
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_PCI_IO_PROTOCOL PciIoTemplate =
{
  PciIoPollMem,
  PciIoPollIo,
  {
      PciIoMemRead,
      PciIoMemWrite,
  },
  {
      PciIoIoRead,
      PciIoIoWrite,
  },
  {
      PciIoPciRead,
      PciIoPciWrite,
  },
  PciIoCopyMem,
  PciIoMap,
  PciIoUnmap,
  PciIoAllocateBuffer,
  PciIoFreeBuffer,
  PciIoFlush,
  PciIoGetLocation,
  PciIoAttributes,
  PciIoGetBarAttributes,
  PciIoSetBarAttributes,
  0,
  0
};

EFI_STATUS
EFIAPI
PciEmulationEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
  EFI_PCI_IO_PRIVATE_DATA *Private;

  // Create a private structure
  Private = AllocatePool(sizeof(EFI_PCI_IO_PRIVATE_DATA));
  if (Private == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  Private->Signature              = EFI_PCI_IO_PRIVATE_DATA_SIGNATURE;      // Fill in signature
  Private->RootBridge.Signature   = PCI_ROOT_BRIDGE_SIGNATURE;              // Fake Root Bridge structure needs a signature too
  Private->RootBridge.MemoryStart = FixedPcdGet32(PcdEHCIBase) + 0x100;     // Get the USB capability register base
  Private->RootBridge.MemorySize  = FixedPcdGet32(PcdEHCILength) - 0x100;   // Calculate the total size of the USB registers.
  Private->Segment                = 0;                                      // Default to segment zero

  // Create fake PCI configuration space.
  Private->ConfigSpace = AllocateZeroPool(sizeof(PCI_TYPE00));
  if (Private->ConfigSpace == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    FreePool(Private);
    return Status;
  }

  // Configure PCI configuration space
  Private->ConfigSpace->Hdr.VendorId = 0x10DE;
  Private->ConfigSpace->Hdr.DeviceId = 0xF001;
  Private->ConfigSpace->Hdr.ClassCode[0] = 0x20;
  Private->ConfigSpace->Hdr.ClassCode[1] = 0x03;
  Private->ConfigSpace->Hdr.ClassCode[2] = 0x0C;
  Private->ConfigSpace->Device.Bar[0] = Private->RootBridge.MemoryStart;

  Handle = NULL;

  // Unique device path.
  CopyMem(&Private->DevicePath, &PciIoDevicePathTemplate, sizeof(PciIoDevicePathTemplate));
  Private->DevicePath.AcpiDevicePath.UID = 0;

  // Copy protocol structure
  CopyMem(&Private->PciIoProtocol, &PciIoTemplate, sizeof(PciIoTemplate));

  Status = gBS->InstallMultipleProtocolInterfaces(&Handle,
                                                  &gEfiPciIoProtocolGuid,       &Private->PciIoProtocol,
                                                  &gEfiDevicePathProtocolGuid,  &Private->DevicePath,
                                                  NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "PciEmulationEntryPoint InstallMultipleProtocolInterfaces() failed.\n"));
  }

  return Status;
}
