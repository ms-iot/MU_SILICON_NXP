# IoT Core NXP BSP Repository Support Notice

This repository is no longer supported and is now read-only. The BSP and documentation for the iMX6, iMX7 and iMX8 processors for Windows 10 IoT is now published by NXP [here](https://www.nxp.com/design/software/embedded-software/windows-10-iot-core-for-i.mx-applications-processors:IMXWIN10IOT). Support for the BSP is available in NXP's [community forums](https://community.nxp.com/community/imx/content?filterID=contentstatus%5Bpublished%5D%7Ecategory%5Bwindows%5D).

The following repositories have been archived and made read-only on GitHub. If you have a fork of any of these repositories they will remain active, but no new updates will be supplied by Microsoft. Development has stopped in these repositories and all future work will be released by NXP in their BSP.

The repositories to be archived are:
https://github.com/ms-iot/imx-iotcore
https://github.com/ms-iot/u-boot
https://github.com/ms-iot/edk2
https://github.com/ms-iot/imx-edk2-platforms
https://github.com/ms-iot/mu_platform_nxp
https://github.com/ms-iot/mu_silicon_nxp

The NXP BSP only has board support for NXP EVK and Sabre devices. For other previously supported devices, contact the manufacturer for further support.

All trusted application binaries in this repository have been removed. The sources in this repository can still be built, but to generate a usable image, the trusted application firmware must be manually built and added according to the instructions on the NXP website.

# NXP Silicon Code

This code module is intended to be used as a part of [Project MU](https://microsoft.github.io/mu/).

For platform examples, see our [NXP Platform repo](https://github.com/ms-iot/MU_PLATFORM_NXP)
