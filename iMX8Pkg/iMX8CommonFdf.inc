#
#  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
#  Copyright (c) 2013-2018, ARM Limited. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

# Continuation of [FV.FvMain]
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  #
  # PI DXE Drivers producing Architectural Protocols (EFI Services)
  #
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf

  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Timers
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  #
  # Multiple Console IO support
  #
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  INF MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  #
  # FAT filesystem + GPT/MBR partitioning
  #
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF Microsoft/Drivers/SdMmcDxe/SdMmcDxe.inf
  INF iMXPlatformPkg/Drivers/SdhcDxe/SdhcDxe.inf

  # FV FileSystem
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

  #
  # Usb Support
  #
!if $(CONFIG_USB) == TRUE
!if $(IMX_FAMILY) == IMX8MM
# TODO: add Ehci package
!else
  INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
!endif
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  INF MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf
!endif

!if $(CONFIG_HEADLESS) != TRUE
  #
  # Graphics Output Protocol
  #
  INF ArmPlatformPkg/Drivers/LcdGraphicsOutputDxe/LcdGraphicsOutputDxe.inf
!endif

  #
  # UEFI applications
  #
  INF ShellPkg/Application/Shell/Shell.inf

  #
  # iMX8M platform driver
  #
  INF iMX8Pkg/Drivers/iMX8Dxe/iMX8Dxe.inf

  #
  # Bds
  #
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Application/UiApp/UiApp.inf

  #
  # EBC
  #
  INF MdeModulePkg/Universal/EbcDxe/EbcDxe.inf

  #
  # Authenticated Variables and OPTEE TAs
  #

!if $(CONFIG_OPTEE) == TRUE
  INF Microsoft/OpteeClientPkg/Application/OpteeClientApiTest/OpteeClientApiTest.inf
#  INF SecurityPkg/Application/VariableAuthTestDxe/VariableAuthTestDxe.inf

  # Hello World TA UUID in UEFI format: 8aaaf200-2450-11e4-abe2-0002a5d5c51b
  FILE FREEFORM = 8AAAF200-2450-11E4-ABE2-0002A5D5C51B {
    SECTION RAW = Microsoft/OpteeClientPkg/Bin/HelloWorldTa/Arm64/Test/8aaaf200-2450-11e4-abe2-0002a5d5c51b.ta
  }
!endif

!if $(CONFIG_MEASURED_BOOT) == TRUE
  #
  # fTPM
  #
  INF SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf

  # fTPM Service TA UUID in UEFI format: bc50d971-d4c9-42c4-82cb-343fb7f37896
  FILE FREEFORM = BC50D971-D4C9-42C4-82CB-343FB7F37896 {
    SECTION RAW = Microsoft/OpteeClientPkg/Bin/fTpmTa/Arm64/Test/bc50d971-d4c9-42c4-82cb-343fb7f37896.ta
  }
!endif

[FV.FVMAIN_COMPACT]
FvAlignment        = 8
ERASE_POLARITY     = 1
MEMORY_MAPPED      = TRUE
STICKY_WRITE       = TRUE
LOCK_CAP           = TRUE
LOCK_STATUS        = TRUE
WRITE_DISABLED_CAP = TRUE
WRITE_ENABLED_CAP  = TRUE
WRITE_STATUS       = TRUE
WRITE_LOCK_CAP     = TRUE
WRITE_LOCK_STATUS  = TRUE
READ_DISABLED_CAP  = TRUE
READ_ENABLED_CAP   = TRUE
READ_STATUS        = TRUE
READ_LOCK_CAP      = TRUE
READ_LOCK_STATUS   = TRUE

  INF ArmPlatformPkg/PrePi/PeiUniCore.inf

  FILE FV_IMAGE = 9E21FD93-9C72-4c15-8C4B-E77F1DB2D792 {
    SECTION GUIDED EE4E5898-3914-4259-9D6E-DC7BD79403CF PROCESSING_REQUIRED = TRUE {
      SECTION FV_IMAGE = FVMAIN
    }
  }


