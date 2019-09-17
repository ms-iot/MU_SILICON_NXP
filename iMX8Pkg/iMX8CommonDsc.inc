#
#  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
#  Copyright (c) 2011-2018, ARM Limited. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#

[Defines]
  INCLUDE_TFTP_COMMAND  = FALSE

[BuildOptions]
  GCC:*_*_ARM_PLATFORM_FLAGS = -march=armv8-a -fno-inline -fno-inline-smallfunctions -fno-inline-functions-called-once
#
# For each enabled CONFIG_* flag in the PlatformBuild files, define that flag in C code.
# Note: The = sign in BuildOptions section appends to the existing macro, while
# == can be used to replace its value. See the EDK2 DSC specs.
#

# CPU_* compiler flag is used to select constants for iMX8.h
  GCC:*_*_*_CC_FLAGS = -D$(BOARD_NAME) -DCPU_$(IMX_FAMILY)

!if $(CONFIG_HEADLESS) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_HEADLESS=1
!endif

!if $(CONFIG_DUMP_SYMBOL_INFO) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_DUMP_SYMBOL_INFO=1
!endif

!if $(CONFIG_MPCORE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_MPCORE=1
!endif

!if $(CONFIG_USB) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_USB=1
!endif

!if $(CONFIG_PCIE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_PCIE=1
!endif

!if $(CONFIG_OPTEE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_OPTEE=1
!endif

!if $(CONFIG_OPTEE_PROFILE) == TRUE
  GCC:*_*_*_CC_FLAGS = -DOPTEE_PROFILE=1
!endif

!if $(CONFIG_AUTH_VAR) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_AUTH_VAR=1
!endif

!if $(CONFIG_MEASURED_BOOT) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_MEASURED_BOOT=1
!endif

!if $(CONFIG_SECURE_BOOT) == TRUE
  GCC:*_*_*_CC_FLAGS = -DCONFIG_SECURE_BOOT=1
!endif


[BuildOptions.common.EDKII.DXE_CORE,BuildOptions.common.EDKII.DXE_DRIVER,BuildOptions.common.EDKII.UEFI_DRIVER,BuildOptions.common.EDKII.UEFI_APPLICATION]
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000

[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  GCC:*_*_ARM_DLINK_FLAGS = -z common-page-size=0x1000
  GCC:*_*_AARCH64_DLINK_FLAGS = -z common-page-size=0x10000

[LibraryClasses.common]
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf

  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmMmuLib|ArmPkg/Library/ArmMmuLib/ArmMmuBaseLib.inf
  ArmSmcLib|ArmPkg/Library/ArmSmcLib/ArmSmcLib.inf

  BaseBinSecurityLib|MdePkg/Library/BaseBinSecurityLibNull/BaseBinSecurityLibNull.inf
  DeviceBootManagerLib|iMXPlatformPkg/Library/DeviceBootManagerLib/DeviceBootManagerLib.inf
  ResetSystemLib|MdeModulePkg/Library/DxeResetSystemLib/DxeResetSystemLib.inf
  SecurityLockAuditLib|MdeModulePkg/Library/SecurityLockAuditLibNull/SecurityLockAuditLibNull.inf

  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf

  # USB Requirements
!if $(CONFIG_USB) == TRUE
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
!endif

!if $(CONFIG_HEADLESS) != TRUE
  LcdHwLib|ArmPlatformPkg/Library/LcdHwNullLib/LcdHwNullLib.inf
  LcdPlatformLib|iMX8Pkg/Library/HdLcdiMX8Lib/HdLcdiMX8Lib.inf
!endif

!if $(CONFIG_FRONTPAGE) == TRUE
  MsPlatformDevicesLib|iMX8Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf
  DfciDeviceIdSupportLib|iMX8Pkg/Library/DfciDeviceIdSupportLibStub/Imx8DeviceIdSupportLibStub.inf
  PlatformThemeLib|iMX8Pkg/Library/PlatformThemeLib/PlatformThemeLib.inf
!endif

  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf

  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

  #
  # Human Interface Infrastructure (HII)
  #
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf

  #
  # Assume everything is fixed at build
  #
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

  BaseMemoryLib|MdePkg/Library/BaseMemoryLibOptDxe/BaseMemoryLibOptDxe.inf

  # ARM Architectural Libraries
  CacheMaintenanceLib|ArmPkg/Library/ArmCacheMaintenanceLib/ArmCacheMaintenanceLib.inf
  DefaultExceptionHandlerLib|ArmPkg/Library/DefaultExceptionHandlerLib/DefaultExceptionHandlerLib.inf
  CpuExceptionHandlerLib|ArmPkg/Library/ArmExceptionLib/ArmExceptionLib.inf
  ArmDisassemblerLib|ArmPkg/Library/ArmDisassemblerLib/ArmDisassemblerLib.inf
  ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicLib.inf
  ArmGicArchLib|ArmPkg/Library/ArmGicArchLib/ArmGicArchLib.inf
  ArmPlatformStackLib|ArmPlatformPkg/Library/ArmPlatformStackLib/ArmPlatformStackLib.inf
  ArmGenericTimerCounterLib|ArmPkg/Library/ArmGenericTimerPhyCounterLib/ArmGenericTimerPhyCounterLib.inf

  PlatformPeiLib|ArmPlatformPkg/PlatformPei/PlatformPeiLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf

  # iMX8 specifics
  iMXIoMuxLib|iMX8Pkg/Library/iMX8IoMuxLib/iMX8IoMuxLib.inf
  iMX8ClkPwrLib|iMX8Pkg/Library/iMX8ClkPwrLib/iMX8ClkPwrLib.inf
  SerialPortLib|iMXPlatformPkg/Library/UartSerialPortLib/UartSerialPortLib.inf

  # ARM Real Time Clock
  RealTimeClockLib|iMXPlatformPkg/Library/VirtualRealTimeClockLib/VirtualRealTimeClockLib.inf
#  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  TimeBaseLib|EmbeddedPkg/Library/TimeBaseLib/TimeBaseLib.inf
  TimerLib|ArmPkg/Library/ArmArchTimerLib/ArmArchTimerLib.inf

  #
  # Serial, Console and Debugging
  #
  # enable debug prints in both release and debug builds
!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
#  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif

  #
  # Uncomment (and comment out the next line) For RealView Debugger. The Standard IO window
  # in the debugger will show load and unload commands for symbols. You can cut and paste this
  # into the command window to load symbols. We should be able to use a script to do this, but
  # the version of RVD I have does not support scripts accessing system memory.
  #
!if $(CONFIG_DUMP_SYMBOL_INFO) == TRUE
  #PeCoffExtraActionLib|ArmPkg/Library/RvdPeCoffExtraActionLib/RvdPeCoffExtraActionLib.inf
  #PeCoffExtraActionLib|ArmPkg/Library/DebugPeCoffExtraActionLib/DebugPeCoffExtraActionLib.inf
  PeCoffExtraActionLib|Microsoft/Library/LauterbachPeCoffExtraActionLib/LauterbachPeCoffExtraActionLib.inf
!else
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
!endif

  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
  DebugAgentTimerLib|EmbeddedPkg/Library/DebugAgentTimerLibNull/DebugAgentTimerLibNull.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  SemihostLib|ArmPkg/Library/SemihostLib/SemihostLib.inf

  # BDS Libraries
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  PlatformBootManagerLib|ArmPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf

  AcpiLib|EmbeddedPkg/Library/AcpiLib/AcpiLib.inf
#  FdtLib|EmbeddedPkg/Library/FdtLib/FdtLib.inf

  # RunAxf support via Dynamic Shell Command protocol
  # It uses the Shell libraries.
  ArmShellCmdRunAxfLib|ARM/Library/ArmShellCmdRunAxf/ArmShellCmdRunAxf.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf

[LibraryClasses.common.SEC]
  ArmGicArchLib|ArmPkg/Library/ArmGicArchSecLib/ArmGicArchSecLib.inf
  DebugAgentLib|ArmPkg/Library/DebugAgentSymbolsBaseLib/DebugAgentSymbolsBaseLib.inf
  ExtractGuidedSectionLib|EmbeddedPkg/Library/PrePiExtractGuidedSectionLib/PrePiExtractGuidedSectionLib.inf
  HobLib|EmbeddedPkg/Library/PrePiHobLib/PrePiHobLib.inf
  LzmaDecompressLib|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  MemoryAllocationLib|EmbeddedPkg/Library/PrePiMemoryAllocationLib/PrePiMemoryAllocationLib.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  PlatformPeiLib|ArmPlatformPkg/PlatformPei/PlatformPeiLib.inf
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf
  PrePiHobListPointerLib|ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf

[LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION, LibraryClasses.common.DXE_RUNTIME_DRIVER, LibraryClasses.common.DXE_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  NonDiscoverableDeviceRegistrationLib|MdeModulePkg/Library/NonDiscoverableDeviceRegistrationLib/NonDiscoverableDeviceRegistrationLib.inf

!ifdef $(EDK2_SKIP_PEICORE)
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf
  ExtractGuidedSectionLib|EmbeddedPkg/Library/PrePiExtractGuidedSectionLib/PrePiExtractGuidedSectionLib.inf
  LzmaDecompressLib|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  MemoryAllocationLib|EmbeddedPkg/Library/PrePiMemoryAllocationLib/PrePiMemoryAllocationLib.inf
  HobLib|EmbeddedPkg/Library/PrePiHobLib/PrePiHobLib.inf
  PrePiHobListPointerLib|ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
!endif

[LibraryClasses.common.PEI_CORE]
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf

  PeiServicesTablePointerLib|ArmPkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf

[LibraryClasses.common.PEIM]
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PeiResourcePublicationLib|MdePkg/Library/PeiResourcePublicationLib/PeiResourcePublicationLib.inf
#  ResetSystemLib|MdeModulePkg/Library/PeiResetSystemLib/PeiResetSystemLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf

  PeiServicesTablePointerLib|ArmPkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf

[LibraryClasses.common.SEC, LibraryClasses.common.PEIM]
  MemoryInitPeiLib|ArmPlatformPkg/MemoryInitPei/MemoryInitPeiLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf

[LibraryClasses.common.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  PerformanceLib|MdeModulePkg/Library/DxeCorePerformanceLib/DxeCorePerformanceLib.inf

[LibraryClasses.common.DXE_DRIVER]
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

  #
  # Security libraries
  #
!if $(CONFIG_OPTEE) == TRUE
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  OpteeClientApiLib|Microsoft/OpteeClientPkg/Library/OpteeClientApiLib/OpteeClientApiLib.inf
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterDxe.inf
  Tpm2DeviceLibOptee|Microsoft/OpteeClientPkg/Library/Tpm2DeviceLibOptee/Tpm2DeviceLibOptee.inf
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
!endif

!if $(CONFIG_MEASURED_BOOT) == TRUE
  OemTpm2InitLib|SecurityPkg/Library/OemTpm2InitLibNull/OemTpm2InitLib.inf
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  Tcg2PhysicalPresenceLib|SecurityPkg/Library/DxeTcg2PhysicalPresenceLib/DxeTcg2PhysicalPresenceLib.inf
  Tcg2PhysicalPresencePromptLib|SecurityPkg/Library/Tcg2PhysicalPresencePromptLib/Tcg2PhysicalPresencePromptLibConsole.inf
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf
!else
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
!endif

[LibraryClasses.common.UEFI_APPLICATION]
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf

  # UiApp dependencies
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf

!if $(CONFIG_OPTEE) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  OpteeClientApiLib|Microsoft/OpteeClientPkg/Library/OpteeClientApiLib/OpteeClientApiLib.inf
!endif

  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf

[LibraryClasses.common.UEFI_DRIVER]
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
!if $(TARGET) != RELEASE
  DebugLib|MdePkg/Library/DxeRuntimeDebugLibSerialPort/DxeRuntimeDebugLibSerialPort.inf
!endif
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
  ResetUtilityLib|MdeModulePkg/Library/ResetUtilityLib/ResetUtilityLib.inf
!if $(CONFIG_OPTEE) == TRUE
  OpteeClientApiLib|Microsoft/OpteeClientPkg/Library/OpteeClientApiLib/OpteeClientApiLib.inf
  !if $(CONFIG_MEASURED_BOOT) == TRUE
    TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  !else
    TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
  !endif
!else
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
!endif
[LibraryClasses.ARM, LibraryClasses.AARCH64]
  #
  # It is not possible to prevent the ARM compiler for generic intrinsic functions.
  # This library provides the instrinsic functions generate by a given compiler.
  # [LibraryClasses.ARM] and NULL mean link this library into all ARM images.
  #
  NULL|ArmPkg/Library/CompilerIntrinsicsLib/CompilerIntrinsicsLib.inf

  # Add support for GCC stack protector
  NULL|MdePkg/Library/BaseStackCheckLib/BaseStackCheckLib.inf


[BuildOptions]
  RVCT:RELEASE_*_*_CC_FLAGS  = -DMDEPKG_NDEBUG

  GCC:RELEASE_*_*_CC_FLAGS  = -DMDEPKG_NDEBUG

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]
  gEfiMdePkgTokenSpaceGuid.PcdComponentNameDisable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnosticsDisable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdComponentName2Disable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnostics2Disable|TRUE

  # Use the Vector Table location in CpuDxe. We will not copy the Vector Table at PcdCpuVectorBaseAddress
  gArmTokenSpaceGuid.PcdRelocateVectorTable|FALSE

  gEmbeddedTokenSpaceGuid.PcdPrePiProduceMemoryTypeInformationHob|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdTurnOffUsbLegacySupport|TRUE

  ## Indicates if S3 performance data will be supported in ACPI FPDT table.
  #   TRUE  - S3 performance data will be supported in ACPI FPDT table.
  #   FALSE - S3 performance data will not be supported in ACPI FPDT table.
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwarePerformanceDataTableS3Support|FALSE

[PcdsFixedAtBuild.common]
!ifdef $(FIRMWARE_VER)
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"$(FIRMWARE_VER)"
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE

  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x01000000

  gEfiMdePkgTokenSpaceGuid.PcdMaximumUnicodeStringLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdMaximumAsciiStringLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdSpinLockTimeout|10000000
  gEfiMdePkgTokenSpaceGuid.PcdDebugClearMemoryValue|0xAF
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
  gEfiMdePkgTokenSpaceGuid.PcdPostCodePropertyMask|0
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|320

  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|0

  # DEBUG_ASSERT_ENABLED       0x01
  # DEBUG_PRINT_ENABLED        0x02
  # DEBUG_CODE_ENABLED         0x04
  # CLEAR_MEMORY_ENABLED       0x08
  # ASSERT_BREAKPOINT_ENABLED  0x10
  # ASSERT_DEADLOOP_ENABLED    0x20
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x21
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2f
!endif

  #  DEBUG_INIT      0x00000001  // Initialization
  #  DEBUG_WARN      0x00000002  // Warnings
  #  DEBUG_LOAD      0x00000004  // Load events
  #  DEBUG_FS        0x00000008  // EFI File system
  #  DEBUG_POOL      0x00000010  // Alloc & Free (pool)
  #  DEBUG_PAGE      0x00000020  // Alloc & Free (page)
  #  DEBUG_INFO      0x00000040  // Informational debug messages
  #  DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
  #  DEBUG_VARIABLE  0x00000100  // Variable
  #  DEBUG_BM        0x00000400  // Boot Manager
  #  DEBUG_BLKIO     0x00001000  // BlkIo Driver
  #  DEBUG_NET       0x00004000  // SNP Driver
  #  DEBUG_OPTEE     0x00008000  // OPTEE Driver
  #  DEBUG_UNDI      0x00010000  // UNDI Driver
  #  DEBUG_LOADFILE  0x00020000  // LoadFile
  #  DEBUG_EVENT     0x00080000  // Event messages
  #  DEBUG_GCD       0x00100000  // Global Coherency Database changes
  #  DEBUG_CACHE     0x00200000  // Memory range cachability changes
  #  DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may
  #                              // significantly impact boot performance
  #  DEBUG_ERROR     0x80000000  // Error
#  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8012004F
# Turn on FixedDebugPrintErrorLevel for more debug!!!
#  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x80080246
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80080246
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07

  #
  # Optional feature to help prevent EFI memory map fragments
  # Turned on and off via: PcdPrePiProduceMemoryTypeInformationHob
  # Values are in EFI Pages (4K). DXE Core will make sure that
  # at least this much of each type of memory can be allocated
  # from a single memory range. This way you only end up with
  # maximum of two fragements for each type in the memory map
  # (the memory used, and the free memory that was prereserved
  # but not used).
  #
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiACPIReclaimMemory|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiACPIMemoryNVS|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiReservedMemoryType|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiRuntimeServicesData|80
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiRuntimeServicesCode|65
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiBootServicesCode|400
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiBootServicesData|20000
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiLoaderCode|20
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiLoaderData|0

########################
#
# iMXPlatformPkg PCDs
#
########################
[PcdsFixedAtBuild.common]
!if $(IMX_FAMILY) == IMX8MM
  #
  # iMX8MM Plaform Memory Layout
  #                                        PcdSystemMemoryBase
  # +-------------------+===> (0x40000000) PcdArmLcdDdrFrameBufferBase (Frame buffer)
  # | HDMI framebuffer  |  ^
  # |                   |  |  (0x007E9000) PcdArmLcdDdrFrameBufferSize ~8MB
  # |                   |  v
  # +-------------------+===> (0x407E9000) PcdGlobalDataBaseAddress
  # | Global Data       |  ^
  # |                   |  |  (0x00001000) PcdGlobalDataSize 4KB
  # |                   |  v
  # +-------------------+===> (0x407EA000) 
  # | Operating System  |  ^
  # | Memory            |  |
  # |                   |  |
  # |                   |  v
  # +-------------------+===> (0xBE000000) PcdTrustZonePrivateMemoryBase (OPTEE image base address)
  # | TZ Private Memory |  ^
  # | (OPTEE)           |  |  (0x01E00000) PcdTrustZonePrivateMemorySize 30MB
  # |                   |  v
  # +-------------------+===> (0xBFE00000) PcdTrustZoneSharedMemoryBase (includes mobj bookkeeping page)
  # | TZ Shared Memory  |  ^
  # |                   |  |  (0x00200000) PcdTrustZoneSharedMemorySize 2MB
  # |                   |  v
  # +-------------------|===>

!if $(CONFIG_OPTEE) == TRUE
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemoryBase|0xBE000000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemorySize|0x01E00000

  #
  # TrustZone shared memory (2Mb)
  # This memory is managed by the normal world but shared with the OpTEE OS.
  # It must match OpTEE optee_os/core/arch/arm/plat-imx/platform_config.h:
  #    CFG_SHMEM_START & CFG_SHMEM_SIZE
  # NOTE: The first page of the SHMEM is owned by OPTEE for mobj bookkeeping
  # and we should not touch it. We will skip the first 4K of SHMEM and take that
  # into account for SHMEM size in PcdTrustZoneSharedMemorySize.
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemoryBase|0xBFE00000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemorySize|0x00200000
!endif
!else  # IMX8MQ
  #
  # iMX8 Plaform Memory Layout
  #                                        PcdSystemMemoryBase
  # +-------------------+===> (0x40000000) PcdArmLcdDdrFrameBufferBase (Frame buffer)
  # | HDMI framebuffer  |  ^
  # |                   |  |  (0x007E9000) PcdArmLcdDdrFrameBufferSize ~8MB
  # |                   |  v
  # +-------------------+===> (0x407E9000) PcdGlobalDataBaseAddress
  # | Global Data       |  ^
  # |                   |  |  (0x00001000) PcdGlobalDataSize 4KB
  # |                   |  v
  # +-------------------+===> (0x407EA000) 
  # | Operating System  |  ^
  # | Memory            |  |
  # |                   |  |
  # |                   |  v
  # +-------------------+===> (0xFE000000) PcdTrustZonePrivateMemoryBase (OPTEE image base address)
  # | TZ Private Memory |  ^
  # | (OPTEE)           |  |  (0x01E00000) PcdTrustZonePrivateMemorySize 30MB
  # |                   |  v
  # +-------------------+===> (0xFFE00000) PcdTrustZoneSharedMemoryBase (includes mobj bookkeeping page)
  # | TZ Shared Memory  |  ^
  # |                   |  |  (0x00200000) PcdTrustZoneSharedMemorySize 2MB
  # |                   |  v
  # +-------------------|===>

!if $(CONFIG_OPTEE) == TRUE
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemoryBase|0xFE000000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZonePrivateMemorySize|0x01E00000

  #
  # TrustZone shared memory (2Mb)
  # This memory is managed by the normal world but shared with the OpTEE OS.
  # It must match OpTEE optee_os/core/arch/arm/plat-imx/platform_config.h:
  #    CFG_SHMEM_START & CFG_SHMEM_SIZE
  # NOTE: The first page of the SHMEM is owned by OPTEE for mobj bookkeeping
  # and we should not touch it. We will skip the first 4K of SHMEM and take that
  # into account for SHMEM size in PcdTrustZoneSharedMemorySize.
  #
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemoryBase|0xFFE00000
  gOpteeClientPkgTokenSpaceGuid.PcdTrustZoneSharedMemorySize|0x00200000
!endif
!endif

  #
  # iMXPlatformPkg
  #

  # System Memory base
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000

  # GOP driver memory
!if $(CONFIG_HEADLESS) == TRUE
  # Global data area
  giMXPlatformTokenSpaceGuid.PcdGlobalDataBaseAddress|0x40000000
  giMXPlatformTokenSpaceGuid.PcdGlobalDataSize|0x1000

  # Reserved for TPM2 ACPI
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferBase|0x40001000
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferSize|0x3000
!else
  gArmPlatformTokenSpaceGuid.PcdArmLcdDdrFrameBufferBase|0x40000000
  gArmPlatformTokenSpaceGuid.PcdArmLcdDdrFrameBufferSize|0x007E9000	# ~8 MB (4 * 1920 * 1080)

  # Global data area
  giMXPlatformTokenSpaceGuid.PcdGlobalDataBaseAddress|0x407E9000
  giMXPlatformTokenSpaceGuid.PcdGlobalDataSize|0x1000

  # Reserved for TPM2 ACPI
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferBase|0x407EA000
  gOpteeClientPkgTokenSpaceGuid.PcdTpm2AcpiBufferSize|0x3000
!endif

!if $(IMX_FAMILY) == IMX8MM
  # SDHC memory
  giMXPlatformTokenSpaceGuid.PcdSdhc1Base|0x30B40000
  giMXPlatformTokenSpaceGuid.PcdSdhc2Base|0x30B50000
  giMXPlatformTokenSpaceGuid.PcdSdhc3Base|0x30B60000
  giMXPlatformTokenSpaceGuid.PcdSdhc4Base|0x00000000
!else
  # SDHC memory
  giMXPlatformTokenSpaceGuid.PcdSdhc1Base|0x30B40000
  giMXPlatformTokenSpaceGuid.PcdSdhc2Base|0x30B50000
  giMXPlatformTokenSpaceGuid.PcdSdhc3Base|0x00000000
  giMXPlatformTokenSpaceGuid.PcdSdhc4Base|0x00000000
!endif

  # RunAxf support via Dynamic Shell Command protocol
  # We want to use the Shell Libraries but don't want it to initialise
  # automatically. We initialise the libraries when the command is called by the
  # Shell.
  gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE

  # use the TTY terminal type
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|4

  # GUID of the UI app
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  #
  # Enable strict image permissions for all images. (This applies
  # only to images that were built with >= 4 KB section alignment.)
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdImageProtectionPolicy|0x3

  #
  # Enable NX memory protection for all non-code regions, including OEM and OS
  # reserved ones, with the exception of LoaderData regions, of which OS loaders
  # (i.e., GRUB) may assume that its contents are executable.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeNxMemoryProtectionPolicy|0xC000000000007FD1

  #
  # Enable the non-executable DXE stack. (This gets set up by DxeIpl)
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetNxForStack|TRUE

!if TRUE == FALSE
[PcdsDynamicHii.common.DEFAULT]
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|10
!endif

########################
#
# MsPkg PCDs
#
########################
[PcdsFixedAtBuild.common]

!if $(CONFIG_SECURE_BOOT) == TRUE
  gMsPkgTokenSpaceGuid.PcdSecureBootEnable|TRUE
!else
  gMsPkgTokenSpaceGuid.PcdSecureBootEnable|FALSE
!endif

########################
#
# SecurityPkg PCDs
#
########################

[PcdsFixedAtBuild.common]
!if $(CONFIG_MEASURED_BOOT) == TRUE
  #
  # override the default values from SecurityPkg to ensure images from all sources are verified in secure boot
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x04
  gEfiSecurityPkgTokenSpaceGuid.PcdFixedMediaImageVerificationPolicy|0x04
  gEfiSecurityPkgTokenSpaceGuid.PcdRemovableMediaImageVerificationPolicy|0x04

  #
  # PcdTpmInstanceGuid tells the Tcg2 Driver which TPM version is being used.
  # The below GUID is TPMv2 GUID.
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{ 0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17 }

  # Options are defined in Tcg2PhysicalPresenceLib.h Currently no PPI is supported.
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2PhysicalPresenceFlags|0x0

[PcdsDynamicExDefault.common.DEFAULT]
  # Tcg2Dxe registers SHA1, SHA256, SHA384, and SHA512
  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2HashAlgorithmBitmap|0x0f
!endif

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # PEI Phase modules
  #
  ArmPlatformPkg/PrePi/PeiUniCore.inf

  #
  # DXE
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
  }
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  #
  # Architectural Protocols
  #
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf {
!if $(CONFIG_MEASURED_BOOT) == TRUE
    <LibraryClasses>
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
      NULL|SecurityPkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
!endif
  }

  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf {
    <LibraryClasses>
      ResetSystemLib|MdeModulePkg/Library/RuntimeResetSystemLib/RuntimeResetSystemLib.inf
  }
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  ArmPkg/Drivers/TimerDxe/TimerDxe.inf
# disable - doesn't seem to be powered on
#  ArmPkg/Drivers/GenericWatchdogDxe/GenericWatchdogDxe.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

!if $(CONFIG_HEADLESS) != TRUE
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
!endif
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  #
  # FAT filesystem + GPT/MBR partitioning
  #
  FatPkg/EnhancedFatDxe/Fat.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  Microsoft/Drivers/SdMmcDxe/SdMmcDxe.inf

  # FV Filesystem
  MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

  # Unicode support (required)
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  #
  # iMX8 platform driver
  #
  iMX8Pkg/Drivers/iMX8Dxe/iMX8Dxe.inf
  iMXPlatformPkg/Drivers/SdhcDxe/SdhcDxe.inf

  #
  # GOP driver
  #
!if $(CONFIG_HEADLESS) != TRUE
  ArmPlatformPkg/Drivers/LcdGraphicsOutputDxe/LcdGraphicsOutputDxe.inf
!endif

  #
  # Bds
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
!if $(CONFIG_FRONTPAGE) != TRUE
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
!endif
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
!if $(CONFIG_FRONTPAGE) == TRUE
      PlatformBootManagerLib|MsCorePkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
!else
      PlatformBootManagerLib|ArmPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
!endif
  }
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }

!if TRUE == FALSE
  #
  # Networking stack
  #
  MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
  MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
  MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
  MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
  MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
  MdeModulePkg/Universal/Network/VlanConfigDxe/VlanConfigDxe.inf
  MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
  MdeModulePkg/Universal/Network/Tcp4Dxe/Tcp4Dxe.inf
  MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
  MdeModulePkg/Universal/Network/UefiPxeBcDxe/UefiPxeBcDxe.inf
  MdeModulePkg/Universal/Network/IScsiDxe/IScsiDxe.inf
!endif

  #
  # Usb Support
  #
!if $(CONFIG_USB) == TRUE
!if $(IMX_FAMILY) == IMX8MM
  # TODO : add Ehci packages
!else
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
!endif
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf
!endif
  #
  # UEFI application (Shell Embedded Boot Loader)
  #
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
#      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
  }

!if $(INCLUDE_TFTP_COMMAND) == TRUE
  ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf
!endif

!if $(CONFIG_OPTEE) == TRUE
  Microsoft/OpteeClientPkg/Application/OpteeClientApiTest/OpteeClientApiTest.inf
!endif

!if $(CONFIG_EDK_TESTS) == TRUE
  Microsoft/Application/StorageTest/StorageTest.inf
  Microsoft/Application/AuthVarTest/AuthVarTest.inf
!endif

!if $(CONFIG_MEASURED_BOOT) == TRUE
  #
  # FTPM
  #
  SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      Tpm2DeviceLib|Microsoft/OpteeClientPkg/Library/Tpm2DeviceLibOptee/Tpm2DeviceLibOptee.inf

    <PcdsFixedAtBuild>
      !if $(CONFIG_OPTEE_PROFILE) == TRUE
        gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
      !else
        gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0
      !endif
  }

  Microsoft/OpteeClientPkg/Drivers/AuthVarOpteeRuntimeDxe/AuthVarsDxe.inf
!else
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Universal/Variable/RuntimeDxe/PropertyBasedVarLockLib.inf

    <PcdsFixedAtBuild>
    #
    # Make VariableRuntimeDxe work at emulated non-volatile variable mode.
    #
    gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE
  }
!endif

[Components.AARCH64]
  #
  # EBC
  #
  MdeModulePkg/Universal/EbcDxe/EbcDxe.inf
