/*
* Description: iMX8MQuad SPI Controllers
*
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
*/

Device (SPI1)
{
  Name (_HID, "NXP0105")
  Name (_HRV, 0x1)  // REV_0001
  Name (_UID, 0x1)
  Method (_STA)
  {
    Return(0x0)
  }
  Method (_CRS, 0x0, NotSerialized) {
    Name (RBUF, ResourceTemplate () {
      MEMORY32FIXED(ReadWrite, 0x30820000, 0x4000, )
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 63 }
    })
    Return(RBUF)
  }
}

Device (SPI2)
{
  Name (_HID, "NXP0105")
  Name (_HRV, 0x1)  // REV_0001
  Name (_UID, 0x2)
  Method (_STA)
  {
    Return(0x0)
  }
  Method (_CRS, 0x0, NotSerialized) {
    Name (RBUF, ResourceTemplate () {
      MEMORY32FIXED(ReadWrite, 0x30830000, 0x4000, )
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 64 }
    })
    Return(RBUF)
  }
}

Device (SPI3)
{
  Name (_HID, "NXP0105")
  Name (_HRV, 0x1)  // REV_0001
  Name (_UID, 0x3)
  Method (_STA)
  {
    Return(0x0)
  }
  Method (_CRS, 0x0, NotSerialized) {
    Name (RBUF, ResourceTemplate () {
      MEMORY32FIXED(ReadWrite, 0x30840000, 0x4000, )
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 65 }
    })
    Return(RBUF)
  }
}