################################################################################
#
# Rules are use with the [FV] section's module INF type to define
# how an FFS file is created for a given INF file. The following Rule are the default
# rules for the different module type. User can add the customized rules to define the
# content of the FFS file.
#
################################################################################


############################################################################
# Example of a DXE_DRIVER FFS file with a Checksum encapsulation section   #
############################################################################
#
#[Rule.Common.DXE_DRIVER]
#  FILE DRIVER = $(NAMED_GUID) {
#    DXE_DEPEX    DXE_DEPEX               Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
#    COMPRESS PI_STD {
#      GUIDED {
#        PE32     PE32                    $(INF_OUTPUT)/$(MODULE_NAME).efi
#        UI       STRING="$(MODULE_NAME)" Optional
#        VERSION  STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
#      }
#    }
#  }
#
############################################################################

#
# These SEC rules are used for ArmPlatformPkg/PrePi module.
# ArmPlatformPkg/PrePi is declared as a SEC module to make GenFv patch the
# UEFI Firmware to jump to ArmPlatformPkg/PrePi entrypoint
#
[Rule.ARM.SEC]
  FILE SEC = $(NAMED_GUID) RELOCS_STRIPPED {
    TE  TE    Align = 32                $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.AARCH64.SEC]
  FILE SEC = $(NAMED_GUID) RELOCS_STRIPPED FIXED {
    TE  TE    Align = Auto              $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

# A shim specific rule is required to ensure the alignment is 4K.
# Otherwise BaseTools pick up the AArch32 alignment (ie: 32)
[Rule.ARM.SEC.SHIM]
  FILE SEC = $(NAMED_GUID) RELOCS_STRIPPED {
    TE  TE    Align = 4K                $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.Common.PEI_CORE]
  FILE PEI_CORE = $(NAMED_GUID) FIXED {
    TE     TE                           $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI     STRING ="$(MODULE_NAME)" Optional
  }

[Rule.Common.PEIM]
  FILE PEIM = $(NAMED_GUID) FIXED {
     PEI_DEPEX PEI_DEPEX Optional       $(INF_OUTPUT)/$(MODULE_NAME).depex
     TE       TE                        $(INF_OUTPUT)/$(MODULE_NAME).efi
     UI       STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_CORE]
  FILE DXE_CORE = $(NAMED_GUID) {
    PE32     PE32                       $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI       STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.UEFI_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.DXE_RUNTIME_DRIVER]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX    DXE_DEPEX              Optional $(INF_OUTPUT)/$(MODULE_NAME).depex
    PE32         PE32                   $(INF_OUTPUT)/$(MODULE_NAME).efi
    UI           STRING="$(MODULE_NAME)" Optional
  }

[Rule.Common.UEFI_APPLICATION]
  FILE APPLICATION = $(NAMED_GUID) {
    UI     STRING ="$(MODULE_NAME)" Optional
    PE32   PE32                         $(INF_OUTPUT)/$(MODULE_NAME).efi
  }

[Rule.Common.UEFI_DRIVER.BINARY]
  FILE DRIVER = $(NAMED_GUID) {
    DXE_DEPEX DXE_DEPEX Optional      |.depex
    PE32      PE32                    |.efi
    UI        STRING="$(MODULE_NAME)" Optional
    VERSION   STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
  }

[Rule.Common.UEFI_APPLICATION.BINARY]
  FILE APPLICATION = $(NAMED_GUID) {
    PE32      PE32                    |.efi
    UI        STRING="$(MODULE_NAME)" Optional
    VERSION   STRING="$(INF_VERSION)" Optional BUILD_NUM=$(BUILD_NUMBER)
  }

[Rule.Common.USER_DEFINED.ACPITABLE]
  FILE FREEFORM = $(NAMED_GUID) {
    RAW ACPI               |.acpi
    RAW ASL                |.aml
  }
